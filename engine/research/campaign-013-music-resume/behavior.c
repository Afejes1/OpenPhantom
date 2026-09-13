#include "api.h"
#include <stdio.h>

int op_music_lock_depth;
int op_music_paused;

static int checks;
static int failures;
static int stage;
static int initial_depth;
static int initial_paused;
static int lock_delta;
static int backend_delta;
static int unlock_delta;
static int replacement_called;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void __stdcall replacement_unlock(void)
{
    check(stage == 2, "replacement unlock is third");
    check(op_music_lock_depth == initial_depth + lock_delta + 1 + backend_delta,
          "replacement unlock sees backend-mutated depth");
    check(op_music_paused == 73, "unlock sees backend-mutated paused state");
    ++stage;
    ++replacement_called;
    op_music_lock_depth += unlock_delta;
    op_music_paused = 91;
}

static void __stdcall initial_unlock(void)
{
    check(0, "backend must replace unlock callback");
}

static void __stdcall lock_stub(void)
{
    check(stage == 0, "lock is first");
    check(op_music_lock_depth == initial_depth, "lock sees initial depth");
    check(op_music_paused == initial_paused, "paused unchanged before lock");
    ++stage;
    op_music_lock_depth += lock_delta;
    op_music_paused = 41;
}

op_music_callback op_music_lock = lock_stub;
op_music_callback op_music_unlock = initial_unlock;

void op_backend_resume(void)
{
    check(stage == 1, "backend is second");
    check(op_music_lock_depth == initial_depth + lock_delta + 1,
          "backend sees increment after lock mutation");
    check(op_music_paused == 41, "backend sees lock-mutated paused state");
    ++stage;
    op_music_lock_depth += backend_delta;
    op_music_paused = 73;
    op_music_unlock = replacement_unlock;
}

static void run_case(int depth, int paused, int ld, int bd, int ud)
{
    initial_depth = depth;
    initial_paused = paused;
    lock_delta = ld;
    backend_delta = bd;
    unlock_delta = ud;
    op_music_lock_depth = depth;
    op_music_paused = paused;
    op_music_unlock = initial_unlock;
    stage = 0;
    replacement_called = 0;

    op_music_resume();

    check(stage == 3, "all callbacks occur in order");
    check(replacement_called == 1, "live unlock callback is reloaded");
    check(op_music_lock_depth == depth + ld + bd + ud,
          "final decrement uses callback-mutated live depth");
    check(op_music_paused == 0, "paused clears only after callbacks");
    check(op_music_lock == lock_stub, "lock callback remains unchanged");
    check(op_music_unlock == replacement_unlock, "replacement callback remains installed");
}

int main(void)
{
    run_case(-20, -1, 3, 5, 7);
    run_case(0, 1, 0, 0, 0);
    run_case(100, 12345, -4, 8, -2);
    printf("Music resume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
