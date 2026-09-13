#ifndef OP_CAMPAIGN_018_WORLD_LOAD_VIA_SKIP_API_H
#define OP_CAMPAIGN_018_WORLD_LOAD_VIA_SKIP_API_H
typedef struct OP_STREAM { void *handle; int mode; } OP_STREAM;
typedef struct OP_HEADER { unsigned char opaque[0x2d4]; unsigned int skip_bytes; unsigned char tail[0x528]; unsigned int payload_size; } OP_HEADER;
int op_world_skip_chunk(OP_STREAM *stream,unsigned int bytes);
int op_world_load_via_skip(OP_STREAM *stream,void *unused_world,OP_HEADER *header);
#endif
