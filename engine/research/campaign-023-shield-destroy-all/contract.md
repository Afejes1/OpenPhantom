# Static contract

Signed local loop calls slot destroy for indices zero through thirty-one in order, ignoring callback returns.

Complete span: 46 bytes at `0x0043B64D`, no trailing gap. Ghidra TPM:/WMAIN.EXE body, callers and callees independently reviewed; cdecl ABI and observed field offsets confirmed from instructions. Team manifest marks this function not_done. No existing research or accepted duplicate was found. Original executable remains static-only.

The record first word is an integer active flag: the allocator writes one there, and the attached actor pointer is at offset 0x10. This corrects the earlier provisional owner-pointer label. The name array covers the remaining record view, not a recovered capacity guarantee.
