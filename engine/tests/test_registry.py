"""Negative tests for accepted-match history, expansion, and stale claims."""
import copy
import json
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import PE, VerificationError
from fixtures import coff, pe, spec, target
from matching import canonical_hash, compare_batch, digest
from registry import (load_registry, preserves, read_history, render_progress,
                      validate_event, validate_current_specs)


def event():
    raw = pe(b"\xc3")
    manifest = target(raw, spec(1))
    result = compare_batch(PE(raw), {"sample": coff(b"\xc3")}, manifest)["functions"][0]
    definition = manifest["functions"][0]
    return {"schema": 1, "kind": "verified-function-build", "previous_sha256": None,
            "target_sha256": manifest["sha256"], "toolchain": {"synthetic": True},
            "whole_executable_match": False, "linked_placement_verified": False, "behavior_passed": True,
            "functions": [{"id": "sample", "spec": definition, "spec_sha256": canonical_hash(definition),
                           "result": result, "exceptions": []}]}


class RegistryTests(unittest.TestCase):
    def test_accounting_exceptions_and_linked_claims_cannot_pass(self):
        good = event()
        validate_event(good)
        mutations = [
            lambda e: e.update(whole_executable_match=True),
            lambda e: e.update(behavior_passed=False),
            lambda e: e["functions"][0].update(exceptions=[{"reason": "ignore a byte"}]),
            lambda e: e["functions"][0]["result"].update(compared_bytes=0),
            lambda e: e["functions"][0]["result"].update(different_bytes=1),
            lambda e: e["functions"][0]["result"].update(resolved_span_sha256="changed"),
            lambda e: e["functions"][0]["result"].update(literal_function_match=True),
            lambda e: e["functions"].append(copy.deepcopy(e["functions"][0]))]
        for mutate in mutations:
            changed = copy.deepcopy(good)
            mutate(changed)
            with self.assertRaises(VerificationError):
                validate_event(changed)

    def test_growth_preserves_previous_definitions_and_toolchain(self):
        previous = event()
        expanded = copy.deepcopy(previous)
        added = copy.deepcopy(previous["functions"][0])
        added["id"] = added["spec"]["id"] = added["result"]["id"] = "second"
        added["spec"]["address"] += 16
        added["result"]["address"] += 16
        added["spec_sha256"] = canonical_hash(added["spec"])
        expanded["functions"].append(added)
        validate_event(expanded)
        preserves(previous, expanded)
        for mutate in (lambda e: e.update(functions=[]),
                       lambda e: e.update(toolchain={"different": True}),
                       lambda e: e["functions"][0]["spec"].update(size=2),
                       lambda e: e["functions"][0]["spec"].update(flags=["/O2", "/MT"])):
            changed = copy.deepcopy(expanded)
            mutate(changed)
            with self.assertRaises(VerificationError):
                preserves(previous, changed)

    def test_history_tampering_and_missing_predecessor_fail(self):
        with tempfile.TemporaryDirectory() as directory:
            folder = Path(directory)
            first = event()
            path = folder / ("000001-%s.json" % canonical_hash(first)[:16])
            path.write_text(json.dumps(first))
            self.assertEqual(len(read_history(folder)), 1)
            first["behavior_passed"] = False
            path.write_text(json.dumps(first))
            with self.assertRaises(VerificationError):
                read_history(folder)
            path.unlink()
            second = event()
            second["previous_sha256"] = "a" * 64
            path = folder / ("000001-%s.json" % canonical_hash(second)[:16])
            path.write_text(json.dumps(second))
            with self.assertRaises(VerificationError):
                read_history(folder)

    def test_current_target_cannot_remove_or_change_an_accepted_spec(self):
        previous = event()
        manifest = {"functions": [copy.deepcopy(previous["functions"][0]["spec"])]}
        validate_current_specs([previous], manifest)
        for changed in ({"functions": []}, {"functions": [{**manifest["functions"][0], "size": 2}]}):
            with self.assertRaises(VerificationError):
                validate_current_specs([previous], changed)

    def test_registered_evidence_files_are_required(self):
        manifest = json.loads((ROOT / "target.json").read_text())
        load_registry(ROOT, manifest)
        changed = copy.deepcopy(manifest)
        changed["functions"].pop()
        with self.assertRaises(VerificationError):
            load_registry(ROOT, changed)

    def test_current_source_claim_detects_new_and_changed_files(self):
        from build import source_snapshot
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            for folder in ("src", "tools", "tests"):
                (root / folder).mkdir()
            for name in ("target.json", "tests/behavior.c", "verify.py"):
                (root / name).write_text("synthetic")
            previous = event()
            previous.update(source_snapshot=source_snapshot(root), build_id="synthetic", source_revision="test")
            registry = {"inventory_scope": "Test", "functions": [{"id": "sample", "module": "test", "quirks": [], "exceptions": []}],
                        "open_program_questions": []}
            manifest = {"functions": [previous["functions"][0]["spec"]]}
            self.assertIn("fingerprint agrees with the latest run: **yes**", render_progress(root, manifest, registry, [previous]))
            (root / "tools/new.py").write_text("new code")
            self.assertIn("current source unverified", render_progress(root, manifest, registry, [previous]))
