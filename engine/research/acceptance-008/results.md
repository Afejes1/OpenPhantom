# Acceptance008 results

Three previously exact functions are now integrated and accepted: chunk-header read, chunk skip and the load-via-skip wrapper. Their complete 237 bytes and eight address operands match under the original VC5 toolchain. All 43 registered functions passed; immutable event 29 records source `d0e874375664db668c283c0cf017a267dfec61b3` and all 77 input hashes.

The new shared fixture passed 7,066 checks with zero failures, including the real header-to-wrapper-to-skip call chain. Every earlier fixture suite also passed. The 100 engine tooling tests, 19 focused-runner tests and [full integration CI](https://github.com/Afejes1/OpenPhantom/actions/runs/34769760503) passed. No failed compilation, warning waiver or byte exclusion was needed.

These are three promotions of earlier research results, not three newly reconstructed functions. The canonical types preserve the two-argument skip ABI and the distinct header fields at 0x2D4 and 0x800. NULL/high-bit cases test argument forwarding to authored backend callbacks without performing real allocation or buffer access. Whole-executable byte identity and final linked placement remain pending.
