#include "api.h"
// FUNCTION: WMAIN 0x00470e37
int op_random_table_next(void)
{
    ++op_random_table_index;
    if (op_random_table_index == 1117)
        op_random_table_index = 0;
    return op_random_table[op_random_table_index];
}
