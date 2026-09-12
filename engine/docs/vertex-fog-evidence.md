# Projected-vertex commit and fog evidence

Status: unverified candidate, retained in [research](../research/projected-vertex-fog/README.md).
Not included in accepted function counts.

Function 0x00402046 (FUN_00402046), 234 bytes through 0x0040212F, immediately
followed by the matched vertex-span lookup at 0x00402130. No alignment padding
occurs between those functions. The mesh submitter calls it at 0x0040FBF7 after
submitting surviving faces. The original is inspected statically only.

The routine always adds requested to the unsigned projected-vertex count at
0x004DD6EC and returns one. When software fog is enabled at 0x004DD6D0, it first
iterates from the old count to the wrapped sum, computing one packed alpha word
per vertex at 0x00595A00. A wrapped end below the starting count skips the loop.

Projected vertices have stride 16; +8 contains reciprocal camera depth. The
render-queue initializer at 0x00401D30 supplies fog start distance (0x004DD6DC),
range (0x004DD6D8), reciprocal start (0x004DD6CC) and reciprocal end (0x004DD6C4)
from the world fog fields. The material flush reads the alpha words when
software fog is enabled and places them in the outgoing vertex's specular word.

At or above reciprocal start, alpha is 255. Below reciprocal end, alpha is zero.
Between the thresholds, the routine computes
255 - truncate(((1 / reciprocal_depth - fog_start) / fog_range) * 255.0),
keeps the low byte, and shifts it into the high byte of an otherwise zero word.
There is no clamp or capacity check here. The availability helper and caller
invariants govern valid buffer storage.

The x87 disassembly stores intermediate floats while retaining the computation
on the x87 stack. It multiplies by an eight-byte 255.0 constant at 0x004A8020.
Its call at 0x004020E8 targets the existing CRT symbol __ftol at 0x0049A44C.
That helper saves the control word, selects truncation, converts ST0 to a signed
64-bit integer, restores the control word and returns the low/high words in
EAX/EDX. Its trustworthy runtime name is preserved.

The two float comparisons inspect C0 only. In particular a quiet-NaN reciprocal
takes the outer below-start path but fails the inner at-or-above-end condition,
leaving alpha zero. This is original VC5 behavior; a modern ordered C comparison
does not establish it. Original-compiler tests therefore own that assertion.

The target definition contains fifteen DIR32 bindings (including three accesses
to the buffer with addend eight), one explicit REL32 call binding, a float32 one
constant and a float64 255 constant. The [relative-call verifier](relative-call-verification.md)
checks each operand and the complete resolved span. No byte exclusion is used.

The generated progress report and clean build records determine acceptance;
candidate source and behavioral intent alone do not establish matching output.
