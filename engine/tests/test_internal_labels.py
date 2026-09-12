"""Synthetic embedded-table checks; no game bytes or executable execution."""
import copy
from pathlib import Path
import struct
import sys
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import COFF, PE, VerificationError
from matching import canonical_hash, compare_function, validate_bindings, verify_reference
from registry import validate_event
from fixtures import coff, pe, spec, target


def label_object(code, relocations=((3, "_table", 6), (8, "_case", 6)),
                 table_value=8, section=1, storage=6, kind=0):
    raw = bytearray(coff(code, relocations, {"_unused": b"\0" * 4}))
    symbols_at = struct.unpack_from("<I", raw, 8)[0]
    for index, symbol in COFF(bytes(raw)).symbols.items():
        if symbol.name in ("_table", "_case"):
            value = table_value if symbol.name == "_table" else 7
            struct.pack_into("<IhHB", raw, symbols_at + index * 18 + 8,
                             value, section, kind, storage)
    return bytes(raw)


class InternalLabelTests(unittest.TestCase):
    def case(self):
        actual = b"\xff\x24\x85" + b"\0" * 4 + b"\xc3" + b"\0" * 4 + b"\x90" * 4
        expected = b"\xff\x24\x85" + struct.pack("<I", 0x401008) + b"\xc3"
        expected += struct.pack("<I", 0x401007) + b"\x90" * 4
        raw = pe(expected, [3, 8])
        definition = spec(16, [
            {"offset": 3, "address": 0x401008, "kind": "dir32-internal", "target_offset": 8},
            {"offset": 8, "address": 0x401007, "kind": "dir32-internal", "target_offset": 7}])
        definition["body_size"] = 8
        return raw, actual, definition

    def test_complete_table_and_padding_match(self):
        raw, actual, definition = self.case()
        original = verify_reference(raw, target(raw, definition))
        result = compare_function(original, COFF(label_object(actual)), definition)
        self.assertEqual(result["status"], "relocation-adjusted-match")
        self.assertEqual(result["internal_dir32_bytes"], 8)
        self.assertEqual(result["dir32_bytes"], 8)
        self.assertEqual(result["compared_bytes"], 8)
        self.assertEqual(result["reference_span_sha256"], result["resolved_span_sha256"])
        self.assertFalse(result["layout_verified"])

    def test_internal_label_identity_cannot_be_substituted(self):
        raw, actual, definition = self.case()
        for changes in ({"table_value": 9}, {"section": 0}, {"section": 2},
                        {"storage": 2}, {"storage": 3}, {"kind": 0x20}):
            with self.subTest(changes=changes), self.assertRaises(VerificationError):
                compare_function(PE(raw), COFF(label_object(actual, **changes)), definition)

    def test_wrong_relocation_kind_addend_or_inventory_fails(self):
        raw, actual, definition = self.case()
        for relocations in (((3, "_table", 20), (8, "_case", 6)), ((3, "_table", 6),),
                            ((3, "_table", 6), (8, "_case", 6), (12, "_case", 6))):
            with self.subTest(relocations=relocations), self.assertRaises(VerificationError):
                compare_function(PE(raw), COFF(label_object(actual, relocations)), definition)
        wrong = bytearray(actual)
        struct.pack_into("<I", wrong, 3, 1)
        with self.assertRaises(VerificationError):
            compare_function(PE(raw), COFF(label_object(bytes(wrong))), definition)

    def test_bad_manifest_destination_and_ambiguous_binding_fail(self):
        _, _, definition = self.case()
        for change in ({"target_offset": -1}, {"target_offset": 16}, {"target_offset": True},
                       {"address": 0x401009}, {"addend": 1}, {"symbol": "_wrong"},
                       {"kind": "dir32"}, {"kind": "rel32-call"}):
            bad = copy.deepcopy(definition)
            bad["bindings"][0].update(change)
            with self.subTest(change=change), self.assertRaises(VerificationError):
                validate_bindings(bad)

    def test_original_target_and_base_relocations_are_required(self):
        raw, _, definition = self.case()
        for offset, value in ((0x203, 0x401009), (0x208, 0x401006)):
            changed = bytearray(raw)
            struct.pack_into("<I", changed, offset, value)
            with self.assertRaises(VerificationError):
                verify_reference(bytes(changed), target(bytes(changed), definition))
        original = PE(raw)
        original.relocations.remove(0x401008)
        _, actual, _ = self.case()
        with self.assertRaises(VerificationError):
            compare_function(original, COFF(label_object(actual)), definition)

    def test_instruction_padding_and_section_length_are_not_excluded(self):
        raw, actual, definition = self.case()
        changed_instruction = b"\xfe" + actual[1:]
        changed_padding = actual[:-1] + b"\xcc"
        for code in (changed_instruction, changed_padding, actual[:-1], actual + b"\x90"):
            with self.subTest(code=code):
                result = compare_function(PE(raw), COFF(label_object(code)), definition)
                self.assertEqual(result["status"], "mismatch")

    def test_history_retains_internal_operand_accounting(self):
        raw, actual, definition = self.case()
        result = compare_function(PE(raw), COFF(label_object(actual)), definition)
        event = {"schema": 1, "kind": "verified-function-build", "whole_executable_match": False,
                 "linked_placement_verified": False, "behavior_passed": True,
                 "functions": [{"id": "sample", "spec": definition,
                                "spec_sha256": canonical_hash(definition), "result": result,
                                "exceptions": []}]}
        validate_event(event)
        bad = copy.deepcopy(event)
        bad["functions"][0]["result"]["internal_dir32_bytes"] = 0
        with self.assertRaises(VerificationError):
            validate_event(bad)


if __name__ == "__main__":
    unittest.main()
