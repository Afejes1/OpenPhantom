"""Focused research builds; never creates accepted function-history records."""
import argparse
import json
from pathlib import Path
import re
import sys
import uuid

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
from build import COMMON_FLAGS, check_lock, environment, load_config
from formats import COFF, VerificationError, require
from matching import MATCHES, canonical_hash, compare_function, digest, load_target, verify_reference
from runners import run_tool

CASES = (("candidate.cpp", "target.json"), ("legacy_candidate.c", "legacy-target.json"),
         ("culling_candidate.c", "culling-target.json"),
         ("collection_candidate.cpp", "collection-target.json"))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("compare", "behavior"))
    parser.add_argument("--reference", type=Path)
    args = parser.parse_args()
    config = load_config(ROOT / "private/toolchain.json")
    lock = json.loads((ROOT / "private/toolchain.lock.json").read_text())
    require(config["runner"]["kind"] == "docker-wibo", "this research fixture requires the locked Docker runner")
    check_lock(config, lock)
    out = ROOT / "build" / ("surface-batch-" + uuid.uuid4().hex[:12])
    out.mkdir()
    inputs = [ROOT / "src/baseline.h", HERE / "batch_surface.h", HERE / "culling.h", HERE / "collection.h"]
    inputs += [HERE / name for pair in CASES for name in pair]
    if args.command == "behavior":
        inputs += [HERE / "focused_behavior.c", HERE / "culling_behavior.c", HERE / "collection_behavior.c", ROOT / "src/push_surface_draw_entry.cpp"]
    # Hash the runner and verifier too; a research report identifies the code that judged it.
    provenance = inputs + [Path(__file__).resolve(), ROOT / "target.json"]
    provenance += [ROOT / "tools" / name for name in
                   ("build.py", "formats.py", "matching.py", "runners.py", "source_policy.py")]
    def snapshot_inputs():
        return {p.relative_to(ROOT).as_posix(): digest(p.read_bytes()) for p in provenance}
    snapshot = snapshot_inputs()
    for path in inputs:
        (out / path.name).write_bytes(path.read_bytes())
    specs = [json.loads((HERE / spec).read_text()) for _, spec in CASES]
    target = load_target(ROOT / "target.json")
    target["functions"] = specs
    (out / "pending-target.json").write_text(json.dumps(target, indent=2) + "\n")
    target = load_target(out / "pending-target.json")
    original = None
    if args.command == "compare":
        require(args.reference is not None, "compare requires --reference")
        original = verify_reference(args.reference.read_bytes(), target)
    report = {"schema": 1, "scope": "unverified-surface-research-candidates",
              "accepted_history_record": False, "whole_executable_match": False,
              "toolchain_lock_sha256": canonical_hash(lock), "source_snapshot": snapshot,
              "commands": [], "comparisons": [], "fixtures": [], "behavior_passed": None,
              "completed": False}
    def invoke(command):
        process, invoked = run_tool(config, command, ROOT, out, environment(config), 60)
        report["commands"].append({"argv": command, "invoked": invoked, "returncode": process.returncode})
        text = process.stdout + process.stderr
        (out / ("command-%02d.log" % len(report["commands"]))).write_text(text, encoding="utf-8")
        require(process.returncode == 0, "command failed; inspect " + str(out))
        require(not re.search(r"\bwarning\b", text, re.I), "warning in focused build; inspect " + str(out))
        return process
    try:
        objects = {}
        for (source, _), spec in zip(CASES, specs):
            obj = out / (spec["id"] + ".obj")
            invoke([config["compiler"], *COMMON_FLAGS, *spec["flags"],
                    "/Fd" + str(out / "probe.pdb"), "/Fo" + str(obj), str(out / source)])
            objects[spec["id"]] = obj
            if original is not None:
                try:
                    result = compare_function(original, COFF(obj.read_bytes()), spec)
                except VerificationError as error:
                    result = {"id": spec["id"], "status": "unresolved", "reason": str(error)}
                report["comparisons"].append(result)
                print(spec["id"] + ": " + result["status"] + "; " + result.get("reason", "complete span agrees"))
        if args.command == "behavior":
            def compile_fixture(source, flags):
                obj = out / (source + ".obj")
                invoke([config["compiler"], *COMMON_FLAGS, *flags,
                        "/Fd" + str(out / "probe.pdb"), "/Fo" + str(obj), str(out / source)])
                return obj
            helper = compile_fixture("push_surface_draw_entry.cpp", ["/O2", "/MT"])
            emission_test = compile_fixture("focused_behavior.c", ["/Od", "/MT"])
            culling_test = compile_fixture("culling_behavior.c", ["/Od", "/MT"])
            collection_test = compile_fixture("collection_behavior.c", ["/Od", "/MT"])
            suites = (("emission", [objects["emit_surface"], objects["emit_legacy_surface"], helper, emission_test]),
                      ("culling", [objects["cull_scan_plane"], culling_test]),
                      ("collection", [objects["gather_static_cell"], collection_test]))
            for name, fixture_objects in suites:
                executable = out / (name + "-focused.exe")
                invoke([config["linker"], "/NOLOGO", "/MACHINE:IX86", "/SUBSYSTEM:CONSOLE",
                        "/INCREMENTAL:NO", "/OUT:" + str(executable), *map(str, fixture_objects),
                        "libcmt.lib", "kernel32.lib"])
                fixture = {"name": name, "sha256": digest(executable.read_bytes()), "passed": False}
                report["fixtures"].append(fixture)
                process = invoke([str(executable)])
                require(digest(executable.read_bytes()) == fixture["sha256"], "fixture changed during execution")
                fixture.update(passed=True, stdout=process.stdout)
                print(process.stdout, end="")
        require(snapshot == snapshot_inputs(), "research inputs changed during build")
        check_lock(config, lock)
        if args.command == "behavior":
            report["behavior_passed"] = True
        report["completed"] = True
    finally:
        (out / "research-result.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        print(out)
    if args.command == "behavior":
        return 0
    return 0 if all(r["status"] in MATCHES for r in report["comparisons"]) else 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (VerificationError, OSError, ValueError, KeyError) as error:
        print("FAILED:", error, file=sys.stderr)
        sys.exit(2)
