# Next preparation: sound buffer creation

No source experiment or execution was performed for this function. The parent and independent reviewer checked the original instructions; this is a preparation record, not an accepted implementation.

`00496AC2..00496BCD` is a complete267-byte interval without alignment tail. The latest contributor manifest calls it an unfinished function in `std/stdsound.c`. It is absent all earlier accepted and research target inventories. The next campaign must verify that exclusion again before allocating work.

Five stack arguments are observed: a nonzero/zero channel selector at+8, sample-rate word at+C, low16 bits per sample at+10, buffer byte count at+14, and flag bits at+18. These names are inferred from field construction; no caller XREF resolves higher-level naming. The current Ghidra decompiler signature is incomplete and must not define the ABI.

The function builds an18-byte wave-format object and a20-byte buffer descriptor. The format occupies a20-byte rounded stack area; its final two padding bytes are not read. The descriptor is five dwords, cleared and then populated. The output slot at EBP-18 is uninitialized before the callback. A separate dword at EBP-1C is initialized to zero; its purpose remains unresolved. Do not absorb it into either structure or invent a semantic local solely to obtain its stack footprint.

It calls device vtable+C with `(device, descriptor, output_address, NULL)`, using the live device pointer word at00862074. The two absolute operand offsets are205 and211; both point to that global. Callback order and stdcall cleanup are directly observed.

The return behavior requires particular care:

| Callback result | Callback-written output | Observed returned word |
| --- | --- | --- |
| HRESULT>=0 | NULL or valid owned object | The output word |
| HRESULT<0 | NULL | The negative HRESULT word still in EAX |
| HRESULT<0 | Valid owned object | Release the object, clear output and return zero |

The null-output failure branch jumps directly to the epilogue and preserves EAX. An earlier reviewer summary incorrectly described every failure return as NULL; the parent flagged the contradiction and the reviewer confirmed the instruction trace. A conventional API summary or pointer-only failure model would lose this behavior.

Never execute a fixture where the failure callback leaves output unwritten: the original reads that uninitialized local. A success callback must also initialize the returned output. Do not introduce a warning waiver, fabricated stack frame, raw bytes or a weakened comparison to accommodate a candidate. Recover the extra initialized local and the defined source representation of the pointer/status return behavior before allocating compiler experiments.

Other work remains parked with its existing limits: C095 resource close/eof/tell differ in extent; C075 resource backend handle and earlier alignment-tail cases remain capped. No cap was reset in this window. Prefer another prepared group of several functions over opening this buffer-creation case prematurely.
