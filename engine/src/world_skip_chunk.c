#include "b3d_chunk_readers.h"

// FUNCTION: WMAIN 0x0041cdf0
int op_world_skip_chunk(OP_B3D_STREAM *stream, unsigned int bytes)
{
    void *buffer;

    if (stream->mode == 0)
    {
        op_stream_seek(stream->handle, (long)bytes, 1);
    }
    else
    {
        buffer = op_allocate(bytes);
        op_stream_read(buffer, 1, (int)bytes, stream);
        op_release(buffer);
    }
    return 1;
}
