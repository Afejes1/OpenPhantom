# Ready assignment: integer segment sidedness test

Coordinator static review: function `0x00408648` ends with RET at `0x00408734`;
next function is `0x00408735`. Complete body/span: 237 bytes, no padding, no PE
absolute relocations and no calls. Ghidra's incoming-reference query returned
none; no runtime usage is inferred. The team reports `bapline_segmentsIntersect`
as not done, file unknown, manifest line1324. This is the adjacent scan-line
utility and a bounded follow-up after scan-line reset.

Interface: eight signed integer coordinates x1,y1,x2,y2,x3,y3,x4,y4, cdecl,
returns integer0 or1. No pointers, memory writes, allocation or global state.

The function forms coefficients A=y2-y1, B=x1-x2, C=x2*y1-x1*y2 for the first
segment's supporting line. It evaluates C + A*x3 + B*y3 and C + A*x4 + B*y4.
If both are nonzero and their XOR is nonnegative, return0 immediately.
Otherwise repeat with coefficients for the second segment (points3/4) and
evaluate the first segment's points1/2. Apply the same rejection; return1 when
neither pair rejects. Original grouping adds C to the A product before adding
the B product. Distinct coefficient and evaluation locals are retained for
each phase; the observed frame has ten meaningful integer locals (40 bytes).
Use readable scalar mathematical locals, never an artificial stack array or
padding variables. /Od /MT matches the observed ordinary unoptimized frame.

The original has NO bounding-box or collinear-overlap check. Disjoint collinear
segments return1. Two distinct zero-length segments also return1. Preserve these
observed rules rather than replacing them with a mathematically stricter library
intersection function. All arithmetic is machine-width x86 integer arithmetic;
for ordinary focused cases choose coordinates with no signed overflow. Explicitly
state whether overflow/INT_MIN behavior is tested, and do not overclaim portable
C semantics from matching VC5 output.

Fixture expectations must be independently specified: crossing, parallel
separation, first-pair rejection, second-pair rejection, endpoint contact,
collinear overlap AND disjoint collinear, zero-length combinations, endpoint
reversal and segment swapping. Return nonzero on failure. A small-coordinate
exhaustive/property pass may support the explicit cases, but distinguish repeated
assertions from distinct behaviors. Use an independent determinant formulation
for any oracle, with safe coordinate bounds, rather than copying the candidate.

The source is independently authored from this prose and static analysis.
No original/decompiled source or binary payload is added to public files.
