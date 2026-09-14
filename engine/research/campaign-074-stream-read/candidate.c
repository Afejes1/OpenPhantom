#include "api.h"
#pragma intrinsic(memcpy)
// FUNCTION: WMAIN 0x00470786
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    unsigned char *cursor;
    int amount, remaining, total;
    cursor = (unsigned char *)destination;
    if (!stream->codec_enabled)
        return op_save_file_read(destination, size, count, stream->file);
    total = 0;
    remaining = count * size;
    while (remaining)
    {
        if (stream->stage_count >= stream->stage_remaining)
            op_stream_refill(stream);
        amount = stream->stage_remaining - stream->stage_count;
        if (!amount)
            return total / size;
        if (amount > remaining)
            amount = remaining;
        memcpy(cursor, stream->staging + stream->stage_count, amount);
        cursor += amount;
        stream->stage_count += amount;
        total += amount;
        remaining -= amount;
    }
    return total / size;
}
