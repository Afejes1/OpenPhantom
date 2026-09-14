# elapsed_ticks

Complete original span 0x00475B1D to0x00475B2D, 16 bytes, no tail. Independent static review verified all operands, signatures and latest not_done status. Ticks are transported as uint32 modular subtraction; seconds use x87 floating return; conversion FILD consumes a signed32-bit argument and multiplies the exact binary32 factor3A83126F.

Focused callback fixtures verify before-state, mutate origins/prior state during the call, then test the wrapper's live loads and writes. Delta saves returned now before subtracting the callback-updated prior and stores now afterward. Floating fixtures save/restore control state and use53-bit round-nearest with finite authored inputs. Conversion expected float32 words were independently generated from exact float32 factor and signed integers, including extrema. No uninitialized callback outputs or original execution. Shared integration must retain controlled source probes separately from canonical connected platform chains.
