# Two-hour continuation results

**Seven new, unique functions accepted: 347 to 354.** These are source functions, not test runs or assertion counts. They add 888 complete bytes and 76 verified address operands. All seven are relocation-adjusted exact matches, with zero byte exclusions.

| Function | Complete bytes | Verified operands | Accepted event |
| --- | ---: | ---: | ---: |
| advance_runtime_fixed_steps | 302 | 36 | 87 |
| pace_frame_clock | 108 | 11 | 88 |
| audio_get_spatial | 46 | 1 | 89 |
| audio_duplicate | 64 | 3 | 89 |
| audio_shutdown | 103 | 11 | 89 |
| audio_set_listener | 141 | 7 | 89 |
| audio_set_master_volume | 124 | 7 | 90 |

[The address-based ledger](new-functions.csv) gives each source path, first accepted commit/build, final category and focused-check count. [Machine-readable results](results.json) include original/resolved span hashes and the separate research ledger.

**597 contributor-unfinished functions remain**, assuming the team's 1,298 done claims are correct. We have accepted 346 of their 943 unfinished functions, plus eight that overlap their done list. The latest comparison is pinned to the September 12, 18:49 manifest. The team's completed code has not been independently validated here. The legacy `team_progress.py` console report still uses the older 1,272/969 split; current-window counts and priorities use the pinned latest 1,298/943 manifest.

The final [event90](../../evidence/runs/000090-3abdc6aedee0f297.json) uses source `7b117d06e95ff1d9b6b95c95ef18eded00dea8a2`, build `20260914-205123-72f844cf` and 597 Git-identical source inputs. All 354 original-toolchain comparisons and the authored shared fixture pass. [Same-source supplemental CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34914926449) passed. Every original 347-function specification and resolved span remains unchanged, and earlier matrix counts are retained at each checkpoint.

Byte matching compares the complete registered original function span with the VC5-generated function. Every COFF address operand is resolved to its independently verified original target, including relative calls, and numeric constants are checked. The entire equal-length span must then match with zero differences. No operand, alignment byte or branch is ignored. This establishes function-level equality after verified address relocation; final executable placement, linked data and whole-executable identity remain separate work.

The retained new-function fixtures contain 369,624 assertions. Their purpose is to check bounded behavior and ABI assumptions, including live callback mutation, timing thresholds, HRESULT branches and the original discarded volume clamp. That assertion total is not a function count or a claim of exhaustive game behavior.

Three useful resource candidates remain research: close 176/173 bytes, EOF 115/113 and tell 113/111. Their final authored fixtures pass 114,960 assertions, but differing complete extents prevent both acceptance and a mismatch percentage. Their source caps and prior histories remain intact.

Review caught and corrected material issues before acceptance: eight historical fixed-step operand offsets failed strict preflight before compilation; a sound callback inferred as a format pointer was recovered as signed volume from two independent callers; and annotation CI required lowercase address hex. The annotation-only correction caused an unfinished build to be stopped and preserved, followed by a fresh build from the corrected commit. No warning waiver, padding or matcher relaxation was added.

Before starting more fresh source, review the [eight historical focused-match candidates](historical-focused-queue.json). They are excluded from accepted counts and require current span, API, symbol and fixture review; preserve their histories. One historical identifier collides with a different accepted function, so reconcile by address.

The next prepared candidate is the [149-byte buffer-volume function](next-buffer-volume.json), with six independently rechecked operands and the same discarded-clamp pattern. Its existing research caller needs address-based identifier reconciliation before integration. No source attempt was started.

The separate [buffer-creation preparation note](next-preparation.md) preserves an unusual negative-HRESULT return path and distinguishes real object size from rounded stack storage. It has no source experiment. Keep that case in preparation until its extra initialized local and source-level return contract are explained; do not replace them with a conventional API assumption.

All changes remain on the personal fork branch. No original game, asset, compiler binary or decompiler output was committed; no original program executed and no upstream/develop merge occurred. The original reference hash is unchanged.
