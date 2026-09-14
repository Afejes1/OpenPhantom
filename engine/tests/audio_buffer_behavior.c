/* Complete frozen callback matrices for the three canonical audio wrappers. */
#include "../src/audio_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int a780_checks, a780_failures, a780_calls, a780_selected, a780_mutate, a780_return_code;
static unsigned int a780_initial_gate, a780_expected_gate, a780_arg_word;
static void *a780_arg_data;
static OP_AUDIO_BUFFER *a780_wanted_this;
static void a780_check(int ok, int line)
{
    ++a780_checks;
    if (!ok)
    {
        ++a780_failures;
        printf("line%d failed\n", line);
    }
}
#define A780_CHECK(x) a780_check(!!(x), __LINE__)
typedef struct A780_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} A780_OWNED_BUFFER;
static A780_OWNED_BUFFER a780_buffers[2], a780_before_buffers[2], a780_expected_buffers[2];
static OP_AUDIO_BUFFER_VT a780_tables[2], a780_before_tables[2];
static unsigned int a780_tokens[3], a780_expected_tokens[3];
static void a780_observe(OP_AUDIO_BUFFER *object, int which)
{
    A780_CHECK(a780_calls == 0);
    A780_CHECK(which == a780_selected);
    A780_CHECK(object == a780_wanted_this);
    A780_CHECK(op_audio_gate_word == a780_initial_gate);
    A780_CHECK(memcmp(a780_buffers, a780_before_buffers, sizeof(a780_buffers)) == 0);
    A780_CHECK(memcmp(a780_tables, a780_before_tables, sizeof(a780_tables)) == 0);
    A780_CHECK(memcmp(a780_tokens, a780_expected_tokens, sizeof(a780_tokens)) == 0);
    if (a780_calls || object != a780_wanted_this)
        exit(2);
    ++a780_calls;
    if (a780_mutate)
    {
        object->vt = &a780_tables[1 - a780_selected];
        a780_buffers[a780_selected].payload[1] = 0xabcdef01u;
        op_audio_gate_word = 0x100u;
    }
}
static int __stdcall a780_callback0(OP_AUDIO_BUFFER *object)
{
    a780_observe(object, 0);
    return a780_return_code;
}
static int __stdcall a780_callback1(OP_AUDIO_BUFFER *object)
{
    a780_observe(object, 1);
    return a780_return_code;
}
static void a780_seed(int expected_calls)
{
    int i;
    memset(a780_buffers, 0x59, sizeof(a780_buffers));
    memset(a780_tables, 0x6a, sizeof(a780_tables));
    a780_tables[0].stop = a780_callback0;
    a780_tables[1].stop = a780_callback1;
    for (i = 0; i < 2; ++i)
        a780_buffers[i].value.vt = &a780_tables[i];
    for (i = 0; i < 3; ++i)
        a780_tokens[i] = 0x78123456u + (unsigned int)i;
    memcpy(a780_before_buffers, a780_buffers, sizeof(a780_buffers));
    memcpy(a780_expected_buffers, a780_buffers, sizeof(a780_buffers));
    memcpy(a780_before_tables, a780_tables, sizeof(a780_tables));
    memcpy(a780_expected_tokens, a780_tokens, sizeof(a780_tokens));
    op_audio_gate_word = a780_initial_gate;
    a780_expected_gate = a780_initial_gate;
    a780_calls = 0;
    if (expected_calls && a780_mutate)
    {
        a780_expected_buffers[a780_selected].value.vt = &a780_tables[1 - a780_selected];
        a780_expected_buffers[a780_selected].payload[1] = 0xabcdef01u;
        a780_expected_gate = 0x100u;
    }
}
static void a780_verify(int expected_calls)
{
    A780_CHECK(a780_calls == expected_calls);
    A780_CHECK(op_audio_gate_word == a780_expected_gate);
    A780_CHECK(memcmp(a780_buffers, a780_expected_buffers, sizeof(a780_buffers)) == 0);
    A780_CHECK(memcmp(a780_tables, a780_before_tables, sizeof(a780_tables)) == 0);
    A780_CHECK(memcmp(a780_tokens, a780_expected_tokens, sizeof(a780_tokens)) == 0);
}
static int op_test_audio_pause(void)
{
    int gi, ri, a, p, result, expected_calls;
    static const unsigned int gates[4] = {0, 1, 0x80000000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};

    for (a780_selected = 0; a780_selected < 2; ++a780_selected)
        for (a780_mutate = 0; a780_mutate < 2; ++a780_mutate)
            for (gi = 0; gi < 4; ++gi)
                for (ri = 0; ri < 5; ++ri)
                    for (a = 0; a < 1; ++a)
                        for (p = 0; p < 2; ++p)
                        {
                            a780_initial_gate = gates[gi];
                            a780_return_code = returns[ri];
                            if (a780_initial_gate && p)
                                continue;
                            expected_calls = a780_initial_gate != 0;
                            a780_arg_word = 0;
                            a780_arg_data = 0;
                            a780_seed(expected_calls);
                            a780_wanted_this = p ? 0 : &a780_buffers[a780_selected].value;
                            result = op_audio_pause(a780_wanted_this);
                            A780_CHECK(result == (a780_initial_gate != 0));
                            a780_verify(expected_calls);
                        }
    printf("audio_pause: %d checks, %d failures\n", a780_checks, a780_failures);
    return a780_failures != 0;
}

