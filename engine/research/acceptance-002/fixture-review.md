# Colormap integration fixture review

Started at `2026-09-13T11:59:58.4059904Z`. The fixture links and calls the real reconstructed load and free functions through the canonical `extended_colormaps.h`; it does not reproduce their control flow.

The fixture defines only the shared colormap count and 128-slot pointer array plus authored backend callbacks. All fixture-local symbols use the `colormap_` prefix because the file is textually included in the existing behavior translation unit. It exports `op_test_colormaps()` and has no `main`.

Expected slot arrays are copied before each target call. Expected append or callback effects are then applied explicitly. Every comparison covers all 128 slots. Callback logs are guarded before indexing and use addresses within owned synthetic storage. Covered behavior includes repeated successful loads followed by ordered freeing, failed load without insertion, capacity 128 returning an unregistered result, loader-time count replacement before append, zero and negative free counts, forwarding a null slot, live contraction, and live expansion. The expansion callback calls the real load function, proving that both reconstructed functions observe the same globals and that the free loop consumes the appended entry.

No original code is executed. Parent review and compilation of the integrated target remain pending.

Worker review finished at 2026-09-13T12:03:28.7226539Z. The reviewed fixture SHA-256 is 6b2dfbafc867698a20c3ec176c3f412880cb808fc7ee0b82cb0d73cc1a69b8e0. Both owned files contain LF-only ASCII text.

Parent review: checked the shared load/free callbacks and pre-call full-array
oracles against static retail instructions. Added the last available slot boundary
(count127 to128) followed by all128 ordered frees, visible live counts, reset and
retained-slot checks. No candidate body, target or matcher changes. An early
preparation test run had99 passes and one missing-fixture error because this file
was still being authored; acceptance requires a fresh complete pass.
