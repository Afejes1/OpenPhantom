"""Explicit VC5 invocation through a locked native or Docker runner."""
import json
import os
import re
from pathlib import Path
import subprocess
import sys
import time
import tempfile
import uuid

from formats import VerificationError, read_binary, require
from matching import canonical_hash, digest
from runners import run_tool, runtime_identity, validate_runner


COMMON_FLAGS = ["/nologo", "/c", "/W4", "/WX", "/Zi"]


def load_config(path):
    config = json.loads(Path(path).read_text(encoding="utf-8"))
    require(config.get("schema") == 1, "unsupported toolchain configuration")
    require(config.get("provenance", "").strip(), "document toolchain acquisition provenance")
    for name in ("compiler", "linker"):
        value = Path(config[name])
        require(value.is_absolute() and value.is_file(), "missing explicit " + name + " path")
    for name in ("bin_dirs", "include_dirs", "lib_dirs"):
        require(config[name], "missing " + name)
        for value in config[name]:
            require(Path(value).is_absolute() and Path(value).is_dir(), "missing directory in " + name)
    for name in ("compiler", "linker"):
        require(Path(config[name]).resolve().parent in [Path(p).resolve() for p in config["bin_dirs"]],
                name + " must be in a fingerprinted bin directory")
    validate_runner(config)
    return config


def environment(config):
    require(os.name == "nt", "this build profile requires native Windows")
    system = os.environ.get("SystemRoot", r"C:\Windows")
    # Deliberate allow-list: CL, _CL_, LINK, user PATH and SDK variables do not leak in.
    env = {key: os.environ[key] for key in ("SystemRoot", "WINDIR", "TEMP", "TMP", "COMSPEC")
           if key in os.environ}
    env.update(PATH=os.pathsep.join(config["bin_dirs"] + [str(Path(system) / "System32"), system]),
               INCLUDE=os.pathsep.join(config["include_dirs"]), LIB=os.pathsep.join(config["lib_dirs"]))
    return env


def tool_files(config):
    files = {}
    for group in ("bin_dirs", "include_dirs", "lib_dirs"):
        for folder in config[group]:
            for path in sorted(Path(folder).rglob("*")):
                if path.is_file():
                    if group == "bin_dirs" and path.suffix.lower() not in (".exe", ".dll"):
                        continue
                    files[str(path.resolve())] = digest(read_binary(path))
    require(files, "empty toolchain inventory")
    require(any(Path(p).name.lower() == "libcmt.lib" for p in files), "LIBCMT.LIB is missing")
    # VC5 RTM uses a DLL C front end and an executable code generator.
    for name in ("c1.dll", "c2.exe"):
        require(any(Path(p).name.lower() == name for p in files), "missing compiler backend " + name)
    return files


def configure(config):
    env = environment(config)
    banners = {}
    root = Path(__file__).resolve().parents[1]
    (root / "build").mkdir(exist_ok=True)
    runtime = runtime_identity(config)
    with tempfile.TemporaryDirectory(prefix="probe-", dir=root / "build") as directory:
        for name in ("compiler", "linker"):
            process, _ = run_tool(config, [config[name]], root, Path(directory), env, 30)
            banners[name] = process.stdout + process.stderr
    require("Compiler Version 11.00.7022" in banners["compiler"], "expected VC5 RTM compiler 11.00.7022")
    require("Version 5.00." in banners["linker"], "expected original 5.00-series linker")
    return {"schema": 1, "config_sha256": canonical_hash(config),
            "banners": banners, "files": tool_files(config), "runtime": runtime,
            "runtime_startup_revision": "unresolved",
            "codegen_calibrated": False}


def check_lock(config, lock):
    require(lock.get("schema") == 1, "unsupported toolchain lock")
    require(lock["config_sha256"] == canonical_hash(config), "toolchain configuration changed")
    require("Compiler Version 11.00.7022" in lock["banners"]["compiler"], "lock is not VC5 RTM")
    require("Version 5.00." in lock["banners"]["linker"], "lock is not the original linker family")
    require(lock.get("runtime", {"kind": "native"}) == runtime_identity(config), "execution runtime changed")
    require(lock["files"] == tool_files(config), "toolchain inventory changed; review and relock explicitly")


