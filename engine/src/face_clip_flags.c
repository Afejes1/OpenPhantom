#include "baseline.h"

/* FUNCTION: WMAIN 0x0040f170 */
unsigned int op_face_clip_flags(const op_face_indices *face,
                               const unsigned char *clip_codes)
{
    unsigned int any_flags = 0;
    unsigned int common_flags = 255;
    const unsigned int *indices = face->vertex_indices;
    unsigned int remaining = face->vertex_count;

    for (; remaining > 0; --remaining) {
        unsigned int flags = clip_codes[*indices++];
        any_flags |= flags;
        common_flags &= flags;
    }
    return (common_flags << 8) | any_flags;
}
