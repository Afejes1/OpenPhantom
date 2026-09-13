#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041d360
int op_world_read_sprite_refs(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header)
{
    unsigned int bytes;
    bytes = header->sprite_ref_count * 0x3c;
    if (bytes != header->payload_size) return 0;
    world->sprites = op_allocate(bytes);
    world->sprite_count = header->sprite_ref_count;
    return op_stream_read(world->sprites, (int)bytes, 1, stream);
}
