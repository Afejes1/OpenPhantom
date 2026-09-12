# Compiler artifacts for the surface research batch

This checkpoint adds inspectable original-compiler artifacts to the focused
research runner. Four candidates remain unverified; the fourteen accepted
functions and nineteen historical verification records are unchanged.

## Recorded artifacts

VC5 RTM 11.00.7022 now receives /FAcs for each candidate. With source and machine
code columns enabled, VC5 names the listing after the source stem with a .cod
extension. The listing comes from compiling our authored source; no original
executable bytes or Ghidra output are copied into it.

Research report schema 2 adds an artifacts array containing the source name,
function symbol, object filename and SHA-256, complete raw function-section size
and SHA-256, relocation count, and listing filename and SHA-256. These fields are
recorded before strict comparison, including when relocation inventory prevents
a comparison from completing. Debug metadata can change the object hash while
the raw function-section hash stays unchanged. A section hash alone does not
verify relocation identities, addresses, constants, or equivalence.

The existing COFF parser requires a single complete function code section.
Missing or empty listings fail the run. Object and listing hashes are rechecked
before completed becomes true, alongside source and toolchain freshness checks.
Reports do not create accepted history entries or change comparator rules.
Listings and binaries remain in ignored private build folders. The checked-in
[receipt](surface-artifacts-20260912.json) contains hashes and results only.

## Verification

The focused comparison compiled all four candidates without warnings. Each has
the required total extent (432, 672, 480, and 896 bytes). The culler still differs
in ten instruction bytes after all 22 relocation operands are verified. The
other three still fail relocation inventory checks. The culler's resolved span
hash is unchanged from the preceding checkpoint, so /FAcs did not resolve its
mismatch or change its compared code.

A separate focused behavior run passed all three authored fixtures: emission,
3,038 culling checks, and 4,184 collector checks at three x87 precisions. All four
raw function-section hashes agree between the comparison and behavior builds.
Both runs used the pinned Docker image with networking disabled, a read-only
root/toolchain, and only the fresh build folder writable. No game was executed.
The existing registry/history and separate team-work-order checks also passed.

A preliminary run expected .asm rather than VC5's actual .cod filename and
failed with completed false. The corrected runner requires the actual listing.
Additional synthetic artifact-tampering tests were not executed; this checkpoint
uses real compiler builds and the completion checks described above. No full
regression, accepted-function rebuild, or recovery checkpoint was run.

## Source investigation retained for the next pass

A returned-vector inline helper did not change the culler's ten differing bytes;
the simpler source was restored.

The collector's original second side-bound rejection compares the negative
vertical extent directly with the upper bound and branches on ordered greater
than (0x004065C2-0x004065C9). The third side bound uses the same ordered relation
at 0x004065EB-0x004065F2. Our current compiler output reverses the operands and
uses the less-than/unordered branch instead. These differ for unordered inputs.
The focused fixture covers selected NaNs, not every infinity/cancellation case;
its pass therefore does not settle this discrepancy. For example, negative
infinite projected Z with positive infinite vertical padding can make only the
second vertical comparison unordered while the first remains ordered. This is
a static counterexample to investigate with a focused fixture.

Writing the rejection as negated less-or-equal produced the same code. A shared
signed extent temporary changed scheduling and expanded the section to 912
bytes. A proposed /Op probe was rejected by the existing compiler-option policy
before compilation. No flag policy was changed. Both source and target were
restored to the preceding 896-byte candidate. No source or compiler experiment
from this pass was promoted to the accepted registry.

## Subsequent collector checkpoint

The [collector evidence](static-cell-evidence.md) now records a source correction
and executed opposite-infinity fixtures for the discrepancy above. Its complete
span and all forty operands verify, with five instruction bytes still different.
The six artifact-provenance tests previously deferred here were subsequently
implemented and passed. This document and its original receipt describe the
historical artifact-tracking checkpoint, not the newer source/test state.
