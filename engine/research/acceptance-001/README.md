# Acceptance checkpoint 001

**Passed with staged integration.** Read the [results](results.md) and
[machine-readable outcome](result.json).

The contributor authorized batch acceptance testing of the September 13 overnight
results before another one-hour reconstruction window. This checkpoint preserves
three distinct requirements: complete synthetic/supplemental regression, the full
registered VC5 build with its immutable record, and fresh native/Docker comparisons
and focused fixtures for all 66 overnight exact cases.

Every selected case must reproduce its complete original span with zero differing
bytes and every address operand verified. Sources, targets and tool fingerprints
must agree with the published case evidence. Fixture checks must retain their
recorded count and pass in locked Docker. No original code executes. Near and
body-only cases are not promoted by this checkpoint.

For the frozen66-case rebuild, check out source commit
`58a1ec4` before running from the repository root with owned private inputs.
Later target/registry expansion intentionally changes the source fingerprint and
requires a separately reviewed manifest; it cannot silently reuse this checkpoint.

```text
python engine/research/acceptance-001/rebuild.py --reference OWNED_WMAIN.EXE
```

The runner writes fresh reports only under ignored engine/build. It delegates all
compilation, isolation and comparison to the existing campaign runner; it does not
change the strict matcher. Failures preserve their receipts and stop the batch.
The final report explicitly distinguishes passing case-batch testing from a shared
engine integration. Research declarations and isolated callback stubs need review
before promotion through the existing registered-build mechanism.
