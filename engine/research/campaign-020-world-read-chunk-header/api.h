#ifndef OP_WORLD_READ_CHUNK_HEADER_API_H
#define OP_WORLD_READ_CHUNK_HEADER_API_H
#include <stddef.h>
typedef struct OP_STREAM
{
    void *handle;
    int mode;
} OP_STREAM;
extern const char *op_chunk_tags[23];
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream);
int op_compare_tag(const char *left, const char *right, unsigned int limit);
int op_world_read_chunk_header(OP_STREAM *stream, unsigned int *payload_size);
#endif