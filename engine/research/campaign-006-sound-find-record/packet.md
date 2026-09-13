# Prepared target: sound_find_record

Team-not-done bp/bapsound.c. Full interval0x00417043..0x0041705B: 24 body/full bytes, no alignment. Coordinator inspected complete body/instructions, next boundary, XREFs and relevant caller/callee ABI. Complete PE relocation inventory and direct targets independently verified against owned original. Pinned VC5 C /Od /MT; immutable target.

Observed cdecl interface:
`void *op_sound_find_record(const char *name); extern void *op_sound_registry; void *op_sound_lookup(void *registry, const char *name);`

Call lookup(current global registry,name), preserve returned pointer unchanged. No name or registry null gate. Callee0x493BDB pushes its[EBP+8]registry and[EBP+C]name to underlying hash search, returning node data at+0xC ornull. Caller0x4169BD tests returned pointer then uses record resource; full callee/body reviewed. Fixture use authored registry/name/returned records, both null and nonnull forwarded pointers, exact argument order, distinct identity and registry mutation retained. No actual lookup implementation or string contents imported.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No target, shared tooling, metadata or Git changes. Existing15min/10candidate attempts/five nonimprovement cap per function; freeze immediately on exact result and report all attempt hashes. Native compile only; authored fixtures execute only in locked Docker. No warnings, exclusions, fabricated frames, raw instructions, original data or original execution. Overall worker deadline04:40:17; final publication04:50:17. Parent independently reviews and runs final verification pairs.
