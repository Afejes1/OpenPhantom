#include "api.h"

// FUNCTION: WMAIN 0x0041668f
void op_sound_get_field(int field, float *output)
{
    switch (field) {
    case 0:
        *output = op_sound_field0;
        break;
    case 1:
        *output = op_sound_field1;
        break;
    case 2:
        *output = op_sound_field2;
        break;
    case 3:
        *output = op_sound_field3;
        break;
    case 4:
        *output = op_sound_field4;
        break;
    case 5:
        *output = (float)op_sound_field5;
        break;
    }
}
