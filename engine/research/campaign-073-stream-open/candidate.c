#include "api.h"
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x00470c66
OP_STREAM *op_stream_open(void *file)
{
    OP_STREAM *stream;
    if (!file)
        return 0;
    stream = (OP_STREAM *)op_allocate(sizeof(OP_STREAM));
    if (!stream)
        return 0;
    memset(stream, 0, sizeof(OP_STREAM));
    stream->file = file;
    stream->bit_mask = 0x80;
    stream->staging_offset = 1;
    return stream;
}
