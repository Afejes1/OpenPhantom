# util_strlwr

Void cdecl. Reads each live signed char, stops on NUL, forwards signed value to callback, stores returned low byte at current location, advances and reads next live byte. All256 initial byte values, three lengths/four alignments, current-byte overwrite and future-byte early termination are covered by an authored observer. Negative inputs are sign-extension observations only, not claims about real CRT tolower domain. Full input snapshots plus independent cursor and final termination assertion catch missing/extra calls and writes. Return high bits are deliberately nonzero; a returned current NUL must not stop later iteration.

Independent static review confirmed all complete spans and all six cohort operands. No alignment tails, byte exclusions, warning waivers, original execution or whole-program claim. Test callbacks and data are authored; original strings and raw decompiler output are not included.
