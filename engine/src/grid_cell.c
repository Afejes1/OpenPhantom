#include "baseline.h"

// FUNCTION: WMAIN 0x00406e22
/* VC5-calibrated: two caller-validated indices select a 128-byte cell.
 * The x extent implied by these strides is 256 cells. Allocation size is unknown.
 */
unsigned char *op_grid_cell(int x, int y)
{
    return op_grid + y * 32768 + x * 128;
}
