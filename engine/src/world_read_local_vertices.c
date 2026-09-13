#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041d983
int op_world_read_local_vertices(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header)
{
    unsigned int size;
    OP_B3D_LOCAL_VERTEX_RECORD *record;
    int i;
    size = header->local_vertex_count * header->local_vertex_record_size;
    if (size != header->payload_size) return 1;
    record = world->local_vertices;
    for (i = 0; i < (int)header->local_vertex_count; ++i) {
        if (op_stream_read(record, (int)header->local_vertex_record_size, 1, stream) == 0)
            return 0;
        ++record;
    }
    world->local_vertex_count = header->local_vertex_count;
    return 1;
}
