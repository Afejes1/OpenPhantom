#ifndef OP_CAMPAIGN_017_WORLD_READ_SPRITE_REFS_API_H
#define OP_CAMPAIGN_017_WORLD_READ_SPRITE_REFS_API_H
typedef struct OP_STREAM { unsigned char opaque[8]; } OP_STREAM;
typedef struct OP_HEADER { unsigned char a[0x2d0]; unsigned int count; unsigned char b[0x52c]; unsigned int payload_size; } OP_HEADER;
typedef struct OP_WORLD { unsigned char a[0xcb4]; unsigned int sprite_count; void *sprites; } OP_WORLD;
void *op_allocate(unsigned int bytes);
int op_stream_read(void *destination,unsigned int size,unsigned int count,OP_STREAM *stream);
int op_world_read_sprite_refs(OP_STREAM *stream,OP_WORLD *world,OP_HEADER *header);
#endif
