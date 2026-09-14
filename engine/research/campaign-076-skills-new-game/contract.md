# skills_new_game

Complete original span 0x00457E60 to 0x00457E93, 51 bytes, no tail. Cdecl void function with no parameters or external calls. The D8-byte shared state at872EE0 contains signed difficulty atC0 (872FA0); typed object/addend bindings preserve that alias instead of declaring separate overlapping globals. Other state words remain opaque. Index globals are at6CFAC0 and872EC8.

New game zeroes the complete state, sets difficulty4, clears active index, then copies its live value into previous index. Easier decrements only signed positive difficulty; harder increments only difficulty below9. Malformed values outside0..9 are not clamped: negative values remain unchanged in easier and increment in harder; values above9 decrement in easier and remain unchanged in harder. No executed operation overflows a signed int.

Fixtures enumerate eight authored whole-state patterns and12 values for each of difficulty, active index and previous index, including both int extrema. Expected state is prepared before the call, and the full state bytes and both independent globals are checked afterward. No callback or original executable execution is involved.
