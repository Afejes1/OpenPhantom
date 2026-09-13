# Static contract

Signed slot bounds and nonzero active flag check precede releasing the sprite-handle field, acquiring the live name, publishing its handle, and copying the terminated name on success. A failed acquire still replaces the field with null. Fixture names are short and owned; no buffer-safety repair.

Complete span: 148 bytes at `0x0043AE53`, no trailing gap. Ghidra TPM:/WMAIN.EXE body, callers and callees independently reviewed; cdecl ABI and observed field offsets confirmed from instructions. Team manifest marks this function not_done. No existing research or accepted duplicate was found. Original executable remains static-only.

The record first word is an integer active flag: the allocator writes one there, and the attached actor pointer is at offset 0x10. This corrects the earlier provisional owner-pointer label. The name array covers the remaining record view, not a recovered capacity guarantee.
