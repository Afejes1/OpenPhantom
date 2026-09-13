# Team manifest and reconstruction work order

Snapshot: 2026-09-12. Scheduling evidence supplied by the contributor; team code/build evidence is pending.

Reported done: **1272**. Reported not done: **969**.
Our last recorded matches cover **12** team-not-done entries and **2** team-done entries.
Team-not-done entries without a local recorded match: **957**.

These are separate status columns, not a combined completion certificate. See [current build evidence](progress.md) for source freshness and byte-match scope.

Research links include focused matches awaiting a batch checkpoint. Only recorded matches contribute to the counts above.

Address plus the pinned retail binary identifies a function. Reported names (including aliases) and file assignments are preserved as supplied, not automatically applied to Ghidra or accepted source definitions.

## Work order

1. **bp/bapsound.c**: current local focus.
2. **bp/bapmusic.c**: queued after the active module.
3. **bp/bapdraw.c**: queued after the active module.
4. **bp/bapmodel.c**: queued after the active module.
5. **bp/bapmap.c**: queued after the active module.

Other reported-not-done modules follow; reported-done functions are deferred pending the team's code. Existing overlapping calibrations remain regression checks.

The manifest asks for file-level coordination. Relay the active file above to the team; no message or exclusive upstream reservation has been made by this tooling.

## Active module

| Original address | Team name | Team status | Local evidence |
|---|---|---|---|
| 0x004157D0 | bapsound_resourceHandler | not_done | Queued; no local match recorded |
| 0x004158F0 | bapsound_moduleProc | not_done | Queued; no local match recorded |
| 0x004159F0 | bapsound_moduleInit | not_done | Queued; no local match recorded |
| 0x00415AB5 | bapsound_moduleShutdown | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-shutdown/README.md) |
| 0x00415B38 | bapsound_registerLevelSounds | not_done | Queued; no local match recorded |
| 0x00415C40 | bapsound_removeLevelSounds | not_done | Queued; no local match recorded |
| 0x00415D13 | FUN_00415d13 | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-tick/README.md) |
| 0x00415D1D | bapsound_periodic | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-periodic/README.md) |
| 0x00415D8A | bapsound_pollChannel | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-poll-channel/README.md) |
| 0x00415E30 | bapsound_updateChannel | not_done | Queued; no local match recorded |
| 0x0041609E | bapsound_initChannels | not_done | Queued; no local match recorded |
| 0x0041612F | bapsound_releaseSlotHandle | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-release-handle/README.md) |
| 0x0041618B | bapsound_loadRef | not_done | Queued; no local match recorded |
| 0x00416232 | bapsound_updatePlaces | not_done | Queued; no local match recorded |
| 0x00416589 | bapsound_freeExtraRefs | not_done | Queued; no local match recorded |
| 0x00416634 | bapsound_resetPlaces | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-reset-places/README.md) |
| 0x0041668F | bapsound_getField | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-get-field/README.md) |
| 0x0041670F | bapsound_getField &#124; bapsound_setField | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-set-field/README.md) |
| 0x00416787 | bapsound_playName | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-play-name/README.md) |
| 0x004167C3 | bapsound_playNameVol | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-play-name-volume/README.md) |
| 0x0041681F | bapsound_play | not_done | Queued; no local match recorded |
| 0x004169BD | bapsound_startChannel | not_done | Queued; no local match recorded |
| 0x00417043 | bapsound_findRef | not_done | Research candidate (not registered): [research](../research/campaign-006-sound-find-record/README.md) |
| 0x0041705B | bapsound_addExtraRef | not_done | Queued; no local match recorded |
| 0x00417143 | bapsound_playCall | not_done | Research candidate (not registered): [research](../research/campaign-008-sound-play-index/README.md) |
| 0x004171A1 | bapsound_playByName | not_done | Queued; no local match recorded |
| 0x004172B4 | bapsound_playVoice | not_done | Queued; no local match recorded |
| 0x00417368 | bapsound_stopChannel | not_done | Research candidate (not registered): [research](../research/campaign-006-sound-stop-channel/README.md) |
| 0x00417379 | bapsound_setMasterVolume | not_done | Queued; no local match recorded |
| 0x00417459 | bapsound_getMasterVolume | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-get-master-volume/README.md) |
| 0x00417477 | bapsound_pauseAll | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-pause-all/README.md) |
| 0x004174EF | bapsound_resumeAll | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-resume-all/README.md) |
| 0x00417567 | bapsound_freeChannel | not_done | Queued; no local match recorded |
| 0x00417659 | bapsound_startSample | not_done | Queued; no local match recorded |
| 0x004176E3 | bapsound_stopAll | not_done | Research candidate (not registered): [research](../research/campaign-006-sound-stop-all/README.md) |
| 0x00417711 | bapsound_activatePlace | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-activate-place/README.md) |
| 0x0041778C | bapsound_deactivatePlace | not_done | Research candidate (not registered): [research](../research/campaign-006-sound-deactivate-place/README.md) |
| 0x004177AE | bapsound_setPlaceActive | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-control-place/README.md) |
| 0x00417826 | bapsound_pinChannel | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-pin-channel/README.md) |
| 0x0041786C | bapsound_getDriver | not_done | Research candidate (not registered): [research](../research/campaign-007-sound-get-driver/README.md) |
| 0x00417876 | bapsound_testCallFlags | not_done | Research candidate (not registered): [research](../research/campaign-006-sound-test-flags/README.md) |
| 0x004178A3 | bapsound_disable | not_done | Research candidate (not registered): [research](../research/campaign-006-sound-disable/README.md) |
| 0x004178D1 | bapsound_disable &#124; bapsound_enable | not_done | Research candidate (not registered): [research](../research/campaign-006-sound-enable/README.md) |
| 0x004178FA | bapsound_isMuted | not_done | Queued; no local match recorded |

## Remaining work by reported file

| Reported file | Team not done | Local recorded matches within that list | Remaining |
|---|---|---|---|
| bp/bapsound.c | 44 | 0 | 44 |
| bp/bapmusic.c | 17 | 0 | 17 |
| bp/bapdraw.c | 18 | 7 | 11 |
| bp/bapmodel.c | 8 | 4 | 4 |
| bp/bapmap.c | 14 | 1 | 13 |
| bp/baplight.c | 6 | 0 | 6 |
| bp/bapobj.c | 4 | 0 | 4 |
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
