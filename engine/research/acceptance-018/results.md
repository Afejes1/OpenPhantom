# Acceptance018 results

The previously exact sprite frame resolver is accepted: 87 complete bytes and one verified operand, bringing 108 to 109 registered functions. The clean original-toolchain build `20260913-185153-a2cd920b` at `d911f98989eef764f40c0eab201f099802ad9848` passes all 109 full-span comparisons and shared authored fixtures. [Event40](../../evidence/runs/000040-a1c65c98aca7bee8.json) records the source snapshot. [Full supplemental CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34788111788) passes on the same source.

The real frame resolver now connects to zap geometry. The focused fixture reports 578 checks and connected geometry 3,755,304 checks; repeated state assertions are not unique behavior counts. Tests exercise null sprites, prepare failure, captured material/frame pointers despite callback retargeting, live sprite changes during random generation, and submission timing. Independent review found no material issue.

The 1e-5 tolerance applies only to authored finite midpoint expectations. Compiled-byte acceptance still requires zero differences with all operands verified. No byte exclusions, warning waivers or artificial padding were added. The original game was not executed; whole-executable identity remains pending. Sprite acquisition and release remain the next integration checkpoint.
