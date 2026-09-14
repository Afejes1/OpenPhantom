#include "gameplay_random.h"
// FUNCTION: WMAIN 0x00470da1
int op_gameplay_roll(int count, unsigned int sides)
{
    int result = 0;
    int i;
    for (i = 1; i <= count; ++i)
        result = result + 1 + (unsigned int)op_gameplay_random() % sides;
    return result;
}
