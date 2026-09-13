#include "api.h"

// FUNCTION: WMAIN 0x0041fc2c
void op_bgl_random_vector(float *vector)
{
    op_random_unit_xyz(vector, vector + 1, vector + 2);
}
