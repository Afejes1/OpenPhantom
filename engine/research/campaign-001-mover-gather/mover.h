#ifndef OP_MOVER_GATHER_RESEARCH_H
#define OP_MOVER_GATHER_RESEARCH_H

#include "baseline.h"

/* Partial, offset-verified records used only by op_mover_gather. */
typedef struct op_mover_record {
    unsigned char bytes[0x40];
} op_mover_record;

typedef struct op_mover_gather_context {
    unsigned char mover_count;
    unsigned char gather_flags;
    unsigned char opaque_02[10];
    unsigned char seed_count;
    unsigned char seed_cell_ids[3];
    op_mover_record movers[1];
} op_mover_gather_context;

typedef struct op_mover_batch_set {
    unsigned char opaque_00[0x24];
    int batch_count;
    unsigned char opaque_28[0xa8];
    /* At +0xd0: repeated 0x9c-byte batches: count at +0, surface array at +0x4c. */
} op_mover_batch_set;

typedef struct op_mover_draw_entry {
    void *surface;
    struct op_mover_draw_entry *next;
    unsigned int opaque_08;
} op_mover_draw_entry;

typedef struct op_mover_world {
    unsigned char opaque_00[0x620];
    unsigned int cell_count;
    op_mover_batch_set *cell_batches[1];
} op_mover_world;

#ifdef __cplusplus
extern "C" {
#endif
extern op_mover_world *op_mover_world_state;
extern unsigned char op_mover_cell_visit[];
extern unsigned char op_mover_surface_seen[];
extern unsigned char op_mover_reject_mask, op_mover_surface_reject_mask;
extern unsigned char op_mover_old_room_mask, op_mover_new_room_mask;
extern int op_mover_fade_active;
extern float op_mover_fade_remaining;
extern unsigned int op_mover_texture_bucket_limit, op_mover_gather_count;
extern unsigned int op_mover_rejected_surface_count;
extern op_mover_draw_entry op_mover_draw_entries[];
extern op_mover_draw_entry *op_mover_buckets[];
void op_mover_queue_surface(void *surface);
void __cdecl op_mover_gather(op_mover_gather_context *context);
#ifdef __cplusplus
}
#endif

#endif
