"""Optional Windows-only VC5 listing diagnostic; never executes generated code."""
import argparse
import json
import os
from pathlib import Path
import re
import sys
import uuid

from probe import (CASES, HERE, ROOT, COMMON_FLAGS, check_lock, environment, load_config,
                   candidate_artifact, function_inventory, verify_artifacts, run_tool,
                   digest, canonical_hash, require, VerificationError)


def comparable_inventory(data, symbol):
    inventory = function_inventory(data, symbol)
    # Debug metadata, host paths and COFF timestamps are not function contents.
    return {key: inventory[key] for key in
            ("function_section_bytes", "function_section_sha256", "rows", "relocations")}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--docker-report", required=True, type=Path)
    args = parser.parse_args()
    require(os.name == "nt", "native listing diagnostic requires Windows")
    report_path = args.docker_report.resolve()
    require(report_path.is_relative_to(ROOT / "build"), "report must be inside private engine/build")
    docker_report_bytes = report_path.read_bytes()
    docker_report = json.loads(docker_report_bytes)
    require(docker_report.get("schema") == 3 and docker_report.get("completed") is True,
            "a completed schema-3 Docker report is required")
    require(docker_report.get("accepted_history_record") is False, "research reports only")
    config = load_config(ROOT / "private/toolchain.json")
    lock = json.loads((ROOT / "private/toolchain.lock.json").read_bytes())
    check_lock(config, lock)
    require(docker_report["toolchain_lock_sha256"] == canonical_hash(lock), "Docker report used a different toolchain lock")
    require(config["runner"]["kind"] == "docker-wibo", "requires the existing locked Docker configuration")
    verify_artifacts(report_path.parent, docker_report["artifacts"])
    def check_sources():
        for name, expected in docker_report["source_snapshot"].items():
            path = (ROOT / name).resolve()
            require(path.is_relative_to(ROOT), "source outside engine")
            require(digest(path.read_bytes()) == expected, "Docker report source changed: " + name)
    check_sources()
    out = ROOT / "build" / ("native-listings-" + uuid.uuid4().hex[:12])
    out.mkdir()
    for name in ("batch_surface.h", "collection.h", "culling.h", "legacy_collection.h"):
        (out / name).write_bytes((HERE / name).read_bytes())
    (out / "baseline.h").write_bytes((ROOT / "src/baseline.h").read_bytes())
    record = {"schema": 1, "kind": "native-listing-diagnostic",
              "accepted_history_record": False, "whole_executable_match": False,
              "generated_code_executed": False, "completed": False,
              "docker_report_sha256": digest(docker_report_bytes),
              "toolchain_lock_sha256": docker_report["toolchain_lock_sha256"],
              "diagnostic_script_sha256": digest(Path(__file__).read_bytes()),
              "source_snapshot": docker_report["source_snapshot"], "commands": [], "artifacts": []}
    native = dict(config)
    native["runner"] = {"kind": "native"}  # The saved configuration/lock is never edited.
    expected_artifacts = {artifact["id"]: artifact for artifact in docker_report["artifacts"]}
    try:
        for source, target in CASES:
            spec = json.loads((HERE / target).read_bytes())
            expected = expected_artifacts[spec["id"]]
            require(Path(expected["object"]).name == expected["object"], "invalid Docker object path")
            src = out / source
            src.write_bytes((HERE / source).read_bytes())
            obj = out / (spec["id"] + ".obj")
            command = [native["compiler"], *COMMON_FLAGS, *spec["flags"], "/FAcs",
                       "/Fd" + str(out / "probe.pdb"), "/Fo" + str(obj), str(src)]
            process, invoked = run_tool(native, command, ROOT, out, environment(native), 60)
            record["commands"].append({"argv": invoked, "returncode": process.returncode})
            output = process.stdout + process.stderr
            (out / (spec["id"] + ".log")).write_text(output, encoding="utf-8")
            require(process.returncode == 0 and not re.search(r"\bwarning\b", output, re.I),
                    "native compiler failed or warned; inspect " + str(out))
            artifact = candidate_artifact(source, spec, obj, out / Path(source).with_suffix(".cod"))
            record["artifacts"].append(artifact)
            require(artifact["assembly_listing_has_terminators"], "native listing is incomplete")
            original = (report_path.parent / expected["object"]).read_bytes()
            require(comparable_inventory(original, spec["symbol"]) ==
                    comparable_inventory(obj.read_bytes(), spec["symbol"]),
                    "native function bytes or relocations differ from Docker")
            artifact["docker_function_and_relocations_equal"] = True
            print(spec["id"] + ": complete listing; function bytes and relocations agree with Docker")
        check_sources()
        check_lock(config, lock)
        verify_artifacts(out, record["artifacts"])
        verify_artifacts(report_path.parent, docker_report["artifacts"])
        require(report_path.read_bytes() == docker_report_bytes, "Docker report changed")
        require(digest(Path(__file__).read_bytes()) == record["diagnostic_script_sha256"], "diagnostic script changed")
        record["completed"] = True
    finally:
        (out / "native-listing-result.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
        print(out)
    return 0


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (VerificationError, OSError, ValueError, KeyError) as error:
        print("FAILED: " + str(error), file=sys.stderr)
        sys.exit(2)
