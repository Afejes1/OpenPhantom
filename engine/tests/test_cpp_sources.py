"""Original C++ frontend use must not bypass provenance or source policy."""
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from build import check_frontends, source_snapshot
from formats import VerificationError
from matching import source_language
from source_policy import validate_source_policy


class CppSourceTests(unittest.TestCase):
    def test_language_is_explicit_and_must_agree_with_extension(self):
        self.assertEqual(source_language({"source": "src/a.c"}), "c")
        self.assertEqual(source_language({"source": "src/a.cpp", "language": "c++"}), "c++")
        for spec in ({"source": "src/a.cpp", "language": "c"}, {"source": "src/a.c", "language": "c++"}, {"source": "src/a.asm"}):
            with self.assertRaises(VerificationError):
                source_language(spec)

    def test_cpp_requires_fingerprinted_original_frontend(self):
        target = {"functions": [{"source": "src/a.cpp", "language": "c++"}]}
        with self.assertRaises(VerificationError):
            check_frontends(target, {"files": {"bin/c1.dll": "synthetic"}})
        check_frontends(target, {"files": {"bin/c1xx.dll": "synthetic"}})

    def test_cpp_changes_invalidate_source_fingerprints(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "target.json").write_text("{}")
            for folder in ("src", "tests"):
                (root / folder).mkdir()
                (root / folder / "a.cpp").write_text("// original synthetic input")
            before = source_snapshot(root)
            self.assertIn("src/a.cpp", before)
            self.assertIn("tests/a.cpp", before)
            for folder in ("src", "tests"):
                path = root / folder / "a.cpp"
                path.write_text("// changed synthetic input")
                self.assertNotEqual(before[folder + "/a.cpp"], source_snapshot(root)[folder + "/a.cpp"])

    def test_cpp_cannot_hide_raw_emission_or_unrecorded_diagnostics(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "src").mkdir()
            path = root / "src/a.cpp"
            target = {"functions": [{"source": "src/a.cpp"}]}
            for code in ("__asm _emit 0x90", "__declspec(naked) void a() {}", "#pragma warning(disable:4725)\n#pragma warning(4:4725)"):
                path.write_text(code)
                with self.assertRaises(VerificationError):
                    validate_source_policy(root, target)
            path.write_text('extern "C" void a() {}')
            validate_source_policy(root, target)
