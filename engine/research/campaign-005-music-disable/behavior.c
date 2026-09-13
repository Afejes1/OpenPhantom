#include "api.h"
#include <limits.h>
#include <stdio.h>

const char op_music_disabled_key[] = "authored-disabled";
int op_music_state;
int op_music_sequence;
int op_music_resume_state;
int op_music_resume_sequence;
static int checks;
static int failures;
static int calls;
static int writer_result;
static int mutate_state;
static int mutate_sequence;

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
    check(calls == 0, "writer is first callback");
    check(key == op_music_disabled_key && value == 1, "writer arguments");
    ++calls;
    op_music_state = mutate_state;
    op_music_sequence = mutate_sequence;
    return writer_result;
}

void op_music_detach(void)
{
    check(calls == 1, "detach follows writer and copies");
    check(op_music_resume_state == mutate_state &&
          op_music_resume_sequence == mutate_sequence,
          "detach observes post-writer copies");
    ++calls;
    op_music_state = 91;
    op_music_sequence = 92;
    op_music_resume_state = 93;
    op_music_resume_sequence = 94;
}

static void run_case(int result, int state, int sequence)
{
    calls = 0;
    writer_result = result;
    mutate_state = state;
    mutate_sequence = sequence;
    op_music_state = 1;
    op_music_sequence = 2;
    op_music_resume_state = 3;
    op_music_resume_sequence = 4;
    op_music_disable();
    check(calls == 2, "both callbacks run regardless of writer result");
    check(op_music_state == 91 && op_music_sequence == 92 &&
          op_music_resume_state == 93 && op_music_resume_sequence == 94,
          "detach mutations remain after return");
}

int main(void)
{
    run_case(0, INT_MIN, INT_MAX);
    run_case(-7, -31, 42);
    run_case(1, 100, 200);
    printf("Music disable: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}