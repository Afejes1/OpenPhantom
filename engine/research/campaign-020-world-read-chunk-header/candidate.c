#include "api.h"
typedef struct OP_CHUNK_HEADER {
    char tag[8];
    unsigned int payload_size;
} OP_CHUNK_HEADER;
typedef char op_chunk_header_size[(sizeof(OP_CHUNK_HEADER) == 12) ? 1 : -1];
typedef char op_chunk_header_payload[(offsetof(OP_CHUNK_HEADER, payload_size) == 8) ? 1 : -1];
// FUNCTION: WMAIN 0x0041cd80
int op_world_read_chunk_header(OP_STREAM *stream, unsigned int *payload_size)
{
    OP_CHUNK_HEADER chunk;
    int i = 0;
    *payload_size = 0;
    if (op_stream_read(&chunk, 12, 1, stream) == 0) return -1;
    while (i < 23 && op_compare_tag(chunk.tag, op_chunk_tags[i], 8) != 0)
        ++i;
    *payload_size = chunk.payload_size;
    return i;
}