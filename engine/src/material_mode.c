#include "baseline.h"

// FUNCTION: WMAIN 0x0040e840
/* Candidate: input 1 selects zero, every other value selects two.
 * The explicit switch preserves the observed /Od control-flow shape.
 */
int op_material_mode(int mode)
{
    int result = 2;
    switch (mode) {
    case 1:
        result = 0;
        break;
    }
    return result;
}
