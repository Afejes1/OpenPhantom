"""Focused queue-flush research. A body match never accepts missing alignment."""
import argparse
import json
from pathlib import Path
import re
import sys
import uuid

ROOT = Path(__file__).resolve().parents[2]
HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "research/surface-emission"))
from probe import (COMMON_FLAGS, COFF, VerificationError, candidate_artifact,
                   canonical_hash, check_lock, compare_function, digest,
                   environment, load_config, load_target, require, run_tool,
                   verify_artifacts, verify_reference)
from native_listing_diagnostic import comparable_inventory


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("compare", "behavior", "native-listing"))
    parser.add_argument("--reference", type=Path)
    parser.add_argument("--docker-report", type=Path)
    args = parser.parse_args()
    config = load_config(ROOT / "private/toolchain.json")
    lock = json.loads((ROOT / "private/toolchain.lock.json").read_bytes())
    check_lock(config, lock)
    require(config["runner"]["kind"] == "docker-wibo", "requires the locked Docker configuration")
    inputs = [HERE / name for name in ("candidate.c", "flush.h", "behavior.c", "target.json")]
    inputs += [ROOT / "src/baseline.h"]
    provenance = inputs + [Path(__file__).resolve(), ROOT / "target.json"]
    provenance += [ROOT / "research/surface-emission" / name for name in
                   ("probe.py", "native_listing_diagnostic.py")]
    provenance += [ROOT / "tools" / name for name in
                   ("build.py", "formats.py", "matching.py", "runners.py", "source_policy.py")]
    def source_snapshot():
        return {p.relative_to(ROOT).as_posix(): digest(p.read_bytes()) for p in provenance}
    snapshot = source_snapshot()
    spec = json.loads((HERE / "target.json").read_bytes())
    target = load_target(ROOT / "target.json")
    target["functions"] = [spec]
    original = None
    if args.command == "compare":
        require(args.reference is not None, "compare requires --reference")
        original = verify_reference(args.reference.read_bytes(), target)
    peer = None
    if args.command == "native-listing":
        require(sys.platform == "win32", "native listing requires Windows")
        require(args.docker_report is not None, "native listing requires --docker-report")
        peer_path = args.docker_report.resolve()
        require(peer_path.is_relative_to(ROOT / "build"), "Docker report must be in engine/build")
        peer_bytes = peer_path.read_bytes()
        peer = json.loads(peer_bytes)
        require(peer.get("kind") == "queue-flush-focused-research" and
                peer.get("command") in ("compare", "behavior") and peer.get("completed") is True,
                "requires a completed queue-flush Docker report")
        require(peer.get("accepted_history_record") is False, "research evidence only")
        require(peer["source_snapshot"] == snapshot, "Docker source snapshot is stale")
        require(peer["toolchain_lock_sha256"] == canonical_hash(lock), "Docker toolchain lock differs")
        verify_artifacts(peer_path.parent, peer["artifacts"])
        config = dict(config)
        config["runner"] = {"kind": "native"}  # Saved Docker configuration is unchanged.
    out = ROOT / "build" / ("queue-flush-" + uuid.uuid4().hex[:12])
    out.mkdir()
    for path in inputs:
        (out / path.name).write_bytes(path.read_bytes())
    # Keep the pending full extent visible and validate it with the common loader.
    (out / "pending-target.json").write_text(json.dumps(target, indent=2) + "\n")
    target = load_target(out / "pending-target.json")
    report = {"schema": 1, "kind": "queue-flush-focused-research", "command": args.command,
              "accepted_history_record": False, "whole_executable_match": False,
              "toolchain_lock_sha256": canonical_hash(lock), "source_snapshot": snapshot,
              "completed": False, "commands": [], "artifacts": [], "artifact_notices": [],
              "full_span_comparison": None, "body_diagnostic": None,
              "alignment_verified": False, "fixture": None,
              "generated_code_executed": False}
    def invoke(command):
        process, invoked = run_tool(config, command, ROOT, out, environment(config), 60)
        report["commands"].append({"argv": command, "invoked": invoked, "returncode": process.returncode})
        output = process.stdout + process.stderr
        (out / ("command-%02d.log" % len(report["commands"]))).write_text(output, encoding="utf-8")
        require(process.returncode == 0, "command failed; inspect " + str(out))
        require(not re.search(r"\bwarning\b", output, re.I), "compiler/linker warning; inspect " + str(out))
        return process
    try:
        obj = out / "flush_material_buckets.obj"
        invoke([config["compiler"], *COMMON_FLAGS, *spec["flags"], "/FAcs",
                "/Fd" + str(out / "probe.pdb"), "/Fo" + str(obj), str(out / "candidate.c")])
        artifact = candidate_artifact("candidate.c", spec, obj, out / "candidate.cod")
        report["artifacts"].append(artifact)
        if not artifact["assembly_listing_has_terminators"]:
            report["artifact_notices"].append("Compiler listing lacks terminators; complete object inventory retained")
        if original is not None:
            report["full_span_comparison"] = compare_function(original, COFF(obj.read_bytes()), spec)
            body = dict(spec)
            body["size"] = spec["body_size"]
            # This diagnostic does not shorten target.json or become accepted evidence.
            report["body_diagnostic"] = compare_function(original, COFF(obj.read_bytes()), body)
            report["reference_alignment_bytes"] = spec["size"] - spec["body_size"]
            report["reference_alignment_sha256"] = digest(original.read_va(
                spec["address"] + spec["body_size"], report["reference_alignment_bytes"]))
            print("Full span: " + report["full_span_comparison"]["status"])
            print("Body only: " + report["body_diagnostic"]["status"] + "; alignment remains unverified")
        if args.command == "behavior":
            fixture_obj = out / "behavior.obj"
            invoke([config["compiler"], *COMMON_FLAGS, "/Od", "/MT",
                    "/Fd" + str(out / "probe.pdb"), "/Fo" + str(fixture_obj), str(out / "behavior.c")])
            executable = out / "queue-flush-focused.exe"
            invoke([config["linker"], "/NOLOGO", "/MACHINE:IX86", "/SUBSYSTEM:CONSOLE",
                    "/INCREMENTAL:NO", "/OUT:" + str(executable), str(obj), str(fixture_obj),
                    "libcmt.lib", "kernel32.lib"])
            report["fixture"] = {"sha256": digest(executable.read_bytes()), "passed": False}
            report["generated_code_execution_attempted"] = True
            report["generated_code_executed"] = None
            process = invoke([str(executable)])
            report["generated_code_executed"] = True
            require(digest(executable.read_bytes()) == report["fixture"]["sha256"], "fixture changed")
            report["fixture"].update(passed=True, stdout=process.stdout)
            print(process.stdout, end="")
        if peer is not None:
            require(artifact["assembly_listing_has_terminators"], "native listing is incomplete")
            peer_artifact = peer["artifacts"][0]
            require(comparable_inventory(obj.read_bytes(), spec["symbol"]) == comparable_inventory(
                (peer_path.parent / peer_artifact["object"]).read_bytes(), spec["symbol"]),
                "native function bytes/relocations differ from Docker")
            report["docker_function_and_relocations_equal"] = True
            report["docker_report_sha256"] = digest(peer_bytes)
            require(peer_path.read_bytes() == peer_bytes, "Docker report changed")
            print("Complete native listing; function bytes and relocations agree with Docker")
        verify_artifacts(out, report["artifacts"])
        require(source_snapshot() == snapshot, "research source changed during build")
        check_lock(load_config(ROOT / "private/toolchain.json"), lock)
        report["completed"] = True
    finally:
        (out / "research-result.json").write_text(json.dumps(report, indent=2) + "\n", encoding="utf-8")
        print(out)
    if args.command == "compare":
        # The body-only diagnostic can never turn a missing-alignment result green.
        return 0 if report["full_span_comparison"]["status"] in ("raw-code-match", "relocation-adjusted-match") else 1
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (VerificationError, OSError, ValueError, KeyError) as error:
        print("FAILED:", error, file=sys.stderr)
        sys.exit(2)
