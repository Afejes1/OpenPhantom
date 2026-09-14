# window_set_handle

Void cdecl stores the full32-bit window-handle word. Caller498B10 supplies it; getter result is passed as HWND to SetCapture at48D080. Explicit unsigned32 storage transports all bits without host-width pointer assumptions. Fixtures never dereference a fabricated handle and preserve other documented state.

Independent static review confirmed99 full bytes, nine data operands, cdecl ABI and caller context. No external calls or alignment tails. Fixtures vary216 initial states across six word patterns, preserving all documented globals and guarded input. Global adjacency is not assumed as a fixture guard. No original executable, OS operation, exclusion, warning waiver or whole-executable claim.
