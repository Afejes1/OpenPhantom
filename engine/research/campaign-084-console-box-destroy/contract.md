# console_box_destroy

Full original interval0x0048C519 through0x0048C539, 32bytes, no trailing bytes. Latest team manifest marks unfinished; no prior implementation target/cap found. Parent static preparation began19:31UTC; source packet created2026-09-14T19:38:03.922029+00:00. Cdecl wrappers and stdcall API pointer signatures checked against instruction-level pushes and callee accesses.

Box layout covers the observed0x90-byte allocation and buffer at0x78. Destroy calls heapfree on the initial buffer then reloads the original box pointer, ignoring callback mutations to the stored buffer. The heapfree observer never frees synthetic owned objects. Cursor state uses signed-positive show gating and full32-bit depth storage; hide increments the live depth after any callback, with unsigned wrap modeled explicitly. Cursorinfo is8bytes. Clear calls the actual already accepted setAttr body as fixture support, then fill with2000spaces and origin(0,0), then reloads the live default attribute for restoration; row fill uses80spaces and the signed16-bit row and has no restoration. API callbacks check low16 attribute semantics, stage order, live handle reloads, final/global mutations and output pointer ownership. All fill output objects are exactly4bytes; no rounded stack padding access.

No original execution, padding bytes, exclusions or warning waivers.
