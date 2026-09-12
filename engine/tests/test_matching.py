import copy
import json
from pathlib import Path
import struct
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import COFF, PE, VerificationError
from matching import (check_regression, compare_batch, compare_function, compare_whole,
                      load_target, verify_reference)
from fixtures import coff, pe, spec, target


class ComparisonTests(unittest.TestCase):
    def test_raw_code_equality_is_not_linked_function_proof(self):
        result = compare_function(PE(pe(b"\x90\xc3")), COFF(coff(b"\x90\xc3")), spec(2))
        self.assertEqual(result["status"], "raw-code-match")
        self.assertEqual(result["compared_bytes"], 2)
        self.assertFalse(result["literal_function_match"])
        self.assertFalse(result["layout_verified"])

    def test_instruction_and_length_mutations_fail(self):
        for code in (b"\x91\xc3", b"\xc3", b"\x90\xc3\xcc"):
            with self.subTest(code=code):
                self.assertEqual(compare_function(PE(pe(b"\x90\xc3")), COFF(coff(code)), spec(2))["status"], "mismatch")

    def test_padding_is_compared_even_when_body_extent_is_shorter(self):
        definition = spec(4)
        definition["body_size"] = 2
        original = PE(pe(b"\x90\xc3\x90\x90"))
        good = compare_function(original, COFF(coff(b"\x90\xc3\x90\x90")), definition)
        self.assertEqual(good["body_size"], 2)
        self.assertEqual(good["compared_bytes"], 4)
        self.assertEqual(good["reference_span_sha256"], good["resolved_span_sha256"])
        for code in (b"\x90\xc3", b"\x90\xc3\x90\xcc"):
            self.assertEqual(compare_function(original, COFF(coff(code)), definition)["status"], "mismatch")

    def relocation_case(self, symbol="_global", addend=0, kind=6):
        original = PE(pe(b"\xa1" + struct.pack("<I", 0x402000) + b"\xc3", [1]))
        obj = COFF(coff(b"\xa1" + struct.pack("<I", addend) + b"\xc3", [(1, symbol, kind)]))
        definition = spec(6, [{"offset": 1, "address": 0x402000, "symbol": "_global"}])
        return original, obj, definition

    def test_proven_external_relocation(self):
        result = compare_function(*self.relocation_case())
        self.assertEqual(result["status"], "relocation-adjusted-match")
        self.assertEqual(result["adjusted_bytes"], 4)
        self.assertTrue(result["relocated_bytes_equal"])
        self.assertEqual(result["reference_span_sha256"], result["resolved_span_sha256"])
        self.assertEqual(result["compared_bytes"], 2)

    def test_wrong_target_or_addend_never_gets_wildcarded(self):
        for changes in ({"symbol": "_wrong"}, {"addend": 1}, {"kind": 20}):
            with self.subTest(changes=changes), self.assertRaises(VerificationError):
                compare_function(*self.relocation_case(**changes))

    def test_constant_definition_must_match(self):
        definition = spec(6, [{"offset": 1, "address": 0x402000, "float32": 1.0}])
        original = PE(pe(b"\xa1" + struct.pack("<I", 0x402000) + b"\xc3", [1], struct.pack("<f", 1.0)))
        for value in (1.0, 2.0):
            obj = COFF(coff(b"\xa1\0\0\0\0\xc3", [(1, "_constant", 6)], {"_constant": struct.pack("<f", value)}))
            if value == 1.0:
                self.assertEqual(compare_function(original, obj, definition)["status"], "relocation-adjusted-match")
            else:
                with self.assertRaises(VerificationError):
                    compare_function(original, obj, definition)

    def test_pe_must_confirm_every_address_adjustment(self):
        original, obj, definition = self.relocation_case()
        original.relocations.clear()
        with self.assertRaises(VerificationError):
            compare_function(original, obj, definition)

    def test_missing_binding_or_missing_relocation_fails(self):
        original, obj, definition = self.relocation_case()
        definition["bindings"] = []
        with self.assertRaises(VerificationError):
            compare_function(original, obj, definition)
        original, _, definition = self.relocation_case()
        with self.assertRaises(VerificationError):
            compare_function(original, COFF(coff(b"\xa1\0\0\0\0\xc3")), definition)

    def test_reference_identity_and_operand_inventory(self):
        raw = pe(b"\xa1" + struct.pack("<I", 0x402000) + b"\xc3", [1])
        definition = spec(6, [{"offset": 1, "address": 0x402000, "symbol": "_global"}])
        manifest = target(raw, definition)
        self.assertIsInstance(verify_reference(raw, manifest), PE)
        with self.assertRaises(VerificationError):
            verify_reference(raw[:-1], manifest)
        definition["bindings"][0]["address"] += 4
        with self.assertRaises(VerificationError):
            verify_reference(raw, manifest)

    def test_missing_object_is_unresolved_not_skipped(self):
        raw = pe(b"\xc3")
        report = compare_batch(PE(raw), {}, target(raw, spec(1)))
        self.assertFalse(report["complete"])
        self.assertEqual(report["functions"][0]["status"], "unresolved")

    def test_regression_rejects_deleted_or_weakened_records(self):
        raw = pe(b"\xc3")
        good = compare_batch(PE(raw), {"sample": coff(b"\xc3")}, target(raw, spec(1)))
        check_regression(good, good)
        mutations = [lambda r: r.update(functions=[]), lambda r: r.update(complete=False),
                     lambda r: r.update(target_spec_sha256="changed"),
                     lambda r: r.update(target_sha256="changed"),
                     lambda r: r["functions"][0].update(adjusted_bytes=1),
                     lambda r: r["functions"][0].update(compared_bytes=0),
                     lambda r: r["functions"][0].update(status="unresolved"),
                     lambda r: r["functions"][0].update(different_bytes=1),
                     lambda r: r["functions"].append(copy.deepcopy(r["functions"][0]))]
        for mutation in mutations:
            changed = copy.deepcopy(good)
            mutation(changed)
            with self.assertRaises(VerificationError):
                check_regression(changed, good)

    def test_whole_file_includes_headers_and_length(self):
        original = pe(b"\xc3")
        self.assertTrue(compare_whole(original, original)["whole_executable_match"])
        mutated = bytearray(original)
        mutated[0x88] = 1
        for candidate in (bytes(mutated), original + b"\0", original[:-1]):
            self.assertFalse(compare_whole(original, candidate)["whole_executable_match"])


