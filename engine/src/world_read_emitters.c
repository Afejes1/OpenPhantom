#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041d302
int op_world_read_emitters(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header)
{
    unsigned int bytes;
    bytes = header->emitter_count * header->emitter_record_size;
    if (bytes != header->payload_size) return 0;
    world->emitter_count = header->emitter_count;
    return op_stream_read(world->emitters, (int)bytes, 1, stream);
}
