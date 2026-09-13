# Prepared target: sound_deactivate_place

Team-not-done bp/bapsound.c. Full interval0x0041778C..0x004177AE: 34 body/full bytes, no alignment. Coordinator inspected complete body/instructions, next boundary, XREFs and relevant caller/callee ABI. Complete PE relocation inventory and direct targets independently verified against owned original. Pinned VC5 C /Od /MT; immutable target.

Observed cdecl interface:
`void op_sound_deactivate_place(op_sound_placement *placement); extern int op_sound_initialized;`

If initialized==0 exit; otherwise if placement==null exit; otherwise store32-bit0 at placement+0x30 only. Any nonzero initialized value enters. Partial placement type: opaque bytes[0x30], int active, opaque tail[8], total0x3C. The sole caller0x4177AE independently indexes placement records with stride0x3C before passing a pointer. Do not confuse with separate SCAL descriptor stride0x40. All other bytes, including tail, remain unchanged; no sound-release callback is called. Fixture validates sizeof/offsetof, null gate, zero/non-1 initialized, active0/nonzero and full-record byte preservation except active, with adjacent guards.

Worker owns candidate.c/api.h/behavior.c/README.md/worker-log.json only. No target, shared tooling, metadata or Git changes. Existing15min/10candidate attempts/five nonimprovement cap per function; freeze immediately on exact result and report all attempt hashes. Native compile only; authored fixtures execute only in locked Docker. No warnings, exclusions, fabricated frames, raw instructions, original data or original execution. Overall worker deadline04:40:17; final publication04:50:17. Parent independently reviews and runs final verification pairs.
