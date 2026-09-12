import copy
import json
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))
from formats import VerificationError
from source_policy import validate_source_policy


class DiagnosticPolicyTests(unittest.TestCase):
    def test_only_the_recorded_balanced_diagnostic_is_allowed(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "src").mkdir()
            path = root / "src/kernel.c"
            target = {"functions": [{"source": "src/kernel.c", "diagnostic_waivers": [4725]}]}
            valid = "#pragma warning(disable: 4725)\nvoid kernel(void) {}\n#pragma warning(4: 4725)\n"
            path.write_text(valid)
            validate_source_policy(root, target)
            for invalid in (valid.replace("4725", "4100"), valid.replace("warning(4:", "warning(default:"), valid.split("#pragma warning(4")[0],
                            valid.replace("4725)", "4725 4100)"),
                            valid + "\n__declspec(naked) void raw(void) {}",
                            valid + "\n__asm _emit 0x90"):
                path.write_text(invalid)
                with self.assertRaises(VerificationError):
                    validate_source_policy(root, target)

    def test_suppression_without_target_evidence_is_rejected(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "src").mkdir()
            (root / "src/kernel.c").write_text("#pragma warning(disable: 4725)\n#pragma warning(4: 4725)\n")
            with self.assertRaises(VerificationError):
                validate_source_policy(root, {"functions": [{"source": "src/kernel.c"}]})

    def test_real_source_and_metadata_agree(self):
        target = json.loads((ROOT / "target.json").read_text())
        validate_source_policy(ROOT, target)
        modified = copy.deepcopy(target)
        modified["functions"][-1]["diagnostic_waivers"] = [4100]
        with self.assertRaises(VerificationError):
            validate_source_policy(ROOT, modified)
