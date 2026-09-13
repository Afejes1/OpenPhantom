# world skip chunk

**Complete 93-byte relocation-adjusted match**, with every call operand verified and zero differing bytes. Final independent native/Docker verification passes55 focused checks.

Reviewed the two stack arguments and real seek/read/allocate/free callees. Original calls read(buffer,1,bytes,stream), always returns1 and preserves the allocated local through callbacks. Parent replaced integer pointer casts with owned byte addresses and added allocator-visible stream checks, release-time state and a normal positive-size case.

External callbacks are authored fixtures. Shared types, actual backend integration and final linked executable placement remain pending. The explicit verified native-listing fallback was used after complete native/Docker code and fixup equality; the empty Docker listing is retained privately.

See [result](result.json), [all worker experiments](worker-log.json), [static contract](packet.md), and [parent review](../campaign-017/review.json).
