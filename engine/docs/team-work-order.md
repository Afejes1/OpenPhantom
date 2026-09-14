# Team manifest and reconstruction work order

Snapshot: 2026-09-12. Scheduling evidence supplied by the contributor; team code/build evidence is pending.

Reported done: **1272**. Reported not done: **969**.
Our last recorded matches cover **198** team-not-done entries and **5** team-done entries.
Team-not-done entries without a local recorded match: **771**.

These are separate status columns, not a combined completion certificate. See [current build evidence](progress.md) for source freshness and byte-match scope.

Research links include focused matches awaiting a batch checkpoint. Only recorded matches contribute to the counts above.

Address plus the pinned retail binary identifies a function. Reported names (including aliases) and file assignments are preserved as supplied, not automatically applied to Ghidra or accepted source definitions.

## Work order

1. **bp/bapworld.c**: current local focus.
2. **bp/bapsound.c**: queued after the active module.
3. **bp/bapmusic.c**: queued after the active module.
4. **bp/bapdraw.c**: queued after the active module.
5. **bp/bapmodel.c**: queued after the active module.
6. **bp/bapmap.c**: queued after the active module.

Other reported-not-done modules follow; reported-done functions are deferred pending the team's code. Existing overlapping calibrations remain regression checks.

The manifest asks for file-level coordination. Relay the active file above to the team; no message or exclusive upstream reservation has been made by this tooling.

## Active module

| Original address | Team name | Team status | Local evidence |
|---|---|---|---|
| 0x0041C9E0 | bapworld_moduleProc | not_done | Queued; no local match recorded |
| 0x0041CBBF | bapworld_alloc | not_done | Queued; no local match recorded |
| 0x0041CC80 | bapworld_free | not_done | Queued; no local match recorded |
| 0x0041CD40 | bapworld_freeGraphics | not_done | Queued; no local match recorded |
| 0x0041CD80 | bapworld_readChunkTag | not_done | Recorded match: [world_read_chunk_header](../src/world_read_chunk_header.c) |
| 0x0041CDF0 | bapworld_skipChunk | not_done | Recorded match: [world_skip_chunk](../src/world_skip_chunk.c) |
| 0x0041CE4D | bapworld_allocFromHeader | not_done | Queued; no local match recorded |
| 0x0041D302 | bapworld_loadPool | not_done | Recorded match: [world_read_emitters](../src/world_read_emitters.c) |
| 0x0041D360 | bapworld_loadPool | not_done | Recorded match: [world_read_sprite_refs](../src/world_read_sprite_refs.c) |
| 0x0041D3CC | bapworld_loadPool | not_done | Queued; no local match recorded |
| 0x0041D4AB | bapworld_loadPool | not_done | Queued; no local match recorded |
| 0x0041D5A3 | bapworld_loadPals | not_done | Recorded match: [world_read_palettes](../src/world_read_palettes.c) |
| 0x0041D5FD | bapworld_loadDlg | not_done | Queued; no local match recorded |
| 0x0041D696 | bapworld_loadGmap | not_done | Queued; no local match recorded |
| 0x0041D8EE | bapworld_loadGvrt | not_done | Recorded match: [world_read_global_vertices](../src/world_read_global_vertices.c) |
| 0x0041D983 | bapworld_loadLvrt | not_done | Recorded match: [world_read_local_vertices](../src/world_read_local_vertices.c) |
| 0x0041DA1B | bapworld_loadMat | not_done | Queued; no local match recorded |
| 0x0041DAB0 | bapworld_loadTex | not_done | Queued; no local match recorded |
| 0x0041DBD1 | bapworld_loadLights | not_done | Queued; no local match recorded |
| 0x0041DC69 | bapworld_loadLights | not_done | Queued; no local match recorded |
| 0x0041DD01 | bapworld_loadAdef | not_done | Queued; no local match recorded |
| 0x0041DEEA | bapworld_loadAmap | not_done | Queued; no local match recorded |
| 0x0041E131 | bapworld_loadEnmy | not_done | Queued; no local match recorded |
| 0x0041E24D | bapworld_loadAi | not_done | Queued; no local match recorded |
| 0x0041E373 | bapworld_freeScripts | not_done | Research candidate (not registered): [research](../research/campaign-017-world-free-scripts/README.md) |
| 0x0041E3BD | bapworld_loadAnam | not_done | Recorded match: [world_read_animation_names](../src/world_read_animation_names.c) |
| 0x0041E4BA | bapworld_loadViaSkip | not_done | Recorded match: [world_load_via_skip](../src/world_load_via_skip.c) |
| 0x0041E4DA | bapmap_bindGeometry | not_done | Queued; no local match recorded |
| 0x0041E7E4 | bapworld_loadChunk | not_done | Queued; no local match recorded |
| 0x0041EABC | bapworld_mangleNames | not_done | Recorded match: [world_mangle_names](../src/world_mangle_names.c) |
| 0x0041EB5A | bapworld_restartClock | not_done | Recorded match: [world_restart_clock](../src/world_restart_clock.c) |
| 0x0041EB94 | bapworld_loadWithHeader &#124; bapworld_load | not_done | Queued; no local match recorded |
| 0x0041EEAA | bapworld_freeMaterials | not_done | Queued; no local match recorded |
| 0x0041EF3F | bapworld_freePtrArray0x220 | not_done | Queued; no local match recorded |
| 0x0041EFB5 | bapworld_freePtrArray0x220 &#124; bapworld_freePtrArray0x620 | not_done | Queued; no local match recorded |
| 0x0041F02B | bapworld_freeTailBlocks | not_done | Queued; no local match recorded |
| 0x0041F0C9 | bapmap_setWorldClock | not_done | Recorded match: [world_set_clock](../src/world_set_clock.c) |
| 0x0041F10C | baplight_setAmbient | not_done | Recorded match: [world_set_ambient](../src/world_set_ambient.c) |
| 0x0041F14A | baplight_applyLevelFog | not_done | Recorded match: [world_apply_fog](../src/world_apply_fog.c) |

