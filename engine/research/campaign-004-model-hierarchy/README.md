# Recursive model hierarchy submission

The independently reviewed C implementation preserves hierarchy preorder,
sibling-chain order, hidden-mesh versus amputated-subtree behavior, matrix and
mesh selection, and callback-driven state/count changes. The full original
interval is 144 bytes: 135 body bytes and nine following NOP alignment bytes.

**Research candidate: 8/144 bytes differ (5.556%).** All five address operands,
including the direct recursive call, are verified. All 17 focused assertions pass.
The original pointer calculation uses EAX while the candidate uses ESI/ECX;
these eight bytes are not excluded. This exceeds the 5% near-match scheduling
threshold and is neither exact nor accepted. Native and locked-Docker code and
relocation inventories agree. See the [receipt](result.json).

The worker made five compiler invocations and froze two source variants. Its
initial do-loop omitted the original unsigned precheck; the retained natural
for-loop restores it. A failed header/source edit and an empty Docker listing
are retained in [the attempt log](worker-log.json). The final worker run was
limited by the matcher's former external-symbol-only call support.

The coordinator added strict direct-recursion verification with failure tests,
added a source annotation, formatted the fixture and expanded it from 9 to 17
checks. Final fresh native/Docker verification found the eight-byte mismatch;
no semantic candidate rewrite, target change, warning waiver or byte exclusion
was made. The verified native-listing fallback was needed for the empty Docker
listing. Public files contain authored source and hash evidence only.

Reproduce with the pinned compiler and owned reference. Both commands return 1
because the full-span byte mismatch remains even when the fixture passes:

```powershell
python engine/research/campaign-001/verify.py --case engine/research/campaign-004-model-hierarchy --runner native --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE
python engine/research/campaign-001/verify.py --case engine/research/campaign-004-model-hierarchy --runner docker --reference C:/Dev/Ghidra/projects/TPM/WMAIN.EXE --behavior --expect-report <native-report-path> --native-listing-fallback
```

Final native report: engine/build/campaign-001-queue_model_hierarchy-75cda616cd84/research-result.json.
Final Docker report: engine/build/campaign-001-queue_model_hierarchy-f4c60f335c05/research-result.json.
Original game code was never executed; final executable layout remains unverified.
