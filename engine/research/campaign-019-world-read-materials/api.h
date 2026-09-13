#ifndef OP_WORLD_READ_MATERIALS_API_H
#define OP_WORLD_READ_MATERIALS_API_H
typedef struct OP_STREAM
{
    void *handle;
    int mode;
} OP_STREAM;
typedef struct OP_HEADER
{
    unsigned char before_count[0x58];
    unsigned int count;
    unsigned int disk_size;
    unsigned char before_payload[0x7a0];
    unsigned int payload_size;
} OP_HEADER;
typedef struct OP_RECORD
{
    unsigned char bytes[0x28];
} OP_RECORD;
typedef struct OP_WORLD
{
    unsigned char before_count[0xb0];
    unsigned int count;
    unsigned char before_records[4];
    OP_RECORD *records;
} OP_WORLD;
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream);
int op_world_read_materials(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header);
#endif
