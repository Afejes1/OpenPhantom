"""Run the original tools natively or inside a narrowly mounted Docker container."""
from pathlib import Path
import re
import subprocess
import uuid

from formats import require


def validate_runner(config):
    runner = config.get("runner", {"kind": "native"})
    require(runner["kind"] in ("native", "docker-wibo"), "unknown toolchain runner")
    if runner["kind"] == "docker-wibo":
        require(re.fullmatch(r"sha256:[0-9a-f]{64}", runner["image"]), "pin the Docker image by its local content ID")
        docker = Path(runner["docker"])
        require(docker.is_absolute() and docker.is_file(), "missing explicit Docker CLI")
        base = Path(runner["toolchain_root"]).resolve()
        require(base.is_dir(), "missing mounted toolchain root")
        for key in ("compiler", "linker"):
            require(Path(config[key]).resolve().is_relative_to(base), "tool outside mounted toolchain")
        for key in ("bin_dirs", "include_dirs", "lib_dirs"):
            require(all(Path(p).resolve().is_relative_to(base) for p in config[key]), "search path outside mounted toolchain")
    return runner


def container_command(config, command, root, out, name):
    runner = validate_runner(config)
    root, out = root.resolve(), out.resolve()
    require(out.is_relative_to(root / "build"), "container output must be a dedicated engine/build directory")
    require(out != root / "build", "do not mount every build writable")
    toolroot = Path(runner["toolchain_root"]).resolve()
    mounts = [(toolroot, "/toolchain", True), (root / "src", "/source/src", True),
              (root / "tests", "/source/tests", True), (out, "/build", False)]

    def translate_path(value, windows=True):
        path = Path(value).resolve()
        for host, guest, _ in mounts:
            if path.is_relative_to(host):
                mapped = guest + "/" + path.relative_to(host).as_posix()
                return "Z:" + mapped.replace("/", "\\") if windows else mapped
        raise ValueError("path outside container mounts: " + str(value))

    arguments = []
    for arg in command[1:]:
        prefix = next((p for p in ("/Fd", "/Fo", "/OUT:", "/PDB:", "/MAP:") if arg.startswith(p)), "")
        if prefix:
            arguments.append(prefix + translate_path(arg[len(prefix):]))
        elif Path(arg).is_absolute() and not arg.startswith("/"):
            arguments.append(translate_path(arg))
        else:
            arguments.append(arg)
    result = [runner["docker"], "run", "--rm", "--pull", "never", "--name", name, "--network", "none", "--read-only",
              "--cap-drop", "ALL", "--security-opt", "no-new-privileges", "--pids-limit", "128",
              "--memory", "512m", "--cpus", "2", "--user", "65534:65534",
              "--tmpfs", "/tmp:rw,nosuid,size=128m", "--workdir", "/build"]
    for host, guest, readonly in mounts:
        require(host.is_dir() and "," not in str(host), "invalid bind mount source")
        result += ["--mount", "type=bind,source=" + str(host) + ",target=" + guest + (",readonly" if readonly else "")]
    guest_env = {"INCLUDE": ";".join(translate_path(p) for p in config["include_dirs"]),
                 "LIB": ";".join(translate_path(p) for p in config["lib_dirs"]),
                 "PATH": ":".join(translate_path(p, False) for p in config["bin_dirs"]) + ":/usr/local/bin:/usr/bin:/bin",
                 "TEMP": r"Z:\tmp", "TMP": r"Z:\tmp", "HOME": "/tmp"}
    for key, value in guest_env.items():
        result += ["--env", key + "=" + value]
    result += ["--entrypoint", "/usr/local/bin/wibo", runner["image"], translate_path(command[0], False), *arguments]
    return result


def runtime_identity(config):
    runner = validate_runner(config)
    if runner["kind"] == "native":
        return {"kind": "native"}
    inspected = subprocess.run([runner["docker"], "image", "inspect", "--format", "{{.Id}} {{.Os}} {{.Architecture}}", runner["image"]],
                               capture_output=True, text=True, timeout=30, check=True)
    require(inspected.stdout.strip() == runner["image"] + " linux amd64", "Docker runtime image changed or has the wrong platform")
    return {"kind": "docker-wibo", "image": runner["image"], "platform": "linux/amd64"}


def run_tool(config, command, root, out, env, timeout):
    runner = validate_runner(config)
    name = "openphantom-vc5-" + uuid.uuid4().hex
    argv = container_command(config, command, root, out, name) if runner["kind"] == "docker-wibo" else command
    try:
        process = subprocess.run(argv, cwd=out, env=None if runner["kind"] == "docker-wibo" else env,
                                 capture_output=True, text=True, errors="replace", timeout=timeout, check=False)
    except subprocess.TimeoutExpired:
        if runner["kind"] == "docker-wibo":
            subprocess.run([runner["docker"], "rm", "--force", name], capture_output=True, timeout=30, check=False)
        raise
    return process, argv
