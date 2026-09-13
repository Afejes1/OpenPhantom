# Campaign018 results

Six team-reported unfinished functions matched on their first worker compile.
Independent parent VC5/native and Docker verification confirmed639 complete bytes,
11 call bindings and241 passing fixture checks. No candidate body corrections,
failed compiler attempts, byte exclusions, new warning waivers or matcher changes.

| Function | Full bytes | Calls | Fixture checks |
| --- | ---: | ---: | ---: |
| world_free_graphics | 64 | 3 | 35 |
| world_load_via_skip | 32 | 1 | 36 |
| world_free_materials | 149 | 2 | 45 |
| world_free_pointer_table_220 | 118 | 1 | 44 |
| world_free_pointer_table_620 | 118 | 1 | 21 |
| world_free_tail_blocks | 158 | 3 | 60 |

One Sol medium worker handled the queue sequentially. Parent review expanded five
fixtures for callback-visible states, NULL slots, signed bounds and complete
branch combinations; source bodies were unchanged. The sixth fixture needed no
behavior changes. Twelve parent candidate compiles and six fixture compiles
passed. Measured compiler/runner times are in metrics.json; these are elapsed
operations, not a controlled model benchmark or total model reasoning time.

Ghidra now has six evidence-backed names, signatures and comments; all six new
signatures re-decompiled with the verified cdecl arguments, and the world destructor
was re-decompiled to confirm the recovered cleanup call sequence. The project was
saved. The older skip helper still has stale thiscall metadata; wrapper comments
warn that its displayed extra argument is not part of the actual two-argument ABI.

Pointer array capacities remain partial type evidence. The table620 research view
exposes only four tested slots and does not claim the original complete capacity.
Its complete compiled function matches, but canonical storage must be reconciled
before integrated C-domain claims. Real release requires a valid allocator pointer;
NULL forwarding in the tail fixture only demonstrates what the original code
passes. The original executable and real allocator/backend were not executed.

This adds six focused exact function results, not six immutable acceptance
promotions. Registered accepted history remains26 functions/23 records following
acceptance003. Final linked placement and whole-executable identity remain pending.
The next prepared packet has six world readers; keep the same zero-difference gate
and time caps. Full regression is reserved for the next integration checkpoint.
