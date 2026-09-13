"""Synthetic checks for the explicit Docker listing fallback; no game inputs."""
import json
import struct
import sys
import tempfile
import unittest
from pathlib import Path
from unittest.mock import patch

import verify
sys.path.insert(0, str(verify.ROOT / "tests"))
from fixtures import coff


class NativeListingFallbackTests(unittest.TestCase):
    def setUp(self):
        (verify.ROOT / "build").mkdir(exist_ok=True)
        self.temp = tempfile.TemporaryDirectory(prefix="listing-fallback-", dir=verify.ROOT / "build")
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.native = self.root / "native"
        self.docker = self.root / "docker"
        self.native.mkdir()
        self.docker.mkdir()
        self.code = b"\xa1\0\0\0\0\xc3"
        self.obj = self.docker / "candidate.obj"
        self.obj.write_bytes(coff(self.code, [(1, "_global", 6)]))
        (self.native / "candidate.obj").write_bytes(self.obj.read_bytes())
        self.listing = self.docker / "candidate.cod"
        self.listing.write_bytes(b"")
        (self.native / "candidate.cod").write_text(
            "_candidate PROC NEAR\n mov eax, DWORD PTR _global\n ret 0\n_candidate ENDP\nEND\n")
        self.spec = {"id": "candidate", "symbol": "_candidate"}
        artifact = verify.capture_artifact("candidate.c", self.spec,
                    self.native / "candidate.obj", self.native / "candidate.cod")
        self.peer = {"completed": True, "runner": "native", "artifacts": [artifact]}
        self.peer_path = self.native / "research-result.json"
        self.peer_path.write_text(json.dumps(self.peer))

    def select(self, **kwargs):
        options = dict(runner="docker", allow_fallback=True, peer=self.peer, peer_path=self.peer_path)
        options.update(kwargs)
        return verify.select_listing(self.listing, self.obj, "_candidate", **options)

    def test_matching_code_uses_complete_native_listing_and_retains_empty_original(self):
        changed = bytearray(self.obj.read_bytes())
        struct.pack_into("<I", changed, 4, 12345)
        self.obj.write_bytes(changed)
        selected, note = self.select()
        self.assertEqual(self.listing.read_bytes(), b"")
        self.assertEqual(selected.read_bytes(), (self.native / "candidate.cod").read_bytes())
        self.assertEqual(note["native_peer_report_sha256"], verify.digest(self.peer_path.read_bytes()))
        self.assertTrue(note["function_and_relocations_equal_before_fixture"])
        artifact = verify.capture_artifact("candidate.c", self.spec, self.obj, selected)
        artifact["listing_fallback"] = note
        verify.verify_artifacts(self.docker, [artifact])
        self.listing.write_bytes(b"changed")
        with self.assertRaisesRegex(verify.VerificationError, "empty Docker listing changed"):
            verify.verify_artifacts(self.docker, [artifact])

    def test_default_and_native_mode_still_reject_empty_listing(self):
        for options in ({"allow_fallback": False}, {"runner": "native"}):
            with self.subTest(options=options):
                with self.assertRaisesRegex(verify.VerificationError, "listing is empty"):
                    self.select(**options)

    def test_missing_incomplete_or_docker_peer_is_rejected(self):
        for peer in (None, dict(self.peer, completed=False), dict(self.peer, runner="docker")):
            with self.subTest(peer=peer):
                with self.assertRaisesRegex(verify.VerificationError, "complete native peer"):
                    self.select(peer=peer)

    def test_code_padding_and_fixup_changes_are_rejected(self):
        variants = [coff(self.code + b"\x90", [(1, "_global", 6)]),
                    coff(b"\xa1\0\0\0\0\x90", [(1, "_global", 6)]),
                    coff(self.code, [(1, "_other", 6)])]
        for data in variants:
            with self.subTest(data=data):
                self.obj.write_bytes(data)
                with self.assertRaisesRegex(verify.VerificationError, "code or relocations differ"):
                    self.select()

    def test_truncated_native_listing_is_rejected_even_with_updated_hash(self):
        p = self.native / "candidate.cod"
        p.write_text("_candidate PROC NEAR\n ret 0\n")
        self.peer["artifacts"][0] = verify.capture_artifact("candidate.c", self.spec,
                                                        self.native / "candidate.obj", p)
        with self.assertRaisesRegex(verify.VerificationError, "native peer listing is incomplete"):
            self.select()

    def test_mutated_native_artifacts_are_rejected(self):
        p = self.native / "candidate.cod"
        p.write_bytes(p.read_bytes() + b"changed")
        with self.assertRaisesRegex(verify.VerificationError, "artifact changed"):
            self.select()

    def test_whitespace_is_not_a_zero_byte_listing(self):
        self.listing.write_bytes(b" \n")
        with self.assertRaisesRegex(verify.VerificationError, "zero-byte Docker artifact"):
            self.select()

    def test_nonempty_docker_listing_keeps_existing_handling(self):
        self.listing.write_bytes(b"partial compiler listing")
        selected, note = self.select(peer=None)
        self.assertEqual(selected, self.listing)
        self.assertIsNone(note)

    def test_fallback_mode_requires_peer_before_loading_tools(self):
        with patch.object(verify, "load_config") as config:
            for argv in (["--runner", "native", "--expect-report", "x"], ["--runner", "docker"]):
                with patch.object(sys, "argv", ["verify.py", "--case", "unused", "--native-listing-fallback", *argv]):
                    with self.assertRaisesRegex(verify.VerificationError, "requires Docker and --expect-report"):
                        verify.main()
            config.assert_not_called()


if __name__ == "__main__":
    unittest.main()
