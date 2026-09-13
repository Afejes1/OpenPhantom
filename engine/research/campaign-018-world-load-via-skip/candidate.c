#include "api.h"
// FUNCTION: WMAIN 0x0041e4ba
int op_world_load_via_skip(OP_STREAM *stream, void *unused_world, OP_HEADER *header)
{
    (void)unused_world;
    op_world_skip_chunk(stream, header->skip_bytes);
    return 1;
}
