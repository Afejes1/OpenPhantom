#ifndef OP_CAMPAIGN_004_NEARBY_SURFACES_API_H
#define OP_CAMPAIGN_004_NEARBY_SURFACES_API_H

typedef struct op_query_cell { int x, y, count; } op_query_cell;
typedef void (__cdecl *op_query_assert_fn)(const char *reason, const char *file, int line);
typedef struct op_query_services_state {
    unsigned char opaque_00[0x18];
    op_query_assert_fn assert_failed;
} op_query_services_state;

extern void *op_query_active_world;
extern int op_query_generation;
extern int op_query_cached_generation;
extern int op_query_polygon_count;
extern int op_query_cached_cell_count;
extern op_query_cell op_query_cached_cells[25];
extern unsigned char op_query_visited[];
extern op_query_services_state *op_query_services;
extern const char op_query_assert_file[];
extern const char op_query_radius_assert[];
extern const char op_query_capacity_assert[];
int op_query_small_cells(op_query_cell *cells, const float *position, float radius);
int op_query_square_cells(op_query_cell *cells, const float *position, float radius, int maximum);
int op_query_append_cell(void *world, int x, int y, int offset, int mode);
void op_collect_nearby_surfaces(void *world, const float *position, float radius);
#endif