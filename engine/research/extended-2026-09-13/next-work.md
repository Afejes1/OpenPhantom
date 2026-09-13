# Next bounded batch

The current shared baseline has40 accepted functions and28 immutable records. No implementation remains active. Fourteen eligible exact functions from this window are queued for shared integration; retain the six near matches, one research candidate and two team-done overlaps in their separate categories.

Recommended first slice: integrate the stream/chunk helpers `world_read_chunk_header` (Campaign020,112bytes), `world_skip_chunk` (Campaign017,93bytes), and `world_load_via_skip` (Campaign018,32bytes). Reconfirm the shared stream/header layout before source changes. The wrapper uses header offset0x2D4, not current payload0x800. Its real skip callee uses two cdecl stack arguments; the stale Ghidra thiscall annotation must not introduce an argument. The wrapper ignores skip failure and returns1. The chunk reader consumes12bytes, has a23-entry live comparison table, and must preserve zero-read versus negative-success behavior and callback mutation ordering. Use authored tags, no original strings.

Prepare one shared fixture that calls the actual reconstructed helpers, observes callbacks and complete state, and retains their full spans and existing address bindings. Then one clean original-toolchain build/test/compare/record checkpoint and full supplemental CI. Do not rerun broad regression for every intervening research commit.

Other exact queues include cleanup helpers from017/018, world allocate/destroy from020, and grid/cell helpers from021. Pointer-table620 capacity remains unresolved; do not promote a four-slot fixture view as the actual full table. Cell wrapper integration depends on preserved near-match distinctions and the parked content-flags padding case. Do not reset exhausted source timeboxes.

Keep zero differing bytes for exact acceptance. Use one writer plus independent review, manifest eligibility before implementation, and preserve all failed attempts. The user must authorize a new execution window; the current4.5-hour heartbeat is paused.
