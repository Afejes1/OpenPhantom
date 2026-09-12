#!/usr/bin/env python3
"""Native VC5 build, private comparison, and regression entry point."""
import argparse
import importlib.metadata
import json
import os
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
from build import (check_lock, configure, environment, load_config, run_build,
                   validate_build)
from formats import VerificationError, read_binary, require
from matching import (canonical_hash, check_regression, compare_batch,
                      compare_whole, digest, load_target, verify_reference)


def private_path(path):
    path = Path(path).resolve()
    require(any(path.is_relative_to(ROOT / folder) for folder in ("build", "private")),
            "generated reports and configuration must stay under engine/build or engine/private")
    return path


def write_json(path, value):
    path = private_path(path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(value, indent=2) + "\n", encoding="utf-8")


def prerequisites(args, target):
    require(args.reference, "supply --reference with your retail WMAIN.EXE")
    original_data = read_binary(args.reference)
    original = verify_reference(original_data, target)
    config = load_config(args.config)
    lock = json.loads(Path(args.lock).read_text(encoding="utf-8"))
    check_lock(config, lock)
    return original_data, original, config, lock


def behavior_record(out, record):
    path = out / "behavior.json"
    if not path.exists():
        return {"passed": False, "reason": "original-toolchain behavioral tests not run"}
    value = json.loads(path.read_text(encoding="utf-8"))
    require(value["build_sha256"] == canonical_hash(record), "stale behavioral result")
    return value


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=["reference", "configure", "preflight", "build", "test", "compare", "check", "accept", "whole", "diagnose"])
    parser.add_argument("--reference", type=Path)
    parser.add_argument("--config", type=Path, default=ROOT / "private/toolchain.json")
    parser.add_argument("--lock", type=Path, default=ROOT / "private/toolchain.lock.json")
    parser.add_argument("--build", type=Path)
    parser.add_argument("--baseline", type=Path)
    args = parser.parse_args(argv)
    target = load_target(ROOT / "target.json")
    if args.command == "reference":
        require(args.reference, "supply --reference")
        verify_reference(read_binary(args.reference), target)
        print("Retail identity, function extents, constants and relocation operands verified.")
        return 0
    if args.command == "configure":
        require(not args.lock.exists(), "lock already exists; review changes and choose a new lock filename")
        private_path(args.lock)
        lock = configure(load_config(args.config))
        write_json(args.lock, lock)
        print("Toolchain fingerprint recorded; codegen calibration remains unverified.")
        return 0
    original_data, original, config, lock = prerequisites(args, target)
    if args.command == "preflight":
        print("Retail identity and original-toolchain fingerprint verified; calibration still requires compare/check.")
        return 0
    if args.command == "build":
        print(run_build(ROOT, target, config, lock))
        return 0
    require(args.build, "supply --build with a completed build directory")
    out = private_path(args.build)
    record = validate_build(ROOT, out, target, lock)
    if args.command == "test":
        require(os.name == "nt", "native Windows test runner required")
        # Only the hashed, newly built fixture executable is ever executed here.
        process = subprocess.run([str(out / "WMAIN.EXE")], cwd=out, env=environment(config),
                                 capture_output=True, text=True, errors="replace", timeout=30, check=False)
        validate_build(ROOT, out, target, lock)
        result = {"passed": process.returncode == 0, "returncode": process.returncode,
                  "build_sha256": canonical_hash(record), "stdout": process.stdout, "stderr": process.stderr}
        write_json(out / "behavior.json", result)
        print(process.stdout)
        return 0 if result["passed"] else 1
    if args.command == "whole":
        result = compare_whole(original_data, read_binary(out / "WMAIN.EXE"))
        result["build_sha256"] = canonical_hash(record)
        write_json(out / "whole.json", result)
        print(json.dumps(result, indent=2))
        return 0 if result["whole_executable_match"] else 1
    if args.command == "diagnose":
        require(importlib.metadata.version("reccmp") == "0.1.7", "install pinned reccmp==0.1.7")
        # User/build files are ignored. JSON string literals are valid YAML scalars.
        user = "targets:\n  WMAIN:\n    path: " + json.dumps(str(args.reference.resolve())) + "\n"
        (ROOT / "reccmp-user.yml").write_text(user, encoding="utf-8")
        build = ("project: " + json.dumps(str(ROOT)) + "\ntargets:\n  WMAIN:\n    path: " + json.dumps(str(out / "WMAIN.EXE"))
                 + "\n    pdb: " + json.dumps(str(out / "WMAIN.PDB")) + "\n")
        (out / "reccmp-build.yml").write_text(build, encoding="utf-8")
        executable = Path(sys.executable).parent / ("reccmp-reccmp.exe" if os.name == "nt" else "reccmp-reccmp")
        require(executable.is_file(), "reccmp entry point missing from this Python environment")
        return subprocess.run([str(executable), "--target", "WMAIN", "--html", str(out / "diagnostic.html")],
                              cwd=out, timeout=120, check=False).returncode
    objects = {s["id"]: read_binary(out / (s["id"] + ".obj")) for s in target["functions"]}
    result = compare_batch(original, objects, target)
    result["build_sha256"] = canonical_hash(record)
    result["toolchain_lock_sha256"] = canonical_hash(lock)
    result["code_complete"] = result["complete"]
    result["behavior"] = behavior_record(out, record)
    result["complete"] = result["code_complete"] and result["behavior"]["passed"]
    write_json(out / "comparison.json", result)
    for function in result["functions"]:
        print("%s: %s%s" % (function["id"], function["status"],
                            "; " + function["reason"] if "reason" in function else ""))
    print("VC5 behavioral tests:", "passed" if result["behavior"]["passed"] else "not passed")
    if args.command in ("check", "accept"):
        require(args.baseline, "supply --baseline (use a new private filename for accept)")
        if args.command == "check":
            baseline = json.loads(args.baseline.read_text(encoding="utf-8"))
            require(result["toolchain_lock_sha256"] == baseline["toolchain_lock_sha256"], "baseline toolchain changed")
            check_regression(result, baseline)
        else:
            require(result["complete"], "cannot accept an incomplete or failing run")
            require(not args.baseline.exists(), "baseline already exists; choose a new filename")
            write_json(args.baseline, result)
    return 0 if result["complete"] else 1


if __name__ == "__main__":
    try:
        sys.exit(main())
    except (VerificationError, OSError, KeyError, TypeError, ValueError,
            subprocess.SubprocessError, importlib.metadata.PackageNotFoundError) as error:
        print("BLOCKED:", str(error), file=sys.stderr)
        sys.exit(2)
