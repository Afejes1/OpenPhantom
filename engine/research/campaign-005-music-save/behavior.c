#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_music_state;
int op_music_sequence;
int op_music_cached_ids[2];
static int checks;
static int failures;
static int writer_result;
static int expected_tag;
static int expected_state;
static int expected_sequence;
static int callback_calls;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

int op_music_write_frame(int tag, const void *data, int size,
                         unsigned short version)
{
    const int *ids = (const int *)data;
    ++callback_calls;
    check(tag == expected_tag, "tag forwarded unchanged");
    check(data == op_music_cached_ids, "cache base pointer passed");
    check(size == 8 && version == 2, "frame size and version");
    check(ids[0] == expected_state && ids[1] == expected_sequence,
          "both IDs copied before callback");
    op_music_cached_ids[0] = 101;
    op_music_cached_ids[1] = 102;
    op_music_state = 103;
    op_music_sequence = 104;
    return writer_result;
}

static void run_case(int tag, int state, int sequence, int result, int expected)
{
    expected_tag = tag;
    expected_state = state;
    expected_sequence = sequence;
    writer_result = result;
    callback_calls = 0;
    op_music_state = state;
    op_music_sequence = sequence;
    op_music_cached_ids[0] = 0;
    op_music_cached_ids[1] = 0;
    check(op_music_save_cached_ids(tag) == expected, "normalized callback result");
    check(callback_calls == 1, "one frame callback");
    check(op_music_cached_ids[0] == 101 && op_music_cached_ids[1] == 102 &&
          op_music_state == 103 && op_music_sequence == 104,
          "callback mutations remain after return");
}

int main(void)
{
    run_case(7, INT_MIN, INT_MAX, 0, 0);
    run_case(-9, -31, 42, -1, 1);
    run_case(INT_MAX, 1, 2, 19, 1);
    printf("Music save IDs: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}