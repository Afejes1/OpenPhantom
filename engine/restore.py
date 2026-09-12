#!/usr/bin/env python3
"""Pack, inspect, or restore a private original-toolchain checkpoint."""
import argparse
from pathlib import Path
import subprocess
import sys

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError, require
from recovery import inspect_bundle, pack, restore


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("pack", "inspect", "restore"))
    parser.add_argument("--config", type=Path, default=ROOT / "private/toolchain.json")
    parser.add_argument("--lock", type=Path, default=ROOT / "private/toolchain.lock.json")
    parser.add_argument("--destination", type=Path)
    parser.add_argument("--bundle", type=Path)
    parser.add_argument("--manifest-sha256")
    parser.add_argument("--docker", type=Path)
    args = parser.parse_args()
    if args.command == "pack":
        require(args.destination, "supply a new private destination")
        folder, sha = pack(ROOT, args.config, args.lock, args.destination)
        print(folder)
        print("Retain this manifest SHA-256 separately:", sha)
    else:
        require(args.bundle and args.manifest_sha256, "supply the bundle and its separately retained manifest SHA-256")
        if args.command == "inspect":
            manifest = inspect_bundle(args.bundle, args.manifest_sha256)
            print("Recovery inventory verified; source revision:", manifest["source_revision"])
        else:
            require(args.destination and args.docker, "supply a new private destination and installed Docker CLI")
            print(restore(ROOT, args.bundle, args.manifest_sha256, args.destination, args.docker))


if __name__ == "__main__":
    try:
        main()
    except (VerificationError, OSError, KeyError, TypeError, ValueError, subprocess.SubprocessError) as error:
        print("BLOCKED:", error, file=sys.stderr)
        sys.exit(2)
