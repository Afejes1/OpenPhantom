# Requested music state setter

The candidate preserves the two early exits, caches a changed request before
locking, reloads state for the backend after lock, and preserves the backend
result while unlocking and updating the live depth counter. Its first native
compile matches the complete 107-byte target with all eleven bindings verified.

Worker native report: `engine/build/campaign-001-music_set_state-3169af2a68bf/research-result.json`.
The fixture covers both gates, non-one initialization, negative and boundary IDs,
callback-visible ordering and state, live depth changes, callback mutations, and
unchanged zero/negative/positive backend returns. Coordinator final verification is complete. The original executable was not executed.
Final parent review and fresh native/Docker verification confirm the complete 107-byte relocation-adjusted match with 45 focused checks passing.
See [result.json](result.json) for source, toolchain, object, fixture and report hashes.
The verified native-listing fallback was used after code/fixup equality; no bytes
were excluded. Final executable layout and accepted-history promotion remain pending.