## Remaining work by reported file

| Reported file | Team not done | Local recorded matches within that list | Remaining |
|---|---|---|---|
| bp/bapworld.c | 39 | 14 | 25 |
| bp/bapsound.c | 44 | 0 | 44 |
| bp/bapmusic.c | 17 | 0 | 17 |
| bp/bapdraw.c | 18 | 7 | 11 |
| bp/bapmodel.c | 8 | 4 | 4 |
| bp/bapmap.c | 14 | 1 | 13 |
| bp/baplight.c | 6 | 0 | 6 |
| bp/bapobj.c | 4 | 0 | 4 |
| bp/bapview.c | 6 | 0 | 6 |
| bp/bapvrt.c | 19 | 6 | 13 |
| bp/bgl.c | 21 | 7 | 14 |
| bp/bppartic.c | 5 | 0 | 5 |
| bp/bpsprite.c | 1 | 0 | 1 |
| bp/extended.c | 22 | 2 | 20 |
| bp/features.c | 1 | 0 | 1 |
| bp/texture.c | 7 | 4 | 3 |
| game/aiext.c | 30 | 0 | 30 |
| game/enemy.c | 9 | 0 | 9 |
| game/fxfade.c | 34 | 28 | 6 |
| game/fxprint.c | 6 | 2 | 4 |
| game/fxshield.c | 24 | 14 | 10 |
| game/fxzappo.c | 14 | 10 | 4 |
| game/main.c | 26 | 5 | 21 |
| game/menu.c | 10 | 0 | 10 |
| game/player.c | 13 | 0 | 13 |
| game/save.c | 22 | 11 | 11 |
| game/shot.c | 6 | 0 | 6 |
| rdroid/rdcanvas.c | 4 | 3 | 1 |
| rdroid/rdclip.c | 2 | 0 | 2 |
| rdroid/rdcolor.c | 10 | 7 | 3 |
| rdroid/rdface.c | 4 | 4 | 0 |
| rdroid/rdfont.c | 2 | 0 | 2 |
| rdroid/rdkeyf.c | 8 | 6 | 2 |
| rdroid/rdlight.c | 8 | 4 | 4 |
| rdroid/rdmat.c | 17 | 5 | 12 |
| rdroid/rdmatrix.c | 9 | 3 | 6 |
| rdroid/rdmodel3.c | 3 | 0 | 3 |
| rdroid/rdpolyl.c | 4 | 0 | 4 |
| rdroid/rdpuppet.c | 20 | 5 | 15 |
| rdroid/rdsprite.c | 5 | 0 | 5 |
| std/std3d.c | 11 | 0 | 11 |
| std/stdconf.c | 16 | 0 | 16 |
| std/stdcont.c | 1 | 0 | 1 |
| std/stddisp.c | 7 | 0 | 7 |
| std/stdlinkl.c | 10 | 9 | 1 |
| std/stdmath.c | 24 | 0 | 24 |
| std/stdmusic.c | 7 | 0 | 7 |
| std/stdplat.c | 15 | 0 | 15 |
| std/stdsound.c | 29 | 0 | 29 |
| std/stdutil.c | 12 | 0 | 12 |
| std/stdvideo.c | 8 | 0 | 8 |
| std/stdwin95.c | 14 | 0 | 14 |
| std/wkernel.c | 14 | 0 | 14 |
| swift/stdbmp.c | 1 | 0 | 1 |
| swift/sw3d.c | 10 | 1 | 9 |
| swift/swchkbox.c | 2 | 0 | 2 |
| swift/swedit.c | 2 | 0 | 2 |
| swift/swlistbx.c | 7 | 0 | 7 |
| swift/swslider.c | 6 | 0 | 6 |
| swift/swtext.c | 5 | 0 | 5 |
| swift/swwidget.c | 12 | 0 | 12 |
| util/control.c | 3 | 0 | 3 |
| util/debug.c | 16 | 0 | 16 |
| util/fileutil.c | 5 | 4 | 1 |
| util/font3d.c | 21 | 18 | 3 |
| util/inter.c | 14 | 0 | 14 |
| util/module.c | 20 | 6 | 14 |
| util/pzip.c | 30 | 0 | 30 |
| util/res.c | 11 | 0 | 11 |
| util/taskman.c | 7 | 0 | 7 |
| File not known yet | 107 | 8 | 99 |

## Reconciliation when team source arrives

Retain this dated snapshot. Import a new dated snapshot when status changes. Join by retail identity and address, compare module assignments and aliases, then compile the team's source with the recorded original toolchain. Only our existing strict verifier can grant local acceptance; an upstream done label never does.

Attachment SHA-256: `d54b2be13ec58964de27f520c0dccab6daff38d566e8722e26c624e82a8b0a85`. Full normalized inventory: [JSON](../planning/team-manifest-20260912.json).
