# palette_resource_set_name

Copy the NUL-terminated input into name storage005bacf0 with the observed intrinsic strcpy sequence, acquire_resource(0x50414c54,name_storage), store its returned pointer to current005badf4 and return1 even when acquisition returns null. The byte-copy extent and terminator are checked for authored lengths0/1/3/4/31/259. The fixture reserves260 bytes before an8-byte trailing canary; this is a bounded tested domain rather than a universal safe-input claim. No overlapping source/destination or overlong name is tested. Acquisition sees the completed copy and can mutate source/name/current; the returned pointer overwrites the callback's current-pointer mutation.

All storage and strings are authored. Complete guarded map/name/global snapshots and callback order are checked. Original program execution, copied game data, byte exclusions and warning waivers are prohibited. Individual functions remain separate from shared acceptance and whole-executable identity.
