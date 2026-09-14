# canvas_init

Initialize40-byte canvas. Store flags/raster/two opaque words. Bit0 chooses supplied signed rectangle; otherwise use origin0 and signed raster dimensions minus1. Centers use inclusive signed spans divided by immutable float2, added to signed origins. Return1. Domain excludes signed overflow; raster may be NULL only for supplied rectangle. Literal center-bit fixtures include negative/reversed/zero/odd/even spans.

Team not_done; full spans and operands independently reviewed then confirmed by parent disassembly. VC5 RTM /Od /MT; no exclusions, flag changes or warning waivers. Fifteen-minute/ten-attempt per-function cap, five non-improvement stop. Authored fixtures execute isolated Docker; original executable is never executed.
