#include "gameplay_random.h"
// FUNCTION: WMAIN 0x00470cf0
int op_gameplay_random(void)
{
    int hi, lo, test;
    hi = op_gameplay_random_state / 127773;
    lo = op_gameplay_random_state % 127773;
    test = 16807 * lo - 2836 * hi;
    if (test > 0)
        op_gameplay_random_state = test;
    else
        op_gameplay_random_state = test + 2147483647;
    return op_gameplay_random_state;
}
