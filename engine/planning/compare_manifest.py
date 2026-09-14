"""Join a contributor snapshot to one immutable local acceptance event by address.

This reports historical evidence at the selected event. It grants no acceptance
and makes no claim about the contributor's unverified source or a whole binary.
"""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError, require
from registry import read_history
from team_manifest import REFERENCE_SHA256, validate_manifest


def compare(manifest, event):
    validate_manifest(manifest)
    require(event["target_sha256"] == REFERENCE_SHA256, "different retail target")
    rows = {row["address"]: row for row in manifest["functions"]}
    local = {}
    for function in event["functions"]:
        result = function["result"]
        require(not function["exceptions"] and
                result["status"] in ("raw-code-match", "relocation-adjusted-match") and
                result["different_bytes"] == 0 and
                result["size"] == result["candidate_size"], "event contains a non-exact function")
        address = function["spec"]["address"]
        require(address not in local and address in rows, "duplicate or unlisted local address")
        local[address] = function
    covered_open = sum(rows[address]["reported_status"] == "not_done" for address in local)
    counts = dict(manifest_functions=len(rows), team_reported_done=manifest["reported_counts"]["done"],
                  team_reported_not_done=manifest["reported_counts"]["not_done"],
                  local_accepted=len(local), local_within_team_not_done=covered_open,
                  local_overlapping_team_done=len(local) - covered_open,
                  remaining_if_team_claims_are_correct=manifest["reported_counts"]["not_done"] - covered_open)
    counts["combined_covered_if_team_claims_are_correct"] = counts["team_reported_done"] + covered_open
    require(counts["combined_covered_if_team_claims_are_correct"] +
            counts["remaining_if_team_claims_are_correct"] == len(rows), "partition mismatch")
    functions = []
    for address, row in sorted(rows.items()):
        function = local.get(address)
        functions.append(dict(address="0x%08X" % address, team_reported_file=row["reported_file"],
                              team_reported_name=row["reported_name"], team_reported_status=row["reported_status"],
                              local_accepted_at_event=function is not None,
                              local_id=function["id"] if function else None,
                              local_source=function["spec"]["source"] if function else None,
                              local_match_status=function["result"]["status"] if function else None,
                              remaining=row["reported_status"] == "not_done" and function is None))
    return dict(schema=1, counts=counts, local_match_categories=dict(Counter(
                function["result"]["status"] for function in local.values())),
                team_source_independently_verified=False, whole_executable_match=False,
                scope="Application addresses in this manifest; CRT and any omitted functions are outside its denominator.",
                functions=functions)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--manifest", type=Path, required=True)
    parser.add_argument("--event", type=Path, required=True)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    history = read_history(ROOT / "evidence/runs")
    event = json.loads(args.event.read_bytes())
    require(event in history, "event is not in the validated immutable history")
    manifest = json.loads(args.manifest.read_bytes())
    report = compare(manifest, event)
    sha = lambda path: hashlib.sha256(path.read_bytes()).hexdigest()
    report["provenance"] = dict(manifest=args.manifest.as_posix(), manifest_sha256=sha(args.manifest),
                                contributor_attachment_sha256=manifest["source_sha256"],
                                accepted_event=args.event.as_posix(), accepted_event_sha256=sha(args.event),
                                source_commit=event["source_revision"], build_id=event["build_id"],
                                generator_sha256=sha(Path(__file__)))
    args.output.write_bytes((json.dumps(report, indent=2) + "\n").encode("ascii"))
    print(json.dumps(report["counts"]))


if __name__ == "__main__":
    try:
        main()
    except (VerificationError, OSError, KeyError, TypeError, ValueError) as error:
        print("BLOCKED:", error, file=sys.stderr)
        sys.exit(2)
