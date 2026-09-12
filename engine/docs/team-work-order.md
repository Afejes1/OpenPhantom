# Team manifest and reconstruction work order

Snapshot: 2026-09-12. Scheduling evidence supplied by the contributor; team code/build evidence is pending.

Reported done: **1272**. Reported not done: **969**.
Our last recorded matches cover **12** team-not-done entries and **2** team-done entries.
Team-not-done entries without a local recorded match: **957**.

These are separate status columns, not a combined completion certificate. See [current build evidence](progress.md) for source freshness and byte-match scope.

Research links include focused matches awaiting a batch checkpoint. Only recorded matches contribute to the counts above.

Address plus the pinned retail binary identifies a function. Reported names (including aliases) and file assignments are preserved as supplied, not automatically applied to Ghidra or accepted source definitions.

## Work order

1. **bp/bapdraw.c**: current local focus.
2. **bp/bapmodel.c**: queued after the active module.
3. **bp/bapmap.c**: queued after the active module.

Other reported-not-done modules follow; reported-done functions are deferred pending the team's code. Existing overlapping calibrations remain regression checks.

The manifest asks for file-level coordination. Relay the active file above to the team; no message or exclusive upstream reservation has been made by this tooling.

## Active module

| Original address | Team name | Team status | Local evidence |
|---|---|---|---|
| 0x00401D30 | bapdraw_setFrameState | not_done | Recorded match: [set_frame_state](../src/set_frame_state.c) |
| 0x00401EB8 | bapdraw_queuePoly | not_done | Recorded match: [queue_face_packet](../src/queue_face_packet.c) |
| 0x00402021 | bapdraw_reservePoly | not_done | Recorded match: [peek_face_packet](../src/peek_face_packet.c) |
| 0x00402046 | bapdraw_commitVerts | not_done | Research candidate (not registered): [research](../research/projected-vertex-fog/README.md) |
| 0x00402130 | bapdraw_reserveVerts | not_done | Recorded match: [peek_projected_vertices](../src/peek_projected_vertices.c) |
| 0x00402155 | bapdraw_flushQueue | not_done | Queued; no local match recorded |
| 0x004025B0 | bapdraw_clipSlopePlane &#124; bapdraw_clipPoly &#124; bapdraw_clipDepthPlane &#124; bapdraw_clipSwap | not_done | Queued; no local match recorded |
| 0x00403EF0 | bapdrawOld_stepScan | not_done | Recorded match: [step_scan](../src/step_scan.c) |
| 0x00403FA0 | bapdrawOld_cullAgainstPlane | not_done | Research candidate (not registered): [research](../research/surface-emission/README.md) |
| 0x00404180 | bapdrawOld_frameSetup | not_done | Queued; no local match recorded |
| 0x00404730 | bapdraw_drawWorld | not_done | Queued; no local match recorded |
| 0x004056C0 | bapdrawOld_gatherCell | not_done | Research candidate (not registered): [research](../research/surface-emission/README.md) |
| 0x004059F0 | bapdrawOld_pushDrawEntry | not_done | Recorded match: [push_surface_draw_entry](../src/push_surface_draw_entry.cpp) |
| 0x00405A50 | bapdrawOld_emitFace | not_done | Research candidate (not registered): [research](../research/surface-emission/README.md) |
| 0x00405CF0 | bapdraw_gatherCellMovers | not_done | Queued; no local match recorded |
| 0x00406300 | bapdraw_emitFace | not_done | Research candidate (not registered): [research](../research/surface-emission/README.md) |
| 0x004064B0 | bapdraw_gatherCell | not_done | Research candidate (not registered): [research](../research/surface-emission/README.md) |
| 0x00406830 | bapdraw_submitGathered | not_done | Recorded match: [submit_gathered_surfaces](../src/submit_gathered_surfaces.c) |

## Remaining work by reported file

