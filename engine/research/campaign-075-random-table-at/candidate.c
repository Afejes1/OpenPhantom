#include "api.h"
// FUNCTION: WMAIN 0x00470e85
int op_random_table_at(int index)
{
    if (index > 1117)
        index %= 1117;
    return op_random_table[index];
}
