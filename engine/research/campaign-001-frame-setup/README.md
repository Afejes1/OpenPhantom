# Legacy frame scan setup campaign

The team reports `bapdrawOld_frameSetup` in `bp/bapdraw.c` as not done. Static
analysis identifies the retail function at `0x00404180`. Its body ends at the
`RET` at `0x00404728` (1,449 bytes), followed by seven observed `NOP` alignment
bytes before the next function at `0x00404730`. The pending target deliberately
keeps the strict 1,456-byte span. It does not shrink the target or insert raw
bytes.

`candidate.c` is an authored readable reconstruction. It copies the active
camera transform and signed viewport bounds, expands an eight-value octant row
into sixteen flags, derives two fixed-point direction integers, applies the
pitch-dependent forward/backward scan limits, initializes and advances the
scan line, fills 255 positive and 255 negative offsets around a zero center,
and resets the frame scan sentinels. `frame.h` records only partial layouts.

`behavior.c` is a synthetic fixture with independent constants. It checks the
copy and signed conversions, octant expansion, fixed-point line endpoints,
pitch normalization branches, limit clamping, callback arguments, all 511
offset slots and final sentinel values. It executes no original game code or
rendering backend.

The binding inventory is intentionally empty and therefore pending. `case.json` leaves `target` null and retains `target.json` as `reference_layout`, so the strict matcher cannot consume an incomplete binding specification. The retail
body references many globals, helper calls and floating constants; those
bindings have not yet received an independent complete inventory review. The
common runner should report strict comparison as not attempted rather than treating this
file as an accepted target. No byte masks, exclusions, waivers, registry edits
or accepted-history changes are present.

From the engine directory, use the common campaign runner:

    python research/campaign-001/verify.py --case research/campaign-001-frame-setup --runner native
    python research/campaign-001/verify.py --case research/campaign-001-frame-setup --runner docker --behavior
    python research/campaign-001/verify.py --case research/campaign-001-frame-setup --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE

Native mode compiles only. Fixture linking and execution are restricted to the
locked Docker runner. The last command reads the original statically for reference-layout diagnostics;
strict comparison remains not attempted until the complete binding inventory is reviewed.

## Reviewed pilot checkpoint

The final coordinator fixture passes 6,548 assertions, including raw transform
payload copies, callback-visible line steps, all offset slots, ratio branches
and octant boundaries. Native and Docker emit identical function code and
relocations: 1,376 bytes and 135 relocations. This is not an original-byte match.
[The receipt](result.json) pins the final source and private report hashes.
x87 intermediate precision and integrated behavior remain unverified.
