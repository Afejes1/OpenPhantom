#include "gameplay_random.h"
// FUNCTION: WMAIN 0x00470d56
int op_gameplay_seed(int seed)
{
    int previous;
    previous = op_gameplay_random_state;
    if (seed < 0)
        seed = -seed;
    if (seed >= 2147483647)
        seed -= 2147483647;
    if (!seed)
        seed = 1;
    op_gameplay_random_state = seed;
    return previous;
}
