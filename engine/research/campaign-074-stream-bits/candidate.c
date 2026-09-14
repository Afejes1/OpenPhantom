#include "api.h"
// FUNCTION: WMAIN 0x00470b95
unsigned int op_stream_bits(OP_STREAM *stream, int count)
{
    unsigned int mask, result;
    mask = 1u << (count - 1);
    result = 0;
    while (mask)
    {
        if (stream->bit_mask == 0x80)
        {
            if (stream->buffer_position >= stream->buffer_limit)
                return 0;
            stream->bit_accumulator = stream->output[stream->buffer_position];
            ++stream->buffer_position;
        }
        if (stream->bit_accumulator & stream->bit_mask)
            result |= mask;
        mask >>= 1;
        stream->bit_mask >>= 1;
        if (!stream->bit_mask)
            stream->bit_mask = 0x80;
    }
    return result;
}
