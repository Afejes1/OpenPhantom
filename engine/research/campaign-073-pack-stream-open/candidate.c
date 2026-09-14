#include "api.h"
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x004706e5
OP_PACK_STREAM *op_pack_stream_open(void *file)
{
    OP_PACK_STREAM *stream;
    if (!file)
        return 0;
    stream = (OP_PACK_STREAM *)op_allocate(sizeof(OP_PACK_STREAM));
    if (!stream)
    {
        op_close_diagnostic_stream(file);
        return stream;
    }
    memset(stream, 0, sizeof(OP_PACK_STREAM));
    stream->base.file = file;
    stream->base.bit_mask = 0x80;
    stream->base.packing = 1;
    stream->base.staging_offset = 1;
    stream->enabled = 1;
    stream->window_count = 0x1000;
    stream->window_remaining = 0;
    stream->window_position = 0;
    return stream;
}
