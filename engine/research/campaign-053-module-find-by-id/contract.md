# module_find_by_id

Starting at head006d64c4, follow next@0 and return the first module whose id@8 equals the signed input bit pattern, otherwise null. Guarded acyclic lists cover0..5nodes, rotations, duplicates, missing and signed boundary IDs; no cycle/capacity claim.

The36-byte node extent and16-byte name are independently observed in register_subsystem_callback allocation/copy: next0,previous4,id8,status12,opaque callback word16,name20. Callback signature is not inferred here. Bit meanings beyond observed flag mapping remain unnamed. All graph storage, strings, lookup/compare/free services are authored; no original code is executed. No byte exclusions or warning waivers. Shared acceptance and whole-executable identity remain separate.
