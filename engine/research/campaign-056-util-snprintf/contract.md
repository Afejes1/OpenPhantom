# util_snprintf

Void cdecl variadic wrapper forwards destination, unsigned size-1, format and va_list to backend; ignores result, writes destination[size-1]=0, clears local va_list. Fixture domain requires size>=1 and enough owned storage: no guard is invented for the original size-zero underwrite. Observer decodes signed int, pointer and promoted double arguments; checks callback identity/order and independent guard snapshots, writes bounded sentinels, and returns negative/zero/large results. Final terminator is predicted before return. This tests forwarding and termination, not CRT formatting semantics.

Independent static review confirmed all complete spans and all six cohort operands. No alignment tails, byte exclusions, warning waivers, original execution or whole-program claim. Test callbacks and data are authored; original strings and raw decompiler output are not included.
