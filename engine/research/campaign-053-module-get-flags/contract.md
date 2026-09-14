# module_get_flags

Find module by ID. Missing returns0. Requested bit1 exposes status bit0 asresultbit0; requested bit2 exposes statusbit1 asresultbit1; requestedbit4 exposes statusbit3 asresultbit2. Other requested bits are ignored. Signed one-bit fields plus &1 reproduce observed SHL/SAR extraction without hand-written negative shifts. A literal16-entry table independently defines expected mappings; lookup may mutate raw status and global head/tail before returning.

The36-byte node extent and16-byte name are independently observed in register_subsystem_callback allocation/copy: next0,previous4,id8,status12,opaque callback word16,name20. Callback signature is not inferred here. Bit meanings beyond observed flag mapping remain unnamed. All graph storage, strings, lookup/compare/free services are authored; no original code is executed. No byte exclusions or warning waivers. Shared acceptance and whole-executable identity remain separate.
