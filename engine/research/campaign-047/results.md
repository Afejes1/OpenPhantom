# Campaign047 results

Two previously untouched team-not_done numerical helpers match144 complete bytes. A third is preserved as research. All three independent native/Docker pairs agree on their respective candidates and all3,122 authored fixture checks pass. No exclusions or warning waivers.

| Function | Full bytes | Differences | Status | Checks |
| --- | ---: | ---: | --- | ---: |
| [point_plane_distance](../campaign-047-point-plane-distance/result.json) | 60 | 0 | Raw exact | 131 |
| [vector_angle](../campaign-047-vector-angle/result.json) | 84 | 0 | Relocation exact | 1729 |
| [vector_snap_small](../campaign-047-vector-snap-small/result.json) | 248 | 25 | Research | 1262 |

Point-plane distance matched its first compile. Angle matched its fifth: explicit Y/Z double promotions and the final float store reproduce the observed x87 scheduling and captured binary32 word. Its interim C4244 failure remains recorded; no diagnostic was waived. C++ frontend experiments did not improve either unfinished candidate and were reverted. Snap's widened ternary increased its extent, so the initial natural C candidate is retained. Its25/248 differences exceed the5percent scheduling threshold.

Literal bit expectations cover finite dyadic point/plane arithmetic, read-only aliases and negative zero. Angle coverage includes exact+1 bypass, a dot rounded to+1 before the word test, adjacent values, captured input under callback mutations and authored helper returns. Snap tests boundaries, signed zeros, infinities and quietNaNs: the original VC5 unordered comparison behavior clears a component, while the modern compiler retains it; the fixture explicitly distinguishes these behaviors. Floating-point control is saved and restored. Helper mathematics, arbitrary nonfinite point/plane behavior and whole-executable identity remain outside these claims.

Four exact functions from campaigns046/047 await the next shared acceptance checkpoint. No original executable or game data was executed or published.
