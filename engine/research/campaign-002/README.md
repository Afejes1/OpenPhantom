# Campaign 002: bounded reconstruction

[Results](results.md) and [machine-readable receipts](results.json) record the
completed execution of the [fork campaign rules](../../docs/campaign-rules.md).
One Sol worker at medium reasoning handled three sequential prepared assignments
with independent coordinator review. The baseline commit is
`3c8f6752ab810cfb6eacd6a5e5bce221c65437b1`.

- [Frame operand inventory](frame-binding-evidence.md): all 153 original operands
  reviewed, strict comparison enabled, full extent mismatch retained.
- [Scan-line reset](../campaign-002-line-reset/README.md): 400-byte resolved match.
- [Segment predicate](../campaign-002-segment-test/README.md): 237-byte raw match.
- [Review](review.json), [experiment metrics](experiment-metrics.json) and
  [historical scheduling triage](triage.json) preserve findings and limits.

The existing campaign-001 verification runner and pinned original toolchain are
reused. Each helper README includes reproduction commands. The existing frame
README has its commands and new receipt. Licensed compiler provisioning and the
private original reference are prerequisites; neither is published as game data.

The 45-minute window ends at 02:01:02 UTC on 2026-09-13. Candidate verification
finished at 01:44:59 UTC; publication is the final step. No further worker task
or automatic subsequent campaign is authorized by this manifest.
