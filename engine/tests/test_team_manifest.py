"""Team status is scheduling evidence and must never imply local acceptance."""
import copy
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError
from team_manifest import (REFERENCE_MD5, REFERENCE_SHA256, parse_manifest,
                           render_team_progress, validate_manifest, verified_by_address)

SAMPLE = ("WMAIN.EXE (829,952 bytes, md5 " + REFERENCE_MD5 + ")\n"
          "Done (1)\n0x00401000 bp/first.c label_one\n"
          "Not done (1)\n0x00402000 (file not known yet) alias_a | alias_b\n").encode()


class TeamManifestTests(unittest.TestCase):
    def test_parse_preserves_aliases_unknown_file_and_provenance(self):
        doc = validate_manifest(parse_manifest(SAMPLE))
        self.assertEqual(doc["reported_counts"], {"done": 1, "not_done": 1})
        self.assertIsNone(doc["functions"][1]["reported_file"])
        self.assertEqual(doc["functions"][1]["reported_name"], "alias_a | alias_b")
        self.assertEqual(doc["functions"][1]["source_line"], 5)
        self.assertEqual(len(doc["source_sha256"]), 64)

    def test_wrong_binary_or_missing_identity_rejected(self):
        for data in (SAMPLE.replace(b"829,952", b"829,953"), SAMPLE.replace(REFERENCE_MD5.encode(), b"0" * 32), SAMPLE.split(b"\n", 1)[1]):
            with self.assertRaises(VerificationError):
                parse_manifest(data)

    def test_conflicting_addresses_and_duplicate_sections_rejected(self):
        for data in (SAMPLE.replace(b"00402000", b"00401000"), SAMPLE + b"Done (0)\n"):
            with self.assertRaises(VerificationError):
                parse_manifest(data)

    def test_counts_and_missing_sections_rejected(self):
        for data in (SAMPLE.replace(b"Done (1)", b"Done (2)"), SAMPLE.split(b"Not done")[0]):
            with self.assertRaises(VerificationError):
                parse_manifest(data)

    def test_malformed_rows_paths_and_runtime_addresses_rejected(self):
        for data in (SAMPLE.replace(b"bp/first.c", b"../first.c"), SAMPLE.replace(b"00402000", b"00499214"), SAMPLE.replace(b"0x00402000", b"0xbroken")):
            with self.assertRaises(VerificationError):
                parse_manifest(data)

    def test_normalized_snapshot_rejects_manual_status_or_count_drift(self):
        for field, value in (("reported_status", "verified"), ("address", 0x401000), ("reported_file", "../bad.c")):
            doc = parse_manifest(SAMPLE)
            doc["functions"][1][field] = value
            with self.assertRaises(VerificationError):
                validate_manifest(doc)

    def test_team_done_alone_cannot_grant_a_local_match(self):
        target = {"functions": [{"id": "local", "address": 0x401000}], "size": 829952, "sha256": REFERENCE_SHA256}
        order = {"schema": 1, "module_order": ["bp/first.c"], "pending_candidates": []}
        report = render_team_progress(parse_manifest(SAMPLE), order, target, [])
        self.assertIn("**0** team-done entries", report)
        self.assertIn("Deferred; team reports done, local verification pending", report)
        self.assertEqual(verified_by_address(target, []), {})

    def test_changed_spec_does_not_inherit_a_recorded_match(self):
        spec = {"id": "local", "address": 0x401000, "size": 8}
        target = {"functions": [spec]}
        history = [{"functions": [{"id": "local", "spec": copy.deepcopy(spec), "exceptions": []}]}]
        self.assertIn(0x401000, verified_by_address(target, history))
        spec["size"] = 7
        self.assertEqual(verified_by_address(target, history), {})

    def test_report_requires_matching_binary_and_known_priority_file(self):
        doc = parse_manifest(SAMPLE)
        target = {"functions": [], "size": 829952, "sha256": REFERENCE_SHA256}
        order = {"schema": 1, "module_order": ["bp/first.c"], "pending_candidates": []}
        target["sha256"] = "0" * 64
        with self.assertRaises(VerificationError):
            render_team_progress(doc, order, target, [])
        target["sha256"] = REFERENCE_SHA256
        order["module_order"] = ["bp/missing.c"]
        with self.assertRaises(VerificationError):
            render_team_progress(doc, order, target, [])


if __name__ == "__main__":
    unittest.main()
