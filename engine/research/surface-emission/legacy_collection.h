#ifndef OP_LEGACY_CELL_RESEARCH_H
#define OP_LEGACY_CELL_RESEARCH_H
#include "batch_surface.h"
/* Partial views established by 0x004056C0; these are not complete game types. */
typedef struct op_legacy_cell {
    unsigned char surface_count, flags, opaque_02[2];
    float bound_a, bound_b;
    unsigned char linked_groups_enabled, linked_group[3];
} op_legacy_cell;
typedef struct op_legacy_group {
    int active;
    unsigned char opaque_04[0x20];
    int part_count;
    unsigned char opaque_28[0x3c];
    int gathered;
    unsigned char opaque_68[0x68];
} op_legacy_group;
typedef struct op_legacy_part {
    int surface_count;
    unsigned char opaque_04[0x48];
    op_emission_surface **surfaces;
    unsigned char opaque_50[0x4c];
} op_legacy_part;
typedef struct op_legacy_world_prefix {
    unsigned char opaque_00[0x620];
    unsigned int group_count;
} op_legacy_world_prefix;
/* Reference arrays begin at record +8. No count limit is observed here. */
typedef struct op_legacy_surface_reference {
    op_emission_surface *surface;
    unsigned int seen_index;
    unsigned int opaque_08;
} op_legacy_surface_reference;
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned int op_legacy_reference_visit_count;
void op_gather_legacy_cell(op_legacy_cell *cell, const float *position);
#ifdef __cplusplus
}
#endif
#endif
