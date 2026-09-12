"""Private, fingerprinted recovery bundles; never overwrite an existing checkout."""
import hashlib
import json
from pathlib import Path, PurePosixPath
import re
import shutil
import subprocess

from build import check_lock, load_config, tool_files
from formats import require
from matching import canonical_hash
from registry import portable_toolchain

MAX_FILES = 4096
MAX_BYTES = 2 * 1024 ** 3


def file_hash(path):
    with Path(path).open("rb") as stream:
        return hashlib.file_digest(stream, "sha256").hexdigest()


def safe_name(name):
    require(isinstance(name, str) and name and "\\" not in name, "invalid bundle path")
    path = PurePosixPath(name)
    require(not path.is_absolute() and path.as_posix() == name, "noncanonical bundle path")
    for part in path.parts:
        require(part not in (".", "..") and not part.endswith((" ", ".")), "unsafe bundle component")
        require(not any(c in part for c in ':<>|"*?') and all(ord(c) >= 32 for c in part), "invalid Windows filename")
        require(part.split(".")[0].upper() not in {"CON", "PRN", "AUX", "NUL", *("COM%d" % n for n in range(10)), *("LPT%d" % n for n in range(10))},
                "reserved Windows filename")
    return path


def regular_tree(folder):
    paths = {}
    folded = set()
    require(folder.is_dir() and not folder.is_symlink() and not folder.is_junction(), "bundle is not a regular directory")
    for path in folder.rglob("*"):
        require(not path.is_symlink() and not path.is_junction(), "links are not allowed in a recovery tree")
        if path.is_file():
            name = path.relative_to(folder).as_posix()
            safe_name(name)
            require(name.casefold() not in folded, "case-colliding bundle paths")
            folded.add(name.casefold())
            paths[name] = path
        else:
            require(path.is_dir(), "unsupported filesystem entry")
    return paths


def new_private_directory(root, destination):
    root = root.resolve()
    destination = Path(destination)
    # Check existing parents before resolve, which could otherwise hide a junction.
    for parent in (destination, *destination.parents):
        require(not parent.is_symlink() and not parent.is_junction(), "restore destination contains a link")
    destination = destination.resolve()
    require(destination.is_relative_to(root / "private") and destination != root / "private",
            "recovery output must be a new directory beneath engine/private")
    require(not destination.exists(), "recovery destination already exists; choose a new name")
    destination.mkdir(parents=True, exist_ok=False)
    return destination


def git(root, *args):
    return subprocess.check_output(["git", *args], cwd=root, text=True, stderr=subprocess.STDOUT).strip()


def pack(root, config_path, lock_path, destination):
    config = load_config(config_path)
    lock = json.loads(Path(lock_path).read_text(encoding="utf-8"))
    check_lock(config, lock)
    require(not git(root.parent, "status", "--porcelain"), "commit the checkout before taking a recovery checkpoint")
    portable = portable_toolchain(config, lock)
    revision = git(root.parent, "rev-parse", "HEAD")
    destination = new_private_directory(root, destination)
    base = Path(config["runner"]["toolchain_root"]).resolve()
    tree = regular_tree(base)
    # Copy precisely the fingerprinted tool inventory. No private game/reference files.
    for name, sha in portable["files"].items():
        require(name in tree and file_hash(tree[name]) == sha, "tool changed while archiving")
        out = destination / "toolchain" / name
        out.parent.mkdir(parents=True, exist_ok=True)
        with tree[name].open("rb") as source, out.open("xb") as target:
            shutil.copyfileobj(source, target)
        require(file_hash(out) == sha, "archived tool hash differs")
    git(root.parent, "bundle", "create", str(destination / "source.bundle"), "HEAD")
    require(git(root.parent, "rev-parse", "HEAD") == revision and not git(root.parent, "status", "--porcelain"),
            "source changed while archiving")
    subprocess.run([config["runner"]["docker"], "image", "save", "--output", str(destination / "runtime-image.tar"),
                    config["runner"]["image"]], check=True, timeout=180)
    check_lock(config, lock)
    files = {name: {"sha256": file_hash(path), "size": path.stat().st_size}
             for name, path in regular_tree(destination).items()}
    manifest = {"schema": 1, "kind": "private-openphantom-recovery", "source_revision": revision,
                "toolchain": portable, "provenance": config["provenance"], "files": files,
                "reference_included": False, "host_prerequisites": ["Windows", "Python 3.13+", "Git", "Docker Linux engine"]}
    with (destination / "manifest.json").open("x", encoding="utf-8", newline="\n") as stream:
        stream.write(json.dumps(manifest, indent=2) + "\n")
    sha = file_hash(destination / "manifest.json")
    inspect_bundle(destination, sha)
    return destination, sha


