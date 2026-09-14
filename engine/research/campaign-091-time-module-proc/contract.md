# Time module initializer

Full interval00475AB0..00475B1D,109bytes,no tail; ten operands verified from instruction stream and independent packet. One cdecl int message parameter. Only message1 initializes: mode0, platform tick capture, signed tick division by1000.0f into startseconds, previous ticks0, elapsed_ticks then signed_ms_to_seconds into previousseconds, result0. Others preserve state andreturn2. The unsigned stored tickword is explicitly reinterpreted as signed for original FILD semantics.

Focused support files are include-only copies of actual accepted platform_ticks,elapsed_ticks,signed_ms_to_seconds. External time/querycounter APIs supply initializedvalues and inspect every state transition. Both platform paths execute, query return values includezero/-1 whilecounteroutput is initialized, and callback mutations test first-store overwrites and second-call live start reload. Literal exactly representable input/expected tables use finite signed tick multiples1000 and deltas -2000..2000 under saved/restored PC53 RN. No NaN, huge conversion, API-output-failure or all-floating-boundary claim. All time/platform globals and callback identities are checked; shared integration must retain actual callees and restore API pointers and platform control state.

No original execution, exclusions, padding, shortenedspan or warningwaivers.
