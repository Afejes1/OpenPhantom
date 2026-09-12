"""Synthetic archive tests: no compiler or original executable is needed."""
import copy
import json
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError
from recovery import file_hash, inspect_bundle, new_private_directory, safe_name


class RecoveryTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.bundle = self.root / "bundle"
        (self.bundle / "toolchain/bin").mkdir(parents=True)
        for name in ("source.bundle", "runtime-image.tar", "toolchain/bin/cl.exe"):
            (self.bundle / name).write_bytes(b"SYNTHETIC NONEXECUTABLE PAYLOAD")
        self.manifest = {
            "schema": 1, "kind": "private-openphantom-recovery", "source_revision": "a" * 40,
            "reference_included": False,
            "toolchain": {"runtime": {"kind": "docker-wibo", "image": "sha256:" + "b" * 64},
                          "files": {"bin/cl.exe": file_hash(self.bundle / "toolchain/bin/cl.exe")},
                          "paths": {"compiler": "bin/cl.exe", "bin_dirs": ["bin"]}},
            "files": {n: {"sha256": file_hash(self.bundle / n), "size": (self.bundle / n).stat().st_size}
                      for n in ("source.bundle", "runtime-image.tar", "toolchain/bin/cl.exe")}}

    def seal(self):
        path = self.bundle / "manifest.json"
        path.write_text(json.dumps(self.manifest), encoding="utf-8")
        return file_hash(path)

    def test_valid_inventory_and_exact_manifest_pin(self):
        sha = self.seal()
        inspect_bundle(self.bundle, sha)
        with self.assertRaises(VerificationError):
            inspect_bundle(self.bundle, "0" * 64)

    def test_changed_missing_extra_and_truncated_files_rejected(self):
        for name in ("source.bundle", "runtime-image.tar", "toolchain/bin/cl.exe"):
            sha = self.seal()
            path = self.bundle / name
            original = path.read_bytes()
            for value in (b"CHANGED", original[:-1]):
                path.write_bytes(value)
                with self.assertRaises(VerificationError):
                    inspect_bundle(self.bundle, sha)
            path.unlink()
            with self.assertRaises(VerificationError):
                inspect_bundle(self.bundle, sha)
            path.write_bytes(original)
        sha = self.seal()
        (self.bundle / "unlisted.exe").write_bytes(b"extra")
        with self.assertRaises(VerificationError):
            inspect_bundle(self.bundle, sha)

    def test_original_file_payload_is_not_an_allowed_inventory_extension(self):
        self.manifest["reference_included"] = True
        with self.assertRaises(VerificationError):
            inspect_bundle(self.bundle, self.seal())

    def test_unsafe_names_and_case_collisions_rejected(self):
        for name in ("../x", "a/../x", "/x", "C:/x", "a\\x", "bin/con.txt", "bin/x:stream",
                     "bin/trailing.", "bin/trailing ", "bin//x", "bin/./x"):
            with self.subTest(name=name), self.assertRaises(VerificationError):
                safe_name(name)
        safe_name("bin/compiler.dll")

    def test_manifest_cannot_redirect_restored_tool_paths(self):
        self.manifest["toolchain"]["paths"]["compiler"] = "../outside.exe"
        with self.assertRaises(VerificationError):
            inspect_bundle(self.bundle, self.seal())

    def test_tool_fingerprint_is_checked_independently(self):
        self.manifest["toolchain"]["files"]["bin/cl.exe"] = "c" * 64
        with self.assertRaises(VerificationError):
            inspect_bundle(self.bundle, self.seal())

    def test_destination_is_new_and_private(self):
        destination = self.root / "private/new-restore"
        self.assertEqual(new_private_directory(self.root, destination), destination.resolve())
        with self.assertRaises(VerificationError):
            new_private_directory(self.root, destination)
        with self.assertRaises(VerificationError):
            new_private_directory(self.root, self.root / "outside")
        with self.assertRaises(VerificationError):
            new_private_directory(self.root, self.root / "private/../../escape")

    def test_links_are_rejected_before_write(self):
        with patch.object(Path, "is_junction", return_value=True), self.assertRaises(VerificationError):
            new_private_directory(self.root, self.root / "private/new")

    def test_oversized_manifest_fails_before_payload_processing(self):
        self.manifest["files"]["runtime-image.tar"]["size"] = 3 * 1024 ** 3
        with self.assertRaises(VerificationError):
            inspect_bundle(self.bundle, self.seal())
