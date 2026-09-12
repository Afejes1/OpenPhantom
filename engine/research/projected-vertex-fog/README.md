# Unverified projected-vertex fog candidate

Address 0x00402046, team module bp/bapdraw.c, team name bapdraw_commitVerts.
Status: in progress, not accepted and not part of the verified build target.

The C candidate passes the previously exercised synthetic behavior cases but
has not reproduced the complete original instruction sequence. Its double one
constant produces a QWORD reciprocal operand where the original uses DWORD;
local stack slots also differ. An inline-assembly experiment added unwanted
VC5 callee-save instructions and was discarded. No waiver or byte exclusion
was accepted for this candidate.

Retain candidate.c, fog_behavior.h, the exact pending target/registry entries,
and integration.patch for resuming this work. The patch captures the previous
header, test and target wiring; review it before applying to a later baseline.
These files are excluded from the accepted build, source-annotation scan and
synthetic behavior fixture. See ../../docs/vertex-fog-evidence.md for analysis.

The gathered-surface wrapper and frame setup are now accepted separately.
Further original-compiler probes confirmed that neither C++ nor /Op- or /Oa
fixes the reciprocal width; a float reciprocal, including a comma-expression
variant, still reverses the load/divide operands. No new candidate was accepted.
These targeted experiments leave the same constant-width and stack-slot work
outstanding. Continue other team-not-done functions in bp/bapdraw.c while keeping
this candidate visible in the work order.
