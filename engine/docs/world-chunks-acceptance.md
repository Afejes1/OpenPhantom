# World chunk stream acceptance

Acceptance008 integrates three previously exact helpers: chunk-header read (112 bytes), chunk skip (93 bytes), and the load-via-skip wrapper (32 bytes), with eight verified address operands. The canonical header exposes skip_bytes at0x2D4 while preserving all earlier fields, payload_size at0x800 and the0x804 extent. The stream remains an eight-byte handle/mode view.

The skip helper has two cdecl arguments. Its read count uses the canonical signed declaration with an explicit cast that preserves the original argument word. The wrapper has three arguments and ignores its world pointer. It calls the actual reconstructed skip helper.

The authored fixture checks all23 tag positions, unknown tags, zero and negative read results, live table replacement, bounded comparison and embedded NUL behavior. Whole guarded stream/header/output, table, tag pool, unused world and owned buffer state are checked at callback boundaries and after calls. Direct skip, wrapper with owned/null world, and real header-to-wrapper-to-skip chains cover seek/nonseek modes, zero/normal/high-bit lengths, null allocation, failed backend results and captured allocation identity. Backend fixtures log large lengths without allocating or dereferencing them.

All prior40 target/registry entries are preserved. Existing callback routes remain active outside this fixture. Final acceptance is recorded only after clean original-toolchain build, isolated authored tests, complete-span comparison, immutable record and full integration CI. Research receipts remain separate from accepted history. No original game execution, byte exclusion, warning waiver or whole-executable placement claim.
