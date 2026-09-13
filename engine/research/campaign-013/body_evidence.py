"""Read-only reproduction of body evidence; full-span mismatches stay authoritative.

Usage: python engine/research/campaign-013/body_evidence.py --result CASE/result.json
       --reference PATH/TO/OWNED/WMAIN.EXE
Run each case through campaign-001/verify.py with native and Docker first.
Original bytes and compiler objects remain private and are never emitted here.
"""
import argparse
import hashlib
import json
from pathlib import Path
import sys

ENGINE = Path(__file__).resolve().parents[2]
ROOT = ENGINE.parent
sys.path.insert(0, str(ENGINE / "tools"))
from formats import COFF, require
from matching import MATCHES, compare_function, load_target, verify_reference


def read(path):
    return json.loads(path.read_bytes())


def sha(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def extract(result_path, reference_path, native_report=None, docker_report=None):
    result = read(result_path)
    require((native_report is None) == (docker_report is None), "supply both fresh runner reports")
    if native_report is not None:
        for runner, report_path in (("native", native_report), ("docker", docker_report)):
            report_path = report_path.resolve()
            result[runner + "_report"] = str(report_path)
            result[runner + "_report_sha256"] = sha(report_path)
    target_path = result_path.parent / "target.json"
    target = read(target_path)
    target_key = target_path.resolve().relative_to(ENGINE).as_posix()
    require(result["source_snapshot"].get(target_key) == sha(target_path), "target snapshot differs")
    require(result["id"] == target["id"], "target identity differs")
    body_size = target["body_size"]
    require(0 < body_size < target["size"], "no separately recorded trailing extent")
    identity = load_target(ENGINE / "target.json")
    identity["functions"] = [target]
    original = verify_reference(reference_path.read_bytes(), identity)
    require(all(b["offset"] + 4 <= body_size for b in target["bindings"]),
            "operand outside recorded body")
    tail = original.read_va(target["address"] + body_size, target["size"] - body_size)
    require(set(tail) == {0xcc}, "expected original INT3 padding")
    for relative, digest in result["source_snapshot"].items():
        require(sha(ENGINE / relative) == digest, "source snapshot changed")
    comparisons = {}
    snapshots = []
    for runner in ("native", "docker"):
        report_path = ROOT / result[runner + "_report"]
        require(sha(report_path) == result[runner + "_report_sha256"], "report hash changed")
        report = read(report_path)
        require(report["completed"] and report["runner"] == runner, "incomplete runner receipt")
        require(report["source_snapshot"] == result["source_snapshot"], "runner source differs")
        require(report["toolchain_lock_sha256"] == result["toolchain_lock_sha256"], "toolchain differs")
        require(not report["byte_exclusions"] and not report["warning_waivers"], "exclusions present")
        if runner == "docker":
            require(report["fixture"]["passed"], "focused fixture failed")
        artifact = report["artifacts"][0]
        object_path = report_path.parent / artifact["object"]
        require(sha(object_path) == artifact["object_sha256"], "object hash changed")
        obj = COFF(object_path.read_bytes())
        full = compare_function(original, obj, target)
        require(full["status"] == "mismatch" and full["candidate_size"] == body_size
                and full["size"] == target["size"], "full-span mismatch not reproduced")
        body = compare_function(original, obj, dict(target, size=body_size))
        require(body["status"] in MATCHES and body["different_bytes"] == 0,
                "recorded body does not match")
        require(body["reference_span_sha256"] == body["resolved_span_sha256"], "body hashes differ")
        comparisons[runner] = body
        snapshots.append(body["resolved_span_sha256"])
    require(snapshots[0] == snapshots[1], "native and Docker body disagree")
    return dict(status="body-exact-layout-pending", body_size=body_size,
                full_size=target["size"], unresolved_trailing_bytes=len(tail),
                trailing_kind="original INT3 inter-function padding",
                full_target_sha256=sha(target_path), native=comparisons["native"],
                docker=comparisons["docker"], full_function_exact=False,
                tool="research/campaign-013/body_evidence.py", tool_sha256=sha(Path(__file__)),
                method="Both complete original spans and all operands are verified first; their extent mismatches remain. The existing strict comparer then checks the pre-recorded body extent against each preserved object. No target mutation, new compilation, inserted padding or byte exclusions.")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--result", type=Path, required=True)
    parser.add_argument("--reference", type=Path, required=True)
    parser.add_argument("--native-report", type=Path)
    parser.add_argument("--docker-report", type=Path)
    args = parser.parse_args()
    print(json.dumps(extract(args.result.resolve(), args.reference.resolve(),
                             args.native_report, args.docker_report), indent=2))
