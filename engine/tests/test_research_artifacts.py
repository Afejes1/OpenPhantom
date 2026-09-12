"""Synthetic research artifact provenance checks; no game or compiler inputs."""
import importlib.util
import copy
import json
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
        self.listing.write_text("_candidate PROC NEAR\n  00000 a1 00 00 00 00 mov eax, DWORD PTR _global\n  00005 c3 ret 0\n_candidate ENDP\n_TEXT ENDS\nEND\n")
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
        self.assertTrue(artifact["assembly_listing_has_terminators"])
        self.assertEqual(artifact["function_inventory_sha256"],
                         digest((self.out / artifact["function_inventory"]).read_bytes()))
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
        self.artifact()
        for path in (self.obj, self.listing, self.obj.with_suffix(".function.json")):
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

    def test_truncated_listing_is_visible_and_inventory_is_complete(self):
        self.listing.write_text("_candidate PROC NEAR\n  00000 a1 00")
        artifact = self.artifact()
        self.assertFalse(artifact["assembly_listing_has_terminators"])
        inventory = json.loads((self.out / artifact["function_inventory"]).read_bytes())
        actual = b"".join(bytes.fromhex(row["hex"]) for row in inventory["rows"])
        self.assertEqual(actual, self.code)
        self.assertEqual(inventory["relocations"][0]["symbol"]["name"], "_global")
        self.assertEqual(inventory["relocations"][0]["kind"], 6)
        probe.verify_artifacts(self.out, [artifact])

    def test_terminators_must_belong_to_function_and_end_file(self):
        variants = [b"_other PROC NEAR\n_other ENDP\nEND\n",
                    b"_candidate PROC NEAR\n_candidate ENDP\n",
                    b"_candidate PROC NEAR\n; _candidate ENDP\nEND\n",
                    b"_candidate ENDP\n_candidate PROC NEAR\nEND\n",
                    b"_candidate PROC NEAR\n_candidate ENDP\nEND\ntrailing fragment"]
        for listing in variants:
            with self.subTest(listing=listing):
                self.assertFalse(probe.listing_terminators(listing, "_candidate"))

    def test_inventory_rejects_lost_padding_holes_bytes_and_fixups(self):
        original = probe.function_inventory(self.obj.read_bytes(), "_candidate")
        variants = []
        changed = copy.deepcopy(original); changed["rows"][0]["hex"] = self.code[:-1].hex(); variants.append(changed)
        changed = copy.deepcopy(original); changed["rows"][0]["offset"] = 1; variants.append(changed)
        changed = copy.deepcopy(original); changed["rows"][0]["hex"] = (self.code[:-1] + b"X").hex(); variants.append(changed)
        changed = copy.deepcopy(original); changed["relocations"] = []; variants.append(changed)
        changed = copy.deepcopy(original); changed["relocations"][0]["addend"] = 4; variants.append(changed)
        changed = copy.deepcopy(original); changed["relocations"][0]["symbol"]["name"] = "_wrong"; variants.append(changed)
        for inventory in variants:
            with self.subTest(inventory=inventory):
                with self.assertRaisesRegex(VerificationError, "inventory differs"):
                    probe.verify_inventory(self.obj.read_bytes(), "_candidate", inventory)

    def test_inventory_covers_multiple_rows_and_unaligned_tail(self):
        code = b"\x90" * 33 + b"\xc3" + b"\x90" * 7
        self.obj.write_bytes(coff(code))
        inventory = probe.function_inventory(self.obj.read_bytes(), "_candidate")
        self.assertEqual([row["offset"] for row in inventory["rows"]], [0, 16, 32])
        self.assertEqual(b"".join(bytes.fromhex(row["hex"]) for row in inventory["rows"]), code)
        self.assertEqual(inventory["function_section_bytes"], 41)
        self.assertEqual(inventory["relocations"], [])

    def test_even_updated_inventory_hash_cannot_accept_wrong_bytes(self):
        artifact = self.artifact()
        path = self.out / artifact["function_inventory"]
        inventory = json.loads(path.read_bytes()); inventory["rows"] = []
        path.write_text(json.dumps(inventory))
        artifact["function_inventory_sha256"] = digest(path.read_bytes())
        with self.assertRaisesRegex(VerificationError, "inventory differs"):
            probe.verify_artifacts(self.out, [artifact])


if __name__ == "__main__":
    unittest.main()
