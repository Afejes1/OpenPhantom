# audio_play

Full interval 0x00496C3B to0x00496C6F, 52 bytes including every original instruction; no tail. Verified by static Ghidra disassembly and next function. Latest contributor manifest marks this address not_done.

All wrappers use cdecl stack arguments; virtual callbacks are stdcall. Ghidra's phantom ECX parameter on play/unlock conflicts with actual pushes and is not adopted. Pause callback slot48 takes (buffer); gate862074 zero skips and returns0, active calls once and returns1 regardless of callback status or gate mutation. Play slot30 takes (buffer,0,0,loop!=0); unlock slot4C takes (buffer,data,bytes,NULL,0). Play/unlock return1 only for exactly zero callback status, including positive nonzero failures. Their original code contains no address relocation operands. Unlock caller49706B pushes byte count,data,buffer and removes12 stack bytes.

Fixture uses two owned tables/objects, snapshots before execution, tests zero/negative/positive callback results, all gate patterns, and callback mutations of table pointer,payload and gate. It separately expects argument identities and exact call counts. Pause NULL is only executed with gate0. Unlock data identities include NULL and owned tokens, with full32-bit count patterns that callbacks inspect without accessing corresponding ranges. No original binary is executed or published.
