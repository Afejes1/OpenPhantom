#include "mover.h"

/*
 * WMAIN 0x00405CF0.  The input is a 16-byte header followed by 0x40-byte
 * mover records.  Several mover fields are also valid draw-surface fields;
 * this routine expands their referenced cell batches before visiting animation
 * maps and direct mover surfaces.  All offsets below are observed accesses.
 */
static unsigned char byte_at(const void *record, unsigned int offset)
{
    return ((const unsigned char *)record)[offset];
}
static unsigned short word_at(const void *record, unsigned int offset)
{
    const unsigned char *p = (const unsigned char *)record + offset;
    return (unsigned short)(p[0] | ((unsigned short)p[1] << 8));
}
static int signed_short_at(const void *record, unsigned int offset)
{
    return (short)word_at(record, offset);
}
static void *pointer_at(const void *record, unsigned int offset)
{
    return *(void * const *)((const unsigned char *)record + offset);
}
static void set_byte(void *record, unsigned int offset, unsigned char value)
{
    ((unsigned char *)record)[offset] = value;
}

static unsigned char packed_alpha(unsigned char packed)
{
    unsigned int value = packed & 0x1f;
    return (unsigned char)(value * 8 + (value >> 2));
}

/* This is deliberately local instead of calling 0x00406300: 0x00405CF0
 * contains two independently inlined variants, including their cache policy. */
static void gather_surface(void *surface, int count_rejected_room)
{
    unsigned char membership;
    unsigned char packed;
    unsigned char opacity;
    unsigned int bucket;
    int scaled = 0;
    if ((byte_at(surface, 0x28) & 4) == 0) return;
    membership = byte_at(surface, 0x38);
    if ((membership & op_mover_reject_mask) != 0) return;
    if ((byte_at(surface, 0x0c) & op_mover_surface_reject_mask) != 0) return;
    if ((byte_at(surface, 0x28) & 0x10) != 0) return;
    if (op_mover_cell_visit[byte_at(surface, 0x2a)] == 2) return;
    if ((byte_at(surface, 0x2d) & 1) != 0 &&
        op_mover_surface_seen[word_at(surface, 0x30)] != 0) return;

    /* The mover expansion has a second, redundant room check in the original. */
    if (count_rejected_room && (byte_at(surface, 0x38) & op_mover_reject_mask) != 0) {
        ++op_mover_rejected_surface_count;
        return;
    }
    packed = byte_at(surface, 0x2b);
    opacity = (unsigned char)(packed ? packed_alpha(packed) : 0);
    set_byte(surface, 0x0e, opacity);
    if (op_mover_fade_active) {
        int has_fade_path = 0;
        if ((membership & op_mover_new_room_mask) != 0 &&
            (membership & op_mover_old_room_mask) != 0) return;
        if ((membership & op_mover_new_room_mask) != 0) {
            scaled = opacity ? opacity : 255;
            scaled = (int)(scaled * op_mover_fade_remaining);
            has_fade_path = 1;
        } else if ((membership & op_mover_old_room_mask) != 0) {
            scaled = opacity ? opacity : 255;
            scaled = (int)(scaled * (1.0f - op_mover_fade_remaining));
            has_fade_path = 1;
        }
        if (has_fade_path) {
            if (scaled < 0) scaled = 0;
            else if (scaled > 255) scaled = 255;
            if (scaled == 0) return;
            opacity = (unsigned char)scaled;
            set_byte(surface, 0x0e, opacity);
        }
    }
    bucket = byte_at(surface, 0x24);
    if ((int)bucket >= (int)op_mover_texture_bucket_limit) return;
    if (packed != 0 || opacity != 0) {
        bucket = op_mover_texture_bucket_limit;
        if (opacity == 0) set_byte(surface, 0x0e, packed_alpha(packed));
    }
    op_mover_draw_entries[op_mover_gather_count].next = op_mover_buckets[bucket];
    op_mover_buckets[bucket] = &op_mover_draw_entries[op_mover_gather_count];
    op_mover_draw_entries[op_mover_gather_count++].surface = surface;
    if ((byte_at(surface, 0x2d) & 1) != 0)
        op_mover_surface_seen[word_at(surface, 0x30)] = 1;
}

