#include "api.h"

// FUNCTION: WMAIN 0x004288d1
void op_extended_pick_face_update(OP_EXTENDED_FACE *face)
{
    if (face->trail_value >= 1.0f) {
        face->update = op_capture_model_face_trail;
        face->trail_state = 0;
    }
    if (face->material != 0 && face->material->cel_count > 1U) {
        face->update = op_extended_advance_cel;
        face->cel = op_random15() % face->material->cel_count;
    }
}
