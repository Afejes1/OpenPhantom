import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError
from runners import container_command, run_tool, runtime_identity, validate_runner


@unittest.skipUnless(os.name == "nt", "Windows Docker host path translation")
class DockerRunnerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix="vc5 test ")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.tools = self.root / "private/compiler"
        self.out = self.root / "build/fresh"
        for path in [self.tools / d for d in ("bin", "include", "lib")] + [self.root / "src", self.root / "tests", self.out]:
            path.mkdir(parents=True)
        self.docker = self.root / "docker.exe"
        self.docker.write_bytes(b"SYNTHETIC")
        self.config = {"compiler": str(self.tools / "bin/cl.exe"), "linker": str(self.tools / "bin/link.exe"),
                       "bin_dirs": [str(self.tools / "bin")], "include_dirs": [str(self.tools / "include")],
                       "lib_dirs": [str(self.tools / "lib")],
                       "runner": {"kind": "docker-wibo", "docker": str(self.docker),
                                  "image": "sha256:" + "a" * 64, "toolchain_root": str(self.tools)}}
        self.command = [self.config["compiler"], "/c", "/Fo" + str(self.out / "sample.obj"), str(self.root / "src/sample.c")]

    def test_mounts_and_windows_arguments_are_narrow_and_explicit(self):
        argv = container_command(self.config, self.command, self.root, self.out, "test-vc5")
        mounts = [argv[i + 1] for i, value in enumerate(argv) if value == "--mount"]
        self.assertEqual(len(mounts), 4)
        self.assertEqual(sum(value.endswith(",readonly") for value in mounts), 3)
        self.assertIn("type=bind,source=" + str(self.out) + ",target=/build", mounts)
        self.assertIn(r"/FoZ:\build\sample.obj", argv)
        self.assertIn(r"Z:\source\src\sample.c", argv)
        self.assertIn(r"INCLUDE=Z:\toolchain\include", argv)
        self.assertEqual(argv[argv.index("--network") + 1], "none")
        self.assertEqual(argv[argv.index("--user") + 1], "65534:65534")
        self.assertIn("--read-only", argv)
        self.assertNotIn("--privileged", argv)
        self.assertNotIn("/var/run/docker.sock", " ".join(argv))

    def test_mutable_image_tag_and_external_search_path_are_rejected(self):
        self.config["runner"]["image"] = "runtime:latest"
        with self.assertRaises(VerificationError):
            validate_runner(self.config)
        self.config["runner"]["image"] = "sha256:" + "a" * 64
        self.config["include_dirs"] = [str(self.root)]
        with self.assertRaises(VerificationError):
            validate_runner(self.config)

    def test_unmounted_input_and_broad_output_directory_are_rejected(self):
        for out in (self.root, self.root / "build", self.root / "private"):
            with self.assertRaises(VerificationError):
                container_command(self.config, self.command, self.root, out, "test-vc5")
        with self.assertRaises(ValueError):
            container_command(self.config, [self.config["compiler"], str(self.root / "private/reference.exe")], self.root, self.out, "test-vc5")

    def test_runtime_identity_must_match_pinned_linux_image(self):
        result = subprocess.CompletedProcess([], 0, "sha256:" + "b" * 64 + " linux amd64", "")
        with patch("runners.subprocess.run", return_value=result), self.assertRaises(VerificationError):
            runtime_identity(self.config)

    def test_timeout_removes_only_the_named_task_container(self):
        failure = subprocess.TimeoutExpired([], 1)
        cleaned = subprocess.CompletedProcess([], 0, "", "")
        with patch("runners.subprocess.run", side_effect=[failure, cleaned]) as calls:
            with self.assertRaises(subprocess.TimeoutExpired):
                run_tool(self.config, self.command, self.root, self.out, {}, 1)
        launched = calls.call_args_list[0].args[0]
        cleanup = calls.call_args_list[1].args[0]
        self.assertEqual(cleanup, [str(self.docker), "rm", "--force", launched[launched.index("--name") + 1]])


if __name__ == "__main__":
    unittest.main()
