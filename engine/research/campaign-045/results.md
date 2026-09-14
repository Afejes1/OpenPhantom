# Campaign045 results

Two previously untouched, team-not_done palette builders are preserved as research candidates. Both now compile to the complete original function lengths with all five address operands verified, but they do not match the original bytes.

| Function | Full/candidate bytes | Different bytes | Difference |
| --- | ---: | ---: | ---: |
| [colormap_build_blend](../campaign-045-colormap-build-blend/result.json) | 375/375 | 41 | 10.93% |
| [colormap_build_mono](../campaign-045-colormap-build-mono/result.json) | 428/428 | 57 | 13.32% |

There were three implementation attempts per function: an initial C4244 compile failure, an explicit byte-conversion correction, and row-pointer indexing that restored the full extent. Failures are retained. Remaining differences appear in local stack placement; identifying meaningful local declarations/names is a future research question, not permission for artificial padding.

Independent native/Docker pairs agree on each candidate. Four Mono cases traverse all64x256 cells; five Blend cases traverse all256x256 cells, including finite extrapolation. Guards, complete output/palette snapshots, literal floating-point inputs, callback order and low-byte wrap checks pass. The assertion counts mostly come from repeated checks during these complete table traversals and do not mean millions of independent behaviors. The nearest-colour callback remains an authored observer.

Run python engine/research/campaign-045/check_oracles.py to reproduce the authored expected words. No game content, byte exclusions or warning waivers were added. Neither function is accepted or classified as a near match. Whole-executable identity remains pending.
