#include "api.h"
// FUNCTION: WMAIN 0x0041d8ee
int op_world_read_global_vertices(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header)
{
    unsigned int size;
    OP_RECORD *record;
    int i;
    size = header->count * header->disk_size;
    if (size != header->payload_size) return 0;
    record = world->records;
    for (i = 0; i < (int)header->count; ++i) {
        if (op_stream_read(record, (int)header->disk_size, 1, stream) == 0)
            return 0;
        ++record;
    }
    world->count = header->count;
    return 1;
}

