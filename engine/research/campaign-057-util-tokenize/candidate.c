#include "api.h"
// FUNCTION: WMAIN 0x004972de
#pragma intrinsic(strlen)
char *op_util_tokenize(char *cursor, char *out, unsigned int size, const char *delimiters)
{
    char *delimiter;
    unsigned int token_length;
    unsigned int span;
    if (out)
        *out = 0;
    span = op_span_delimiters(cursor, delimiters);
    cursor += span;
    delimiter = op_find_delimiter(cursor, delimiters);
    if (!delimiter)
        token_length = strlen(cursor);
    else
        token_length = (unsigned int)(delimiter - cursor);
    token_length = token_length < size - 1 ? token_length : size - 1;
    if (out)
    {
        op_copy_keyframe_name(out, cursor, token_length);
        out[token_length] = 0;
    }
    return delimiter;
}
