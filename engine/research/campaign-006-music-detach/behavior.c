#include "api.h"
#include <stdio.h>
#include <string.h>

int op_music_initialized;
int op_music_paused;
int op_music_lock_depth;
static int checks;
static int failures;
static int calls;
static int expected_resume;
static int expected_calls;
static int state_result;
static int sequence_result;
static int original_depth;
static int stop_delta;
static int terminate_delta;
static int shutdown_delta;
static int unlock_delta;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

void op_music_resume(void)
{
    check(expected_resume && calls == 0, "optional resume is first");
    ++calls;
    op_music_paused = -5;
    op_music_initialized = 2;
}

int op_music_set_state(int state)
{
    check(calls == expected_resume, "state follows optional resume");
    check(state == 1000, "state sentinel");
    ++calls;
    op_music_initialized = 3;
    return state_result;
}

int op_music_set_sequence(int sequence, unsigned int mode,
                          unsigned int threshold, float radius)
{
    unsigned long radius_bits;
    memcpy(&radius_bits, &radius, sizeof radius_bits);
    check(calls == expected_resume + 1, "sequence follows state");
    check(sequence == 2000 && mode == 0 && threshold == 0 && radius_bits == 0,
          "sequence sentinel and positive-zero controls");
    ++calls;
    op_music_initialized = 4;
    return sequence_result;
}

static void __stdcall lock_stub(void)
{
    check(calls == expected_resume + 2, "lock follows playback reset calls");
    check(op_music_lock_depth == original_depth, "lock sees old depth");
    check(op_music_initialized == 4, "lock sees callback-mutated initialized");
    ++calls;
}

void op_music_stop_all(void)
{
    check(calls == expected_resume + 3, "stop follows lock and increment");
    check(op_music_lock_depth == original_depth + 1,
          "stop sees incremented depth");
    ++calls;
    op_music_lock_depth += stop_delta;
    op_music_initialized = 5;
}

void op_music_terminate_script(void)
{
    check(calls == expected_resume + 4, "terminate follows stop");
    ++calls;
    op_music_lock_depth += terminate_delta;
    op_music_initialized = 6;
}

static void __cdecl shutdown_stub(void)
{
    check(calls == expected_resume + 5, "shutdown follows terminate");
    ++calls;
    op_music_lock_depth += shutdown_delta;
    op_music_initialized = 7;
}

static void __stdcall unlock_stub(void)
{
    check(calls == expected_resume + 6, "unlock follows shutdown");
    check(op_music_initialized == 7, "initialized remains set through unlock");
    check(op_music_lock_depth == original_depth + 1 + stop_delta +
          terminate_delta + shutdown_delta, "unlock sees live depth");
    ++calls;
    op_music_lock_depth += unlock_delta;
    op_music_initialized = 8;
}

op_music_lock_fn op_music_lock = lock_stub;
op_music_lock_fn op_music_unlock = unlock_stub;
op_music_shutdown_fn op_music_shutdown = shutdown_stub;

static void gated(void)
{
    calls = 0;
    op_music_initialized = 0;
    op_music_paused = -9;
    op_music_lock_depth = 17;
    op_music_detach();
    check(calls == 0, "uninitialized path performs no callbacks");
    check(op_music_initialized == 0 && op_music_paused == -9 &&
          op_music_lock_depth == 17, "uninitialized path preserves globals");
}

static void active(int initialized, int paused, int state_return,
                   int sequence_return)
{
    calls = 0;
    expected_resume = paused != 0;
    expected_calls = expected_resume + 7;
    state_result = state_return;
    sequence_result = sequence_return;
    original_depth = 100;
    op_music_initialized = initialized;
    op_music_paused = paused;
    op_music_lock_depth = original_depth;
    stop_delta = 3;
    terminate_delta = 5;
    shutdown_delta = 7;
    unlock_delta = 11;
    op_music_detach();
    check(calls == expected_calls, "all required callbacks run despite failures");
    check(op_music_initialized == 0, "initialized cleared after unlock");
    check(op_music_lock_depth == 126, "final decrement uses live depth");
    check(op_music_paused == (expected_resume ? -5 : paused),
          "detach changes paused only through resume stub");
}

int main(void)
{
    gated();
    active(1, 0, 0, 0);
    active(-1, 1, -7, -8);
    active(9, -3, 11, 12);
    printf("Music detach: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}