| Reported file | Team not done | Local recorded matches within that list | Remaining |
|---|---|---|---|
| bp/bapdraw.c | 18 | 7 | 11 |
| bp/bapmodel.c | 8 | 4 | 4 |
| bp/bapmap.c | 14 | 1 | 13 |
| bp/baplight.c | 6 | 0 | 6 |
| bp/bapmusic.c | 17 | 0 | 17 |
| bp/bapobj.c | 4 | 0 | 4 |
| bp/bapsound.c | 44 | 0 | 44 |
| bp/bapview.c | 6 | 0 | 6 |
| bp/bapvrt.c | 19 | 0 | 19 |
| bp/bapworld.c | 39 | 0 | 39 |
| bp/bgl.c | 21 | 0 | 21 |
| bp/bppartic.c | 5 | 0 | 5 |
| bp/bpsprite.c | 1 | 0 | 1 |
| bp/extended.c | 22 | 0 | 22 |
| bp/features.c | 1 | 0 | 1 |
| bp/texture.c | 7 | 0 | 7 |
| game/aiext.c | 30 | 0 | 30 |
| game/enemy.c | 9 | 0 | 9 |
| game/fxfade.c | 34 | 0 | 34 |
| game/fxprint.c | 6 | 0 | 6 |
| game/fxshield.c | 24 | 0 | 24 |
| game/fxzappo.c | 14 | 0 | 14 |
| game/main.c | 26 | 0 | 26 |
| game/menu.c | 10 | 0 | 10 |
| game/player.c | 13 | 0 | 13 |
| game/save.c | 22 | 0 | 22 |
| game/shot.c | 6 | 0 | 6 |
| rdroid/rdcanvas.c | 4 | 0 | 4 |
| rdroid/rdclip.c | 2 | 0 | 2 |
| rdroid/rdcolor.c | 10 | 0 | 10 |
| rdroid/rdface.c | 4 | 0 | 4 |
| rdroid/rdfont.c | 2 | 0 | 2 |
| rdroid/rdkeyf.c | 8 | 0 | 8 |
| rdroid/rdlight.c | 8 | 0 | 8 |
| rdroid/rdmat.c | 17 | 0 | 17 |
| rdroid/rdmatrix.c | 9 | 0 | 9 |
| rdroid/rdmodel3.c | 3 | 0 | 3 |
| rdroid/rdpolyl.c | 4 | 0 | 4 |
| rdroid/rdpuppet.c | 20 | 0 | 20 |
| rdroid/rdsprite.c | 5 | 0 | 5 |
| std/std3d.c | 11 | 0 | 11 |
| std/stdconf.c | 16 | 0 | 16 |
| std/stdcont.c | 1 | 0 | 1 |
| std/stddisp.c | 7 | 0 | 7 |
| std/stdlinkl.c | 10 | 0 | 10 |
| std/stdmath.c | 24 | 0 | 24 |
| std/stdmusic.c | 7 | 0 | 7 |
| std/stdplat.c | 15 | 0 | 15 |
| std/stdsound.c | 29 | 0 | 29 |
| std/stdutil.c | 12 | 0 | 12 |
| std/stdvideo.c | 8 | 0 | 8 |
| std/stdwin95.c | 14 | 0 | 14 |
| std/wkernel.c | 14 | 0 | 14 |
| swift/stdbmp.c | 1 | 0 | 1 |
| swift/sw3d.c | 10 | 0 | 10 |
| swift/swchkbox.c | 2 | 0 | 2 |
| swift/swedit.c | 2 | 0 | 2 |
| swift/swlistbx.c | 7 | 0 | 7 |
| swift/swslider.c | 6 | 0 | 6 |
| swift/swtext.c | 5 | 0 | 5 |
| swift/swwidget.c | 12 | 0 | 12 |
| util/control.c | 3 | 0 | 3 |
| util/debug.c | 16 | 0 | 16 |
| util/fileutil.c | 5 | 0 | 5 |
| util/font3d.c | 21 | 0 | 21 |
| util/inter.c | 14 | 0 | 14 |
| util/module.c | 20 | 0 | 20 |
| util/pzip.c | 30 | 0 | 30 |
| util/res.c | 11 | 0 | 11 |
| util/taskman.c | 7 | 0 | 7 |
| File not known yet | 107 | 0 | 107 |

## Reconciliation when team source arrives

Retain this dated snapshot. Import a new dated snapshot when status changes. Join by retail identity and address, compare module assignments and aliases, then compile the team's source with the recorded original toolchain. Only our existing strict verifier can grant local acceptance; an upstream done label never does.

Attachment SHA-256: `d54b2be13ec58964de27f520c0dccab6daff38d566e8722e26c624e82a8b0a85`. Full normalized inventory: [JSON](../planning/team-manifest-20260912.json).