static void gather_seed_cell(unsigned int cell_id)
{
    op_mover_batch_set *set = op_mover_world_state->cell_batches[cell_id];
    int batch_index;
    /* Seed path deliberately has no pre-mark or null guard: trusted map IDs. */
    for (batch_index = 0; batch_index < set->batch_count; ++batch_index) {
        unsigned char *batch = (unsigned char *)set + 0xd0 + batch_index * 0x9c;
        int item_index, item_count = *(int *)batch;
        void **surfaces = *(void ***)(batch + 0x4c);
        for (item_index = 0; item_index < item_count; ++item_index)
            gather_surface(surfaces[item_index], 0);
    }
    op_mover_cell_visit[cell_id] = 2;
}
static void gather_mover_cell(unsigned int cell_id)
{
    op_mover_batch_set *set;
    int batch_index;
    op_mover_cell_visit[cell_id] = 1;
    set = op_mover_world_state->cell_batches[cell_id];
    /* A null or empty set intentionally retains the in-progress state. */
    if (set == 0 || *(int *)set == 0) return;
    *(int *)((unsigned char *)set + 0x64) = 1;
    for (batch_index = 0; batch_index < set->batch_count; ++batch_index) {
        unsigned char *batch = (unsigned char *)set + 0xd0 + batch_index * 0x9c;
        int item_index, item_count = *(int *)batch;
        void **surfaces = *(void ***)(batch + 0x4c);
        for (item_index = 0; item_index < item_count; ++item_index)
            gather_surface(surfaces[item_index], 1);
    }
    op_mover_cell_visit[cell_id] = 2;
}

extern "C" void __cdecl op_mover_gather(op_mover_gather_context *context)
{
    unsigned int i;
    if (context->seed_count != 0) {
        for (i = 0; i < 3; ++i) {
            unsigned int cell_id = context->seed_cell_ids[i];
            if (cell_id != 0 && op_mover_cell_visit[cell_id] == 0)
                gather_seed_cell(cell_id);
        }
    }
    if ((context->gather_flags & 1) != 0) {
        for (i = 0; i < context->mover_count; ++i) {
            op_mover_record *mover = &context->movers[i];
            unsigned int cell_id = byte_at(mover, 0x2a);
            if (signed_short_at(mover, 0) == -1 ||
                (byte_at(mover, 0x28) & 0x0c) == 0 ||
                cell_id >= op_mover_world_state->cell_count ||
                op_mover_cell_visit[cell_id] != 0) continue;
            gather_mover_cell(cell_id);
        }
    }
    if ((context->gather_flags & 0x40) != 0) {
        for (i = 0; i < context->mover_count; ++i) {
            op_mover_record *mover = &context->movers[i];
            unsigned int map_index;
            if (signed_short_at(mover, 0) != -1 || byte_at(mover, 2) == 0 ||
                byte_at(mover, 2) >= 6) continue;
            for (map_index = 0; map_index < byte_at(mover, 2); ++map_index) {
                unsigned char *map = (unsigned char *)mover + 8 + map_index * 12;
                void *surface = pointer_at(map, 0);
                if (op_mover_surface_seen[*(unsigned int *)(map + 4)] == 0 &&
                    surface != 0 && byte_at(surface, 0x25) != 0)
                    op_mover_queue_surface(surface);
            }
        }
    }
    if ((context->gather_flags & 0x80) != 0) {
        for (i = 0; i < context->mover_count; ++i) {
            op_mover_record *mover = &context->movers[i];
            if (signed_short_at(mover, 0) != -1 && (byte_at(mover, 0x2c) & 0x80) != 0)
                op_mover_queue_surface(mover);
        }
    }
}
