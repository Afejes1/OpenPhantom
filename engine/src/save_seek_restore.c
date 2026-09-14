#include "save_stream.h"
// FUNCTION: WMAIN 0x00451fcd
int op_save_seek_restore(long offset, int origin)
{
    return op_stream_seek(op_save_input_stream, offset, origin);
}
