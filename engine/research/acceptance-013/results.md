# Acceptance013: connected effects persistence and creation

Six functions totaling 1,752 complete bytes were promoted, bringing accepted coverage from 82 to 88 functions. Every function passed full-span original VC5 comparison, including all 71 relocation operands, and the shared authored behavior suite passed. The connected fixture exercises real halo creation, shield allocation/texture loading and effect save/load paths; it includes callback mutation and failure cases.

Event34 records the initial accepted original-toolchain build. Supplemental CI identified two portability issues: deprecated fixture string copies and a modern definite-assignment warning. Fixture copies now use explicit-length memcpy; only the supplemental compiler initializes the affected local. The VC5 source path is unchanged. Six new sources were added to the supplemental CMake list.

The final clean replay is event35, build `20260913-171152-8a741ca1`, source `0c6278c5bee0944ca7409526b8d5c0f4680f7f94`. All 88 functions match again and the shared behavior suite passes. This adds no further functions. Supplemental source inputs are identical to passing [CI run34782873274](https://github.com/Afejes1/OpenPhantom/actions/runs/34782873274). Tool tests and focused-runner tests also passed at this checkpoint.

No original game was executed. No byte exclusions, warning waivers or artificial padding were introduced. This verifies function code with documented relocation binding; whole-executable layout and byte identity remain pending.
