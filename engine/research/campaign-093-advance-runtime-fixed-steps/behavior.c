#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
static unsigned int word(float f)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.f = f;
    return v.u;
}
static float real(unsigned int u)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.u = u;
    return v.f;
}
typedef struct ROW
{
    unsigned int sim, target, step, clock, ticks, world, payload, generation;
} ROW;
typedef struct CASE
{
    unsigned int incoming, initial_target, initial_sim, gate, mutation, saved, target, simulation, alpha, generation,
        final_world, count;
    ROW rows[7];
} CASE;
#include "oracle.h"
float op_skills_frame_delta, op_time_mode, op_task_target, op_task_simulation;
unsigned int op_fixed_rate_gate, op_fixed_generation;
const float op_fixed_max_delta = 0.1f;
OP_B3D_WORLD *op_active_world;
OP_TASK_RECORD op_tasks[64], *op_task_current;
unsigned char op_task_pending[20];
unsigned int op_task_count, op_task_cursor;
int op_task_sentinel;
OP_MODULE *op_module_head, *op_module_tail;
int op_module_initialized, op_module_open;
#include "world_set_clock_support.c"
#include "task_run_all_support.c"
#include "module_broadcast_event_support.c"
#include "module_find_by_id_support.c"
static OP_B3D_WORLD worlds[2], expected_worlds[2];
static OP_TASK_RECORD expected_tasks[64];
static OP_MODULE modules[2], expected_modules[2];
static unsigned char expected_pending[20];
static unsigned int token, iteration, module_calls, wanted_target, wanted_step, wanted_generation, wanted_world;
static void *output;
static const CASE *current;
void op_release(void *p)
{
    (void)p;
    CHECK(0);
    exit(2);
}
static void verify(unsigned int cursor, unsigned int countdown)
{
    expected_tasks[0].countdown = countdown;
    CHECK(!memcmp(worlds, expected_worlds, sizeof(worlds)));
    CHECK(!memcmp(op_tasks, expected_tasks, sizeof(op_tasks)));
    CHECK(!memcmp(modules, expected_modules, sizeof(modules)));
    CHECK(!memcmp(op_task_pending, expected_pending, sizeof(expected_pending)));
    CHECK(op_task_cursor == cursor && op_task_count == 1 && op_task_current == &op_tasks[0]);
    CHECK(op_task_sentinel == -711 && output == &token && token == 0xdeadbeefu);
    CHECK(word(op_task_target) == wanted_target && word(op_task_simulation) == current->rows[iteration].sim);
    CHECK(word(op_skills_frame_delta) == wanted_step && word(op_time_mode) == 0x3f900000u);
    CHECK(op_fixed_generation == wanted_generation && op_fixed_rate_gate == current->gate);
    CHECK(op_active_world == &worlds[wanted_world]);
    CHECK(op_module_head == &modules[0] && op_module_tail == &modules[1]);
    CHECK(op_module_initialized == 91 && op_module_open == -31);
}
static int update(void)
{
    const ROW *r;
    OP_B3D_WORLD *e;
    CHECK(iteration < current->count && module_calls == iteration * 2);
    if (iteration >= current->count || module_calls != iteration * 2)
        exit(2);
    r = &current->rows[iteration];
    e = &expected_worlds[r->world];
    e->previous_ticks = e->current_ticks;
    e->current_ticks = (int)r->ticks;
    e->delta = real(r->clock) - e->seconds;
    e->seconds = real(r->clock);
    wanted_target = r->target;
    wanted_step = r->step;
    wanted_generation = r->generation;
    wanted_world = r->world;
    verify(0, 0);
    if (current->mutation && iteration == 0)
    {
        op_active_world = &worlds[1];
        wanted_world = 1;
        op_task_target = real(0x3d400000u);
        wanted_target = 0x3d400000u;
        op_skills_frame_delta = real(0x3c800000u);
        wanted_step = 0x3c800000u;
    }
    return 1;
}
static int module_event(unsigned int event, unsigned int zero, unsigned int payload, unsigned int phase)
{
    CHECK(iteration < current->count && module_calls == iteration * 2 + phase);
    if (iteration >= current->count || module_calls != iteration * 2 + phase)
        exit(2);
    CHECK(event == 14 && zero == 0 && payload == current->rows[iteration].payload);
    verify(64, 1);
    if (current->mutation && iteration == 0)
    {
        if (phase == 0)
        {
            op_fixed_generation = wanted_generation = 0xffffffffu;
        }
        else
        {
            op_skills_frame_delta = real(0x3d000000u);
            wanted_step = 0x3d000000u;
        }
    }
    ++module_calls;
    if (phase == 1)
        ++iteration;
    return -9;
}
static int tail_event(unsigned int e, unsigned int z, unsigned int p)
{
    return module_event(e, z, p, 0);
}
static int head_event(unsigned int e, unsigned int z, unsigned int p)
{
    return module_event(e, z, p, 1);
}
int main(void)
{
    unsigned int c, rotation, argument, saved = _controlfp(0, 0), before_tokens;
    static const unsigned int arguments[4] = {0u, 1u, 0x80000000u, 0xffffffffu};
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (c = 0; c < sizeof(cases) / sizeof(cases[0]); ++c)
        for (rotation = 0; rotation < 4; ++rotation)
            for (argument = 0; argument < 4; ++argument)
            {
                current = &cases[c];
                iteration = module_calls = 0;
                memset(worlds, (int)(0x91 + rotation), sizeof(worlds));
                worlds[0].seconds = worlds[1].seconds = 0;
                worlds[0].current_ticks = worlds[1].current_ticks = 0;
                memcpy(expected_worlds, worlds, sizeof(worlds));
                memset(op_tasks, 0, sizeof(op_tasks));
                op_tasks[0].update = update;
                op_tasks[0].out = &output;
                op_tasks[0].data = &token;
                op_tasks[0].countdown = 1;
                memcpy(expected_tasks, op_tasks, sizeof(op_tasks));
                memset(op_task_pending, (int)(0x35 + rotation), sizeof(op_task_pending));
                memcpy(expected_pending, op_task_pending, sizeof(expected_pending));
                memset(modules, 0, sizeof(modules));
                modules[0].next = &modules[1];
                modules[1].previous = &modules[0];
                modules[0].callback_word = (unsigned int)head_event;
                modules[1].callback_word = (unsigned int)tail_event;
                memcpy(expected_modules, modules, sizeof(modules));
                op_module_head = &modules[0];
                op_module_tail = &modules[1];
                op_module_initialized = 91;
                op_module_open = -31;
                token = before_tokens = 0xdeadbeefu;
                output = NULL;
                op_task_sentinel = -711;
                op_task_count = 1;
                op_task_cursor = 123;
                op_task_current = &op_tasks[3];
                op_skills_frame_delta = real(current->incoming);
                op_task_target = real(current->initial_target);
                op_task_simulation = real(current->initial_sim);
                op_time_mode = real(0x3f900000u);
                op_fixed_rate_gate = current->gate;
                op_fixed_generation = 0xfffffffeu;
                op_active_world = &worlds[0];
                op_advance_runtime_fixed_steps(arguments[argument]);
                CHECK(iteration == current->count && module_calls == 2 * current->count);
                CHECK(word(op_skills_frame_delta) == current->saved && word(op_task_target) == current->target);
                CHECK(word(op_task_simulation) == current->simulation && word(op_time_mode) == current->alpha);
                CHECK(op_fixed_generation == current->generation && op_active_world == &worlds[current->final_world]);
                CHECK(op_fixed_rate_gate == current->gate && token == before_tokens && op_task_sentinel == -711);
                CHECK(op_task_cursor == (current->count ? 64u : 123u));
                CHECK(op_task_current == &op_tasks[current->count ? 0 : 3]);
                CHECK(output == (current->count ? &token : NULL));
                CHECK(op_task_count == 1);
                CHECK(!memcmp(worlds, expected_worlds, sizeof(worlds)));
                CHECK(!memcmp(op_tasks, expected_tasks, sizeof(op_tasks)));
                CHECK(!memcmp(modules, expected_modules, sizeof(modules)));
                CHECK(!memcmp(op_task_pending, expected_pending, sizeof(expected_pending)));
                CHECK(op_module_head == &modules[0] && op_module_tail == &modules[1]);
                CHECK(op_module_initialized == 91 && op_module_open == -31);
            }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("advance_runtime_fixed_steps: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
