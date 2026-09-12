"""Synthetic research artifact provenance checks; no game or compiler inputs."""
import importlib.util
from pathlib import Path
import struct
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from fixtures import coff
from formats import VerificationError
from matching import digest

MODULE = importlib.util.spec_from_file_location(
    "surface_research_probe", ROOT / "research/surface-emission/probe.py")
probe = importlib.util.module_from_spec(MODULE)
MODULE.loader.exec_module(probe)


class ResearchArtifactTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.out = Path(self.temp.name)
        self.obj = self.out / "sample.obj"
        self.listing = self.out / "sample.cod"
        self.code = b"\xa1\0\0\0\0\xc3\x90\x90"
        self.obj.write_bytes(coff(self.code, [(1, "_global", 6)]))
        self.listing.write_text("Synthetic compiler listing, not game code.\n")
        self.spec = {"id": "sample", "symbol": "_candidate"}

    def artifact(self):
        return probe.candidate_artifact("sample.c", self.spec, self.obj, self.listing)

    def test_complete_section_and_relocations_recorded_without_match_claim(self):
        artifact = self.artifact()
        self.assertEqual(artifact["function_section_bytes"], len(self.code))
        self.assertEqual(artifact["function_section_sha256"], digest(self.code))
        self.assertEqual(artifact["relocation_count"], 1)
        self.assertEqual(artifact["object_sha256"], digest(self.obj.read_bytes()))
        self.assertEqual(artifact["assembly_listing_sha256"], digest(self.listing.read_bytes()))
        self.assertEqual(artifact["object"], "sample.obj")
        self.assertNotIn("status", artifact)
        probe.verify_artifacts(self.out, [artifact])

    def test_metadata_changes_object_hash_but_not_function_hash(self):
        first = self.artifact()
        changed = bytearray(self.obj.read_bytes())
        struct.pack_into("<I", changed, 4, 12345)  # Synthetic COFF timestamp only.
        self.obj.write_bytes(changed)
        second = self.artifact()
        self.assertNotEqual(first["object_sha256"], second["object_sha256"])
        self.assertEqual(first["function_section_sha256"], second["function_section_sha256"])

    def test_missing_and_empty_listing_rejected(self):
        self.listing.unlink()
        with self.assertRaises(OSError):
            self.artifact()
        self.listing.write_text(" \n")
        with self.assertRaisesRegex(VerificationError, "listing is empty"):
            self.artifact()

    def test_wrong_function_symbol_rejected(self):
        self.spec["symbol"] = "_missing"
        with self.assertRaisesRegex(VerificationError, "function symbol"):
            self.artifact()

    def test_object_and_listing_mutations_rejected_before_completion(self):
        for path in (self.obj, self.listing):
            with self.subTest(path=path.name):
                artifact = self.artifact()
                original = path.read_bytes()
                path.write_bytes(original + b"CHANGED")
                with self.assertRaisesRegex(VerificationError, "artifact changed"):
                    probe.verify_artifacts(self.out, [artifact])
                path.write_bytes(original)

    def test_deleted_artifact_rejected_before_completion(self):
        artifact = self.artifact()
        self.obj.unlink()
        with self.assertRaises(OSError):
            probe.verify_artifacts(self.out, [artifact])


if __name__ == "__main__":
    unittest.main()
