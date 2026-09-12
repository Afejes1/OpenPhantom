import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from build import (check_lock, configure, environment, load_config, run_build,
                   source_snapshot, tool_files, validate_build)
from formats import VerificationError
from matching import canonical_hash, digest


class ProvenanceTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for folder in ("bin", "include", "lib", "src", "tools", "tests"):
            (self.root / folder).mkdir()
        for name in ("cl.exe", "link.exe", "c1.dll", "c2.exe"):
            (self.root / "bin" / name).write_bytes(b"SYNTHETIC NOT EXECUTABLE")
        (self.root / "lib/libcmt.lib").write_bytes(b"SYNTHETIC LIBRARY")
        (self.root / "include/float.h").write_text("/* synthetic */")
        self.config = {"schema": 1, "provenance": "Synthetic test only",
                       "compiler": str(self.root / "bin/cl.exe"), "linker": str(self.root / "bin/link.exe"),
                       "bin_dirs": [str(self.root / "bin")], "include_dirs": [str(self.root / "include")],
                       "lib_dirs": [str(self.root / "lib")]}

    def lock(self):
        return {"schema": 1, "config_sha256": canonical_hash(self.config), "files": tool_files(self.config),
                "banners": {"compiler": "Compiler Version 11.00.7022", "linker": "Version 5.00.7022"}}

    def test_compiler_backend_header_library_and_added_file_changes_rejected(self):
        for relative in ("bin/c2.exe", "include/float.h", "lib/libcmt.lib", "include/new.h"):
            lock = self.lock()
            path = self.root / relative
            old = path.read_bytes() if path.exists() else None
            path.write_bytes(b"CHANGED")
            with self.assertRaises(VerificationError):
                check_lock(self.config, lock)
            if old is None:
                path.unlink()
            else:
                path.write_bytes(old)

    def test_missing_vc5_codegen_executable_is_rejected(self):
        (self.root / "bin/c2.exe").unlink()
        (self.root / "bin/c2.dll").write_bytes(b"WRONG BACKEND LAYOUT")
        with self.assertRaises(VerificationError):
            tool_files(self.config)

    def test_changed_configuration_rejected(self):
        lock = self.lock()
        self.config["provenance"] = "Changed origin"
        with self.assertRaises(VerificationError):
            check_lock(self.config, lock)

    def test_compiler_must_be_in_locked_directory(self):
        external = self.root / "outside.exe"
        external.write_bytes(b"SYNTHETIC")
        self.config["compiler"] = str(external)
        path = self.root / "config.json"
        path.write_text(json.dumps(self.config))
        with self.assertRaises(VerificationError):
            load_config(path)

    @unittest.skipUnless(os.name == "nt", "native Windows environment profile")
    def test_compiler_options_and_user_path_do_not_leak(self):
        with patch.dict(os.environ, {"CL": "/O2", "_CL_": "/MD", "LINK": "/DEBUG", "PATH": "UNTRUSTED"}):
            env = environment(self.config)
        for key in ("CL", "_CL_", "LINK"):
            self.assertNotIn(key, env)
        self.assertNotIn("UNTRUSTED", env["PATH"])

    @unittest.skipUnless(os.name == "nt", "native Windows build profile")
    def test_modern_compiler_banner_is_rejected(self):
        completed = subprocess.CompletedProcess([], 0, "Compiler Version 19.44.35228\nVersion 14.44.35228", "")
        with patch("build.subprocess.run", return_value=completed), self.assertRaises(VerificationError):
            configure(self.config)

    @unittest.skipUnless(os.name == "nt", "native Windows build profile")
    def test_command_line_warning_with_success_exit_is_rejected(self):
        target = self.prepare_sources()
        warning = subprocess.CompletedProcess([], 0, "Command line warning D4002 : ignoring unknown option", "")
        with patch("build.subprocess.run", return_value=warning), patch("build.subprocess.check_output", return_value="fake-revision"):
            with self.assertRaises(VerificationError):
                run_build(self.root, target, self.config, self.lock())
        records = list((self.root / "build").glob("*/build.json"))
        self.assertFalse(json.loads(records[0].read_text())["complete"])

    def prepare_sources(self):
        for name in ("target.json", "verify.py", "src/candidate.c", "tools/driver.py", "tests/behavior.c"):
            (self.root / name).write_text("synthetic input")
        return {"functions": [{"id": "candidate", "source": "src/candidate.c", "flags": ["/Od", "/MT"]}]}

    def prepare_record(self):
        target = self.prepare_sources()
        out = self.root / "output"
        out.mkdir()
        names = ["candidate.obj", "WMAIN.EXE", "WMAIN.PDB", "WMAIN.MAP"]
        for name in names:
            (out / name).write_bytes(b"SYNTHETIC ARTIFACT")
        lock = self.lock()
        record = {"schema": 1, "complete": True, "target_spec_sha256": canonical_hash(target),
                  "source_snapshot": source_snapshot(self.root), "toolchain_lock_sha256": canonical_hash(lock),
                  "files": {name: digest((out / name).read_bytes()) for name in names}}
        (out / "build.json").write_text(json.dumps(record))
        return out, target, lock

    def test_source_or_driver_changes_make_build_stale(self):
        out, target, lock = self.prepare_record()
        validate_build(self.root, out, target, lock)
        for name in ("src/candidate.c", "tools/driver.py", "verify.py"):
            old = (self.root / name).read_bytes()
            (self.root / name).write_bytes(b"changed")
            with self.assertRaises(VerificationError):
                validate_build(self.root, out, target, lock)
            (self.root / name).write_bytes(old)

    def test_replaced_or_missing_artifacts_are_rejected(self):
        out, target, lock = self.prepare_record()
        (out / "candidate.obj").write_bytes(b"changed")
        with self.assertRaises(VerificationError):
            validate_build(self.root, out, target, lock)
        (out / "candidate.obj").unlink()
        with self.assertRaises(OSError):
            validate_build(self.root, out, target, lock)

    @unittest.skipUnless(os.name == "nt", "native Windows build profile")
    def test_failed_compiler_cannot_leave_successful_build_record(self):
        target = self.prepare_sources()
        failure = subprocess.CompletedProcess([], 1, "synthetic compiler failure", "")
        with patch("build.subprocess.run", return_value=failure), patch("build.subprocess.check_output", return_value="fake-revision"):
            with self.assertRaises(VerificationError):
                run_build(self.root, target, self.config, self.lock())
        records = list((self.root / "build").glob("*/build.json"))
        self.assertEqual(len(records), 1)
        self.assertFalse(json.loads(records[0].read_text())["complete"])


if __name__ == "__main__":
    unittest.main()
