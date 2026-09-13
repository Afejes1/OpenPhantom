"""Focused campaign compilation and fixtures; never accepts reconstruction history."""
import argparse
import json
import re
import sys
import time
import struct
from dataclasses import asdict
import uuid
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "research/surface-emission"))
from probe import (COMMON_FLAGS, COFF, VerificationError, canonical_hash, check_lock,
                   compare_function, candidate_artifact, digest, environment,
                   load_config, load_target, require, run_tool, verify_artifacts as verify_function_artifacts,
                   verify_reference)
from native_listing_diagnostic import comparable_inventory
from matching import MATCHES
from formats import span, unpack
from probe import listing_terminators



def unit_inventory(data, symbol):
    obj = COFF(data)
    matches = [s for s in obj.symbols.values() if s.name == symbol and s.storage == 2 and s.kind & 0x20 and s.section > 0]
    require(len(matches) == 1, "candidate function missing or ambiguous")
    result = []
    for number, section in enumerate(obj.sections, 1):
        if not section.size or not section.flags & 0x20:
            continue
        code = span(data, section.offset, section.size)
        relocations = []
        for i in range(section.reloc_count):
            offset, index, kind = unpack("<IIH", data, section.reloc_offset + i * 10)
            require(index in obj.symbols and offset + 4 <= len(code), "invalid code relocation")
            relocations.append({"offset": offset, "kind": kind, "symbol": asdict(obj.symbols[index]),
                                "addend": struct.unpack_from("<I", code, offset)[0]})
        result.append({"section": number, "name": section.name, "size": len(code),
                       "sha256": digest(code), "rows": [{"offset": i, "hex": code[i:i + 16].hex(" ")} for i in range(0, len(code), 16)],
                       "symbols": [asdict(s) for s in obj.symbols.values() if s.section == number and s.kind & 0x20],
                       "relocations": relocations})
    require(result, "object contains no code")
    return {"schema": 1, "kind": "authored-translation-unit-code", "symbol": symbol, "sections": result}


def capture_artifact(source, spec, obj, listing):
    try:
        return candidate_artifact(source, spec, obj, listing)
    except VerificationError as error:
        if str(error) not in ("first-batch objects must contain one code section", "multiple functions in object", "function must occupy the complete code section"):
            raise
    inventory = unit_inventory(obj.read_bytes(), spec["symbol"])
    inventory_path = obj.with_suffix(".unit.json")
    inventory_path.write_bytes((json.dumps(inventory, indent=2) + "\n").encode())
    listing_bytes = listing.read_bytes()
    require(bool(listing_bytes.strip()), "empty assembly listing")
    return {"id": spec["id"], "source": source, "symbol": spec["symbol"],
            "inventory_scope": "translation-unit", "object": obj.name, "object_sha256": digest(obj.read_bytes()),
            "code_section_bytes": sum(s["size"] for s in inventory["sections"]),
            "assembly_listing": listing.name, "assembly_listing_sha256": digest(listing_bytes),
            "assembly_listing_has_terminators": listing_terminators(listing_bytes, spec["symbol"]),
            "function_inventory": inventory_path.name, "function_inventory_sha256": digest(inventory_path.read_bytes()),
            "strict_function_comparable": False}


def verify_artifacts(out, artifacts):
    for artifact in artifacts:
        fallback = artifact.get("listing_fallback")
        if fallback is not None:
            original_listing = out / fallback["original_docker_listing"]
            require(original_listing.stat().st_size == 0 and
                    digest(original_listing.read_bytes()) == fallback["original_docker_listing_sha256"],
                    "original empty Docker listing changed")
        if artifact.get("inventory_scope") != "translation-unit":
            verify_function_artifacts(out, [artifact])
            continue
        for name, checksum in (("object", "object_sha256"), ("assembly_listing", "assembly_listing_sha256"), ("function_inventory", "function_inventory_sha256")):
            require(digest((out / artifact[name]).read_bytes()) == artifact[checksum], "translation-unit artifact changed")
        require(json.loads((out / artifact["function_inventory"]).read_bytes()) == unit_inventory((out / artifact["object"]).read_bytes(), artifact["symbol"]), "translation-unit inventory differs")


def comparable_artifact(out, artifact):
    data = (out / artifact["object"]).read_bytes()
    if artifact.get("inventory_scope") == "translation-unit":
        return unit_inventory(data, artifact["symbol"])
    return comparable_inventory(data, artifact["symbol"])



