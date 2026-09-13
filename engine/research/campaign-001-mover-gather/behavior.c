#include "mover.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>

op_mover_world *op_mover_world_state;
unsigned char op_mover_cell_visit[256], op_mover_surface_seen[0x10000];
unsigned char op_mover_reject_mask, op_mover_surface_reject_mask;
unsigned char op_mover_old_room_mask, op_mover_new_room_mask;
int op_mover_fade_active;
float op_mover_fade_remaining;
unsigned int op_mover_texture_bucket_limit, op_mover_gather_count, op_mover_rejected_surface_count;
op_mover_draw_entry op_mover_draw_entries[32];
op_mover_draw_entry *op_mover_buckets[32];
static unsigned int queued_count;
static void *queued[8];
void op_mover_queue_surface(void *surface) { queued[queued_count++] = surface; }
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok) { printf("failure at %d\n", line); ++failures; }
}
#define CHECK(x) check(!!(x), __LINE__)
static unsigned char context_storage[16 + 2 * 0x40];
static unsigned char world_storage[0x628 + 256 * 4];
static unsigned char set_storage[0xd0 + 0x9c];
static unsigned char surface_a[0x40], surface_b[0x40], attachment[0x40];
static void *fixture_surfaces[2];
static void reset(void)
{
    memset(context_storage, 0, sizeof(context_storage)); memset(world_storage, 0, sizeof(world_storage));
    memset(set_storage, 0, sizeof(set_storage)); memset(surface_a, 0, sizeof(surface_a));
    memset(surface_b, 0, sizeof(surface_b)); memset(attachment, 0, sizeof(attachment));
    memset(op_mover_cell_visit, 0, sizeof(op_mover_cell_visit)); memset(op_mover_surface_seen, 0, sizeof(op_mover_surface_seen));
    memset(op_mover_buckets, 0, sizeof(op_mover_buckets));
    memset(op_mover_draw_entries, 0x5a, sizeof(op_mover_draw_entries));
    memset(queued, 0, sizeof(queued));
    op_mover_world_state = (op_mover_world *)world_storage; op_mover_world_state->cell_count = 8;
    ((op_mover_batch_set **)(world_storage + 0x624))[3] = (op_mover_batch_set *)set_storage;
    *(int *)set_storage = 1; *(int *)(set_storage + 0x24) = 1; *(int *)(set_storage + 0xd0) = 2;
    fixture_surfaces[0] = surface_a; fixture_surfaces[1] = surface_b;
    *(void ***)(set_storage + 0xd0 + 0x4c) = fixture_surfaces;
    surface_a[0x28] = surface_b[0x28] = 4; surface_a[0x24] = surface_b[0x24] = 2;
    surface_a[0x25] = surface_b[0x25] = 1; op_mover_texture_bucket_limit = 8;
    op_mover_fade_active = 0; op_mover_fade_remaining = .5f; op_mover_reject_mask = op_mover_surface_reject_mask = 0; op_mover_old_room_mask = op_mover_new_room_mask = 0; op_mover_gather_count = op_mover_rejected_surface_count = queued_count = 0;
}
static op_mover_gather_context *seed_case(void)
{
    op_mover_gather_context *context = (op_mover_gather_context *)context_storage;
    reset();
    context->seed_count = 1;
    context->seed_cell_ids[0] = 3;
    return context;
}
static op_mover_gather_context *mover_case(void)
{
    op_mover_gather_context *context = (op_mover_gather_context *)context_storage;
    reset();
    context->gather_flags = 1;
    context->mover_count = 1;
    context->movers[0].bytes[2] = 2; /* Deliberately differs from the cell ID. */
    context->movers[0].bytes[0x2a] = 3;
    context->movers[0].bytes[0x28] = 4;
    return context;
}

