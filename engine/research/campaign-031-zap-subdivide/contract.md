# zap_subdivide contract

- Address: `0x0043D784`; full span326 bytes.
- ABI: `void __cdecl(int start, int end, int depth, float amplitude)`.
- Team manifest: `not_done`.

Depth0 returns before any point or RNG access. Otherwise compute signed midpoint (start+end)/2 within safe nonnegative bounds; for x,y,z capture the endpoint sum before each RNG call, then write the midpoint using the observed float constants. Decrement depth, scale amplitude by the binary32 two-thirds constant, recurse left then right. Fixtures cover depth0..4 and owned19-vector scratch, literal depth2 RNG preorder, nonzero depth1 jitter and mutations proving captured x sums versus later live y/z loads. Negative depth, overflowing index sums and out-of-range buffers are excluded source domains, not excluded bytes.

Exact requires zero complete-span differences after every validated address binding, matching native/Docker code and fixups, and a passing isolated authored fixture. Near means at most5% differing bytes at the same complete extent, never accepted as exact. Only authored source/contracts/digest evidence are public; original bytes, assets, resource strings and toolchain binaries remain private. No original execution, warning waivers or byte exclusions.
