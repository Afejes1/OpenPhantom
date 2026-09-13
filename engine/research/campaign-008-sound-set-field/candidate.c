#include "api.h"

// FUNCTION: WMAIN 0x0041670f
void op_sound_set_field(int field, float value)
{
    switch (field) {
    case 0:
        op_sound_field0 = value;
        break;
    case 1:
        op_sound_field1 = value;
        break;
    case 2:
        op_sound_field2 = value;
        break;
    case 3:
        op_sound_field3 = value;
        break;
    case 4:
        op_sound_field4 = value;
        break;
    case 5:
        op_sound_field5 = (int)value;
        break;
    }
}
