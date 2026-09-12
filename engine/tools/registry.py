"""Public, chained build evidence. Original bytes and private paths stay local."""
import json
from pathlib import Path
import re
from formats import require
from matching import (MATCHES, canonical_hash, digest, binding_kind,
                      expected_operand, validate_bindings, source_language)
from source_policy import validate_source_policy


def load_registry(root, target):
    validate_source_policy(root, target)
    value = json.loads((root / "registry.json").read_text(encoding="utf-8"))
    require(value.get("schema") == 1 and value.get("target") == target["target"], "invalid registry")
    entries = value["functions"]
    require(len({s["id"] for s in entries}) == len(entries), "duplicate registry entry")
    require({s["id"] for s in entries} == {s["id"] for s in target["functions"]}, "registry and target inventories differ")
    specs = {s["id"]: s for s in target["functions"]}
    for entry in entries:
        for key in ("module", "analysis_name", "purpose"):
            require(isinstance(entry[key], str) and entry[key].strip(), "missing registry " + key)
        require(entry["confidence"] in ("HIGH", "MEDIUM", "LOW"), "invalid confidence")
        require(isinstance(entry["quirks"], list) and all(isinstance(q, str) and q for q in entry["quirks"]), "invalid quirks")
        waivers = entry.get("diagnostic_waivers", [])
        require([w["code"] for w in waivers] == specs[entry["id"]].get("diagnostic_waivers", []), "diagnostic waiver metadata differs")
        for waiver in waivers:
            require(waiver.get("reason") and waiver.get("scope"), "incomplete diagnostic waiver")
            evidence = (root / waiver["evidence"]).resolve()
            require(evidence.is_relative_to(root.resolve()) and evidence.is_file(), "missing diagnostic waiver evidence")
        require(isinstance(entry["exceptions"], list), "invalid exceptions")
        for exception in entry["exceptions"]:
            require(all(exception.get(k) for k in ("id", "reason", "scope", "evidence")), "incomplete exception")
        for key in ("evidence", "behavior_tests"):
            require(entry[key], "missing " + key)
            for name in entry[key]:
                path = (root / name).resolve()
                require(path.is_relative_to(root.resolve()) and path.is_file(), "missing evidence: " + name)
    return value


def portable_toolchain(config, lock):
    require(config["runner"]["kind"] == "docker-wibo", "portable evidence requires Docker")
    base = Path(config["runner"]["toolchain_root"]).resolve()
    files = {Path(p).resolve().relative_to(base).as_posix(): sha for p, sha in lock["files"].items()}
    paths = {key: [Path(p).resolve().relative_to(base).as_posix() for p in config[key]]
             for key in ("bin_dirs", "include_dirs", "lib_dirs")}
    paths.update({key: Path(config[key]).resolve().relative_to(base).as_posix() for key in ("compiler", "linker")})
    return {"schema": 1, "files": files, "paths": paths, "runtime": lock["runtime"],
            "compiler_version": "11.00.7022", "runtime_startup_revision": lock["runtime_startup_revision"]}


