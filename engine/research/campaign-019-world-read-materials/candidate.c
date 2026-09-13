#include "api.h"
// FUNCTION: WMAIN 0x0041da1b
int op_world_read_materials(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header)
{
    int i;
    OP_RECORD *record;
    unsigned int size;
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