def select_listing(listing, obj, symbol, runner, allow_fallback, peer, peer_path):
    """Use a complete native listing only after identical-source peer validation.

    main validates the peer source snapshot and toolchain lock before calling.
    Byte/fixup equality is independently checked here before fixture execution.
    The original empty Docker artifact stays intact and explicitly identified.
    """
    if listing.read_bytes().strip():
        return listing, None
    require(allow_fallback and runner == "docker", "compiler assembly listing is empty")
    require(listing.stat().st_size == 0, "listing fallback requires a zero-byte Docker artifact")
    require(peer is not None and peer_path is not None and peer.get("runner") == "native"
            and peer.get("completed") is True, "complete native peer required for listing fallback")
    require(len(peer["artifacts"]) == 1, "single-function native peer required")
    expected = peer["artifacts"][0]
    require(expected.get("inventory_scope") is None and expected["symbol"] == symbol,
            "single-function native peer required")
    for key in ("object", "assembly_listing", "function_inventory"):
        require(Path(expected[key]).name == expected[key], "invalid native peer artifact path")
    verify_artifacts(peer_path.parent, [expected])
    native_obj = peer_path.parent / expected["object"]
    require(comparable_inventory(obj.read_bytes(), symbol) ==
            comparable_inventory(native_obj.read_bytes(), symbol),
            "native/Docker code or relocations differ before listing fallback")
    native_listing = (peer_path.parent / expected["assembly_listing"]).read_bytes()
    require(listing_terminators(native_listing, symbol), "native peer listing is incomplete")
    copied = listing.with_name(listing.stem + ".native.cod")
    copied.write_bytes(native_listing)
    return copied, {
        "kind": "verified-native-peer-listing",
        "original_docker_listing": listing.name,
        "original_docker_listing_sha256": digest(listing.read_bytes()),
        "native_peer_report_sha256": digest(peer_path.read_bytes()),
        "native_peer_object_sha256": digest(native_obj.read_bytes()),
        "native_peer_listing_sha256": digest(native_listing),
        "function_and_relocations_equal_before_fixture": True,
    }


def read_case(folder):
    folder = folder.resolve()
    require(folder.is_relative_to(ROOT / "research"), "case must be within research")
    path = folder / "case.json"
    case = json.loads(path.read_bytes())
    require(case.get("schema") == 1, "unsupported campaign case")
    require(re.fullmatch(r"[a-z][a-z0-9_]*", case["id"]), "invalid case id")
    require(re.fullmatch(r"_[A-Za-z][A-Za-z0-9_]*", case["symbol"]), "invalid C-linkage symbol")
    require(case["flags"] in (["/Od", "/MT"], ["/O2", "/MT"], ["/Ox", "/MT"]), "unapproved compiler options")
    inputs = []
    names = set()
    for item in case["inputs"]:
        source = (folder / item).resolve()
        require(any(source.is_relative_to(ROOT / part) for part in ("src", "research", "tests")), "input outside source directories")
        require(source.is_file() and source.suffix in (".c", ".cpp", ".h"), "input must be source/header")
        require(source.name.lower() not in names, "duplicate input basename")
        names.add(source.name.lower())
        inputs.append(source)
    for key in ("candidate", "fixture"):
        value = case.get(key)
        if value is not None:
            require(Path(value).name == value and Path(value).suffix in (".c", ".cpp"), "invalid translation unit")
            require((folder / value).resolve() in inputs, "translation unit missing from inputs")
    require(case.get("candidate") is not None, "candidate missing")
    for value in case.get("extra_fixture_sources", []):
        require(Path(value).name == value and Path(value).suffix in (".c", ".cpp"), "invalid extra source")
        require(any(source.name == value for source in inputs), "extra source missing from inputs")
        require(value not in (case["candidate"], case.get("fixture")), "duplicate fixture source")
    target_path = None
    if case.get("target") is not None:
        target_path = (folder / case["target"]).resolve()
        require(target_path.is_relative_to(folder) and target_path.suffix == ".json", "target outside case")
    if case.get("reference_layout") is not None:
        layout = (folder / case["reference_layout"]).resolve()
        require(layout.is_relative_to(folder) and layout.suffix == ".json", "reference layout outside case")
    return case, inputs, path, target_path


