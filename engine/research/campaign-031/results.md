# Campaign031 results

Four exact functions cover **414 complete bytes**. Five authored fixtures pass **8,512,480 checks**; native and locked-Docker function code and relocations agree. Ripple update remains **near-only:4/433 differing bytes (0.924%)**. Its finite behavior tests passing does not promote it to exact.

Function | Full / candidate bytes | Category | Checks
---|---|---|---
[overlay_size](../campaign-031-overlay-size/contract.md) | 10 / 10 | exact-function-match | 16
[projection_token](../campaign-031-projection-token/contract.md) | 16 / 16 | exact-function-match | 2060
[halo_flicker](../campaign-031-halo-flicker/contract.md) | 62 / 62 | exact-function-match | 350
[zap_subdivide](../campaign-031-zap-subdivide/contract.md) | 326 / 326 | exact-function-match | 62
[ripple_tick](../campaign-031-ripple-tick/contract.md) | 433 / 433 | near-match | 8509992

Ten implementation attempts: three first-compile exacts, two attempts for flicker, and five for the parked ripple update. One explicit double experiment failed the warning-as-error gate; narrowing was made explicit, then the best ordinary float candidate was restored. Existing authored local-order evidence avoided another diagnostic compile. Four token-alignment NOPs were naturally reproduced by VC5/O2; no padding was inserted. Original source signedness of the token counter remains unresolved, so the contract promises matching32-bit behavior rather than recovered declaration spelling.