class FormatTests(unittest.TestCase):
    def test_pe_truncation_and_bad_header_fail(self):
        raw = pe(b"\xc3")
        for length in (0, 2, 63, 128, 200, 495, 512, 1024):
            with self.subTest(length=length), self.assertRaises(VerificationError):
                PE(raw[:length])
        for offset, fmt, value in [(0x84, "<H", 0x8664), (0x98, "<H", 0x20b),
                                   (60, "<I", 0xffffffff), (0x98 + 92, "<I", 17)]:
            changed = bytearray(raw)
            struct.pack_into(fmt, changed, offset, value)
            with self.assertRaises(VerificationError):
                PE(bytes(changed))

    def test_pe_overlapping_sections_and_unbacked_va(self):
        raw = bytearray(pe(b"\xc3"))
        struct.pack_into("<I", raw, 0x178 + 40 + 12, 0x1000)
        with self.assertRaises(VerificationError):
            PE(bytes(raw))
        with self.assertRaises(VerificationError):
            PE(pe(b"\xc3")).read_va(0x401001, 1)

    def test_malformed_relocation_blocks_fail(self):
        for at, fmt, value in [(0x604, "<I", 0), (0x604, "<I", 1000),
                               (0x608, "<H", 0x7001), (0x60a, "<H", 0x3001)]:
            raw = bytearray(pe(b"\xa1\0\0\0\0\xc3", [1]))
            struct.pack_into(fmt, raw, at, value)
            with self.subTest(value=value), self.assertRaises(VerificationError):
                PE(bytes(raw))

    def test_coff_truncation_and_bad_symbol(self):
        raw = coff(b"\xc3")
        for length in (0, 19, 59, 60, 65, len(raw) - 1):
            with self.subTest(length=length), self.assertRaises(VerificationError):
                COFF(raw[:length])
        with self.assertRaises(VerificationError):
            COFF(raw).function("_missing")

    def test_missing_or_overlapping_relocations_rejected(self):
        raw = bytearray(coff(b"\xa1\0\0\0\0\xc3", [(1, "_global", 6)]))
        reloc_at = struct.unpack_from("<I", raw, 20 + 24)[0]
        struct.pack_into("<I", raw, reloc_at + 4, 99)
        with self.assertRaises(VerificationError):
            COFF(bytes(raw)).function("_candidate")
        raw = coff(b"\xa1\0\0\0\0\xc3", [(1, "_global", 6), (2, "_global", 6)])
        with self.assertRaises(VerificationError):
            COFF(raw).function("_candidate")


class ManifestAndCLITests(unittest.TestCase):
    def test_real_manifest_is_structurally_valid_without_retail_input(self):
        manifest = load_target(ROOT / "target.json")
        self.assertEqual(sum(s["size"] for s in manifest["functions"]), 593)

    def test_invalid_binding_and_source_paths(self):
        for mutation in (lambda m: m["functions"][0].update(source="../elsewhere.c"),
                         lambda m: m["functions"][0].update(bindings=[{"offset": 1, "address": 0, "symbol": "x"}]),
                         lambda m: m.update(functions=[])):
            manifest = target(pe(b"\xc3"), spec(1))
            mutation(manifest)
            with tempfile.TemporaryDirectory() as directory:
                path = Path(directory) / "target.json"
                path.write_text(json.dumps(manifest), encoding="utf-8")
                with self.assertRaises(VerificationError):
                    load_target(path)

    def test_missing_reference_is_failure_not_skip(self):
        result = subprocess.run([sys.executable, str(ROOT / "verify.py"), "preflight"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 2)
        self.assertIn("BLOCKED", result.stderr)


if __name__ == "__main__":
    unittest.main()