int main(void)
{
    op_mover_gather_context *context;
    int i;
    CHECK(sizeof(op_mover_record) == 0x40 && sizeof(op_mover_draw_entry) == 12);
    CHECK(offsetof(op_mover_gather_context, movers) == 0x10);
    CHECK(offsetof(op_mover_gather_context, seed_count) == 0x0c);
    CHECK(offsetof(op_mover_world, cell_batches) == 0x624 && offsetof(op_mover_world, cell_count) == 0x620);
    CHECK(offsetof(op_mover_batch_set, batch_count) == 0x24);

    context = seed_case(); context->seed_cell_ids[1] = 3;
    op_mover_gather(context);
    CHECK(op_mover_gather_count == 2 && op_mover_cell_visit[3] == 2);
    CHECK(op_mover_buckets[2] == &op_mover_draw_entries[1]);
    CHECK(op_mover_draw_entries[1].next == &op_mover_draw_entries[0] && op_mover_draw_entries[0].next == 0);
    CHECK(op_mover_draw_entries[0].surface == surface_a && op_mover_draw_entries[1].surface == surface_b);
    CHECK(op_mover_draw_entries[0].opaque_08 == 0x5a5a5a5aU);

    context = seed_case(); context->seed_cell_ids[0] = 0; context->seed_cell_ids[2] = 3;
    op_mover_gather(context); CHECK(op_mover_gather_count == 2);

    context = seed_case(); surface_a[0x2a] = surface_b[0x2a] = 3;
    op_mover_gather(context); CHECK(op_mover_gather_count == 2 && op_mover_cell_visit[3] == 2);

    context = seed_case(); surface_a[0x2a] = 4; op_mover_cell_visit[4] = 2;
    op_mover_gather(context); CHECK(op_mover_gather_count == 1 && op_mover_draw_entries[0].surface == surface_b);

    context = seed_case(); surface_a[0x38] = surface_b[0x38] = 1; op_mover_reject_mask = 1;
    op_mover_gather(context); CHECK(op_mover_gather_count == 0 && op_mover_rejected_surface_count == 0);
    context = mover_case(); surface_a[0x38] = surface_b[0x38] = 1; op_mover_reject_mask = 1;
    op_mover_gather(context); CHECK(op_mover_gather_count == 0 && op_mover_rejected_surface_count == 0);

    context = seed_case(); surface_a[0x38] = surface_b[0x38] = 1;
    op_mover_new_room_mask = 1; op_mover_fade_active = 1; op_mover_fade_remaining = -1.0f / 255.0f;
    op_mover_gather(context); CHECK(op_mover_gather_count == 0);

    context = seed_case(); surface_a[0x38] = surface_b[0x38] = 1;
    op_mover_new_room_mask = 1; op_mover_fade_active = 1; op_mover_fade_remaining = 0.25f;
    op_mover_gather(context); CHECK(op_mover_gather_count == 2 && surface_a[0x0e] == 63 && surface_b[0x0e] == 63);
    CHECK(op_mover_buckets[8] == &op_mover_draw_entries[1] && op_mover_buckets[2] == 0);

    context = seed_case(); surface_a[0x38] = surface_b[0x38] = 2;
    op_mover_old_room_mask = 2; op_mover_fade_active = 1; op_mover_fade_remaining = 0.25f;
    op_mover_gather(context); CHECK(op_mover_gather_count == 2 && surface_a[0x0e] == 191);

    context = seed_case(); surface_a[0x38] = surface_b[0x38] = 3;
    op_mover_old_room_mask = 2; op_mover_new_room_mask = 1; op_mover_fade_active = 1;
    op_mover_gather(context); CHECK(op_mover_gather_count == 0 && surface_a[0x0e] == 0);

    context = seed_case(); surface_a[0x38] = surface_b[0x38] = 1;
    op_mover_new_room_mask = 1; op_mover_fade_active = 1; op_mover_fade_remaining = 2.0f;
    op_mover_gather(context); CHECK(op_mover_gather_count == 2 && surface_a[0x0e] == 255);

    context = seed_case(); surface_a[0x2b] = surface_b[0x2b] = 0x20;
    op_mover_gather(context); CHECK(op_mover_gather_count == 2 && surface_a[0x0e] == 0 && op_mover_buckets[8] != 0);

    context = seed_case(); surface_a[0x2b] = 4; surface_a[0x24] = 8;
    op_mover_gather(context); CHECK(op_mover_gather_count == 1 && surface_a[0x0e] == 33);

    context = seed_case(); surface_a[0x2d] = surface_b[0x2d] = 1;
    *(unsigned short *)(surface_a + 0x30) = *(unsigned short *)(surface_b + 0x30) = 7;
    op_mover_gather(context); CHECK(op_mover_gather_count == 1 && op_mover_surface_seen[7] == 1);

    context = seed_case(); surface_a[0x28] = 0; surface_b[0x28] = 0x14;
    op_mover_gather(context); CHECK(op_mover_gather_count == 0);

    context = seed_case(); op_mover_texture_bucket_limit = 0xffffffffU;
    op_mover_gather(context); CHECK(op_mover_gather_count == 0);

    context = mover_case(); op_mover_gather(context);
    CHECK(*(int *)(set_storage + 0x64) == 1 && op_mover_gather_count == 2 && op_mover_cell_visit[3] == 2);

    context = mover_case(); ((op_mover_batch_set **)(world_storage + 0x624))[3] = 0;
    op_mover_gather(context); CHECK(op_mover_cell_visit[3] == 1 && op_mover_gather_count == 0);

    context = mover_case(); *(int *)set_storage = 0;
    op_mover_gather(context); CHECK(op_mover_cell_visit[3] == 1 && op_mover_gather_count == 0 && *(int *)(set_storage + 0x64) == 0);

    context = mover_case(); *(int *)(set_storage + 0x24) = 0;
    op_mover_gather(context); CHECK(op_mover_cell_visit[3] == 2 && op_mover_gather_count == 0 && *(int *)(set_storage + 0x64) == 1);

    context = mover_case(); context->movers[0].bytes[0x2a] = 8;
    op_mover_gather(context); CHECK(op_mover_gather_count == 0 && op_mover_cell_visit[8] == 0);

    context = mover_case(); context->gather_flags = 0x40;
    *(short *)context->movers[0].bytes = -1; context->movers[0].bytes[2] = 5;
    for (i = 0; i < 5; ++i) {
        *(void **)(context->movers[0].bytes + 8 + i * 12) = attachment;
        *(unsigned int *)(context->movers[0].bytes + 12 + i * 12) = (unsigned int)i;
    }
    attachment[0x25] = 1; op_mover_surface_seen[2] = 1;
    op_mover_gather(context); CHECK(queued_count == 4 && queued[0] == attachment);

    context = mover_case(); context->gather_flags = 0x40;
    *(short *)context->movers[0].bytes = -1; context->movers[0].bytes[2] = 6;
    op_mover_gather(context); CHECK(queued_count == 0);

    context = mover_case(); context->gather_flags = 0x40;
    *(short *)context->movers[0].bytes = -1; context->movers[0].bytes[2] = 1;
    *(void **)(context->movers[0].bytes + 8) = 0;
    *(unsigned int *)(context->movers[0].bytes + 12) = 0;
    op_mover_gather(context); CHECK(queued_count == 0);

    context = mover_case(); context->gather_flags = 0x80;
    context->movers[0].bytes[0x2c] = 0x80;
    op_mover_gather(context); CHECK(queued_count == 1 && queued[0] == &context->movers[0]);

    context = mover_case(); context->gather_flags = 0x80;
    *(short *)context->movers[0].bytes = -1; context->movers[0].bytes[0x2c] = 0x80;
    op_mover_gather(context); CHECK(queued_count == 0);

    printf("Mover gather: %d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
