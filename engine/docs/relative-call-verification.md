# Verified relative calls and double constants

The x86 COFF relocation types are documented in Microsoft's
[PE/COFF specification](https://learn.microsoft.com/en-us/windows/win32/debug/pe-format#intel-386-processors).
DIR32 (6) denotes a 32-bit absolute address. REL32 (20) denotes a relative
displacement used by x86 calls and branches. This verifier implements only
external near CALL operands for REL32, not arbitrary relative branches.

A new relative binding explicitly declares kind rel32-call, the operand offset,
the original destination address and the external COFF symbol. It requires:

- A zero object addend and the expected COFF REL32 type.
- An E8 opcode immediately preceding the declared operand in both inputs.
- An original displacement equal to target minus the address following the
  four-byte operand, with x86 32-bit displacement arithmetic.
- A destination backed by the original code section and no overlapping PE
  base relocation. Relative calls do not belong in the PE base-relocation table.
- The declared undefined external symbol with zero symbol value.
- Complete byte equality after resolving that operand in a comparison buffer.

The instruction boundary and semantic destination are reviewed in Ghidra and
documented per function. The verifier is not a general x86 disassembler and does
not independently recover instruction boundaries from arbitrary byte streams.
An original-identity mismatch or undeclared/unsupported relocation fails.

Absolute bindings keep their existing definitions and must exactly account for
all PE base relocations in each compared function. Typed float64 constants check
all eight pointed-to bytes in both the object and original, while the address
operand itself remains four bytes. Float32, float64 and external-symbol binding
forms are mutually exclusive. No source object or original image is rewritten.

Reports separately count DIR32 and REL32-call bytes. Prior evidence with only
absolute operands remains valid without rewriting its records. The registry
checks both categories and derives raw equality from the declared unrelocated
and resolved operand values; a zero-displacement call may already have equal
raw bytes without proving its final link placement.

Synthetic tests cover forward/backward and zero displacements, mixed absolute
and relative operands, changed symbols/opcodes/targets/addends, unsupported
relocation types, overlapping base relocations, truncated or wrong double
constants, unbound instruction mutations, and history-accounting failures.
