"""Synthetic tests for verified near calls and double-precision constants."""
import copy
import json
from pathlib import Path
import struct
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import COFF, PE, VerificationError
from matching import (canonical_hash, compare_function, load_target,
                      verify_reference)
from registry import validate_event
from fixtures import coff, pe, spec, target


class RelativeCallTests(unittest.TestCase):
    def case(self, backward=False, next_instruction=False):
        address = 0x401010 if backward else 0x401000
        called = 0x401000 if backward else address + (5 if next_instruction else 6)
        displacement = (called - address - 5) & 0xffffffff
        body = b"\xe8" + struct.pack("<I", displacement) + b"\xc3"
        raw = pe((b"\xc3" + b"\x90" * 15 + body) if backward else body + b"\xc3")
        definition = spec(6, [{"offset": 1, "address": called,
                               "symbol": "_helper", "kind": "rel32-call"}])
        definition["address"] = address
        obj = coff(b"\xe8\0\0\0\0\xc3", [(1, "_helper", 20)])
        return raw, obj, definition

    def test_forward_and_backward_calls_resolve_without_base_relocations(self):
        for backward in (False, True):
            raw, obj, definition = self.case(backward=backward)
            original = verify_reference(raw, target(raw, definition))
            result = compare_function(original, COFF(obj), definition)
            self.assertEqual(result["status"], "relocation-adjusted-match")
            self.assertEqual(result["rel32_call_bytes"], 4)
            self.assertEqual(result["dir32_bytes"], 0)
            self.assertEqual(result["compared_bytes"], 2)
            self.assertEqual(result["reference_span_sha256"], result["resolved_span_sha256"])
            self.assertFalse(result["layout_verified"])

    def test_wrong_symbol_kind_addend_and_opcode_fail(self):
        raw, _, definition = self.case()
        cases = [
            coff(b"\xe8\0\0\0\0\xc3", [(1, "_wrong", 20)]),
            coff(b"\xe8\0\0\0\0\xc3", [(1, "_helper", 6)]),
            coff(b"\xe8\1\0\0\0\xc3", [(1, "_helper", 20)]),
            coff(b"\xe9\0\0\0\0\xc3", [(1, "_helper", 20)]),
            coff(b"\xe8\0\0\0\0\xc3"),
        ]
        for obj in cases:
            with self.subTest(obj=obj), self.assertRaises(VerificationError):
                compare_function(PE(raw), COFF(obj), definition)

    def test_wrong_reference_target_opcode_or_noncode_destination_fail(self):
        raw, _, definition = self.case()
        changed = copy.deepcopy(definition)
        changed["bindings"][0]["address"] += 1
        with self.assertRaises(VerificationError):
            verify_reference(raw, target(raw, changed))
        for opcode, called in [(0xe9, 0x401006), (0xe8, 0x402000), (0xe8, 0x401100)]:
            changed = copy.deepcopy(definition)
            changed["bindings"][0]["address"] = called
            body = bytes([opcode]) + struct.pack("<I", called - 0x401005) + b"\xc3\xc3"
            other = pe(body)
            with self.assertRaises(VerificationError):
                verify_reference(other, target(other, changed))

    def test_relative_call_must_not_have_or_overlap_a_base_relocation(self):
        _, obj, definition = self.case()
        body = b"\xe8\1\0\0\0\xc3\xc3\0"
        for at in (0, 1, 2, 3, 4):
            raw = pe(body, [at])
            with self.subTest(at=at), self.assertRaises(VerificationError):
                verify_reference(raw, target(raw, definition))
            with self.assertRaises(VerificationError):
                compare_function(PE(raw), COFF(obj), definition)

    def test_mixed_absolute_and_relative_operands_are_counted_separately(self):
        body = b"\xa1" + struct.pack("<I", 0x402000) + b"\xe8\1\0\0\0\xc3"
        raw = pe(body + b"\xc3", [1])
        definition = spec(11, [
            {"offset": 1, "address": 0x402000, "symbol": "_global"},
            {"offset": 6, "address": 0x40100b, "symbol": "_helper", "kind": "rel32-call"}])
        obj = coff(b"\xa1\0\0\0\0\xe8\0\0\0\0\xc3",
                   [(1, "_global", 6), (6, "_helper", 20)])
        result = compare_function(verify_reference(raw, target(raw, definition)), COFF(obj), definition)
        self.assertEqual((result["dir32_bytes"], result["rel32_call_bytes"]), (4, 4))
        self.assertEqual(result["adjusted_bytes"], 8)
        self.assertEqual(result["compared_bytes"], 3)

    def test_unbound_instruction_and_length_changes_still_fail(self):
        raw, _, definition = self.case()
        for body in (b"\xe8\0\0\0\0\xcc", b"\xe8\0\0\0\0\xc3\x90"):
            result = compare_function(PE(raw), COFF(coff(body, [(1, "_helper", 20)])), definition)
            self.assertEqual(result["status"], "mismatch")

    def test_unknown_binding_kinds_and_ambiguous_constants_are_rejected(self):
        raw, _, definition = self.case()
        cases = [
            {"kind": "ignore"},
            {"kind": "rel32-jump"},
            {"addend": 1},
            {"addend": -1},
            {"float64": 255.0},
            {"offset": 0},
        ]
        for mutation in cases:
            changed = copy.deepcopy(definition)
            changed["bindings"][0].update(mutation)
            with tempfile.TemporaryDirectory() as directory:
                path = Path(directory) / "target.json"
                path.write_text(json.dumps(target(raw, changed)))
                with self.subTest(mutation=mutation), self.assertRaises(VerificationError):
                    load_target(path)

    def test_relative_call_history_preserves_accounting_and_raw_zero_displacement(self):
        for next_instruction in (False, True):
            raw, obj, definition = self.case(next_instruction=next_instruction)
            result = compare_function(PE(raw), COFF(obj), definition)
            event = {"schema": 1, "kind": "verified-function-build", "whole_executable_match": False,
                     "linked_placement_verified": False, "behavior_passed": True,
                     "functions": [{"id": "sample", "spec": definition,
                                    "spec_sha256": canonical_hash(definition), "result": result,
                                    "exceptions": []}]}
            validate_event(event)
            self.assertEqual(result["raw_bytes_equal"], next_instruction)
            for field, value in [("rel32_call_bytes", 0), ("dir32_bytes", 4),
                                 ("raw_bytes_equal", not next_instruction)]:
                bad = copy.deepcopy(event)
                bad["functions"][0]["result"][field] = value
                with self.assertRaises(VerificationError):
                    validate_event(bad)



