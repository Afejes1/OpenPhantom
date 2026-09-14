#include "api.h"
// FUNCTION: WMAIN 0x00470de5
int op_random_table_initialize(int seed)
{
    int previous;
    int index;
    previous = op_gameplay_seed(seed);
    for (index = 0; index < 1117; ++index)
        op_random_table[index] = op_gameplay_random();
    op_random_table_index = 0;
    return previous;
}