#undef A780_CHECK

#include "../src/audio_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int a781_checks, a781_failures, a781_calls, a781_selected, a781_mutate, a781_return_code;
static unsigned int a781_initial_gate, a781_expected_gate, a781_arg_word;
static void *a781_arg_data;
static OP_AUDIO_BUFFER *a781_wanted_this;
static void a781_check(int ok, int line)
{
    ++a781_checks;
    if (!ok)
    {
        ++a781_failures;
        printf("line%d failed\n", line);
    }
}
#define A781_CHECK(x) a781_check(!!(x), __LINE__)
typedef struct A781_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} A781_OWNED_BUFFER;
static A781_OWNED_BUFFER a781_buffers[2], a781_before_buffers[2], a781_expected_buffers[2];
static OP_AUDIO_BUFFER_VT a781_tables[2], a781_before_tables[2];
static unsigned int a781_tokens[3], a781_expected_tokens[3];
static void a781_observe(OP_AUDIO_BUFFER *object, int which)
{
    A781_CHECK(a781_calls == 0);
    A781_CHECK(which == a781_selected);
    A781_CHECK(object == a781_wanted_this);
    A781_CHECK(op_audio_gate_word == a781_initial_gate);
    A781_CHECK(memcmp(a781_buffers, a781_before_buffers, sizeof(a781_buffers)) == 0);
    A781_CHECK(memcmp(a781_tables, a781_before_tables, sizeof(a781_tables)) == 0);
    A781_CHECK(memcmp(a781_tokens, a781_expected_tokens, sizeof(a781_tokens)) == 0);
    if (a781_calls || object != a781_wanted_this)
        exit(2);
    ++a781_calls;
    if (a781_mutate)
    {
        object->vt = &a781_tables[1 - a781_selected];
        a781_buffers[a781_selected].payload[1] = 0xabcdef01u;
        op_audio_gate_word = 0x100u;
    }
}
static int __stdcall a781_callback0(OP_AUDIO_BUFFER *object, unsigned int reserved, unsigned int priority,
                                    unsigned int flags)
{
    A781_CHECK(reserved == 0);
    A781_CHECK(priority == 0);
    A781_CHECK(flags == a781_arg_word);
    a781_observe(object, 0);
    return a781_return_code;
}
static int __stdcall a781_callback1(OP_AUDIO_BUFFER *object, unsigned int reserved, unsigned int priority,
                                    unsigned int flags)
{
    A781_CHECK(reserved == 0);
    A781_CHECK(priority == 0);
    A781_CHECK(flags == a781_arg_word);
    a781_observe(object, 1);
    return a781_return_code;
}
static void a781_seed(int expected_calls)
{
    int i;
    memset(a781_buffers, 0x59, sizeof(a781_buffers));
    memset(a781_tables, 0x6a, sizeof(a781_tables));
    a781_tables[0].play = a781_callback0;
    a781_tables[1].play = a781_callback1;
    for (i = 0; i < 2; ++i)
        a781_buffers[i].value.vt = &a781_tables[i];
    for (i = 0; i < 3; ++i)
        a781_tokens[i] = 0x78123456u + (unsigned int)i;
    memcpy(a781_before_buffers, a781_buffers, sizeof(a781_buffers));
    memcpy(a781_expected_buffers, a781_buffers, sizeof(a781_buffers));
    memcpy(a781_before_tables, a781_tables, sizeof(a781_tables));
    memcpy(a781_expected_tokens, a781_tokens, sizeof(a781_tokens));
    op_audio_gate_word = a781_initial_gate;
    a781_expected_gate = a781_initial_gate;
    a781_calls = 0;
    if (expected_calls && a781_mutate)
    {
        a781_expected_buffers[a781_selected].value.vt = &a781_tables[1 - a781_selected];
        a781_expected_buffers[a781_selected].payload[1] = 0xabcdef01u;
        a781_expected_gate = 0x100u;
    }
}
static void a781_verify(int expected_calls)
{
    A781_CHECK(a781_calls == expected_calls);
    A781_CHECK(op_audio_gate_word == a781_expected_gate);
    A781_CHECK(memcmp(a781_buffers, a781_expected_buffers, sizeof(a781_buffers)) == 0);
    A781_CHECK(memcmp(a781_tables, a781_before_tables, sizeof(a781_tables)) == 0);
    A781_CHECK(memcmp(a781_tokens, a781_expected_tokens, sizeof(a781_tokens)) == 0);
}
static int op_test_audio_play(void)
{
    int gi, ri, a, p, result, expected_calls;
    static const unsigned int gates[4] = {0, 1, 0x80000000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    for (a781_selected = 0; a781_selected < 2; ++a781_selected)
        for (a781_mutate = 0; a781_mutate < 2; ++a781_mutate)
            for (gi = 0; gi < 4; ++gi)
                for (ri = 0; ri < 5; ++ri)
                    for (a = 0; a < 7; ++a)
                        for (p = 0; p < 1; ++p)
                        {
                            a781_initial_gate = gates[gi];
                            a781_return_code = returns[ri];
                            expected_calls = 1;
                            a781_arg_word = values[a] != 0;
                            a781_arg_data = 0;
                            a781_seed(expected_calls);
                            a781_wanted_this = &a781_buffers[a781_selected].value;
                            result = op_audio_play(a781_wanted_this, values[a]);
                            A781_CHECK(result == (a781_return_code == 0));
                            a781_verify(expected_calls);
                        }
    printf("audio_play: %d checks, %d failures\n", a781_checks, a781_failures);
    return a781_failures != 0;
}

#undef A781_CHECK

#include "../src/audio_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int a782_checks, a782_failures, a782_calls, a782_selected, a782_mutate, a782_return_code;
static unsigned int a782_initial_gate, a782_expected_gate, a782_arg_word;
static void *a782_arg_data;
static OP_AUDIO_BUFFER *a782_wanted_this;
static void a782_check(int ok, int line)
{
    ++a782_checks;
    if (!ok)
    {
        ++a782_failures;
        printf("line%d failed\n", line);
    }
}
#define A782_CHECK(x) a782_check(!!(x), __LINE__)
typedef struct A782_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} A782_OWNED_BUFFER;
static A782_OWNED_BUFFER a782_buffers[2], a782_before_buffers[2], a782_expected_buffers[2];
static OP_AUDIO_BUFFER_VT a782_tables[2], a782_before_tables[2];
static unsigned int a782_tokens[3], a782_expected_tokens[3];
static void a782_observe(OP_AUDIO_BUFFER *object, int which)
{
    A782_CHECK(a782_calls == 0);
    A782_CHECK(which == a782_selected);
    A782_CHECK(object == a782_wanted_this);
    A782_CHECK(op_audio_gate_word == a782_initial_gate);
    A782_CHECK(memcmp(a782_buffers, a782_before_buffers, sizeof(a782_buffers)) == 0);
    A782_CHECK(memcmp(a782_tables, a782_before_tables, sizeof(a782_tables)) == 0);
    A782_CHECK(memcmp(a782_tokens, a782_expected_tokens, sizeof(a782_tokens)) == 0);
    if (a782_calls || object != a782_wanted_this)
        exit(2);
    ++a782_calls;
    if (a782_mutate)
    {
        object->vt = &a782_tables[1 - a782_selected];
        a782_buffers[a782_selected].payload[1] = 0xabcdef01u;
        op_audio_gate_word = 0x100u;
    }
}
static int __stdcall a782_callback0(OP_AUDIO_BUFFER *object, void *data, unsigned int bytes, void *second,
                                    unsigned int second_bytes)
{
    A782_CHECK(data == a782_arg_data);
    A782_CHECK(bytes == a782_arg_word);
    A782_CHECK(second == 0);
    A782_CHECK(second_bytes == 0);
    a782_observe(object, 0);
    return a782_return_code;
}
static int __stdcall a782_callback1(OP_AUDIO_BUFFER *object, void *data, unsigned int bytes, void *second,
                                    unsigned int second_bytes)
{
    A782_CHECK(data == a782_arg_data);
    A782_CHECK(bytes == a782_arg_word);
    A782_CHECK(second == 0);
    A782_CHECK(second_bytes == 0);
    a782_observe(object, 1);
    return a782_return_code;
}
static void a782_seed(int expected_calls)
{
    int i;
    memset(a782_buffers, 0x59, sizeof(a782_buffers));
    memset(a782_tables, 0x6a, sizeof(a782_tables));
    a782_tables[0].unlock = a782_callback0;
    a782_tables[1].unlock = a782_callback1;
    for (i = 0; i < 2; ++i)
        a782_buffers[i].value.vt = &a782_tables[i];
    for (i = 0; i < 3; ++i)
        a782_tokens[i] = 0x78123456u + (unsigned int)i;
    memcpy(a782_before_buffers, a782_buffers, sizeof(a782_buffers));
    memcpy(a782_expected_buffers, a782_buffers, sizeof(a782_buffers));
    memcpy(a782_before_tables, a782_tables, sizeof(a782_tables));
    memcpy(a782_expected_tokens, a782_tokens, sizeof(a782_tokens));
    op_audio_gate_word = a782_initial_gate;
    a782_expected_gate = a782_initial_gate;
    a782_calls = 0;
    if (expected_calls && a782_mutate)
    {
        a782_expected_buffers[a782_selected].value.vt = &a782_tables[1 - a782_selected];
        a782_expected_buffers[a782_selected].payload[1] = 0xabcdef01u;
        a782_expected_gate = 0x100u;
    }
}
static void a782_verify(int expected_calls)
{
    A782_CHECK(a782_calls == expected_calls);
    A782_CHECK(op_audio_gate_word == a782_expected_gate);
    A782_CHECK(memcmp(a782_buffers, a782_expected_buffers, sizeof(a782_buffers)) == 0);
    A782_CHECK(memcmp(a782_tables, a782_before_tables, sizeof(a782_tables)) == 0);
    A782_CHECK(memcmp(a782_tokens, a782_expected_tokens, sizeof(a782_tokens)) == 0);
}
static int op_test_audio_unlock(void)
{
    int gi, ri, a, p, result, expected_calls;
    static const unsigned int gates[4] = {0, 1, 0x80000000u, 0xffffffffu};
    static const int returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
    static const unsigned int values[8] = {0, 1, 2, 4, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    for (a782_selected = 0; a782_selected < 2; ++a782_selected)
        for (a782_mutate = 0; a782_mutate < 2; ++a782_mutate)
            for (gi = 0; gi < 4; ++gi)
                for (ri = 0; ri < 5; ++ri)
                    for (a = 0; a < 8; ++a)
                        for (p = 0; p < 3; ++p)
                        {
                            a782_initial_gate = gates[gi];
                            a782_return_code = returns[ri];
                            expected_calls = 1;
                            a782_arg_word = values[a];
                            a782_arg_data = p ? (void *)&a782_tokens[p - 1] : 0;
                            a782_seed(expected_calls);
                            a782_wanted_this = &a782_buffers[a782_selected].value;
                            result = op_audio_unlock(a782_wanted_this, a782_arg_data, a782_arg_word);
                            A782_CHECK(result == (a782_return_code == 0));
                            a782_verify(expected_calls);
                        }
    printf("audio_unlock: %d checks, %d failures\n", a782_checks, a782_failures);
    return a782_failures != 0;
}

#undef A782_CHECK
