#ifndef OP_WORLD_READ_GLOBAL_VERTICES_API_H
#define OP_WORLD_READ_GLOBAL_VERTICES_API_H
typedef struct OP_STREAM
{
    void *handle;
    int mode;
} OP_STREAM;
typedef struct OP_HEADER
{
    unsigned char before_count[0x48];
    unsigned int count;
    unsigned int disk_size;
    unsigned char before_payload[0x7b0];
    unsigned int payload_size;
} OP_HEADER;
typedef struct OP_RECORD
{
    unsigned char bytes[0x20];
} OP_RECORD;
typedef struct OP_WORLD
{
    unsigned char before_count[0x94];
    unsigned int count;
    unsigned char before_records[4];
    OP_RECORD *records;
} OP_WORLD;
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream);
int op_world_read_global_vertices(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header);
#endif
