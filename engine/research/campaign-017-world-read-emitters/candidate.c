#include "api.h"
// FUNCTION: WMAIN 0x0041d302
int op_world_read_emitters(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header)
{
    unsigned int bytes;
    bytes = header->count * header->disk_size;
    if (bytes != header->payload_size) return 0;
    world->emitter_count = header->count;
    return op_stream_read(world->emitters, bytes, 1, stream);
}
