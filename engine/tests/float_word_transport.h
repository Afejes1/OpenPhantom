#ifndef OP_FIXTURE_FLOAT_WORD_TRANSPORT_H
#define OP_FIXTURE_FLOAT_WORD_TRANSPORT_H
#include <string.h>
/* Copy object bytes without float evaluation, including NaN payloads. */
static unsigned int op_fixture_float_word(const float *value)
{
    unsigned int result;
    memcpy(&result, value, sizeof(result));
    return result;
}
static void op_fixture_store_float_word(float *value, unsigned int word)
{
    memcpy(value, &word, sizeof(word));
}
#endif
