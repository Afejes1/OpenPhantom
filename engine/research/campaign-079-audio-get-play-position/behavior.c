#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int checks, failures, calls, selected, mutate, first_result, second_result;
static unsigned int status_word, position_word;
static unsigned int *wanted_output, *status_pointer;
static unsigned int output[3], before_output[3], expected_output[3];
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
typedef struct OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} OWNED_BUFFER;
static OWNED_BUFFER buffers[2], before_buffers[2], expected_buffers[2];
static OP_AUDIO_BUFFER_VT tables[2], before_tables[2];
static void verify_state(int after_first)
{
    CHECK(memcmp(buffers, after_first ? expected_buffers : before_buffers, sizeof(buffers)) == 0);
    CHECK(memcmp(tables, before_tables, sizeof(tables)) == 0);
    CHECK(memcmp(output, before_output, sizeof(output)) == 0);
}
static int observe_status(OP_AUDIO_BUFFER *object, unsigned int *status, int which)
{
    CHECK(calls == 0);
    CHECK(which == selected);
    CHECK(object == &buffers[selected].value);
    CHECK(status != 0 && status != &output[0] && status != &output[1] && status != &output[2]);
    if (calls || !status || status == &output[0] || status == &output[1] || status == &output[2])
        exit(2);
    verify_state(0);
    status_pointer = status;
    *status = status_word;
    ++calls;
    if (mutate)
    {
        object->vt = &tables[1 - selected];
        buffers[selected].payload[1] = 0xabcdef01u;
    }
    return first_result;
}
static int observe_position(OP_AUDIO_BUFFER *object, unsigned int *play, unsigned int *write, int which)
{
    CHECK(calls == 1);
    CHECK(which == (mutate ? 1 - selected : selected));
    CHECK(object == &buffers[selected].value);
    CHECK(play == wanted_output);
    CHECK(write != 0 && write != status_pointer && write != play);
    if (calls != 1 || play != wanted_output || !write || write == status_pointer || write == play)
        exit(2);
    verify_state(1);
    *play = position_word;
    *write = 0x456789abu;
    ++calls;
    return second_result;
}
static int __stdcall status0(OP_AUDIO_BUFFER *object, unsigned int *status)
{
    return observe_status(object, status, 0);
}
static int __stdcall status1(OP_AUDIO_BUFFER *object, unsigned int *status)
{
    return observe_status(object, status, 1);
}
static int __stdcall position0(OP_AUDIO_BUFFER *object, unsigned int *play, unsigned int *write)
{
    return observe_position(object, play, write, 0);
}
static int __stdcall position1(OP_AUDIO_BUFFER *object, unsigned int *play, unsigned int *write)
{
    return observe_position(object, play, write, 1);
}
int main(void)
{
    static const int results[5] = {0, 1, -1, INT_MIN, INT_MAX};
    static const unsigned int statuses[7] = {0, 1, 2, 3, 0x80000000u, 0x80000001u, 0xffffffffu};
    int r, s, t, n, i, result, want_calls;
    for (selected = 0; selected < 2; ++selected)
        for (mutate = 0; mutate < 2; ++mutate)
            for (r = 0; r < 5; ++r)
                for (s = 0; s < 7; ++s)
                    for (t = 0; t < 5; ++t)
                        for (n = 0; n < 2; ++n)
                        {
                            memset(buffers, 0x59, sizeof(buffers));
                            memset(tables, 0x6a, sizeof(tables));
                            tables[0].get_status = status0;
                            tables[1].get_status = status1;
                            tables[0].get_position = position0;
                            tables[1].get_position = position1;
                            for (i = 0; i < 2; ++i)
                                buffers[i].value.vt = &tables[i];
                            for (i = 0; i < 3; ++i)
                                output[i] = 0x12345678u + (unsigned int)i;
                            memcpy(before_buffers, buffers, sizeof(buffers));
                            memcpy(expected_buffers, buffers, sizeof(buffers));
                            memcpy(before_tables, tables, sizeof(tables));
                            memcpy(before_output, output, sizeof(output));
                            memcpy(expected_output, output, sizeof(output));
                            if (mutate)
                            {
                                expected_buffers[selected].value.vt = &tables[1 - selected];
                                expected_buffers[selected].payload[1] = 0xabcdef01u;
                            }
                            first_result = results[r];
                            second_result = results[t];
                            status_word = statuses[s];
                            position_word = (unsigned int)r * 0x1fffffffu + (unsigned int)s * 31337u + (unsigned int)t;
                            wanted_output = n ? &output[1] : 0;
                            want_calls = 1 + (first_result == 0 && (status_word & 1) && n);
                            if (want_calls == 2)
                                expected_output[1] = position_word;
                            calls = 0;
                            status_pointer = 0;
                            result = op_audio_get_play_position(&buffers[selected].value, wanted_output);
                            CHECK(result == (first_result ? -1 : (status_word & 1) ? 1 : 0));
                            CHECK(calls == want_calls);
                            CHECK(memcmp(buffers, expected_buffers, sizeof(buffers)) == 0);
                            CHECK(memcmp(tables, before_tables, sizeof(tables)) == 0);
                            CHECK(memcmp(output, expected_output, sizeof(output)) == 0);
                        }
    printf("audio_get_play_position: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
