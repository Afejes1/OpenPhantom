#include "api.h"
// FUNCTION: WMAIN 0x0041e3bd
int op_world_read_animation_names(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header)
{
    char *cursor;
    int length;
    int i;
    cursor = (char *)world->names;
    if (op_stream_read(cursor, (int)header->payload_size, 1, stream) == 0)
        return 0;
    cursor += header->count * 4;
    for (i = 0; i < (int)header->count; ++i)
    {
        length = world->names[i].length;
        world->names[i].name = cursor;
        cursor += length;
    }
    world->count = header->count;
    world->payload_size = header->payload_size;
    if (world->count != 0 && world->actors == 0)
        world->actors = op_allocate(world->count * 4);
    return 1;
}
