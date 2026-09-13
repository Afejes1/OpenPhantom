#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041d5a3
int op_world_read_palettes(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header)
{
    unsigned int bytes;
    bytes = header->palette_count * 0x300;
    if (bytes != header->payload_size) return 0;
    world->palette_count = header->palette_count;
    world->palette_capacity = header->palette_count;
    return op_stream_read(world->palettes, (int)bytes, 1, stream);
}
