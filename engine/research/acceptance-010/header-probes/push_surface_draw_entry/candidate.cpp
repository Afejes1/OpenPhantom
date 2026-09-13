#include "baseline.h"

/* FUNCTION: WMAIN 0x004059F0 */
void op_push_surface_draw_entry(op_draw_surface *surface)
{
    unsigned int bucket = surface->texture_index;
    unsigned int opacity;
    if (surface->packed_opacity != 0 || surface->opacity != 0) {
        bucket = op_special_surface_bucket;
        if (surface->opacity == 0) {
            opacity = surface->packed_opacity & 0x1f;
            surface->opacity = (unsigned char)(opacity * 8 + (opacity >> 2));
        }
    }
    op_surface_draw_entries[op_gathered_surface_count].next = op_surface_buckets[bucket];
    op_surface_buckets[bucket] = &op_surface_draw_entries[op_gathered_surface_count];
}
