# Green diagnostic segment wrapper

Readable C copies two XYZ triples into a natural two-element local array and
submits count 2, color 0xFF00FF00, null material and width 8 through the polyline
callback. The coordinator formatted the authored fixture for review, then
independently rebuilt both native and Docker outputs. All 26 focused checks
pass, including exact coordinate bits, aliased endpoints, signed zero, quiet
NaN payloads, callback arguments and unchanged input data.

The [receipt](result.json) separates two claims. The entire 76-byte body matches
after resolving its single call operand. The full target remains 77 bytes,
including the original INT3 alignment byte; its strict comparison still fails.
No trap instruction, artificial padding or shortened target was introduced.
No full-span similarity percentage or accepted-function claim is made.

The Docker compiler's empty listing is retained. The explicit fallback uses a
complete native listing only after identical source/toolchain provenance and
complete function/fixup equality. Worker failures remain in worker-log.json.

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-diagnostic-segment --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-003-diagnostic-segment --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path> --native-listing-fallback
```

Both commands return 1 for the full-span mismatch. The separate body diagnostic
uses the established queue-flush method: verify the full target, compare the
complete original body in a separate diagnostic view, and retain the full-span
failure. It does not mutate target.json or acceptance history. Reproduce after
replacing the build-directory placeholder with a completed run:

```python
import json
import sys
from pathlib import Path
sys.path.insert(0, "engine/tools")
from matching import verify_reference, compare_function
from formats import COFF
spec = json.loads(Path("engine/research/campaign-003-diagnostic-segment/target.json").read_text())
identity = json.loads(Path("engine/target.json").read_text())
identity["functions"] = [spec]
original = verify_reference(Path("C:/Dev/Ghidra/projects/TPM/WMAIN.EXE").read_bytes(), identity)
obj = COFF(Path("<private-build-directory>/candidate.obj").read_bytes())
print("Full span:", compare_function(original, obj, spec))
print("Body diagnostic only:", compare_function(original, obj, dict(spec, size=spec["body_size"])))
```
