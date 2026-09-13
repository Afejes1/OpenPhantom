# Campaign 003: related small-function batches

[Results](results.md), [receipts](results.json), [review](review.json) and
[experiment metrics](experiment-metrics.json) record the completed execution.
The baseline commit is `fed5267e1802a84e4d7e3ca86ca79ecef568fc61`.

- [Line advance](../campaign-003-line-step/README.md): 209-byte raw match.
- [Line release](../campaign-003-line-free/README.md): 23-byte resolved match.
- [Polyline strip](../campaign-003-polyline/README.md): eight differing bytes in 478.
- [Diagnostic segment](../campaign-003-diagnostic-segment/README.md): 76-byte body
  match, one alignment byte pending in the full 77-byte target.

Each case includes a prepared packet, authored C/header/fixture, immutable target,
worker attempt log, final receipt and reproduction commands. One Sol worker
processed them sequentially while the coordinator prepared and reviewed work.
[Campaign rules](../../docs/campaign-rules.md) retain exact acceptance requirements,
per-function caps and the distinct 5% research scheduling category.

The existing campaign-001 runner now supports an explicit, tested native-listing
fallback for zero-byte Docker listings. The strict matcher and original toolchain
are unchanged. Private licensed tools and the original owned reference remain
prerequisites. No original game execution occurred.
