#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041d8ee
int op_world_read_global_vertices(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header)
{
    unsigned int size;
    OP_B3D_GLOBAL_VERTEX_RECORD *record;
    int i;
    size = header->global_vertex_count * header->global_vertex_record_size;
    if (size != header->payload_size) return 0;
    record = world->global_vertices;
    for (i = 0; i < (int)header->global_vertex_count; ++i) {
        if (op_stream_read(record, (int)header->global_vertex_record_size, 1, stream) == 0)
            return 0;
        ++record;
    }
    world->global_vertex_count = header->global_vertex_count;
    return 1;
}