class RecursiveCallTests(unittest.TestCase):
    def case(self, padding=b"\x90\x90"):
        body = b"\xe8" + struct.pack("<I", 0xfffffffb) + b"\xc3" + padding
        raw = pe(body)
        definition = spec(len(body), [{"offset": 1, "address": 0x401000,
                                      "symbol": "_candidate", "kind": "rel32-call"}])
        definition["body_size"] = 6
        obj = coff(b"\xe8\0\0\0\0\xc3" + padding, [(1, "_candidate", 20)])
        return raw, obj, definition

    def test_defined_self_call_resolves_and_preserves_full_span(self):
        raw, obj, definition = self.case()
        original = verify_reference(raw, target(raw, definition))
        result = compare_function(original, COFF(obj), definition)
        self.assertEqual(result["status"], "relocation-adjusted-match")
        self.assertEqual((result["size"], result["body_size"]), (8, 6))
        self.assertEqual((result["compared_bytes"], result["rel32_call_bytes"]), (4, 4))
        self.assertEqual(result["different_bytes"], 0)
        self.assertEqual(result["reference_span_sha256"], result["resolved_span_sha256"])
        self.assertFalse(result["layout_verified"])

    def test_same_symbol_cannot_redirect_to_interior_or_another_address(self):
        raw, obj, definition = self.case()
        for address in (0x401001, 0x401005, 0x401007):
            changed = copy.deepcopy(definition)
            changed["bindings"][0]["address"] = address
            body = b"\xe8" + struct.pack("<I", (address - 0x401005) & 0xffffffff) + b"\xc3\x90\x90"
            raw = pe(body)
            original = verify_reference(raw, target(raw, changed))
            with self.subTest(address=address), self.assertRaises(VerificationError):
                compare_function(original, COFF(obj), changed)

    def test_self_call_wrong_symbol_opcode_addend_kind_or_missing_fixup_fail(self):
        raw, _, definition = self.case()
        for code, relocs in [
            (b"\xe8\0\0\0\0\xc3\x90\x90", [(1, "_other", 20)]),
            (b"\xe9\0\0\0\0\xc3\x90\x90", [(1, "_candidate", 20)]),
            (b"\xe8\1\0\0\0\xc3\x90\x90", [(1, "_candidate", 20)]),
            (b"\xe8\0\0\0\0\xc3\x90\x90", [(1, "_candidate", 6)]),
            (b"\xe8\0\0\0\0\xc3\x90\x90", []),
        ]:
            with self.subTest(code=code, relocs=relocs), self.assertRaises(VerificationError):
                compare_function(PE(raw), COFF(coff(code, relocs)), definition)

    def test_self_symbol_must_remain_defined_external_function_at_zero(self):
        raw, obj, definition = self.case()
        sym_at = struct.unpack_from("<I", obj, 8)[0]
        for offset, fmt, value in [(8, "<I", 1), (12, "<h", 0),
                                   (12, "<h", 2), (14, "<H", 0), (16, "<B", 3)]:
            changed = bytearray(obj)
            struct.pack_into(fmt, changed, sym_at + offset, value)
            with self.subTest(offset=offset, value=value), self.assertRaises(VerificationError):
                compare_function(PE(raw), COFF(bytes(changed)), definition)

    def test_self_call_does_not_hide_instruction_padding_or_extent_changes(self):
        raw, _, definition = self.case()
        for code in (b"\xe8\0\0\0\0\xcc\x90\x90",
                     b"\xe8\0\0\0\0\xc3\x90\xcc",
                     b"\xe8\0\0\0\0\xc3"):
            result = compare_function(PE(raw), COFF(coff(code, [(1, "_candidate", 20)])), definition)
            self.assertEqual(result["status"], "mismatch")

    def test_external_call_rule_does_not_accept_a_defined_self_symbol(self):
        raw, obj, definition = self.case()
        definition["bindings"][0]["symbol"] = "_helper"
        with self.assertRaises(VerificationError):
            compare_function(PE(raw), COFF(obj), definition)


