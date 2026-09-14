#include "api.h"
// FUNCTION: WMAIN 0x00470e6c
void op_random_table_shuffle_position(void)
{
    op_random_table_index = (unsigned int)op_gameplay_random() % 1117;
}
