#include "b3d_chunk_readers.h"
// FUNCTION: WMAIN 0x0041e3bd
int op_world_read_animation_names(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header)
{
    char *cursor;
    int length;
    int i;
    cursor = (char *)world->animation_names;
    if (op_stream_read(cursor, (int)header->animation_name_payload_size, 1, stream) == 0)
        return 0;
    cursor += header->animation_name_count * 4;
    for (i = 0; i < (int)header->animation_name_count; ++i)
    {
        length = world->animation_names[i].length;
        world->animation_names[i].name = cursor;
        cursor += length;
    }
    world->animation_name_count = header->animation_name_count;
    world->animation_name_payload_size = header->animation_name_payload_size;
    if (world->animation_name_count != 0 && world->actor_resources == 0)
        world->actor_resources = op_allocate(world->animation_name_count * 4);
    return 1;
}
