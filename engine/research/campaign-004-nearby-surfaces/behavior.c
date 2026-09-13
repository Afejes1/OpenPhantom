#include "api.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

void *op_query_active_world;
int op_query_generation;
int op_query_cached_generation;
int op_query_polygon_count;
int op_query_cached_cell_count;
op_query_cell op_query_cached_cells[25];
unsigned char op_query_visited[256];
const char op_query_assert_file[] = "authored-file";
const char op_query_radius_assert[] = "authored-radius";
const char op_query_capacity_assert[] = "authored-capacity";
static op_query_services_state services;
op_query_services_state *op_query_services = &services;

static int checks, failures, small_calls, square_calls, append_calls, assert_calls;
static int enum_count, append_results[25], append_offsets[25], append_modes[25];
static void *append_worlds[25];
static op_query_cell enum_cells[25];
static const char *last_reason, *last_file;
static int last_line, mutate_generation, mutate_world;
static int assertion_after_appends[4], assertion_polygon_count[4];
static const char *assertion_reasons[4];
static int assertion_lines[4];
static const float *expected_position;
static unsigned int expected_radius_bits;
static void *world_a = (void *)0x11110000;
static void *world_b = (void *)0x22220000;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static void assertion(const char *reason, const char *file, int line)
{
    if (assert_calls >= 4) {
        ++failures;
        return;
    }
    assertion_after_appends[assert_calls] = append_calls;
    assertion_polygon_count[assert_calls] = op_query_polygon_count;
    assertion_reasons[assert_calls] = reason;
    assertion_lines[assert_calls] = line;
    ++assert_calls;
    last_reason = reason;
    last_file = file;
    last_line = line;
}

static int enumerate(op_query_cell *cells)
{
    memcpy(cells, enum_cells, (unsigned int)enum_count * sizeof(*cells));
    return enum_count;
}

int op_query_small_cells(op_query_cell *cells, const float *position, float radius)
{
    unsigned int radius_bits;
    memcpy(&radius_bits, &radius, sizeof(radius_bits));
    check(position == expected_position && radius_bits == expected_radius_bits,
          "small enumerator position and radius arguments");
    ++small_calls;
    return enumerate(cells);
}

int op_query_square_cells(op_query_cell *cells, const float *position,
                          float radius, int maximum)
{
    unsigned int radius_bits;
    memcpy(&radius_bits, &radius, sizeof(radius_bits));
    check(position == expected_position && radius_bits == expected_radius_bits,
          "square enumerator position and radius arguments");
    check(maximum == 25, "square capacity");
    ++square_calls;
    return enumerate(cells);
}

int op_query_append_cell(void *world, int x, int y, int offset, int mode)
{
    int at;
    if (append_calls >= 25) {
        ++failures;
        return 0;
    }
    at = append_calls++;
    check(x == enum_cells[at].x && y == enum_cells[at].y, "append coordinates");
    append_worlds[at] = world;
    append_offsets[at] = offset;
    append_modes[at] = mode;
    if (mutate_world && at == 0)
        op_query_active_world = world_b;
    if (mutate_generation && at == 0)
        op_query_generation = 77;
    return append_results[at];
}

static void setup(void)
{
    memset(enum_cells, 0, sizeof(enum_cells));
    memset(op_query_cached_cells, 0x5a, sizeof(op_query_cached_cells));
    memset(op_query_visited, 0x7c, sizeof(op_query_visited));
    memset(append_results, 0, sizeof(append_results));
    memset(append_offsets, 0, sizeof(append_offsets));
    memset(append_modes, 0xff, sizeof(append_modes));
    services.assert_failed = assertion;
    op_query_active_world = world_a;
    op_query_generation = 10;
    op_query_cached_generation = -1;
    op_query_cached_cell_count = -1;
    op_query_polygon_count = -1;
    enum_count = small_calls = square_calls = append_calls = assert_calls = 0;
    mutate_generation = mutate_world = 0;
    last_reason = last_file = 0;
    last_line = 0;
}

static void set_cell(int i, int x, int y, int added)
{
    enum_cells[i].x = x;
    enum_cells[i].y = y;
    append_results[i] = added;
}

static void run_query(void *world, const float *position, float radius)
{
    expected_position = position;
    memcpy(&expected_radius_bits, &radius, sizeof(expected_radius_bits));
    op_collect_nearby_surfaces(world, position, radius);
}

