# Model frame-state initialization

0x00401D30 through 0x00401EB7 is a 392-byte function with no inter-function
padding. The next function is the matched packet submission at 0x00401EB8.
Team manifest: bp/bapdraw.c, bapdraw_setFrameState, reported not done.
Analysis name: initialize_model_frame_state; purpose confidence HIGH.

The complete body, its four callees, direct callers and global cross-references
were inspected statically. It is called from run_game_frame (0x0043EA09),
run_debug_single_step_frame (0x0046A6BA), and the menu subsystem (0x0045D538).
It resets packet/vertex counters, calls the matched projection publisher,
then calls the backend frame-begin routine at 0x00487C20. This backend resets
two counters, increments a frame word and sets an active flag. The other backend
calls read the current render flags at 0x00487A10 and query the device capability
mask 0x100 at 0x00487B30. Backend implementation names in our fixture describe
these observed contracts; their team-done implementations are not imported.

Frame flags at 0x004DD6E4 are first set to 0x8033, optionally ORed with 0x1800,
then replaced by the backend flags with bits 0x1800 cleared. A mode switch adds
0x1000, 0x1800 or 0x0800 for modes 1, 2 or 3 respectively. Other modes add no
bits. The otherwise overwritten initial write remains in reconstructed C.

The extra flags at 0x004DD6C8 are zeroed. Their downstream reader ORs this word
into the material submission flags; this is an integer flag word, not a depth
bias. An auxiliary word at 0x0059DA00 is set to the Boolean negation of the option
at 0x0089FDEC. The returned direct-XREF set contains only its two writes in this
routine; no broad claim about indirect accesses or its purpose is made. Option
names retain address anchors while their meanings remain unresolved.

Software fog is initially disabled. A nonnull world with bit one at +0x210
sets frame flag 0x40. If the capability query returns zero, software fog is
selected and world floats +0x218/+0x21C are copied into start/end globals.
The code computes both reciprocals, stores end minus start as range, and only
then disables software fog if the range comparison takes its below-zero path.
Flag 0x40 is not cleared by that rejection. Skipped setup preserves old fog
start/end/range/reciprocal values. Equal bounds, including signed zeros, leave
software fog enabled; no clamping or zero-division guard is added.

The original comparison tests x87 C0 only, so an unordered range also disables
software fog. Its range store retains the x87 value for comparison. VC5-only
fixture assertions cover quiet NaN and infinity-minus-infinity. Both fixtures
cover finite ranges, all switch paths, signed-zero reciprocals, infinities,
stale-value preservation, counter reset, real projection integration, backend
call ordering, and unchanged x87 stack/control state. Backend stubs are explicit
interaction tests, not claims that those renderer functions are reconstructed.

The complete C function matches under VC5 RTM /Od /MT after verifying 39 DIR32
operands and four REL32 near calls. No byte exclusion, inline assembly or warning
waiver is introduced. Clean build records determine accepted matching status.