class DoubleConstantTests(unittest.TestCase):
    def case(self, original_value=255.0, candidate_value=255.0):
        definition = spec(7, [{"offset": 2, "address": 0x402000, "float64": 255.0}])
        raw = pe(b"\xdc\x0d" + struct.pack("<I", 0x402000) + b"\xc3",
                 [2], struct.pack("<d", original_value))
        obj = coff(b"\xdc\x0d\0\0\0\0\xc3", [(2, "_double", 6)],
                   {"_double": struct.pack("<d", candidate_value)})
        return raw, obj, definition

    def test_all_eight_constant_bytes_are_checked(self):
        raw, obj, definition = self.case()
        result = compare_function(verify_reference(raw, target(raw, definition)), COFF(obj), definition)
        self.assertEqual(result["status"], "relocation-adjusted-match")
        self.assertEqual(result["adjusted_bytes"], 4)
        for original, candidate in [(254.0, 255.0), (255.0, 254.0)]:
            raw, obj, definition = self.case(original, candidate)
            with self.assertRaises(VerificationError):
                compare_function(PE(raw), COFF(obj), definition)

    def test_truncated_or_ambiguous_double_definitions_fail(self):
        raw, _, definition = self.case()
        short = coff(b"\xdc\x0d\0\0\0\0\xc3", [(2, "_double", 6)],
                     {"_double": struct.pack("<f", 255.0)})
        with self.assertRaises(VerificationError):
            compare_function(PE(raw), COFF(short), definition)
        definition["bindings"][0]["float32"] = 255.0
        with self.assertRaises(VerificationError):
            verify_reference(raw, target(raw, definition))