def inspect_bundle(folder, expected_sha):
    folder = Path(folder).resolve()
    require(re.fullmatch("[0-9a-f]{64}", expected_sha or ""), "supply the previously retained manifest SHA-256")
    require(file_hash(folder / "manifest.json") == expected_sha, "recovery manifest checksum differs")
    value = json.loads((folder / "manifest.json").read_text(encoding="utf-8"))
    require(value.get("schema") == 1 and value.get("kind") == "private-openphantom-recovery", "invalid recovery manifest")
    require(value.get("reference_included") is False, "unexpected game-file bundle")
    require(re.fullmatch("[0-9a-f]{40}", value["source_revision"]), "invalid source revision")
    require(value["toolchain"]["runtime"]["kind"] == "docker-wibo", "unexpected runtime")
    require(re.fullmatch("sha256:[0-9a-f]{64}", value["toolchain"]["runtime"]["image"]), "invalid image ID")
    inventory = value["files"]
    require(0 < len(inventory) <= MAX_FILES, "invalid recovery inventory size")
    require(sum(f["size"] for f in inventory.values()) <= MAX_BYTES, "recovery bundle too large")
    paths = regular_tree(folder)
    require(set(paths) == set(inventory) | {"manifest.json"}, "bundle inventory differs")
    require(set(inventory) == {"source.bundle", "runtime-image.tar"} | {"toolchain/" + n for n in value["toolchain"]["files"]},
            "unexpected recovery payload")
    for name, entry in inventory.items():
        safe_name(name)
        require(0 <= entry["size"] <= MAX_BYTES and paths[name].stat().st_size == entry["size"], "recovery file size differs")
        require(file_hash(paths[name]) == entry["sha256"], "recovery file checksum differs: " + name)
    for name, sha in value["toolchain"]["files"].items():
        require(inventory["toolchain/" + name]["sha256"] == sha, "tool fingerprint differs")
    for key, items in value["toolchain"]["paths"].items():
        for name in items if isinstance(items, list) else [items]:
            safe_name(name)
    return value


def restore(root, folder, expected_sha, destination, docker):
    folder = Path(folder).resolve()
    manifest = inspect_bundle(folder, expected_sha)
    docker = Path(docker).resolve()
    require(docker.is_file(), "supply the installed Docker CLI")
    # Verify bundle structure and advertised commit before importing anything.
    git(root.parent, "bundle", "verify", str(folder / "source.bundle"))
    require(git(root.parent, "bundle", "list-heads", str(folder / "source.bundle"), "HEAD").split()[0] == manifest["source_revision"],
            "source bundle does not name the pinned revision")
    destination = new_private_directory(root, destination)
    repository = destination / "repository"
    git(root.parent, "clone", "--no-checkout", str(folder / "source.bundle"), str(repository))
    git(repository, "checkout", "--detach", manifest["source_revision"])
    engine = repository / "engine"
    toolroot = engine / "private" / "restored-toolchain"
    toolroot.mkdir(parents=True)
    for name, sha in manifest["toolchain"]["files"].items():
        source = folder / "toolchain" / name
        target = toolroot / name
        target.parent.mkdir(parents=True, exist_ok=True)
        with source.open("rb") as stream, target.open("xb") as out:
            shutil.copyfileobj(stream, out)
        require(file_hash(target) == sha, "restored tool hash differs")
    subprocess.run([str(docker), "image", "load", "--input", str(folder / "runtime-image.tar")], check=True, timeout=180)
    config = {"schema": 1, "provenance": manifest["provenance"],
              "runner": {"kind": "docker-wibo", "docker": str(docker), "toolchain_root": str(toolroot),
                         "image": manifest["toolchain"]["runtime"]["image"]}}
    for key, value in manifest["toolchain"]["paths"].items():
        config[key] = [str(toolroot / p) for p in value] if isinstance(value, list) else str(toolroot / value)
    config_path = engine / "private" / "toolchain.json"
    with config_path.open("x", encoding="utf-8", newline="\n") as stream:
        stream.write(json.dumps(config, indent=2) + "\n")
    config = load_config(config_path)
    from runners import runtime_identity
    require(runtime_identity(config) == manifest["toolchain"]["runtime"], "restored runtime differs")
    restored_files = {Path(p).relative_to(toolroot).as_posix(): sha for p, sha in tool_files(config).items()}
    require(restored_files == manifest["toolchain"]["files"], "restored inventory differs")
    receipt = {"schema": 1, "manifest_sha256": expected_sha, "source_revision": manifest["source_revision"],
               "tool_inventory_verified": True, "runtime_verified": True, "rebuild_verified": False,
               "next_step": "Configure a new lock, then build/test/record using your separately supplied retail reference"}
    (destination / "restore-receipt.json").write_text(json.dumps(receipt, indent=2) + "\n", encoding="utf-8")
    return engine
