#include "api.h"
// FUNCTION: WMAIN 0x00470cbc
void *op_stream_close(OP_STREAM *stream)
{
    void *file = 0;
    if (stream)
    {
        file = stream->file;
        op_release(stream);
    }
    return file;
}
