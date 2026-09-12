# Surface emission and legacy scan emission

Static review covers 0x00406300 (428-byte body plus four NOP bytes) and 0x00405A50
(663-byte body plus nine NOP bytes). Both are unfinished in the supplied team
manifest. Candidate sources remain in [research](../research/surface-emission/README.md);
neither has an accepted byte match.

The shared surface accesses establish offsets +0C exclusion flags, +0E runtime
opacity, +1C height, +24 texture index, +27 clip marker, +28 render flags,
+2A visibility group, +2B packed authored opacity, +2C flags, +30 unique index,
+38 room membership and +3B height extent. The declared 64-byte record follows
the stride used by the neighboring cell traversal. Unexamined fields remain
opaque. Synthetic tables are sized for the exercised inputs; their sizes do not
establish the original allocation capacities.

## Current emitter: queue_visible_surface

The full body rejects exclusion flags against 0x0059DF54, hidden bit 0x10,
group bit 4 when the indexed 0x008BFAA0 state is 2, and duplicate bit 0x100
when the indexed 0x008BC920 byte is already set. A room overlap with 0x0059DF44
increments 0x0059DF98 and returns before changing opacity.

Packed opacity expands its low five bits to eight bits. When fade 0x0059DF60
is active, membership in both old mask 0x0059DF80 and new mask 0x0059DF90 skips
the surface after the initial opacity write. New-only membership uses remaining
fraction 0x0059DEAC; old-only uses one minus that fraction. Authored zero becomes
255 before multiplication. CRT __ftol calls at 0x004063CD and 0x00406412 truncate;
integer results clamp to 0..255 and zero returns without insertion.

A signed comparison requires the byte texture index to be below 0x0059DF88.
Nonzero packed flags or runtime opacity choose the extra bucket. The routine
prepends the current twelve-byte entry at 0x008A4880 to heads at 0x008BC880,
increments 0x0059DEBC, writes its surface pointer and then marks uniqueness.
There is no independent entry-capacity check here. Callers at 0x00406289 and
0x004062D1 are in the animated/mover collector 0x00405CF0, which was inspected
for the shared visibility and visitation paths. No strings or imported APIs
appear in this complete emitter; its only calls are the two existing CRT calls.

## Legacy emitter: queue_legacy_scan_surface

Calls at 0x004059AC and 0x004059C9 come from legacy cell collection 0x004056C0.
The initial unsigned count guard rejects 8192 or more before reading the surface.
A separate camera-height gate uses camera pointer 0x008A011C, eye Z at +2C,
and control 0x0059DF40. Hidden/group/duplicate guards follow.

This path uses the low byte of integer 0x0059DF8C as a sixteen-step fade count,
with signed division by 16 and byte storage, not the newer fractional fade.
When inactive it rejects membership in selected mask 0x0059DF70. These checks
occur after the initial authored opacity write. Zero fade output is not itself
a reason to skip insertion. Flag 8 sets the clip marker to 0x80.

Height difference against cached 0x0059DF84 advances position 0x0059DEB0 by the
camera third basis at 0x008BEA04. Delta one has a direct-add path; zero skips
the update. The cached height is written before culling. Optional calls to
0x00403FA0 use plane 2 and conditional surface height extent, or plane 3 and zero.
A culling rejection preserves the already performed opacity/position changes.

Valid texture indices call the already matched insertion helper at 0x004059F0,
then reserve the entry and store its surface pointer. Unlike the newer emitter,
uniqueness is marked after the texture block even if the texture index is invalid.
The complete legacy body has three direct calls (two culler sites and insertion),
and no imported API or string references.

The two descriptive function names express high-confidence observed behavior.
They do not certify recovered original names, equivalent modern-engine behavior,
successful whole-game execution or exact candidate compilation.
