# util_strdup

Cdecl pointer return. Intrinsic strlen determines allocation size including NUL before the allocator call. NULL allocation returns NULL. Success copies the same source pointer live after allocation. Authored allocator can replace or truncate owned input within its original length; returned destination has enough guarded storage. Tests cover lengths0,1,2,3,4,15,31,63, four source/destination alignments, allocation failure/two destinations, and three mutation modes. Expected copied bytes are predicted from authored post-callback source before candidate writes. No NULL or unterminated source, overlap, actual heap, or callback growth beyond allocation.

Independent static review confirmed all complete spans and all six cohort operands. No alignment tails, byte exclusions, warning waivers, original execution or whole-program claim. Test callbacks and data are authored; original strings and raw decompiler output are not included.
