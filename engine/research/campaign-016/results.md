# Campaign 016 results

**Three exact functions,159 full-span bytes.** All82 focused assertions pass.

| Function | Address | Full bytes | Status | Focused checks |
| --- | --- | ---: | --- | ---: |
| [texture_set_owned](../campaign-016-texture-set-owned/README.md) | 0x00429580 | 29 | Exact | 27 |
| [texture_get_material](../campaign-016-texture-get-material/README.md) | 0x0042985f | 33 | Exact | 7 |
| [extended_pick_face_update](../campaign-016-extended-pick-face-update/README.md) | 0x004288d1 | 97 | Exact | 48 |

All three natural C candidates matched on the first worker compile. The material
getter is raw-byte exact; ownership forwarding and face callback selection match
after all five address bindings are verified, including the original1.0f constant.
The selector preserves x87 unordered comparison, independent trail/animation branch
precedence and live material/count reload after the RNG callback.

Independent review strengthened valid callback returns and owned pointers, added
callback-time trail state and old-cel observations, zero-count/NaN combinations,
and original-material preservation when the callback replaces the material. All
expectations are captured before execution and changed only for intended effects.
No candidate source rewrite or final fixture compile failure was needed.

Three worker candidate compiles, six independent final candidate compiles and three
successful fixture builds/executions produced the receipts. All Docker cases used
the existing explicitly verified native-listing fallback; original empty listings
remain private with hashes. Code, relocations, input/toolchain snapshots and exact
staged source blobs agree. See [metrics](metrics.json) and [review](review.json).

Ghidra names and comments were updated and saved; the existing trusted material
getter name was retained. The mesh preparation caller was re-decompiled. No original
binary bytes were changed or executed. Candidate C sources, headers, synthetic
fixtures and metadata are committed; original content, licensed tools and generated
objects remain private. Accepted history stays21functions/21records. These exact
research functions still need shared ABI acceptance; full executable identity is
not established. Shared matcher/build tools and warning rules are unchanged.
