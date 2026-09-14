# Acceptance033 results

Four exact functions bring the accepted baseline from172 to176. The complete996 bytes match with28 verified address operands and no exclusions or warning waivers. Clean source05a67c336f19a56266223891f47c75fd0a54a2dd builds as20260914-003007-0f3d9dfe. All176 comparisons and shared authored behavior pass. [Event56](../../evidence/runs/000056-37296c8cf68d63cc.json) preserves the source snapshot. [Full supplemental CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34806281890) succeeds on the same source.

Reader coverage includes180 direct scenarios and180 repetitions through the real root allocation/load/free chain, totaling26,460 checks. The writer passes4,896 checks; the fallback bridge passes982. Existing colormap lifecycle and extended-list tests now use the real reader's always-success fallback path and explicitly release returned roots. Shared geometry passes131 point-plane and1,729 angle checks with literal numeric expectations and restored floating-point control. Original VC5 targeted fixture compilation and independent applied review found no material blockers.

All four additions are team-not_done. The snap and palette builders remain research. These tests use authored static pools and callback services, not real heap/file I/O or the original game. Individual function matches do not establish whole-executable identity.
