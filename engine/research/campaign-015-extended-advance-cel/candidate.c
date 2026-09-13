#include "api.h"

// FUNCTION: WMAIN 0x00428db6
void op_extended_advance_cel(void *ignored_context, OP_EXTENDED_FACE *face)
{
    OP_EXTENDED_MATERIAL *material;

    (void)ignored_context;
    material = face->material;
    ++face->cel;
    if (face->cel >= material->cel_count) {
        face->cel = 0;
    }
}
