# Legacy frame scan setup

The team reports `bapdrawOld_frameSetup` in `bp/bapdraw.c` as not done. Static
analysis identifies the retail function at `0x00404180`. Its body ends at the
RET at `0x00404728` (1,449 bytes), followed by seven alignment bytes before
`0x00404730`. The strict target retains all 1,456 bytes.

`candidate.c` is an authored reconstruction of camera-state copying, viewport
bounds, octant flags, direction and scan limits, scan-line traversal, the 511
coordinate offsets and reset sentinels. `frame.h` is a partial layout. The
focused fixture uses authored trig/line/step stubs and independent state checks;
it does not execute the original helpers or rendering backend.

## Current comparison

Campaign 002 completed the original-side inventory: 144 absolute operands and
nine direct calls, with all constants and symbolic base/addend mappings reviewed.
The coordinator caught and corrected the octant table's biased pointer: its
base is `0x004AA080`, with addend 16 for the original `0x004AA090` operand.
[Inventory evidence](../campaign-002/frame-binding-evidence.md) records the
mapping, and `target.json` is now enabled by `case.json`.

The strict comparison reports **complete function extent differs**: the candidate
emits 1,376 bytes and 135 COFF relocations, versus the original 1,456-byte target
and 153 bound operands. It stops before comparing candidate operand locations.
There is no justified byte-similarity percentage or exact-match claim.

The final native/Docker code and relocation inventories agree. The focused
Docker fixture passes 6,548 assertions. [The current receipt](result.json)
pins source, tools, target and private artifacts. The campaign-001 result at
commit `3c8f6752ab810cfb6eacd6a5e5bce221c65437b1` remains historical evidence.

From the repository root:

```text
python engine/research/campaign-001/verify.py --case engine/research/campaign-001-frame-setup --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-001-frame-setup --runner docker --behavior --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --expect-report NATIVE_REPORT
```

Both commands return 1 for the strict extent mismatch, even though compilation
and the requested fixture succeed. Native mode compiles only. Execution remains
inside locked Docker; original bytes are read statically. Original x87 precision
boundaries, exact source layout and integrated renderer behavior remain open.
