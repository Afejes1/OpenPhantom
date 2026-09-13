# Campaign 013 results

**Three complete bodies match, with original full-span layout still pending.**
They total130 body bytes; their immutable targets retain35 additional INT3 padding
bytes. There are zero new complete exact function matches, one reviewed near match
and one research candidate. All526 focused checks and10 body-evidence checks pass.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [music_resume](../campaign-013-music-resume/README.md) | 0x00410825 | 75 | Body exact; layout pending | 45 |
| [sound_is_muted](../campaign-013-sound-is-muted/README.md) | 0x004178FA | 54 | Body exact; layout pending | 96 |
| [music_set_sequence](../campaign-013-music-set-sequence/README.md) | 0x0041060E | 190 | Near: 8 differing bytes | 168 |
| [bgl_perp_axis](../campaign-013-bgl-perp-axis/README.md) | 0x0041FAA9 | 245 | Research: 61 differing bytes | 211 |
| [bgl_random_vector](../campaign-013-bgl-random-vector/README.md) | 0x0041FC2C | 36 | Body exact; layout pending | 6 |

Music resume is60 body/75 full bytes, sound muted39/54 and random-vector forwarding
31/36. The compiler objects omit original trailing inter-function INT3 padding.
Full targets, full-span mismatch receipts and complete operand inventories remain
unchanged. Separate body comparisons apply the existing strict matcher to the
body extent recorded from Ghidra before source experiments. Both independent
native and Docker objects have zero differing body bytes after verified relocation.
These results are **not** complete function matches or byte exclusions.

The music-sequence candidate preserves unsigned mode/count/threshold logic,
precondition order, callback ABI, live lock depth, cache timing and backend return.
Three natural source experiments leave8 differences over190bytes (4.2105%).
The final unsigned mode>0 expression was supported by the original JBE instruction
and improved the earlier9-byte mismatch. It stays a reviewed near match.
The reference-axis candidate preserves repeated absolute expressions, minimum2,
finite tie behavior and the original90-degree immediate. It differs61bytes over245
(24.898%) and remains research. An initial read-only worker inventory mislabeled
that immediate as180; independent preparation corrected it before any source build.
No mathematical perpendicular guarantee is inferred for arbitrary input.

Independent review strengthened all five fixtures: explicit truth tables, full
owned storage, unsigned extremes, callback mutation and ordering, saved returns,
quietNaN forwarding, pinned x87 unordered selection, and valid float array storage.
No parent candidate-source rewrite was required. Native operations compiled only;
authored fixtures executed in locked Docker. The original game was not executed
or patched. All candidates are C with pinned VC5 /Od /MT and unchanged strict warnings.

There were9 worker candidate compiles,10 independent final candidate compiles and5
fixture builds/executions, with no failed final fixture builds. Body diagnostics
reuse those preserved objects and compile nothing. 3
Docker cases used the verified native-listing fallback; 2
used their own listing without terminators, with complete COFF inventory retained.
Every report and source snapshot is checked against exact staged Git blobs.

The committed [body reproducer](body_evidence.py) is read-only. It verifies original
identity and full targets, current sources, report and object hashes, toolchain
agreement, passing Docker fixtures, all operands and both full-span mismatches
before reporting body evidence. Its [focused checks](test_body_evidence.py) reject
altered hashes/objects, failed fixtures, changed boundaries and missing operands.
Shared build/matching tools are unchanged; full regression and historical accepted
promotion remain deferred. Accepted history stays14functions/19records.

To reproduce a body case with an owned original/toolchain, first run the existing
native and Docker case commands documented in campaign-001, with the Docker
--expect-report pointing to the fresh native report and --behavior enabled.
Then pass both newly generated report paths:

```text
python engine/research/campaign-013/body_evidence.py --result engine/research/campaign-013-music-resume/result.json --reference OWNED_WMAIN.EXE --native-report FRESH_NATIVE_REPORT.json --docker-report FRESH_DOCKER_REPORT.json
```

Using the recorded original local reports needs no report overrides. The tool
prints hashes and comparison metadata, never original bytes. Original content,
licensed compiler files and generated objects stay outside Git. Function-level
results do not establish final linked executable identity or upstream acceptance.
Random-unit XYZ remains preparation-only deferred for x87/rounding investigation.
The fixed overnight window ends08:48UTC; new implementation stops08:38UTC.
