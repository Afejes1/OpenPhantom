# BGL matrix-state acceptance slice

This slice integrates seven full-span exact Campaign011 cases under one canonical
48-byte matrix type in src/bgl_state.h. Implementations are preserved verbatim
except for the common-header include. All original target intervals, flags,
symbols and bindings are preserved. Splitting files is an integration convenience;
original translation-unit boundaries remain unproven.

The seven functions cover identity-template copy, push/pop, full matrix load/get,
and translation getters. The fixture tests actual linked state transitions,
full48-byte copies, signed depth guards, independent current/stack provenance,
nullable outputs and valid alias order. It uses an intentionally nonuniform
synthetic template to detect lost words; that template is not recovered game data
or a claim about the original identity matrix contents. The matrix type contains
only proven raw32-bit storage and introduces no speculative mathematical fields.

The existing full build compiles these with all14 previously registered functions
and links one diagnostic fixture. tests/behavior.c textually includes the new
fixture so the original verifier needs no topology or source-snapshot change.
The new sources and fixture are within its fingerprinted src/tests directories.
No accepted historical definition is changed. Acceptance occurs only after the
clean committed21-function build, complete comparison and Docker fixture pass
and the existing recorder appends its immutable event.

The remaining59 overnight exact cases stay batch-tested research snapshots.
Sound channel, descriptor, placement and world partial types need canonical
offset-checked definitions; some signed/unsigned declarations conflict.
Independent case stubs are not proof that those remaining functions link together.
See ../research/acceptance-001/results.md for the final checkpoint and receipts.
