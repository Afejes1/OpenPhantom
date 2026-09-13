# Acceptance and one-hour continuation results

**Acceptance passed, followed by nine new exact functions totalling702 full-span
bytes.** One near match and one research candidate remain explicitly parked.
The continuation used one Sol medium implementation worker with independent review.
All433 focused assertions pass, and none of the candidate sources required a parent
rewrite. Work finished within the11:06-12:06 UTC authorization on2026-09-13;
actual completion and the early implementation stop are recorded in [status](status.json).

| Checkpoint | Exact functions | Exact bytes | Other results | Focused checks |
| --- | ---: | ---: | --- | ---: |
| [Sound callbacks014](../campaign-014/results.md) |1|256| One near,9/288 bytes different |229|
| [Extended helpers015](../campaign-015/results.md) |5|287| One research,6/117 bytes different |122|
| [Texture and face selector016](../campaign-016/results.md) |3|159| None |82|

Two new functions match raw bytes; seven match after verified address resolution.
The sound dispatcher's exact256-byte span includes both switch tables. Every exact
case covers its complete original span with zero differing bytes; no alignment,
tables, operands or warnings were excluded. Near means at most5% on a trustworthy
full comparison, not accepted correctness. Attachment's5.128205% stays research.
All eleven cases were team-reported unfinished when selected.

The preceding [acceptance checkpoint](../acceptance-001/results.md) freshly rebuilt
all66 overnight exact cases with VC5 natively and in locked Docker:5,315 bytes and
2,343 focused assertions passed. Full synthetic/regression/CI checks passed as well.
Seven coherent BGL state functions (320 bytes) were integrated through the existing
accepted-build chain. The registry now has21functions/21records. The other59
batch-tested overnight exact functions still need shared ABI integration. The nine
new exact results here also remain research until that checkpoint; they were not
silently promoted. Whole-executable identity remains a separate milestone.

All nine new exact cases matched on their first worker compile. There were13 worker
candidate compiles,22 successful final-pair candidate compiles plus4 from two
preserved failed fixture attempts, and11 successful fixture executions. The two
failed fixture builds executed nothing. Independent review corrected a sentinel
oracle and a post-call expected snapshot, then strengthened callback-time state,
full owned storage and signed/unsigned boundary expectations. VC5 macro and pointer
qualifier warnings were fixed in test source with no waivers. These lessons are
now explicit in the [campaign rules](../../docs/campaign-rules.md).

Measured worker compiler receipts total18.164seconds; successful final native/Docker
verification pairs total77.164seconds. These are runner timings, excluding analysis,
review, tool-call overhead, publication and failed attempts. Docker compilation was
not the main time cost for this workload. The shared weekly meter moved from37% to
39%; that is rounded account-wide observation, not an isolated usage charge or a
controlled model comparison. No paid runner, credit reset or new automation was added.
The old six-hour heartbeat remains paused.

The sources are C using the pinned original VC5 toolchain. Synthetic fixtures run
only in the locked Docker environment. Ghidra names/comments were saved; both sound
callback prototypes were recovered after retrying the parser with plain C syntax.
The original game was never executed or patched. Licensed compiler files, original
game content, generated objects and decompiler output remain outside public Git.
The source/fixture/target snapshots and private receipt hashes are checked against
exact staged Git blobs before feature publication. Full regression was not repeated
during the continuation; feature pushes use the existing focused CI workflow.

Keep one implementation worker and independent review for the next batch. First
integrate the compatible colormap load/free pair with a genuine combined fixture,
then reconcile the face family's partial layouts and per-use signedness. The sound
lifecycle slice follows its shared ABI cleanup. See the concrete
[next integration plan](next-integration.md) and [machine-readable metrics](metrics.json).
