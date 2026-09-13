"""Focused failure checks for the campaign's executable and source boundaries."""
import json
import sys
import struct
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import verify
sys.path.insert(0, str(verify.ROOT / "tests"))
from fixtures import coff


class CampaignBoundaryTests(unittest.TestCase):
    def setUp(self):
        (verify.ROOT / "build").mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(prefix="campaign-boundary-", dir=verify.ROOT / "build")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name).resolve()
        self.folder = self.root / "research/case"
        self.folder.mkdir(parents=True)
        (self.folder / "candidate.c").write_text("void candidate(void) {}\n")
        self.case = {"schema": 1, "id": "candidate", "symbol": "_candidate", "candidate": "candidate.c",
                     "inputs": ["candidate.c"], "fixture": None, "flags": ["/O2", "/MT"], "target": None}

    def read(self):
        (self.folder / "case.json").write_text(json.dumps(self.case))
        with patch.object(verify, "ROOT", self.root):
            return verify.read_case(self.folder)

    def test_native_behavior_rejected_before_toolchain_or_execution(self):
        (self.folder / "behavior.c").write_text("int main(void) { return 0; }\n")
        self.case["inputs"].append("behavior.c")
        self.case["fixture"] = "behavior.c"
        self.read()
        with patch.object(verify, "ROOT", self.root), patch.object(verify, "load_config") as config:
            with patch.object(sys, "argv", ["verify.py", "--case", str(self.folder), "--runner", "native", "--behavior"]):
                with self.assertRaisesRegex(verify.VerificationError, "requires locked Docker"):
                    verify.main()
            config.assert_not_called()

    def test_build_or_private_inputs_are_not_source_inputs(self):
        self.case["inputs"] = ["../../build/secrets.h"]
        with self.assertRaisesRegex(verify.VerificationError, "outside source directories"):
            self.read()

    def test_flat_copy_collision_is_rejected(self):
        extra = self.root / "src"
        extra.mkdir()
        (extra / "candidate.c").write_text("void unrelated(void) {}\n")
        self.case["inputs"].append("../../src/candidate.c")
        with self.assertRaisesRegex(verify.VerificationError, "duplicate input basename"):
            self.read()

    def test_unlisted_candidate_is_rejected(self):
        self.case["inputs"] = []
        with self.assertRaisesRegex(verify.VerificationError, "missing from inputs"):
            self.read()

    def test_extra_compiler_flags_cannot_replace_compile_only(self):
        self.case["flags"] = ["/O2", "/MT", "/link"]
        with self.assertRaisesRegex(verify.VerificationError, "unapproved compiler"):
            self.read()

    def test_target_cannot_escape_case(self):
        self.case["target"] = "../other/target.json"
        with self.assertRaisesRegex(verify.VerificationError, "target outside case"):
            self.read()

    def test_shared_source_header_can_be_an_explicit_input(self):
        extra = self.root / "src"
        extra.mkdir()
        (extra / "baseline.h").write_text("typedef unsigned int word;\n")
        self.case["inputs"].append("../../src/baseline.h")
        case, inputs, _, target = self.read()
        self.assertEqual(len(inputs), 2)
        self.assertIsNone(target)


    def test_reference_layout_cannot_escape_case(self):
        self.case["reference_layout"] = "../other/layout.json"
        with self.assertRaisesRegex(verify.VerificationError, "reference layout outside case"):
            self.read()

    def multiple_function_artifact(self):
        data = bytearray(coff(b"\xc3\x90\xc3", symbol="_candidate"))
        symbols_at, count = struct.unpack_from("<II", data, 8)
        insert = symbols_at + count * 18
        data[insert:insert] = struct.pack("<8sIhHBB", b"_helper", 2, 1, 0x20, 3, 0)
        struct.pack_into("<I", data, 12, count + 1)
        obj = self.folder / "candidate.obj"
        obj.write_bytes(data)
        listing = self.folder / "candidate.cod"
        listing.write_text("_candidate PROC NEAR\n ret 0\n_candidate ENDP\n_helper PROC NEAR\n ret 0\n_helper ENDP\nEND\n")
        artifact = verify.capture_artifact("candidate.c", {"id": "candidate", "symbol": "_candidate"}, obj, listing)
        return artifact

    def test_uninlined_helper_preserves_object_but_cannot_claim_function_match(self):
        artifact = self.multiple_function_artifact()
        self.assertEqual(artifact["inventory_scope"], "translation-unit")
        self.assertFalse(artifact["strict_function_comparable"])
        self.assertEqual(artifact["code_section_bytes"], 3)
        self.assertNotIn("function_section_bytes", artifact)
        verify.verify_artifacts(self.folder, [artifact])
        inv = json.loads((self.folder / artifact["function_inventory"]).read_bytes())
        self.assertEqual(len(inv["sections"][0]["symbols"]), 2)

    def test_edited_translation_unit_inventory_rejected_even_with_updated_hash(self):
        artifact = self.multiple_function_artifact()
        path = self.folder / artifact["function_inventory"]
        inventory = json.loads(path.read_bytes())
        inventory["sections"][0]["rows"][0]["hex"] = "c3 90 90"
        path.write_text(json.dumps(inventory))
        artifact["function_inventory_sha256"] = verify.digest(path.read_bytes())
        with self.assertRaisesRegex(verify.VerificationError, "inventory differs"):
            verify.verify_artifacts(self.folder, [artifact])


if __name__ == "__main__":
    unittest.main()
