#include "api.h"
#include <stdio.h>

int op_music_initialized;
int op_music_lock_depth;
static int checks;
static int failures;
static int calls;
static int original_depth;
static int lock_delta;
static int service_delta;
static int unlock_delta;
static int clear_initialized_in_lock;

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
    check(calls == 0, "lock first");
    check(op_music_lock_depth == original_depth, "lock sees old depth");
    ++calls;
    op_music_lock_depth += lock_delta;
    if (clear_initialized_in_lock)
        op_music_initialized = 0;
}

void op_music_service(void)
{
    check(calls == 1, "service second");
    check(op_music_lock_depth == original_depth + lock_delta + 1,
          "service sees current post-lock increment");
    ++calls;
    op_music_lock_depth += service_delta;
}

static void __stdcall unlock_stub(void)
{
    check(calls == 2, "unlock third");
    check(op_music_lock_depth == original_depth + lock_delta + 1 + service_delta,
          "unlock sees current service-mutated depth");
    ++calls;
    op_music_lock_depth += unlock_delta;
}

op_music_lock_fn op_music_lock = lock_stub;
op_music_lock_fn op_music_unlock = unlock_stub;

static void gated(int initialized)
{
    calls = 0;
    op_music_initialized = initialized;
    op_music_lock_depth = 17;
    op_music_periodic();
    check(calls == 0 && op_music_lock_depth == 17, "only initialized value one enters");
    check(op_music_initialized == initialized, "gated path preserves initialized value");
}

int main(void)
{
    gated(0);
    gated(-1);
    gated(2);

    calls = 0;
    original_depth = 10;
    op_music_initialized = 1;
    op_music_lock_depth = original_depth;
    lock_delta = 0;
    service_delta = 0;
    unlock_delta = 0;
    op_music_periodic();
    check(calls == 3 && op_music_lock_depth == original_depth,
          "ordinary callbacks leave balanced depth");

    calls = 0;
    original_depth = 100;
    clear_initialized_in_lock = 1;
    op_music_lock_depth = original_depth;
    lock_delta = 3;
    service_delta = 5;
    unlock_delta = 7;
    op_music_periodic();
    check(calls == 3 && op_music_lock_depth == 115,
          "callback mutations are reloaded and retained");
    check(op_music_initialized == 0, "lock mutation does not re-gate callbacks or get overwritten");
    printf("Music periodic: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}