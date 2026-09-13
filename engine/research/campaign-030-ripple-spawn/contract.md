# ripple_spawn contract

- Address: `0x0043A2F0`; complete span350 bytes.
- ABI: `int __cdecl(int appearance, OP_VEC3 *position, OP_VEC3 *velocity, float yaw, float radius, float radial_velocity, float render_parameter, float lifetime, int required_type)`.
- Team manifest: `not_done`.

Gate on two global latches; scan256 entries for first inactive row. Return0 before dereferencing position if gated or full. Copy position and optional velocity (otherwise zero), set finite radial-growth state and other fields, capture clock, then allocate token. Callback mutations of earlier fields remain; write returned token and required type after callback and return token. Fixtures cover all257 free/full positions and positive/negative/signed-zero finite growth. NaN radial comparisons and invalid owned-buffer domains are not claimed.

Public inputs are authored C and fixtures, semantic contracts, addresses and digest evidence. Original bytes, resources, compiler binaries and raw analysis remain private. Exact requires zero full-span differences after every validated address fixup, native/Docker code and relocation equality, and passing isolated authored fixtures. No padding, warning waivers or byte exclusions. Shared integration and linked placement remain separate gates.
