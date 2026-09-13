#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

const char op_music_disabled_key[] = "authored-disabled";
int op_music_resume_state;
int op_music_resume_sequence;
static int checks;
static int failures;
static int calls;
static int writer_result;
static int initialize_result;
static int state_result;
static int sequence_result;
static int initialized_state;
static int state_mutated_sequence;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

int op_music_write_option(const char *key, int value)
{
    check(calls == 0, "writer is first");
    check(key == op_music_disabled_key && value == 0, "writer arguments");
    ++calls;
    return writer_result;
}

int op_music_initialize(void)
{
    check(calls == 1, "initialize is second");
    ++calls;
    op_music_resume_state = initialized_state;
    return initialize_result;
}

int op_music_set_state(int state)
{
    check(calls == 2, "state setter is third");
    check(state == initialized_state, "state reload occurs after initialize");
    ++calls;
    op_music_resume_sequence = state_mutated_sequence;
    return state_result;
}

int op_music_set_sequence(int sequence, unsigned int mode,
                          unsigned int threshold, float radius)
{
    unsigned long radius_bits;
    memcpy(&radius_bits, &radius, sizeof(radius_bits));
    check(calls == 3, "sequence setter is fourth");
    check(sequence == state_mutated_sequence,
          "sequence reload occurs after state callback");
    check(mode == 0 && threshold == 0 && radius_bits == 0,
          "sequence control arguments are zero");
    ++calls;
    op_music_resume_state = 71;
    op_music_resume_sequence = 72;
    return sequence_result;
}

static void run_case(int writer, int initialize, int state, int sequence,
                     int saved_state, int saved_sequence)
{
    calls = 0;
    writer_result = writer;
    initialize_result = initialize;
    state_result = state;
    sequence_result = sequence;
    initialized_state = saved_state;
    state_mutated_sequence = saved_sequence;
    op_music_resume_state = 10;
    op_music_resume_sequence = 20;
    op_music_enable();
    check(calls == 4, "all callbacks run regardless of failures");
    check(op_music_resume_state == 71 && op_music_resume_sequence == 72,
          "last callback mutations survive return");
}

int main(void)
{
    run_case(0, 0, 0, 0, INT_MIN, INT_MAX);
    run_case(-1, -2, -3, -4, -123, 456);
    run_case(1, 2, 3, 4, INT_MAX, INT_MIN);
    printf("Music enable: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}