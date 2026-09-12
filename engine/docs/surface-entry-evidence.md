# Surface draw-entry linking

0x004059F0 has a 94-byte body followed by two NOP alignment bytes before
0x00405A50. Team manifest: bp/bapdraw.c, bapdrawOld_pushDrawEntry, not done.
Analysis name: link_surface_draw_entry; purpose confidence HIGH.

The complete body, both direct caller functions and the bucket-index writer
were inspected. The helper defaults to the surface texture byte at +0x24.
If either packed opacity at +0x2B or current opacity at +0x0E is nonzero, it
selects the special bucket index at 0x0059DF88. The world traversal assigns
that index from world.texture_count, so the extra bucket follows the texture
buckets. Existing nonzero opacity is preserved. Otherwise, the low five packed
bits are expanded by multiplying by eight and adding their upper three bits.
A nonzero packed value with low five bits zero still selects the special bucket.

The current gathered-entry index is read from 0x0059DEBC. Each entry is twelve
bytes at 0x008A4880; the next pointer at +4 receives the old bucket head, and
the bucket at 0x008BC880 is updated to the current entry. The helper does not
increment the count or write the entry's surface/third words. The inspected
callers 0x004056C0 and 0x00405A50 perform capacity checks, fill the surface word
and advance the count. Insertion is at the head, so successive calls with
caller-selected new entries form a LIFO chain. Original bucket storage extent
is not inferred from the synthetic fixture's allocation.

## Original compiler frontend choice

The tracked source is .cpp with language c++ in target.json. Actual VC5 RTM
cl.exe uses its fingerprinted c1xx.dll frontend; /O2 /MT and the original linker
remain unchanged. Shared declarations use C linkage so the symbols retain the
expected ABI. No modern C++ features, inline assembly, byte insertion or warning
waiver are used. The full 96-byte section matches after resolving all six DIR32
operands, including the next-field addend four.

The equivalent .c candidate differed in seven instruction bytes in opacity
expansion. Bounded expression/type and target-CPU probes did not match that
candidate. The same expression through VC5 C++ reproduces the original complete
instruction sequence. This establishes a reproducible matching recipe, not the
historical source language or original source text. The team's reported .c
module assignment remains unchanged in the external scheduling manifest.

Source fingerprints and diagnostic/raw-emission policy now include .cpp files.
Git attributes pin .cpp line endings to LF for fresh-checkout reproducibility.
Builds reject missing fingerprinted c1xx.dll or a language/extension conflict;
public history records source path, language, source hash and compiler inventory.
The progress report displays the frontend explicitly. Existing accepted source
definitions and evidence records are not rewritten.

## Behavior evidence

The synthetic fixture checks all 256 packed byte values against zero, partial
and full existing opacity. It checks ordinary/special bucket selection, expansion,
preservation of every other surface byte, next-link updates, untouched bucket
heads, preserved entry fields/count, caller-selected final entry 8191, and a
two-entry LIFO chain. It does not run the original game. Clean original-toolchain
builds and matching records determine acceptance.
