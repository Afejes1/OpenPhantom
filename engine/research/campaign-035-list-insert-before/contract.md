# list_insert_before

Team manifest: not_done in std/stdlinkl.c. Node prev at0, next at4, eight-byte pointer view. No external calls, globals or address operands. Source is authored from static instruction and call-site evidence.

Full interval 0x00493ed9..0x00493f11, 56 bytes; body 56 bytes. No trailing alignment. Pinned VC5 /Od /MT; zero differences over the full span for acceptance.

Owned guarded nodes and pre-call expected snapshots. Mutators require nonNULL nodes; inserted nodes are distinct and detached. Append and traversals use finite acyclic chains. NULL traversal, signed advance boundaries and unchanged payload are exercised where applicable. Cycles and arbitrary alias insertion are outside this fixture contract. Fifteen minutes/ten implementation attempts; stop after five non-improving attempts.
