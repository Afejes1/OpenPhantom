#include "api.h"
#include <stdio.h>

int op_music_lock_depth;
int op_music_paused;
static int checks;
static int failures;
static int calls;
static int original_depth;
static int original_paused;
static int lock_delta;
static int backend_delta;
static int unlock_delta;

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
    check(op_music_paused == original_paused, "lock sees old pause flag");
    ++calls;
    op_music_lock_depth += lock_delta;
    op_music_paused = original_paused + 10;
}

void op_music_pause_backend(void)
{
    check(calls == 1, "backend second");
    check(op_music_lock_depth == original_depth + lock_delta + 1,
          "backend sees current incremented depth");
    check(op_music_paused == original_paused + 10,
          "backend sees callback-mutated old flag");
    ++calls;
    op_music_lock_depth += backend_delta;
    op_music_paused = original_paused + 20;
}

static void __stdcall unlock_stub(void)
{
    check(calls == 2, "unlock third");
    check(op_music_lock_depth == original_depth + lock_delta + 1 + backend_delta,
          "unlock sees current backend-mutated depth");
    check(op_music_paused == original_paused + 20,
          "unlock occurs before final paused write");
    ++calls;
    op_music_lock_depth += unlock_delta;
    op_music_paused = original_paused + 30;
}

op_music_lock_fn op_music_lock = lock_stub;
op_music_lock_fn op_music_unlock = unlock_stub;

static void run_case(int paused, int lock_change, int backend_change,
                     int unlock_change)
{
    calls = 0;
    original_depth = 100;
    original_paused = paused;
    op_music_lock_depth = original_depth;
    op_music_paused = paused;
    lock_delta = lock_change;
    backend_delta = backend_change;
    unlock_delta = unlock_change;
    op_music_pause();
    check(calls == 3, "all callbacks always run");
    check(op_music_lock_depth == original_depth + lock_change +
          backend_change + unlock_change, "final decrement uses current depth");
    check(op_music_paused == 1, "paused flag written last");
}

int main(void)
{
    run_case(0, 0, 0, 0);
    run_case(1, 3, 5, 7);
    run_case(-9, -2, 4, -1);
    printf("Music pause: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}