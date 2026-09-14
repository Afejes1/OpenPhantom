# Packed close preparation blocker

004705AF has a complete310-byte interval through004706E5. The null path clears EAX. The nonnull path captures file+0 into a stack local, then calls the canonical void release helper, and never reloads the capture. No static XREFs were found in the Ghidra query. Therefore a reliable nonnull return value is not established. The obvious nonvoid source fallthrough would need a new warning waiver, and an explicit captured-handle return would add instructions. Do not invent either contract. No candidate compiled; this is a preparation blocker, not accepted work.

The flush path uses live state after encode(stream,0), including flag bytes, output count, accumulator and handle; a future fixture must preserve those mutations. Negative signed stage length can produce an enormous copy, so only owned bounded lengths may execute in fixtures.
