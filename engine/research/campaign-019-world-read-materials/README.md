# world_read_materials

Original VC5 RTM compilation produces 149 complete bytes with 6
remaining differences (4.0268%) after one verified call binding.
Category: **near-match**. The locked-Docker authored fixture passes
112 checks. All six differences are the swapped size/index local stack displacements.

[Contract](contract.md), [source](candidate.c), [fixture](behavior.c),
[receipt](result.json), [review](../campaign-019/review.json).

Research evidence is separate from immutable acceptance. No original executable
was run, and whole-executable placement remains pending. No excluded bytes or
new warning waivers; all mismatches remain unaccepted.
