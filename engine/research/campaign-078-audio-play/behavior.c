#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
unsigned int op_audio_gate_word;
static int checks, failures, calls, selected, mutate, return_code;
static unsigned int initial_gate, expected_gate, arg_word;
static void *arg_data;
static OP_AUDIO_BUFFER *wanted_this;
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
static unsigned int tokens[3], expected_tokens[3];
static void observe(OP_AUDIO_BUFFER *object, int which)
{
    CHECK(calls == 0);
    CHECK(which == selected);
    CHECK(object == wanted_this);
    CHECK(op_audio_gate_word == initial_gate);
    CHECK(memcmp(buffers, before_buffers, sizeof(buffers)) == 0);
    CHECK(memcmp(tables, before_tables, sizeof(tables)) == 0);
    CHECK(memcmp(tokens, expected_tokens, sizeof(tokens)) == 0);
    if (calls || object != wanted_this)
        exit(2);
    ++calls;
    if (mutate)
    {
        object->vt = &tables[1 - selected];
        buffers[selected].payload[1] = 0xabcdef01u;
        op_audio_gate_word = 0x100u;
    }
}
static int __stdcall callback0(OP_AUDIO_BUFFER *object, unsigned int reserved, unsigned int priority,
                               unsigned int flags)
{
    CHECK(reserved == 0);
    CHECK(priority == 0);
    CHECK(flags == arg_word);
    observe(object, 0);
    return return_code;
}
static int __stdcall callback1(OP_AUDIO_BUFFER *object, unsigned int reserved, unsigned int priority,
                               unsigned int flags)
{
    CHECK(reserved == 0);
    CHECK(priority == 0);
    CHECK(flags == arg_word);
    observe(object, 1);
    return return_code;
}
static void seed(int expected_calls)
{
    int i;
    memset(buffers, 0x59, sizeof(buffers));
    memset(tables, 0x6a, sizeof(tables));
    tables[0].play = callback0;
    tables[1].play = callback1;
    for (i = 0; i < 2; ++i)
        buffers[i].value.vt = &tables[i];
    for (i = 0; i < 3; ++i)
        tokens[i] = 0x78123456u + (unsigned int)i;
    memcpy(before_buffers, buffers, sizeof(buffers));
    memcpy(expected_buffers, buffers, sizeof(buffers));
    memcpy(before_tables, tables, sizeof(tables));
    memcpy(expected_tokens, tokens, sizeof(tokens));
    op_audio_gate_word = initial_gate;
    expected_gate = initial_gate;
    calls = 0;
    if (expected_calls && mutate)
    {
        expected_buffers[selected].value.vt = &tables[1 - selected];
        expected_buffers[selected].payload[1] = 0xabcdef01u;
        expected_gate = 0x100u;
    }
}
static void verify(int expected_calls)
{
    CHECK(calls == expected_calls);
    CHECK(op_audio_gate_word == expected_gate);
    CHECK(memcmp(buffers, expected_buffers, sizeof(buffers)) == 0);
    CHECK(memcmp(tables, before_tables, sizeof(tables)) == 0);
    CHECK(memcmp(tokens, expected_tokens, sizeof(tokens)) == 0);
}
int main(void)
{
    int gi, ri, a, p, result, expected_calls;
    static const unsigned int gates[4] = {0, 1, 0x80000000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    for (selected = 0; selected < 2; ++selected)
        for (mutate = 0; mutate < 2; ++mutate)
            for (gi = 0; gi < 4; ++gi)
                for (ri = 0; ri < 5; ++ri)
                    for (a = 0; a < 7; ++a)
                        for (p = 0; p < 1; ++p)
                        {
                            initial_gate = gates[gi];
                            return_code = returns[ri];
                            expected_calls = 1;
                            arg_word = values[a] != 0;
                            arg_data = 0;
                            seed(expected_calls);
                            wanted_this = &buffers[selected].value;
                            result = op_audio_play(wanted_this, values[a]);
                            CHECK(result == (return_code == 0));
                            verify(expected_calls);
                        }
    printf("audio_play: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
