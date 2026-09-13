# Shared colormap tracking acceptance slice

Two frozen Campaign015 functions share signed op_extended_colormap_count and the
128-pointer op_extended_colormaps array through src/extended_colormaps.h. Their
bodies, signatures, flags, target intervals and10 total address bindings remain
unchanged; only the header include differs. Full spans are73 and68 bytes.
Original translation-unit boundaries and final executable layout remain unresolved.

The combined synthetic fixture calls the actual reconstructed functions together.
It observes repeated load/free transitions, failed and capacity-full loads, complete
array retention, callback-mutated counts/slots and count reset. Backend allocation
and freeing are explicit authored callbacks, never original game execution.
The fixture is textually included once by tests/behavior.c to preserve the existing
single-main and source-fingerprint topology; the modern supplemental build uses
those same sources. No matcher, warning, extent or acceptance rule is changed.

Promotion requires a clean committed registered23-function VC5 build, all original
byte comparisons, locked Docker fixture pass and a new event from the existing
recorder. The deliberate integration regression checkpoint runs once. Historical
research receipts keep their original snapshots; the expanded target makes those
older whole-input snapshots historical, not silently current. See
../research/acceptance-002/results.md for completed evidence when available.
