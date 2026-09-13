#ifndef OP_CAMPAIGN_017_WORLD_READ_EMITTERS_API_H
#define OP_CAMPAIGN_017_WORLD_READ_EMITTERS_API_H
typedef struct OP_STREAM { unsigned char opaque[8]; } OP_STREAM;
typedef struct OP_HEADER { unsigned char a[0x2e8]; unsigned int count; unsigned int disk_size; unsigned char b[0x510]; unsigned int payload_size; } OP_HEADER;
typedef struct OP_WORLD { unsigned char a[0xccc]; unsigned int emitter_count; void *emitters; } OP_WORLD;
int op_stream_read(void *destination,unsigned int size,unsigned int count,OP_STREAM *stream);
int op_world_read_emitters(OP_STREAM *stream,OP_WORLD *world,OP_HEADER *header);
#endif