def validate_event(event):
    require(event.get("schema") == 1 and event.get("kind") == "verified-function-build", "invalid history event")
    require(event.get("whole_executable_match") is False and event.get("linked_placement_verified") is False,
            "this recorder cannot certify a retail link")
    require(event["behavior_passed"] is True and event["functions"], "history requires passing code and behavior")
    require(len({f["id"] for f in event["functions"]}) == len(event["functions"]), "duplicate history function")
    for function in event["functions"]:
        spec, result = function["spec"], function["result"]
        require(function["id"] == spec["id"] == result["id"], "history identity differs")
        require(function["spec_sha256"] == canonical_hash(spec), "history specification hash differs")
        require(not function["exceptions"], "exceptions cannot be counted as matching")
        require([w["code"] for w in function.get("diagnostic_waivers", [])] == spec.get("diagnostic_waivers", []),
                "history diagnostic waivers differ")
        require(result["status"] in MATCHES and result["different_bytes"] == 0, "history function does not match")
        require(result["relocated_bytes_equal"] is True, "missing resolved equality")
        require(result["reference_span_sha256"] == result["resolved_span_sha256"], "history span hashes differ")
        require(result["size"] == result["candidate_size"] == spec["size"], "history size differs")
        require(result["address"] == spec["address"], "history address differs")
        require(result["body_size"] == spec.get("body_size", spec["size"]), "history body size differs")
        source_language(spec)
        validate_bindings(spec)
        adjusted = len(spec["bindings"]) * 4
        call_bytes = sum(4 for b in spec["bindings"] if binding_kind(b) == "rel32-call")
        require(result.get("rel32_call_bytes", 0) == call_bytes, "history relative-call accounting differs")
        require(result.get("dir32_bytes", adjusted) == adjusted - call_bytes, "history absolute accounting differs")
        internal_bytes = sum(4 for b in spec["bindings"] if binding_kind(b) == "dir32-internal")
        require(result.get("internal_dir32_bytes", 0) == internal_bytes, "history internal accounting differs")
        require(result["adjusted_bytes"] == adjusted and result["compared_bytes"] + adjusted == spec["size"],
                "history byte accounting differs")
        require(result["status"] == ("relocation-adjusted-match" if adjusted else "raw-code-match"), "wrong matching category")
        expected_raw = all(expected_operand(spec, b) == b.get("addend", 0) for b in spec["bindings"])
        require(result["raw_bytes_equal"] is expected_raw, "inconsistent raw equality")
        require(result["layout_verified"] is False and result["literal_function_match"] is False, "unsupported linked claim")
    return event


def preserves(previous, current):
    require(previous["target_sha256"] == current["target_sha256"], "history reference changed")
    require(previous["toolchain"] == current["toolchain"], "history toolchain changed; separate calibration series required")
    now = {f["id"]: f for f in current["functions"]}
    for old in previous["functions"]:
        require(old["id"] in now, "accepted function removed: " + old["id"])
        new = now[old["id"]]
        require(old["spec"] == new["spec"], "accepted function specification changed: " + old["id"])
        require(old["result"]["reference_span_sha256"] == new["result"]["reference_span_sha256"], "accepted reference span changed")


def read_history(folder):
    history = []
    for path in sorted(folder.glob("*.json")):
        require(re.fullmatch(r"\d{6}-[0-9a-f]{16}\.json", path.name), "unexpected history filename")
        event = validate_event(json.loads(path.read_text(encoding="utf-8")))
        sha = canonical_hash(event)
        require(path.name == "%06d-%s.json" % (len(history) + 1, sha[:16]), "history sequence or digest differs")
        require(event["previous_sha256"] == (canonical_hash(history[-1]) if history else None), "history chain broken")
        if history:
            preserves(history[-1], event)
        history.append(event)
    return history


def portable_commands(commands, root, out, config):
    replacements = [(str(Path(config["runner"]["toolchain_root"]).resolve()), "<TOOLCHAIN>"),
                    (str(out.resolve()), "<BUILD>"), (str(root.resolve()), "<ENGINE>")]
    result = []
    for command in commands:
        require(command["returncode"] == 0, "failed build command")
        argv = []
        for original in command["argv"]:
            value = original
            for source, dest in replacements:
                value = value.replace(source, dest)
            argv.append(value.replace("\\", "/"))
        result.append(argv)
    return result


