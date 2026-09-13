"""Negative acceptance-boundary checks; no compiler or fixture execution."""
from copy import deepcopy
import unittest
import rebuild


class ReceiptChecks(unittest.TestCase):
    def setUp(self):
        case = rebuild.read(rebuild.ENGINE / "research/campaign-005-music-state/result.json")
        self.case = case
        self.receipt = rebuild.read(rebuild.ROOT / case["docker_report"])
        self.peer = case["native_report_sha256"]

    def check(self, value):
        rebuild.validate_receipt(value, self.case, "docker", self.peer)

    def test_preserved_real_receipt(self):
        self.check(self.receipt)

    def test_debug_object_hash_does_not_replace_span_evidence(self):
        value = deepcopy(self.receipt)
        value["comparison"]["object_sha256"] = "f" * 64
        self.check(value)
        value["comparison"]["resolved_span_sha256"] = "f" * 64
        with self.assertRaisesRegex(ValueError, "comparison changed"):
            self.check(value)

    def test_incomplete_or_failed_fixture_rejected(self):
        for path in ("completed", "fixture"):
            value = deepcopy(self.receipt)
            if path == "completed":
                value[path] = False
            else:
                value[path]["passed"] = False
            with self.assertRaises(ValueError):
                self.check(value)

    def test_source_toolchain_and_peer_drift_rejected(self):
        for key in ("source_snapshot", "toolchain_lock_sha256", "peer_report_sha256"):
            value = deepcopy(self.receipt)
            value[key] = {} if key == "source_snapshot" else "0" * 64
            with self.assertRaises(ValueError):
                self.check(value)

    def test_extent_operand_and_mismatch_changes_rejected(self):
        for key in ("size", "candidate_size", "body_size", "adjusted_bytes", "different_bytes"):
            value = deepcopy(self.receipt)
            value["comparison"][key] += 1
            with self.assertRaises(ValueError):
                self.check(value)

    def test_lost_checks_or_failures_rejected(self):
        for stdout in ("", "0 checks, 0 failures", "8 checks, 1 failures"):
            value = deepcopy(self.receipt)
            value["fixture"]["stdout"] = stdout
            with self.assertRaises(ValueError):
                self.check(value)


if __name__ == "__main__":
    unittest.main()
