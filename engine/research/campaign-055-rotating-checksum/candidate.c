#include "api.h"
// FUNCTION: WMAIN 0x00497988
unsigned int op_rotating_checksum(const unsigned char *data, unsigned int count, unsigned int initial)
{
    const unsigned char *cursor;
    unsigned int checksum;
    cursor = data;
    checksum = initial;
    while (count > 0)
    {
        checksum = ((checksum >> 31) + checksum * 2) ^ *cursor;
        --count;
        ++cursor;
    }
    return checksum;
}
