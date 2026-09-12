# Legacy camera-space scan step

Function 0x00403EF0, originally FUN_00403ef0, is reported as
bp/bapdraw.c / bapdrawOld_stepScan / not done in the team's supplied manifest.
Our descriptive name is step_camera_space_grid_position. Neither name is
claimed to be an original recovered symbol.

The complete body ends with RET at 0x00403F9C (173 bytes), followed by three
NOP bytes before the next function at 0x00403FA0. The verifier compares all
176 bytes. Twelve DIR32 operands reference the two basis vectors; each binding
checks the original relocation, the reconstructed symbol and its 0/4/8 addend.

## Static evidence

The three cdecl arguments are a writable float triplet and two signed integer
directions. Only direction +1 adds the selected basis and -1 subtracts it.
Zero and every other integer skip that axis. X acts before Y; each component
is stored as float after X before Y reads it again. If both directions are
ignored the position is never dereferenced. There is no active-path null check.

The first basis occupies 0x008BE9EC/0x008BE9F0/0x008BE9F4; the second occupies
0x008BE9F8/0x008BE9FC/0x008BEA00. The setup routine at 0x00404180 copies twelve
32-bit words from camera+8 into the transform starting at 0x008BE9EC. The world
renderer at 0x00404730 passes that transform to transform_affine_point, then
uses this stepper to advance the camera-space position during grid traversal.

Returned incoming XREFs identify calls at 0x004044AE (setup), 0x00404AAC and
0x00404C06 (world rendering). Setup supplies a scan iterator's two unit-step
values; the world renderer supplies changes in grid coordinates. The neighboring
culler at 0x00403FA0 also uses both basis triplets to select cell corners.
The complete stepper has no calls, imported APIs or string references.

## Matching source choices

[src/step_scan.c](../src/step_scan.c) uses VC5 RTM's C frontend with /O2 /MT.
Ordinary float-only expressions produced a different x87 schedule. Explicit
double promotion on coordinates 1 and 2 of the positive paths reproduces the
observed load/add/store order, while the first coordinate and subtraction paths
retain ordinary float expressions. These are source choices supported by the
compiled output, not evidence of historical declarations or source spelling.

No inline assembly, byte exclusions, diagnostic waiver or changed comparison
extent is needed. The first isolated object comparison matched all 176 bytes
after the twelve checked address resolutions. Acceptance and source freshness
are determined by the full-build records and generated progress report.

## Behavioral checks and limits

[scan_behavior.h](../tests/scan_behavior.h) covers all 49 combinations of seven
directions including both signed integer extremes, distinct fractional bases,
unchanged sentinels and basis storage, and null position on both-inactive paths.
A cancellation case distinguishes the intermediate float store and X-before-Y
order. Full aliasing with basis storage verifies that Y sees X's earlier writes.
Additional cases cover preserved negative zero/NaN payloads on inactive paths,
nonfinite arithmetic, and unchanged x87 control and stack depth.

Only newly compiled synthetic fixtures run. The original game is inspected
statically. Matching this helper does not establish the full world traversal,
its historical translation unit, retail linked placement, or complete executable.
