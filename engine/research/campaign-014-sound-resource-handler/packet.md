# Prepared sound resource handler

Full original 004157D0..004158F0:288bytes including all body, no omitted tail. Ten
operands verified against pinned original identity and complete PE relocations.
Registered by initialize_game_systems as resource type0x42534E44. Caller resource
event shape agrees with default_resource_type_handler. No implementation yet.

C int op_sound_resource_handler(OP_SOUND_RESOURCE_EVENT *event). Fields at0,4,8,12,16:
int operation; const char *name; unsigned int resource_type; void *data; unsigned int size.
Default return0. Operation1 initializes local size0, bufferNULL, handle0; calls
live op_sound_file_api->open(event->name, op_sound_read_mode), then calls live
file_api->size(event->name) even if handle==0. If handle!=0: allocate(allocation_tag,size),
read(handle,buffer,size) even if allocation returnsNULL, then close(handle). If
buffer!=NULL register(event->resource_type,event->name,buffer), IGNORE registration
return, set result1. Op2 free(event->data), return1. Op3 event->size=resource_size(event->data),
return1. Otherops no side effects. Event fields and file_api are live across callbacks.

File interface only observed fields: opaque48bytes, cdecl open(const char*,const char*)
return int @0x30; cdecl close(int) return int @0x34; cdecl read(int,void*,unsigned int)
return int @0x38; opaque20bytes; cdecl size(const char*) return unsigned int @0x50.
Call stack cleanup is8,4,12,4 bytes respectively. Ghidra's initial decompilation
incorrectly displayed five open arguments; original caller pushes exactly TWO.
Mode is authored common string rb matching observed read-only mode; global symbolic
binding retains original address. Allocation is cdecl void*(unsigned int tag,unsigned int bytes),
real wrapper ignores first argument and forwards second to allocator00495290, preservingEAX.
Free is cdecl void(void*), size is cdecl unsigned int(void*) reading allocation length.
Register cdecl int(unsigned int type,const char*name,void*buffer), matches recovered
callee004718B9 and all three stack args. No calling convention guesses or extra guards.

Fixture must exercise0/unknown and1/2/3, open failure with size stillcalled, allocation
failure with readNULL thenclose, success ignoring registerfailure, live interface
replacement/name/type/tag mutation after callbacks and exact order. Own all storage;
stubs logNULL instead of dereferencing it. Preserve request fields except op3 size,
full surrounding storage. Test all pertinent paths/return values and unsigned sizes.
Native compileonly; worker owns candidate/api/fixture/README/log; parent owns immutable
case/target/packet. Preserve existing caps and finish fixture before handoff.
