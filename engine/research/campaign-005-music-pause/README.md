# Pause music

The independently authored C implementation matches the **complete 60-byte
original interval**, with **8 verified address operands and zero differing
bytes** after address resolution. All **36 focused assertions pass**.
Native and locked-Docker emitted code and relocation inventories agree.

See [candidate.c](candidate.c), [api.h](api.h), [the authored fixture](behavior.c),
[the prepared evidence](packet.md), [the immutable target](target.json),
[the final receipt](result.json) and [all worker attempts](worker-log.json).
The coordinator independently reviewed the complete body and callback contracts.
Final review details and fixture corrections are in [the batch review](../campaign-005/review.json).

The worker used 1 candidate compiler invocation(s). The coordinator
then used one fresh native build and one fresh Docker build with the final fixture.
The Docker listing was empty; the existing explicit native-peer fallback supplied
the listing only after verifying identical inputs, toolchain, code and fixups.
The empty original artifact and both listing hashes remain in the receipt. No
byte exclusion, warning waiver or target adjustment was introduced.

Authored import-slot and backend callbacks only; actual DLL/device pause, concurrency and signed overflow are not tested.
This is a focused function match awaiting the separate accepted-history checkpoint;
final linked executable layout and whole-game behavior remain unverified.

Reproduce with the pinned VC5 toolchain and an owned original reference:

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-005-music-pause --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-005-music-pause --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path> --native-listing-fallback
```

The successful final pair returns zero. Only the authored fixture is executed,
in locked Docker. No original game or imported backend code is executed.
