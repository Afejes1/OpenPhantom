#!/usr/bin/env python3
"""Regenerate the scheduling view without changing verification evidence."""
import argparse
import json
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError, require
from matching import load_target
from registry import read_history, validate_current_specs
from team_manifest import render_team_progress


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true")
    args = parser.parse_args()
    target = load_target(ROOT / "target.json")
    history = read_history(ROOT / "evidence/runs")
    validate_current_specs(history, target)
    doc = json.loads((ROOT / "planning/team-manifest-20260912.json").read_text(encoding="utf-8"))
    order = json.loads((ROOT / "planning/work-order.json").read_text(encoding="utf-8"))
    report = render_team_progress(doc, order, target, history)
    path = ROOT / "docs/team-work-order.md"
    if args.check:
        require(path.is_file() and path.read_text(encoding="utf-8") == report, "team work-order report is stale")
    else:
        path.write_text(report, encoding="utf-8", newline="\n")
    print("Team manifest: %d reported done, %d reported not done; local evidence remains separate." %
          (doc["reported_counts"]["done"], doc["reported_counts"]["not_done"]))


if __name__ == "__main__":
    try:
        main()
    except (VerificationError, OSError, KeyError, TypeError, ValueError) as error:
        print("BLOCKED:", error, file=sys.stderr)
        sys.exit(2)
