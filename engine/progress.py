#!/usr/bin/env python3
"""Validate public reconstruction history and regenerate its readable report."""
import argparse
from pathlib import Path
import sys

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError, require
from matching import load_target
from registry import load_registry, read_history, render_progress


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true", help="fail if the committed report needs regeneration")
    args = parser.parse_args()
    target = load_target(ROOT / "target.json")
    registry = load_registry(ROOT, target)
    history = read_history(ROOT / "evidence/runs")
    report = render_progress(ROOT, target, registry, history)
    path = ROOT / "docs/progress.md"
    if args.check:
        require(path.is_file() and path.read_text(encoding="utf-8") == report, "progress report is stale; run python engine/progress.py")
    else:
        path.write_text(report, encoding="utf-8", newline="\n")
    print("Registry and %d chained verification records validated." % len(history))


if __name__ == "__main__":
    try:
        main()
    except (VerificationError, OSError, KeyError, TypeError, ValueError) as error:
        print("BLOCKED:", error, file=sys.stderr)
        sys.exit(2)
