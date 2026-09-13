#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_music_initialized;
int op_music_state;
int op_music_lock_depth;
static int checks;
static int failures;
static int calls;
static int expected_requested;
static int backend_state;
static int backend_result;
static int original_depth;
static int lock_depth_delta;
static int backend_depth_delta;
static int unlock_depth_delta;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void __stdcall lock_stub(void)
{
    check(calls == 0, "lock is first callback");
    check(op_music_state == expected_requested, "lock sees requested state cached");
    check(op_music_lock_depth == original_depth, "lock sees old depth");
    ++calls;
    op_music_state = backend_state;
    op_music_initialized = 0;
    op_music_lock_depth += lock_depth_delta;
}

int op_music_backend_set_state(int state)
{
    check(calls == 1, "backend follows lock and increment");
    check(state == backend_state, "backend receives state reloaded after lock");
    check(op_music_lock_depth == original_depth + lock_depth_delta + 1,
          "backend sees current incremented depth");
    ++calls;
    op_music_state = 71;
    op_music_initialized = -9;
    op_music_lock_depth += backend_depth_delta;
    return backend_result;
}

static void __stdcall unlock_stub(void)
{
    check(calls == 2, "unlock follows backend");
    check(op_music_lock_depth == original_depth + lock_depth_delta + 1 +
          backend_depth_delta, "unlock sees backend-mutated depth");
    ++calls;
    op_music_state = 72;
    op_music_initialized = 73;
    op_music_lock_depth += unlock_depth_delta;
}

op_music_lock_fn op_music_lock = lock_stub;
op_music_lock_fn op_music_unlock = unlock_stub;

static void gated(int initialized, int current, int requested)
{
    calls = 0;
    op_music_initialized = initialized;
    op_music_state = current;
    op_music_lock_depth = 19;
    check(op_music_set_state(requested) == 0, "gated path returns zero");
    check(calls == 0, "gated path performs no callbacks");
    check(op_music_initialized == initialized && op_music_state == current &&
          op_music_lock_depth == 19, "gated path preserves globals");
}

static void active(int initialized, int requested, int changed_state,
                   int result, int lock_delta, int backend_delta,
                   int unlock_delta)
{
    calls = 0;
    op_music_initialized = initialized;
    op_music_state = 5;
    original_depth = 100;
    op_music_lock_depth = original_depth;
    expected_requested = requested;
    backend_state = changed_state;
    backend_result = result;
    lock_depth_delta = lock_delta;
    backend_depth_delta = backend_delta;
    unlock_depth_delta = unlock_delta;
    check(op_music_set_state(requested) == result,
          "backend result preserved across unlock");
    check(calls == 3, "all callbacks run after entry");
    check(op_music_state == 72 && op_music_initialized == 73,
          "callback state mutations remain");
    check(op_music_lock_depth == original_depth + lock_delta + backend_delta +
          unlock_delta, "final decrement uses current depth");
}

int main(void)
{
    gated(0, 11, 12);
    gated(1, INT_MIN, INT_MIN);
    gated(-7, INT_MAX, INT_MAX);
    active(1, INT_MIN, -31, 0, 0, 0, 0);
    active(-1, INT_MAX, 42, -17, 3, 5, 7);
    active(9, -100, 101, 29, -2, 4, -1);
    printf("Music set state: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}