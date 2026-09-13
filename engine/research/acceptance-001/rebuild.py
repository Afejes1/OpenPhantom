"""Re-run the pinned overnight case batch without changing acceptance rules."""
import argparse
import hashlib
import json
from pathlib import Path
import re
import subprocess
import sys
from datetime import datetime, timezone
import uuid

ENGINE = Path(__file__).resolve().parents[2]
ROOT = ENGINE.parent
MATCH = "relocation-adjusted-match"


def require(condition, reason):
    if not condition:
        raise ValueError(reason)


def read(path):
    return json.loads(path.read_bytes())


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def write(path, value):
    path.write_bytes((json.dumps(value, indent=2) + "\n").encode())


def checked_path(relative):
    path = (ROOT / relative).resolve()
    require(path.is_relative_to(ENGINE / "research"), "input outside research")
    require(path.is_file(), "missing input: " + relative)
    return path


def validate_receipt(receipt, published, runner, peer_sha=None):
    require(receipt.get("completed") is True, "incomplete runner report")
    require(receipt.get("id") == published["id"], "case identity differs")
    require(receipt["source_snapshot"] == published["source_snapshot"], "source snapshot differs")
    require(receipt["toolchain_lock_sha256"] == published["toolchain_lock_sha256"], "toolchain differs")
    actual, expected = receipt["comparison"], published["comparison"]
    # Object hashes can vary with debug metadata; every other strict comparison
    # field, including complete extents, binding accounting and span hashes, must
    # reproduce the original published result.
    require(actual.get("status") == expected.get("status") == MATCH, "not an exact match")
    require(actual.get("different_bytes") == 0, "differing bytes")
    require({k: v for k, v in actual.items() if k != "object_sha256"} ==
            {k: v for k, v in expected.items() if k != "object_sha256"}, "comparison changed")
    require(actual["resolved_span_sha256"] == actual["reference_span_sha256"], "span hashes differ")
    if runner == "docker":
        require(receipt.get("peer_report_sha256") == peer_sha, "native peer differs")
        require(receipt.get("native_docker_function_and_relocations_equal") is True,
                "native/Docker function evidence differs")
        fixture = receipt.get("fixture") or {}
        require(fixture.get("passed") is True, "fixture did not pass")
        counts = re.search(r"(\d+) checks, (\d+) failures", fixture.get("stdout", ""))
        require(counts is not None, "missing fixture assertion count")
        require(int(counts[1]) == published["fixture"]["checks"] and int(counts[2]) == 0,
                "fixture assertion count or failures changed")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--reference", type=Path, required=True)
    args = parser.parse_args()
    manifest_path = Path(__file__).with_name("batch.json")
    manifest = read(manifest_path)
    cases = manifest["cases"]
    require(len(cases) == manifest["expected_functions"] == 66, "case count differs")
    require(len({c["id"] for c in cases}) == len(cases), "duplicate case")
    published = {}
    for case in cases:
        result = checked_path(case["result_path"])
        source = checked_path(case["candidate_source"])
        require(sha(result) == case["result_sha256"], "published result changed")
        require(sha(source) == case["candidate_sha256"], "published candidate changed")
        data = read(result)
        require(data["id"] == case["id"] and data["status"] == "focused-relocation-adjusted-match",
                "case is outside exact scope")
        require(data["fixture"]["passed"] is True, "published fixture did not pass")
        published[case["id"]] = data
    require(sum(d["comparison"]["size"] for d in published.values()) == manifest["expected_bytes"],
            "total original extent differs")
    out = ENGINE / "build" / ("acceptance-001-" + uuid.uuid4().hex[:12])
    out.mkdir(exist_ok=False)
    record = {"schema": 1, "kind": "focused-case-batch-reverification", "completed": False,
              "started_utc": datetime.now(timezone.utc).isoformat(),
              "manifest_sha256": sha(manifest_path), "runner_sha256": sha(Path(__file__)),
              "source_revision": subprocess.check_output(["git", "rev-parse", "HEAD"], cwd=ROOT, text=True).strip(),
              "whole_executable_match": False, "registered_history_promotions": 0, "cases": []}
    print("Batch receipt:", out / "batch-result.json", flush=True)
    try:
        for index, case in enumerate(cases, 1):
            row = {"id": case["id"], "case": case["case"], "reports": {}}
            record["cases"].append(row)
            native_path = None
            for runner in ("native", "docker"):
                command = [sys.executable, str(ENGINE / "research/campaign-001/verify.py"),
                           "--case", str(ENGINE / case["case"]), "--reference", str(args.reference.resolve()),
                           "--runner", runner]
                if runner == "docker":
                    command += ["--behavior", "--expect-report", str(native_path), "--native-listing-fallback"]
                process = subprocess.run(command, cwd=ROOT, text=True, capture_output=True, timeout=180)
                (out / (case["id"] + "-" + runner + ".log")).write_bytes((process.stdout + process.stderr).encode())
                require(process.returncode == 0, "runner failed: " + case["id"] + "/" + runner)
                response = json.loads(process.stdout.strip())
                path = Path(response["report"]).resolve()
                require(path.is_relative_to(ENGINE / "build"), "report escaped private build")
                receipt = read(path)
                validate_receipt(receipt, published[case["id"]], runner,
                                 sha(native_path) if native_path is not None else None)
                row["reports"][runner] = {"path": path.relative_to(ROOT).as_posix(), "sha256": sha(path),
                                           "elapsed_seconds": receipt["elapsed_seconds"]}
                if runner == "native":
                    native_path = path
            row.update(passed=True, size=published[case["id"]]["comparison"]["size"],
                       fixture_checks=published[case["id"]]["fixture"]["checks"])
            write(out / "batch-result.json", record)
            print("%d/%d %s: full match and %d focused checks" %
                  (index, len(cases), case["id"], row["fixture_checks"]), flush=True)
        record.update(completed=True, exact_functions=len(cases),
                      exact_bytes=sum(r["size"] for r in record["cases"]),
                      fixture_checks=sum(r["fixture_checks"] for r in record["cases"]))
    except (ValueError, OSError, KeyError, subprocess.SubprocessError) as error:
        record["error"] = str(error)
        raise
    finally:
        record["finished_utc"] = datetime.now(timezone.utc).isoformat()
        write(out / "batch-result.json", record)
    print("PASS:", out / "batch-result.json", flush=True)


if __name__ == "__main__":
    main()
