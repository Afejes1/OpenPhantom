#include "api.h"
// FUNCTION: WMAIN 0x0041d360
int op_world_read_sprite_refs(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header)
{
    unsigned int bytes;
    bytes = header->count * 0x3c;
    if (bytes != header->payload_size) return 0;
    world->sprites = op_allocate(bytes);
    world->sprite_count = header->count;
    return op_stream_read(world->sprites, bytes, 1, stream);
}
