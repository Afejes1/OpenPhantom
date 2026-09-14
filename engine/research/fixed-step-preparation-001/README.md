# Fixed-step preparation 001

This is a reviewed static preparation packet, not an implemented or accepted function. No source candidate or compiler experiment was created. The original function at004756FC has a complete302-byte body ending immediately before0047582A, with36 address operands listed in packet.json.

The frame and debug callers both pass zero and clean four stack bytes. This function reads that word only to forward it to task_run_all. A typed source can preserve the call with an explicitly unused unsigned parameter in the scheduler; its previously accepted body must be strictly re-proven unchanged. Do not use an unsafe function-pointer cast or claim the forwarded word has a recovered behavioral purpose.

Before implementation, recover the shared float views at86871C (interpolation alpha),869294 (target) and868728 (simulation). Current reset helpers intentionally access only zero/rawword bits. Convert their fixture seeding and comparisons to explicit raw-bit transport so type recovery does not silently change test patterns. Frame delta868714 is already a float under the historical skills name. Keep one shared storage definition per original address.

The function clamps input delta above0.1f, accumulates target time, selects step1/32 or1/64 from the unresolved rate gate, and loops while simulation is behind target. Each iteration sets the selected world's clock to the smaller of next simulation time and target, runs the scheduler with the forwarded word, broadcasts event14 with the live step's bits, increments the generation word, and advances simulation with the live step. It then computes interpolation alpha and restores the saved clamped input delta. Accepted world clock, scheduler and module broadcast callees are identified in packet.json.

Start with finite PC53 round-nearest fixtures and precomputed terminating cases: below/equal/above the0.1 threshold; both rates; zero tofive iterations; exact target and either side; two owned worlds; callback changes only when the expected sequence and termination are fixed beforehand. Verify argument bits, state visible at every call, live world/step/global reloads, generation ordering and final restoration. Exclude nonfinite values, zero/negative mutated step and backward/unbounded timing changes until separately justified. Do not count a large matrix as proof outside this domain.

The rate-gate and generation-word semantic names remain unresolved. Preserve neutral names pending broader XREF evidence. This packet creates no new acceptance claim and does not reopen any capped function.

Correction from campaign093: strict PE relocation preflight identified eight incorrect operand offsets in this historical packet. Use the [corrected inventory and audit](../campaign-093-advance-runtime-fixed-steps/inventory-correction.json). Original addresses, roles, extent and count36 were unchanged; no prior accepted evidence used these incorrect offsets.
