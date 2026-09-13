# projection_compare

Read unsigned 32-bit surface/address key at8 and unsigned sequence at64. Lexicographic ascending order with exact -1,0,1 results; do not modify records. Source uses integer address words to avoid unrelated-pointer ordering UB. Ghidra callback is currently a label: function creation failed without changing bytes; static memory disassembly is the authoritative evidence.

All fields outside the partial 68-byte layout remain opaque. Full recorded span includes naturally emitted compiler alignment; no synthetic padding or byte exclusions. Team manifest marks the function not_done. Ten implementation attempts or fifteen minutes; stop after five consecutive non-improving attempts.
