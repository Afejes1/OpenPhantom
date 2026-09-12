"""Record narrow compiler diagnostics separately from byte-comparison exclusions."""
import re
from formats import require


def validate_source_policy(root, target):
    expected = {}
    for spec in target["functions"]:
        codes = spec.get("diagnostic_waivers", [])
        require(codes in ([], [4725]), "unreviewed compiler diagnostic waiver")
        require(spec["source"] not in expected, "one source file per current function is required")
        expected[spec["source"]] = codes
    for path in (root / "src").rglob("*"):
        if path.suffix not in (".c", ".h"):
            continue
        text = path.read_text(encoding="utf-8")
        code = re.sub(r"/\*.*?\*/|//[^\n]*", "", text, flags=re.S)
        relative = path.relative_to(root).as_posix()
        directives = re.findall(r"^\s*#\s*pragma\s+warning\s*\(([^)]*)\)", code, flags=re.M)
        codes = expected.get(relative, [])
        required = ["disable:4725", "4:4725"] if codes else []
        actual = [re.sub(r"\s+", "", value) for value in directives]
        require(actual == required, "unrecorded or unbalanced warning policy in " + relative)
        require(not re.search(r"\b(?:__pragma|_Pragma)\b", code), "unreviewed diagnostic directive")
        require(not re.search(r"\b(?:_emit|__emit)\b|__declspec\s*\(\s*naked\s*\)", code),
                "raw byte insertion or manufactured function frames are not matching C evidence")