def record_event(root, out, target, registry, config, lock, build, comparison):
    require(comparison["complete"] is True and comparison["behavior"]["passed"] is True, "cannot record incomplete verification")
    require(comparison["build_sha256"] == canonical_hash(build), "comparison belongs to another build")
    require(build.get("source_dirty") is False, "commit source changes before recording public evidence")
    folder = root / "evidence" / "runs"
    history = read_history(folder)
    require(not any(e["private_build_sha256"] == canonical_hash(build) for e in history), "build already recorded")
    metadata = {f["id"]: f for f in registry["functions"]}
    results = {f["id"]: f for f in comparison["functions"]}
    event = {"schema": 1, "kind": "verified-function-build", "build_id": out.name,
             "previous_sha256": canonical_hash(history[-1]) if history else None,
             "source_revision": build["source_revision"], "source_snapshot": build["source_snapshot"],
             "private_build_sha256": canonical_hash(build), "target_sha256": target["sha256"],
             "target_spec_sha256": canonical_hash(target), "registry_sha256": canonical_hash(registry),
             "toolchain": portable_toolchain(config, lock),
             "commands": portable_commands(build["commands"], root, out, config),
             "behavior_passed": True, "whole_executable_match": False, "linked_placement_verified": False,
             "functions": [{"id": s["id"], "module": metadata[s["id"]]["module"], "spec": s,
                            "spec_sha256": canonical_hash(s), "exceptions": metadata[s["id"]]["exceptions"],
                            "diagnostic_waivers": metadata[s["id"]].get("diagnostic_waivers", []),
                            "result": results[s["id"]]} for s in target["functions"]]}
    validate_event(event)
    if history:
        preserves(history[-1], event)
    folder.mkdir(parents=True, exist_ok=True)
    path = folder / ("%06d-%s.json" % (len(history) + 1, canonical_hash(event)[:16]))
    with path.open("x", encoding="utf-8", newline="\n") as stream:
        stream.write(json.dumps(event, indent=2) + "\n")
    return path



def validate_current_specs(history, target):
    if not history:
        return
    current = {s["id"]: s for s in target["functions"]}
    for old in history[-1]["functions"]:
        require(current.get(old["id"]) == old["spec"], "accepted target definition removed or changed: " + old["id"])


def render_progress(root, target, registry, history):
    from build import source_snapshot
    validate_current_specs(history, target)
    entries = {f["id"]: f for f in registry["functions"]}
    latest = history[-1] if history else None
    results = {f["id"]: f for f in latest["functions"]} if latest else {}
    snapshot = latest["source_snapshot"] if latest else {}
    source_current = bool(latest) and source_snapshot(root) == snapshot
    matched = 0
    lines = ["# Reconstruction progress", "", registry["inventory_scope"] + ".", "",
             "Generated from registry.json, target.json and chained verification records. No game bytes are stored here.", "",
             "Whole executable: **incomplete**. Original linked placement: **unverified**.", "",
             "| Module | Function / source | Original address | VC5 frontend | Last verified result | Bytes / address bytes |", "|---|---|---|---|---|---|"]
    for spec in target["functions"]:
        previous = results.get(spec["id"])
        valid = bool(previous) and previous["spec"] == spec and not entries[spec["id"]]["exceptions"]
        status = previous["result"]["status"] if valid else "unverified"
        matched += int(valid)
        label = status if source_current or not valid else status + " (historical; current source unverified)"
        lines.append("| %s | [%s](../%s) | 0x%08X | %s | %s | %d / %d |" % (
            entries[spec["id"]]["module"], spec["id"], spec["source"], spec["address"], source_language(spec), label, spec["size"], len(spec["bindings"]) * 4))
    lines += ["", "Registered functions: **%d**. Last recorded matches with unchanged definitions: **%d**." % (len(entries), matched),
              "Current source fingerprint agrees with the latest run: **%s**." % ("yes" if source_current else "no"),
              "This count is not whole-program coverage and does not certify the modern TPM reconstruction.", ""]
    if latest:
        lines += ["Latest build: '%s', source commit '%s'." % (latest["build_id"], latest["source_revision"]),
                  "History contains %d verified build records." % len(history), ""]
    lines += ["## Workarounds and exceptions", ""]
    for entry in registry["functions"]:
        lines.append("- **%s**: %s. Exceptions: %s." % (entry["id"], "; ".join(entry["quirks"]) or "None",
                                                        json.dumps(entry["exceptions"]) if entry["exceptions"] else "none"))
        for waiver in entry.get("diagnostic_waivers", []):
            lines.append("  Compiler diagnostic waiver C%d: %s. Scope: %s. No byte exclusion." %
                         (waiver["code"], waiver["reason"], waiver["scope"]))
    lines += ["", "## Unresolved program work", ""] + ["- " + q for q in registry["open_program_questions"]] + [""]
    return "\n".join(lines)
