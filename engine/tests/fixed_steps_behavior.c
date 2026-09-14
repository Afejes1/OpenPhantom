static int t930_active;
#include "../src/fixed_steps.h"
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
static int t930_checks, t930_failures;
static void t930_check(int ok, int line)
{
    ++t930_checks;
    if (!ok)
    {
        ++t930_failures;
        printf("line %d failed\n", line);
    }
}
#define T930_CHECK(x) t930_check(!!(x), __LINE__)
static unsigned int t930_word(float f)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.f = f;
    return v.u;
}
static float t930_real(unsigned int u)
{
    union {
        float f;
        unsigned int u;
    } v;
    v.u = u;
    return v.f;
}
typedef struct T930_ROW
{
    unsigned int sim, target, step, clock, ticks, world, payload, generation;
} T930_ROW;
typedef struct T930_CASE
{
    unsigned int incoming, initial_target, initial_sim, gate, mutation, saved, target, simulation, alpha, generation,
        final_world, count;
    T930_ROW rows[7];
} T930_CASE;
#include "fixed_steps_oracle.h"
unsigned int op_fixed_rate_gate, op_fixed_generation;
const float op_fixed_max_delta = 0.1f;
static OP_B3D_WORLD t930_worlds[2], t930_expected_worlds[2];
static OP_TASK_RECORD t930_expected_tasks[64];
static OP_MODULE t930_modules[2], t930_expected_modules[2];
static unsigned char t930_expected_pending[20];
static unsigned int t930_token, t930_iteration, t930_module_calls, t930_wanted_target, t930_wanted_step,
    t930_wanted_generation, t930_wanted_world;