def validate_mode(runner, behavior, fixture):
    require(not behavior or runner == "docker", "behavior execution requires locked Docker")
    require(not behavior or fixture is not None, "behavior fixture missing")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--case", type=Path, required=True)
    parser.add_argument("--runner", choices=("native", "docker"), default="docker")
    parser.add_argument("--behavior", action="store_true")
    parser.add_argument("--reference", type=Path)
    parser.add_argument("--expect-report", type=Path)
    parser.add_argument("--native-listing-fallback", action="store_true",
                        help="Use a verified native peer listing if the Docker listing is empty")
    args = parser.parse_args()
    require(not args.native_listing_fallback or
            (args.runner == "docker" and args.expect_report is not None),
            "listing fallback requires Docker and --expect-report")
    started = time.perf_counter()
    case, inputs, case_path, target_path = read_case(args.case)
    validate_mode(args.runner, args.behavior, case.get("fixture"))
    config = load_config(ROOT / "private/toolchain.json")
    require(config["runner"]["kind"] == "docker-wibo", "locked Docker configuration required")
    lock = json.loads((ROOT / "private/toolchain.lock.json").read_bytes())
    check_lock(config, lock)
    provenance = inputs + [case_path, Path(__file__).resolve(), ROOT / "target.json"]
    if target_path is not None:
        provenance.append(target_path)
    layout_path = None
    if case.get("reference_layout") is not None:
        layout_path = (case_path.parent / case["reference_layout"]).resolve()
        provenance.append(layout_path)
    provenance += [ROOT / "research/surface-emission" / name for name in ("probe.py", "native_listing_diagnostic.py")]
    provenance += [ROOT / "tools" / name for name in ("build.py", "formats.py", "matching.py", "runners.py", "source_policy.py")]
    def snapshot():
        return {p.relative_to(ROOT).as_posix(): digest(p.read_bytes()) for p in provenance}
    source_snapshot = snapshot()
    peer = peer_bytes = peer_path = None
    if args.expect_report:
        peer_path = args.expect_report.resolve()
        require(peer_path.is_relative_to(ROOT / "build"), "peer report must be a private build")
        peer_bytes = peer_path.read_bytes()
        peer = json.loads(peer_bytes)
        require(peer.get("kind") == "campaign-focused-verification" and peer.get("completed") is True, "incomplete peer report")
        require(peer.get("id") == case["id"] and peer["runner"] != args.runner, "peer must be opposite runner for same case")
        require(peer["source_snapshot"] == source_snapshot and peer["toolchain_lock_sha256"] == canonical_hash(lock), "peer source or tools differ")
        verify_artifacts(peer_path.parent, peer["artifacts"])
    out = ROOT / "build" / ("campaign-001-" + case["id"] + "-" + uuid.uuid4().hex[:12])
    out.mkdir()
    for source in inputs:
        (out / source.name).write_bytes(source.read_bytes())
    active = dict(config)
    if args.runner == "native":
        active["runner"] = {"kind": "native"}
    report = {"schema": 1, "kind": "campaign-focused-verification", "id": case["id"],
              "runner": args.runner, "accepted_history_record": False, "whole_executable_match": False,
              "layout_verified": False, "source_snapshot": source_snapshot,
              "toolchain_lock_sha256": canonical_hash(lock), "docker_image": config["runner"]["image"],
              "completed": False, "commands": [], "artifacts": [], "artifact_notices": [],
              "comparison": {"status": "not-attempted"}, "fixture": None,
              "generated_code_execution_attempted": False, "generated_code_executed": False,
              "original_game_executed": False, "warning_waivers": [], "byte_exclusions": []}
    def invoke(command, phase):
        begin = time.perf_counter()
        process, invoked = run_tool(active, command, ROOT, out, environment(active), 60)
        report["commands"].append({"phase": phase, "argv": command, "invoked": invoked,
                                   "seconds": round(time.perf_counter() - begin, 6), "returncode": process.returncode})
        output = process.stdout + process.stderr
        (out / ("command-%02d.log" % len(report["commands"]))).write_bytes(output.encode())
        require(process.returncode == 0, "command failed: " + phase)
        require(not re.search(r"\bwarning\b", output, re.I), "warning in " + phase)
        return process
    try:
        spec = {"id": case["id"], "symbol": case["symbol"], "flags": case["flags"]}
        original = None
        if target_path is not None:
            spec = json.loads(target_path.read_bytes())
            require(spec["id"] == case["id"] and spec["symbol"] == case["symbol"] and spec["flags"] == case["flags"], "case/target disagreement")
            require(Path(spec["source"]).suffix == Path(case["candidate"]).suffix, "case/target frontend disagreement")
            target = load_target(ROOT / "target.json")
            target["functions"] = [spec]
            pending = out / "pending-target.json"
            pending.write_bytes((json.dumps(target, indent=2) + "\n").encode())
            target = load_target(pending)
            if args.reference:
                original = verify_reference(args.reference.read_bytes(), target)
        if args.reference and layout_path is not None:
            layout = json.loads(layout_path.read_bytes())
            identity = load_target(ROOT / "target.json")
            reference = verify_reference(args.reference.read_bytes(), identity)
            require(layout["id"] == case["id"] and 0 < layout["body_size"] <= layout["size"], "invalid reference layout")
            reference_span = reference.read_va(layout["address"], layout["size"])
            report["reference_layout"] = {"address": layout["address"], "size": layout["size"],
                "body_size": layout["body_size"], "trailing_bytes": layout["size"] - layout["body_size"],
                "reference_span_sha256": digest(reference_span), "binding_inventory_verified": False,
                "reference_identity_sha256": identity["sha256"]}
        obj = out / "candidate.obj"
        invoke([active["compiler"], *COMMON_FLAGS, *case["flags"], "/FAcs",
                "/Fd" + str(out / "probe.pdb"), "/Fo" + str(obj), str(out / case["candidate"])], "candidate-compile")
        listing = out / (Path(case["candidate"]).stem + ".cod")
        listing, fallback = select_listing(listing, obj, spec["symbol"], args.runner,
                                           args.native_listing_fallback, peer, peer_path)
        artifact = capture_artifact(case["candidate"], spec, obj, listing)
        if fallback is not None:
            artifact["listing_fallback"] = fallback
            report["artifact_notices"].append("Empty Docker listing retained; complete native peer listing copied after identical function/fixup verification")
        report["artifacts"].append(artifact)
        if not artifact["assembly_listing_has_terminators"]:
            report["artifact_notices"].append("Listing lacks terminators; complete COFF inventory retained and checked")
        if artifact.get("inventory_scope") == "translation-unit":
            report["artifact_notices"].append("Multiple-function translation unit retained; no strict per-function extent claim")
        if original is not None:
            try:
                report["comparison"] = compare_function(original, COFF(obj.read_bytes()), spec)
            except VerificationError as error:
                report["comparison"] = {"status": "unresolved", "reason": str(error)}
        if args.behavior:
            objects = [obj]
            for number, source in enumerate([case["fixture"], *case.get("extra_fixture_sources", [])]):
                fixture_obj = out / ("fixture-%d.obj" % number)
                invoke([active["compiler"], *COMMON_FLAGS, "/Od", "/MT",
                        "/Fd" + str(out / "probe.pdb"), "/Fo" + str(fixture_obj), str(out / source)], "fixture-compile")
                objects.append(fixture_obj)
            executable = out / "campaign-fixture.exe"
            invoke([active["linker"], "/NOLOGO", "/MACHINE:IX86", "/SUBSYSTEM:CONSOLE", "/INCREMENTAL:NO",
                    "/OUT:" + str(executable), *map(str, objects), "libcmt.lib", "kernel32.lib"], "fixture-link")
            report["fixture"] = {"sha256": digest(executable.read_bytes()), "passed": False}
            report["generated_code_execution_attempted"] = True
            report["generated_code_executed"] = None
            process = invoke([str(executable)], "fixture-execute")
            report["generated_code_executed"] = True
            require(digest(executable.read_bytes()) == report["fixture"]["sha256"], "fixture changed")
            report["fixture"].update(passed=True, stdout=process.stdout)
        if peer is not None:
            peer_artifact = peer["artifacts"][0]
            require(artifact.get("inventory_scope") == peer_artifact.get("inventory_scope"), "peer artifact scopes differ")
            require(comparable_artifact(out, artifact) == comparable_artifact(peer_path.parent, peer_artifact), "native/Docker code or relocations differ")
            key = "native_docker_translation_unit_code_and_relocations_equal" if artifact.get("inventory_scope") == "translation-unit" else "native_docker_function_and_relocations_equal"
            report[key] = True
            report["peer_report_sha256"] = digest(peer_bytes)
            require(peer_path.read_bytes() == peer_bytes, "peer report changed")
            verify_artifacts(peer_path.parent, peer["artifacts"])
        verify_artifacts(out, report["artifacts"])
        require(snapshot() == source_snapshot, "source changed during verification")
        check_lock(config, lock)
        report["completed"] = True
    except Exception as error:
        report["error"] = str(error)
        raise
    finally:
        report["elapsed_seconds"] = round(time.perf_counter() - started, 6)
        result = out / "research-result.json"
        result.write_bytes((json.dumps(report, indent=2) + "\n").encode())
        print(json.dumps({"report": str(result), "completed": report["completed"], "comparison": report["comparison"], "fixture": report["fixture"], "elapsed_seconds": report["elapsed_seconds"]}))
    if args.reference:
        return 0 if report["comparison"]["status"] in MATCHES else 1
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (VerificationError, OSError, ValueError, KeyError) as error:
        print("FAILED:", error, file=sys.stderr)
        sys.exit(2)
