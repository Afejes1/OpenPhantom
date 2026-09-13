#ifndef OP_CAMPAIGN_017_WORLD_READ_PALETTES_API_H
#define OP_CAMPAIGN_017_WORLD_READ_PALETTES_API_H
typedef struct OP_STREAM { unsigned char opaque[8]; } OP_STREAM;
typedef struct OP_HEADER { unsigned char a[0x20]; unsigned int palette_count; unsigned char b[0x7dc]; unsigned int payload_size; } OP_HEADER;
typedef struct OP_WORLD { unsigned char a[0x64]; unsigned int palette_count; unsigned int palette_capacity; void *palettes; } OP_WORLD;
int op_stream_read(void *destination,unsigned int size,unsigned int count,OP_STREAM *stream);
int op_world_read_palettes(OP_STREAM *stream,OP_WORLD *world,OP_HEADER *header);
#endif