int main(void)
{
    float position[3] = {1.0f, 2.0f, 3.0f};
    unsigned int nan_bits = 0x7fc12345;
    float nan_value;
    op_query_cell tail_before;
    unsigned char zeros[255] = {0};
    int i;

    check(sizeof(op_query_cell) == 12 &&
          offsetof(op_query_services_state, assert_failed) == 0x18,
          "original query-cell stride and service callback offset");
    setup();
    enum_count = 2;
    set_cell(0, 4, 5, 7);
    set_cell(1, -2, 8, 11);
    tail_before = op_query_cached_cells[2];
    mutate_world = mutate_generation = 1;
    run_query((void *)0x33330000, position, 0.49f);
    check(small_calls == 1 && square_calls == 0, "small threshold");
    check(append_calls == 2 && append_offsets[0] == 0 && append_offsets[1] == 7,
          "cumulative append offsets");
    check(append_modes[0] == 0 && append_modes[1] == 0, "append mode zero");
    check(append_worlds[0] == world_a && append_worlds[1] == world_b,
          "active world reload and unused argument");
    check(op_query_polygon_count == 18 && op_query_cached_cell_count == 2 &&
          op_query_cached_generation == 77, "final cached fields and generation reload");
    check(op_query_cached_cells[0].count == 7 && op_query_cached_cells[1].count == 11,
          "cached append counts");
    check(memcmp(&op_query_cached_cells[2], &tail_before, sizeof(tail_before)) == 0,
          "unused cache tail preserved");
    check(memcmp(op_query_visited, zeros, sizeof(zeros)) == 0 &&
          op_query_visited[255] == 0x7c, "exact 255-byte visited clear");

    setup();
    enum_count = 2;
    set_cell(0, 1, 2, 3);
    set_cell(1, 9, 9, 4);
    op_query_cached_generation = op_query_generation;
    op_query_cached_cell_count = 2;
    op_query_cached_cells[0].x = 1;
    op_query_cached_cells[0].y = 2;
    op_query_cached_cells[1].x = 100;
    op_query_cached_cells[1].y = 100;
    run_query(0, position, 0.5f);
    check(square_calls == 1 && append_calls == 0, "first-cell-only cache hit");
    check(op_query_visited[0] == 0x7c, "cache hit avoids rebuild");

    setup();
    enum_count = 1;
    set_cell(0, 7, 8, 4095);
    run_query(0, position, 1.99f);
    check(assert_calls == 0, "4095 and radius below two avoid assertions");

    setup();
    enum_count = 1;
    set_cell(0, 7, 8, 4096);
    run_query(0, position, 2.0f);
    check(assert_calls == 2, "radius and post-append capacity assertions");
    check(last_reason == op_query_capacity_assert && last_file == op_query_assert_file &&
          last_line == 0x395, "capacity assertion arguments and timing");
    check(append_calls == 1, "capacity case appends once");
    check(assertion_after_appends[0] == 0 && assertion_after_appends[1] == 1 &&
          assertion_reasons[0] == op_query_radius_assert &&
          assertion_reasons[1] == op_query_capacity_assert &&
          assertion_lines[0] == 0x36e && assertion_lines[1] == 0x395,
          "radius assertion precedes append and capacity assertion follows it");
    check(assertion_polygon_count[0] == -1 && assertion_polygon_count[1] == -1 &&
          op_query_polygon_count == 4096,
          "published polygon count changes after returning assertions");

    setup();
    memcpy(&nan_value, &nan_bits, sizeof(nan_value));
    run_query(0, position, nan_value);
    check(small_calls == 1 && square_calls == 0 && assert_calls == 0,
          "NaN takes small path without radius assertion");

    setup();
    op_query_cached_generation = op_query_generation;
    op_query_cached_cell_count = 0;
    run_query(0, position, 0.0f);
    op_query_visited[0] = 0x55;
    run_query(0, position, 0.0f);
    check(small_calls == 2 && op_query_visited[0] == 0,
          "empty list rebuilds repeatedly");

    for (i = 0; i < 4; ++i) {
        setup();
        enum_count = 2;
        set_cell(0, 3, 4, 2);
        set_cell(1, 5, 6, 3);
        op_query_cached_generation = op_query_generation;
        op_query_cached_cell_count = 2;
        op_query_cached_cells[0].x = 3;
        op_query_cached_cells[0].y = 4;
        if (i == 0) --op_query_cached_generation;
        if (i == 1) --op_query_cached_cell_count;
        if (i == 2) --op_query_cached_cells[0].x;
        if (i == 3) --op_query_cached_cells[0].y;
        run_query(0, position, 0.0f);
        check(append_calls == 2 && op_query_polygon_count == 5,
              "each generation/count/first-coordinate cache mismatch rebuilds");
        check(op_query_cached_cells[0].x == 3 && op_query_cached_cells[0].y == 4 &&
              op_query_cached_cells[1].x == 5 && op_query_cached_cells[1].y == 6,
              "rebuilt cache retains all enumerated coordinates");
    }

    setup();
    enum_count = 25;
    for (i = 0; i < 25; ++i) set_cell(i, i, -i, 1);
    run_query(0, position, 1.0f);
    check(append_calls == 25 && append_offsets[24] == 24 &&
          op_query_polygon_count == 25 && op_query_cached_cell_count == 25,
          "maximum enumerated cell count and final cumulative offset");

    printf("Nearby surfaces: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}