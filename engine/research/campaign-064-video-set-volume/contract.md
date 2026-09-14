# video_set_volume

Cdecl captures record pointer before global-driver gate. Zero driver skips without dereference; otherwise forwards captured record handle and full volume word to stdcall import. Callback mutation survives. NULL record only tested for gate0. Imported return is unobserved.

Owned guarded24-byte records and104-byte descriptors, complete names/token backing, globals and import identities are verified. All handles/driver values are owned opaque pointers; no Bink, audio, DirectX, OS or original-game code executes. Imported ignored results use synthetic integer returns solely for testing lack of dependence; no exact SDK return type is inferred. OpenMiles is transported as an opaque identity and never invoked. Original op_allocate/op_release and prior C007 op_sound_get_driver identities are reused. Initial review arithmetic/tail mistakes were corrected before preparation:17 operands total and done23/28 with5CC tail. No byte exclusion or warning waiver.
