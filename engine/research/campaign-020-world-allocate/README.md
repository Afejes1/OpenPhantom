# world_allocate

The original VC5 RTM compiler produces a complete 193-byte match
with 2 verified address operands and zero differing bytes.
The independently reviewed locked-Docker fixture passes 43 checks.

[Source](candidate.c), [contract](contract.md), [fixture](behavior.c),
[receipt](result.json), [parent review](../campaign-020/review.json).

This focused result is separate from immutable acceptance. Shared integration
and final executable placement remain pending. No original executable or real
backend was run. There are no excluded bytes or new warning waivers.
