#include "api.h"
#include <stdio.h>

int op_sound_initialized;
void *op_sound_registry;
static int checks;
static int failures;
static int events[20];
static int event_count;
static int expected_release;
static int clear_init_during_resume;
static void *registry_before_gate;
static void *expected_destroy;
static void *registry_after_quick;
static char first_registry;
static char second_registry;
static char final_registry;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void event(int value)
{
    if (event_count >= 20) {
        check(0, "bounded event capture");
        return;
    }
    events[event_count++] = value;
}

void op_sound_resume_all(void)
{
    event(1);
    if (clear_init_during_resume)
        op_sound_initialized = 0;
    op_sound_registry = &first_registry;
}

void op_sound_stop_all(void)
{
    event(2);
    op_sound_registry = &second_registry;
}

void op_sound_free_extra_refs(void)
{
    event(3);
    op_sound_registry = registry_before_gate;
}

void op_sound_release_handle(int index)
{
    event(10 + index);
    check(index == expected_release, "release indices increase from zero");
    ++expected_release;
}

void op_sound_clear_backend(void)
{
    event(30);
}

void op_sound_destroy_registry(void *registry)
{
    event(31);
    check(registry == expected_destroy, "destroy receives current registry");
    op_sound_registry = &first_registry;
}

static void __stdcall quick_shutdown(void)
{
    event(32);
    check(op_sound_initialized == (clear_init_during_resume ? 0 : -9),
          "quick shutdown sees initialized value left by earlier callbacks");
    check(op_sound_registry == 0, "registry cleared before quick shutdown");
    op_sound_initialized = 77;
    op_sound_registry = registry_after_quick;
}

op_sound_shutdown_fn op_sound_quick_shutdown = quick_shutdown;

static void check_order(int has_destroy)
{
    int position = 0;
    int i;

    check(events[position++] == 1, "resume is first");
    check(events[position++] == 2, "stop-all is second");
    check(events[position++] == 3, "free-extra-refs is third");
    for (i = 0; i < 12; ++i)
        check(events[position++] == 10 + i, "release callback order");
    check(events[position++] == 30, "backend clear follows releases");
    if (has_destroy)
        check(events[position++] == 31, "registry destroy follows backend clear");
    check(events[position++] == 32, "quick shutdown is last callback");
    check(position == event_count, "no extra callbacks");
}

static void active_case(void *registry_at_gate)
{
    int has_destroy = registry_at_gate != 0;
    event_count = 0;
    expected_release = 0;
    op_sound_initialized = -9;
    op_sound_registry = &first_registry;
    registry_before_gate = registry_at_gate;
    expected_destroy = registry_at_gate;
    registry_after_quick = &final_registry;
    check(op_sound_shutdown() == 0, "active shutdown returns zero");
    check_order(has_destroy);
    check(expected_release == 12, "all twelve handles released");
    check(op_sound_initialized == 0, "initialized cleared after callbacks");
    check(op_sound_registry == &final_registry,
          "quick shutdown registry mutation retained");
}

int main(void)
{
    op_sound_initialized = 0;
    op_sound_registry = &first_registry;
    event_count = 0;
    check(op_sound_shutdown() == 0, "uninitialized shutdown returns zero");
    check(event_count == 0, "uninitialized shutdown invokes nothing");
    check(op_sound_initialized == 0 && op_sound_registry == &first_registry,
          "uninitialized shutdown preserves globals");
    active_case(0);
    active_case(&second_registry);
    clear_init_during_resume = 1;
    active_case(&second_registry);
    printf("Sound shutdown: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