def source_snapshot(root):
    files = [root / "target.json", *root.glob("*.py")]
    if (root / "registry.json").exists():
        files.append(root / "registry.json")
    for folder in ("src", "tools"):
        files.extend(p for p in (root / folder).rglob("*") if p.suffix in (".c", ".h", ".py"))
    files.append(root / "tests" / "behavior.c")
    return {p.relative_to(root).as_posix(): digest(p.read_bytes()) for p in sorted(files)}


def run_build(root, target, config, lock):
    check_lock(config, lock)
    env = environment(config)
    out = root / "build" / (time.strftime("%Y%m%d-%H%M%S-") + uuid.uuid4().hex[:8])
    out.mkdir(parents=True, exist_ok=False)
    snapshot = source_snapshot(root)
    record = {"schema": 1, "complete": False, "commands": [], "files": {},
              "target_spec_sha256": canonical_hash(target), "source_snapshot": snapshot,
              "toolchain_lock_sha256": canonical_hash(lock), "python": sys.version,
              "environment": env, "compiler_options": COMMON_FLAGS,
              "kind": "diagnostic-console-fixture-not-retail-link"}
    try:
        record["source_revision"] = subprocess.check_output(
            ["git", "rev-parse", "HEAD"], cwd=root, text=True).strip()
        record["source_dirty"] = bool(subprocess.check_output(
            ["git", "status", "--porcelain", "--", "engine"], cwd=root.parent, text=True).strip())
        commands = []
        for spec in target["functions"]:
            commands.append([config["compiler"], *COMMON_FLAGS, *spec["flags"],
                             "/Fd" + str(out / "compiler.pdb"), "/Fo" + str(out / (spec["id"] + ".obj")),
                             str(root / spec["source"])])
        commands.append([config["compiler"], *COMMON_FLAGS, "/Od", "/MT",
                         "/DOP_VC5_BEHAVIOR=1", "/Fd" + str(out / "compiler.pdb"),
                         "/Fo" + str(out / "behavior.obj"), str(root / "tests" / "behavior.c")])
        commands.append([config["linker"], "/NOLOGO", "/MACHINE:IX86", "/SUBSYSTEM:CONSOLE",
                         "/INCREMENTAL:NO", "/DEBUG", "/OUT:" + str(out / "WMAIN.EXE"),
                         "/PDB:" + str(out / "WMAIN.PDB"), "/MAP:" + str(out / "WMAIN.MAP"),
                         *[str(out / (s["id"] + ".obj")) for s in target["functions"]],
                         str(out / "behavior.obj"), "libcmt.lib", "kernel32.lib"])
        for index, command in enumerate(commands):
            completed, invoked = run_tool(config, command, root, out, env, 120)
            (out / ("command-%02d.log" % index)).write_text(completed.stdout + completed.stderr, encoding="utf-8")
            record["commands"].append({"argv": command, "invoked": invoked, "returncode": completed.returncode})
            require(completed.returncode == 0, "original-toolchain build failed; inspect " + str(out))
            require(not re.search(r"\bwarning\b", completed.stdout + completed.stderr, re.IGNORECASE),
                    "original-toolchain emitted a warning; inspect " + str(out))
        require(source_snapshot(root) == snapshot, "sources changed during build")
        check_lock(config, lock)
        for name in [s["id"] + ".obj" for s in target["functions"]] + ["WMAIN.EXE", "WMAIN.PDB", "WMAIN.MAP"]:
            record["files"][name] = digest(read_binary(out / name))
        record["complete"] = True
    finally:
        (out / "build.json").write_text(json.dumps(record, indent=2) + "\n", encoding="utf-8")
    return out


def validate_build(root, out, target, lock):
    record = json.loads((out / "build.json").read_text(encoding="utf-8"))
    require(record.get("schema") == 1 and record.get("complete") is True, "build did not complete")
    require(record["target_spec_sha256"] == canonical_hash(target), "build uses a different target specification")
    require(record["source_snapshot"] == source_snapshot(root), "build is stale relative to source/tooling")
    require(record["toolchain_lock_sha256"] == canonical_hash(lock), "build uses a different toolchain lock")
    names = [s["id"] + ".obj" for s in target["functions"]] + ["WMAIN.EXE", "WMAIN.PDB", "WMAIN.MAP"]
    require(set(record["files"]) == set(names), "build artifact inventory differs")
    for name in names:
        require(record["files"][name] == digest(read_binary(out / name)), "build artifact changed: " + name)
    return record