static void *t930_output;
static const T930_CASE *t930_current;
static void t930_release(void *p)
{
    (void)p;
    T930_CHECK(0);
    exit(2);
}
static void t930_verify(unsigned int cursor, unsigned int countdown)
{
    t930_expected_tasks[0].countdown = countdown;
    T930_CHECK(!memcmp(t930_worlds, t930_expected_worlds, sizeof(t930_worlds)));
    T930_CHECK(!memcmp(op_tasks, t930_expected_tasks, sizeof(op_tasks)));
    T930_CHECK(!memcmp(t930_modules, t930_expected_modules, sizeof(t930_modules)));
    T930_CHECK(!memcmp(op_task_pending, t930_expected_pending, sizeof(t930_expected_pending)));
    T930_CHECK(op_task_cursor == cursor && op_task_count == 1 && op_task_current == &op_tasks[0]);
    T930_CHECK(op_task_sentinel == -711 && t930_output == &t930_token && t930_token == 0xdeadbeefu);
    T930_CHECK(t930_word(op_task_target) == t930_wanted_target &&
               t930_word(op_task_simulation) == t930_current->rows[t930_iteration].sim);
    T930_CHECK(t930_word(op_skills_frame_delta) == t930_wanted_step && t930_word(op_time_mode) == 0x3f900000u);
    T930_CHECK(op_fixed_generation == t930_wanted_generation && op_fixed_rate_gate == t930_current->gate);
    T930_CHECK(op_active_world == &t930_worlds[t930_wanted_world]);
    T930_CHECK(op_module_head == &t930_modules[0] && op_module_tail == &t930_modules[1]);
    T930_CHECK(op_module_initialized == 91 && op_module_open == -31);
}
static int t930_update(void)
{
    const T930_ROW *r;
    OP_B3D_WORLD *e;
    T930_CHECK(t930_iteration < t930_current->count && t930_module_calls == t930_iteration * 2);
    if (t930_iteration >= t930_current->count || t930_module_calls != t930_iteration * 2)
        exit(2);
    r = &t930_current->rows[t930_iteration];
    e = &t930_expected_worlds[r->world];
    e->previous_ticks = e->current_ticks;
    e->current_ticks = (int)r->ticks;
    e->delta = t930_real(r->clock) - e->seconds;
    e->seconds = t930_real(r->clock);
    t930_wanted_target = r->target;
    t930_wanted_step = r->step;
    t930_wanted_generation = r->generation;
    t930_wanted_world = r->world;
    t930_verify(0, 0);
    if (t930_current->mutation && t930_iteration == 0)
    {
        op_active_world = &t930_worlds[1];
        t930_wanted_world = 1;
        op_task_target = t930_real(0x3d400000u);
        t930_wanted_target = 0x3d400000u;
        op_skills_frame_delta = t930_real(0x3c800000u);
        t930_wanted_step = 0x3c800000u;
    }
    return 1;
}
static int t930_module_event(unsigned int event, unsigned int zero, unsigned int payload, unsigned int phase)
{
    T930_CHECK(t930_iteration < t930_current->count && t930_module_calls == t930_iteration * 2 + phase);
    if (t930_iteration >= t930_current->count || t930_module_calls != t930_iteration * 2 + phase)
        exit(2);
    T930_CHECK(event == 14 && zero == 0 && payload == t930_current->rows[t930_iteration].payload);
    t930_verify(64, 1);
    if (t930_current->mutation && t930_iteration == 0)
    {
        if (phase == 0)
        {
            op_fixed_generation = t930_wanted_generation = 0xffffffffu;
        }
        else
        {
            op_skills_frame_delta = t930_real(0x3d000000u);
            t930_wanted_step = 0x3d000000u;
        }
    }
    ++t930_module_calls;
    if (phase == 1)
        ++t930_iteration;
    return -9;
}
static int t930_tail_event(unsigned int e, unsigned int z, unsigned int p)
{
    return t930_module_event(e, z, p, 0);
}
static int t930_head_event(unsigned int e, unsigned int z, unsigned int p)
{
    return t930_module_event(e, z, p, 1);
}
static int op_test_advance_runtime_fixed_steps(void)
{
    OP_MODULE *saved_head = op_module_head, *saved_tail = op_module_tail;
    OP_B3D_WORLD *saved_world = op_active_world;
    OP_TASK_RECORD saved_tasks[64], *saved_current = op_task_current;
    unsigned char saved_pending[20];
    unsigned int saved_count = op_task_count, saved_cursor = op_task_cursor;
    unsigned int saved_rate_gate = op_fixed_rate_gate, saved_generation = op_fixed_generation;
    unsigned int saved_target = op_fixture_float_word(&op_task_target),
                 saved_simulation = op_fixture_float_word(&op_task_simulation);
    unsigned int saved_alpha = op_fixture_float_word(&op_time_mode),
                 saved_delta = op_fixture_float_word(&op_skills_frame_delta);
    int saved_sentinel = op_task_sentinel, saved_initialized = op_module_initialized, saved_open = op_module_open,
        saved_route = t930_active;

    unsigned int c, rotation, argument, saved = _controlfp(0, 0), before_tokens;
    static const unsigned int arguments[4] = {0u, 1u, 0x80000000u, 0xffffffffu};
    memcpy(saved_tasks, op_tasks, sizeof(saved_tasks));
    memcpy(saved_pending, op_task_pending, sizeof(saved_pending));
    t930_active = 1;
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (c = 0; c < sizeof(t930_cases) / sizeof(t930_cases[0]); ++c)
        for (rotation = 0; rotation < 4; ++rotation)
            for (argument = 0; argument < 4; ++argument)
            {
                t930_current = &t930_cases[c];
                t930_iteration = t930_module_calls = 0;
                memset(t930_worlds, (int)(0x91 + rotation), sizeof(t930_worlds));
                t930_worlds[0].seconds = t930_worlds[1].seconds = 0;
                t930_worlds[0].current_ticks = t930_worlds[1].current_ticks = 0;
                memcpy(t930_expected_worlds, t930_worlds, sizeof(t930_worlds));
                memset(op_tasks, 0, sizeof(op_tasks));
                op_tasks[0].update = t930_update;
                op_tasks[0].out = &t930_output;
                op_tasks[0].data = &t930_token;
                op_tasks[0].countdown = 1;
                memcpy(t930_expected_tasks, op_tasks, sizeof(op_tasks));
                memset(op_task_pending, (int)(0x35 + rotation), sizeof(op_task_pending));
                memcpy(t930_expected_pending, op_task_pending, sizeof(t930_expected_pending));
                memset(t930_modules, 0, sizeof(t930_modules));
                t930_modules[0].next = &t930_modules[1];
                t930_modules[1].previous = &t930_modules[0];
                t930_modules[0].callback_word = (unsigned int)t930_head_event;
                t930_modules[1].callback_word = (unsigned int)t930_tail_event;
                memcpy(t930_expected_modules, t930_modules, sizeof(t930_modules));
                op_module_head = &t930_modules[0];
                op_module_tail = &t930_modules[1];
                op_module_initialized = 91;
                op_module_open = -31;
                t930_token = before_tokens = 0xdeadbeefu;
                t930_output = NULL;
                op_task_sentinel = -711;
                op_task_count = 1;
                op_task_cursor = 123;
                op_task_current = &op_tasks[3];
                op_skills_frame_delta = t930_real(t930_current->incoming);
                op_task_target = t930_real(t930_current->initial_target);
                op_task_simulation = t930_real(t930_current->initial_sim);
                op_time_mode = t930_real(0x3f900000u);
                op_fixed_rate_gate = t930_current->gate;
                op_fixed_generation = 0xfffffffeu;
                op_active_world = &t930_worlds[0];
                op_advance_runtime_fixed_steps(arguments[argument]);
                T930_CHECK(t930_iteration == t930_current->count && t930_module_calls == 2 * t930_current->count);
                T930_CHECK(t930_word(op_skills_frame_delta) == t930_current->saved &&
                           t930_word(op_task_target) == t930_current->target);
                T930_CHECK(t930_word(op_task_simulation) == t930_current->simulation &&
                           t930_word(op_time_mode) == t930_current->alpha);
                T930_CHECK(op_fixed_generation == t930_current->generation &&
                           op_active_world == &t930_worlds[t930_current->final_world]);
                T930_CHECK(op_fixed_rate_gate == t930_current->gate && t930_token == before_tokens &&
                           op_task_sentinel == -711);
                T930_CHECK(op_task_cursor == (t930_current->count ? 64u : 123u));
                T930_CHECK(op_task_current == &op_tasks[t930_current->count ? 0 : 3]);
                T930_CHECK(t930_output == (t930_current->count ? &t930_token : NULL));
                T930_CHECK(op_task_count == 1);
                T930_CHECK(!memcmp(t930_worlds, t930_expected_worlds, sizeof(t930_worlds)));
                T930_CHECK(!memcmp(op_tasks, t930_expected_tasks, sizeof(op_tasks)));
                T930_CHECK(!memcmp(t930_modules, t930_expected_modules, sizeof(t930_modules)));
                T930_CHECK(!memcmp(op_task_pending, t930_expected_pending, sizeof(t930_expected_pending)));
                T930_CHECK(op_module_head == &t930_modules[0] && op_module_tail == &t930_modules[1]);
                T930_CHECK(op_module_initialized == 91 && op_module_open == -31);
            }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    op_module_head = saved_head;
    op_module_tail = saved_tail;
    op_module_initialized = saved_initialized;
    op_module_open = saved_open;
    op_active_world = saved_world;
    memcpy(op_tasks, saved_tasks, sizeof(saved_tasks));
    memcpy(op_task_pending, saved_pending, sizeof(saved_pending));
    op_task_current = saved_current;
    op_task_count = saved_count;
    op_task_cursor = saved_cursor;
    op_task_sentinel = saved_sentinel;
    op_fixture_store_float_word(&op_task_target, saved_target);
    op_fixture_store_float_word(&op_task_simulation, saved_simulation);
    op_fixture_store_float_word(&op_time_mode, saved_alpha);
    op_fixture_store_float_word(&op_skills_frame_delta, saved_delta);
    op_fixed_rate_gate = saved_rate_gate;
    op_fixed_generation = saved_generation;
    t930_active = saved_route;
    printf("advance_runtime_fixed_steps: %d checks, %d failures\n", t930_checks, t930_failures);
    return t930_failures != 0;
}

#undef T930_CHECK
