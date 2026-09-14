/* Full focused matrices, with one shared MCI global set and explicit observer assignment. */
#include "../src/music_helpers.h"
unsigned int op_music_gate, op_music_device;
unsigned int(__stdcall *op_music_command)(unsigned int, unsigned int, unsigned int, void *);
#include "../src/music_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int a790_checks, a790_failures, a790_calls, a790_mutate;
static unsigned int a790_before_gate, a790_before_device, a790_status_value, a790_return_value;
static void a790_check(int ok, int line)
{
    ++a790_checks;
    if (!ok)
    {
        ++a790_failures;
        printf("line%d failed\n", line);
    }
}
#define A790_CHECK(x) a790_check(!!(x), __LINE__)
static unsigned int __stdcall a790_command(unsigned int device, unsigned int message, unsigned int flags, void *data)
{
    A790_CHECK(a790_calls == 0);
    A790_CHECK(device == a790_before_device);
    A790_CHECK(op_music_device == a790_before_device);
    A790_CHECK(op_music_gate == a790_before_gate);
    A790_CHECK(message == 0x808);
    A790_CHECK(flags == 0);
    A790_CHECK(data == 0);
    ++a790_calls;
    if (a790_mutate)
    {
        op_music_gate = 0;
        op_music_device = 0xaabbccddu;
    }
    return a790_return_value;
}
static int op_test_music_stop(void)
{
    static const unsigned int gates[4] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int devices[5] = {0, 1, 65535, 0x80000000u, 0xffffffffu};
    static const unsigned int returns[5] = {0, 1, 0x80004005u, 0xffffffffu, 0x20d};
    static const unsigned int values[6] = {0, 1, 0x20c, 0x20d, 0x20e, 0xffffffffu};
    int g, d, r, v;

    op_music_command = a790_command;
    for (g = 0; g < 4; ++g)
        for (d = 0; d < 5; ++d)
            for (r = 0; r < 5; ++r)
                for (v = 0; v < 6; ++v)
                    for (a790_mutate = 0; a790_mutate < 2; ++a790_mutate)
                    {
                        op_music_gate = a790_before_gate = gates[g];
                        op_music_device = a790_before_device = devices[d];
                        a790_calls = 0;
                        a790_return_value = returns[r];
                        a790_status_value = values[v];
                        op_music_stop();
                        A790_CHECK(a790_calls == (a790_before_gate != 0));
                        A790_CHECK(op_music_gate == (a790_before_gate && a790_mutate ? 0 : a790_before_gate));
                        A790_CHECK(op_music_device ==
                                   (a790_before_gate && a790_mutate ? 0xaabbccddu : a790_before_device));
                        A790_CHECK(op_music_command == a790_command);
                    }
    printf("music_stop: %d checks, %d failures\n", a790_checks, a790_failures);
    return a790_failures != 0;
}

#undef A790_CHECK

#include "../src/music_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int a791_checks, a791_failures, a791_calls, a791_mutate;
static unsigned int a791_before_gate, a791_before_device, a791_status_value, a791_return_value;
static void a791_check(int ok, int line)
{
    ++a791_checks;
    if (!ok)
    {
        ++a791_failures;
        printf("line%d failed\n", line);
    }
}
#define A791_CHECK(x) a791_check(!!(x), __LINE__)
static unsigned int __stdcall a791_command(unsigned int device, unsigned int message, unsigned int flags, void *data)
{
    OP_MUSIC_STATUS expected = {0, 0, 4, 0};
    A791_CHECK(a791_calls == 0);
    A791_CHECK(device == a791_before_device);
    A791_CHECK(op_music_device == a791_before_device);
    A791_CHECK(op_music_gate == a791_before_gate);
    A791_CHECK(message == 0x814);
    A791_CHECK(flags == 0x100);
    A791_CHECK(data != 0);
    A791_CHECK(data != (void *)&op_music_gate && data != (void *)&op_music_device);
    if (!data || data == (void *)&op_music_gate || data == (void *)&op_music_device)
        exit(2);
    A791_CHECK(memcmp(data, &expected, sizeof(expected)) == 0);
    ((OP_MUSIC_STATUS *)data)->value = a791_status_value;
    ((OP_MUSIC_STATUS *)data)->callback = 0x5a5a5a5au;
    ((OP_MUSIC_STATUS *)data)->item = 0x33333333u;
    ((OP_MUSIC_STATUS *)data)->track = 0x77777777u;

    ++a791_calls;
    if (a791_mutate)
    {
        op_music_gate = 0;
        op_music_device = 0xaabbccddu;
    }
    return a791_return_value;
}
static int op_test_music_is_playing(void)
{
    static const unsigned int gates[4] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int devices[5] = {0, 1, 65535, 0x80000000u, 0xffffffffu};
    static const unsigned int returns[5] = {0, 1, 0x80004005u, 0xffffffffu, 0x20d};
    static const unsigned int values[6] = {0, 1, 0x20c, 0x20d, 0x20e, 0xffffffffu};
    int g, d, r, v;
    int result;
    op_music_command = a791_command;
    for (g = 0; g < 4; ++g)
        for (d = 0; d < 5; ++d)
            for (r = 0; r < 5; ++r)
                for (v = 0; v < 6; ++v)
                    for (a791_mutate = 0; a791_mutate < 2; ++a791_mutate)
                    {
                        op_music_gate = a791_before_gate = gates[g];
                        op_music_device = a791_before_device = devices[d];
                        a791_calls = 0;
                        a791_return_value = returns[r];
                        a791_status_value = values[v];
                        result = op_music_is_playing();
                        A791_CHECK(result == (a791_before_gate && a791_status_value != 0x20d));
                        A791_CHECK(a791_calls == (a791_before_gate != 0));
                        A791_CHECK(op_music_gate == (a791_before_gate && a791_mutate ? 0 : a791_before_gate));
                        A791_CHECK(op_music_device ==
                                   (a791_before_gate && a791_mutate ? 0xaabbccddu : a791_before_device));
                        A791_CHECK(op_music_command == a791_command);
                    }
    printf("music_is_playing: %d checks, %d failures\n", a791_checks, a791_failures);
    return a791_failures != 0;
}

#undef A791_CHECK

#include "../src/audio_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int a792_checks, a792_failures, a792_calls, a792_selected, a792_mutate, a792_first_result, a792_second_result;
static unsigned int a792_status_word, a792_position_word;
static unsigned int *a792_wanted_output, *a792_status_pointer;
static unsigned int a792_output[3], a792_before_output[3], a792_expected_output[3];
static void a792_check(int ok, int line)
{
    ++a792_checks;
    if (!ok)
    {
        ++a792_failures;
        printf("line%d failed\n", line);
    }
}
#define A792_CHECK(x) a792_check(!!(x), __LINE__)
typedef struct A792_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} A792_OWNED_BUFFER;
static A792_OWNED_BUFFER a792_buffers[2], a792_before_buffers[2], a792_expected_buffers[2];
static OP_AUDIO_BUFFER_VT a792_tables[2], a792_before_tables[2];
static void a792_verify_state(int after_first)
{
    A792_CHECK(memcmp(a792_buffers, after_first ? a792_expected_buffers : a792_before_buffers, sizeof(a792_buffers)) ==
               0);
    A792_CHECK(memcmp(a792_tables, a792_before_tables, sizeof(a792_tables)) == 0);
    A792_CHECK(memcmp(a792_output, a792_before_output, sizeof(a792_output)) == 0);
}
static int a792_observe_status(OP_AUDIO_BUFFER *object, unsigned int *status, int which)
{
    A792_CHECK(a792_calls == 0);
    A792_CHECK(which == a792_selected);
    A792_CHECK(object == &a792_buffers[a792_selected].value);
    A792_CHECK(status != 0 && status != &a792_output[0] && status != &a792_output[1] && status != &a792_output[2]);
    if (a792_calls || !status || status == &a792_output[0] || status == &a792_output[1] || status == &a792_output[2])
        exit(2);
    a792_verify_state(0);
    a792_status_pointer = status;
    *status = a792_status_word;
    ++a792_calls;
    if (a792_mutate)
    {
        object->vt = &a792_tables[1 - a792_selected];
        a792_buffers[a792_selected].payload[1] = 0xabcdef01u;
    }
    return a792_first_result;
}
static int a792_observe_position(OP_AUDIO_BUFFER *object, unsigned int *play, unsigned int *write, int which)
{
    A792_CHECK(a792_calls == 1);
    A792_CHECK(which == (a792_mutate ? 1 - a792_selected : a792_selected));
    A792_CHECK(object == &a792_buffers[a792_selected].value);
    A792_CHECK(play == a792_wanted_output);
    A792_CHECK(write != 0 && write != a792_status_pointer && write != play);
    if (a792_calls != 1 || play != a792_wanted_output || !write || write == a792_status_pointer || write == play)
        exit(2);
    a792_verify_state(1);
    *play = a792_position_word;
    *write = 0x456789abu;
    ++a792_calls;
    return a792_second_result;
}
static int __stdcall a792_status0(OP_AUDIO_BUFFER *object, unsigned int *status)
{
    return a792_observe_status(object, status, 0);
}
static int __stdcall a792_status1(OP_AUDIO_BUFFER *object, unsigned int *status)
{
    return a792_observe_status(object, status, 1);
}
static int __stdcall a792_position0(OP_AUDIO_BUFFER *object, unsigned int *play, unsigned int *write)
{
    return a792_observe_position(object, play, write, 0);
}
static int __stdcall a792_position1(OP_AUDIO_BUFFER *object, unsigned int *play, unsigned int *write)
{
    return a792_observe_position(object, play, write, 1);
}
static int op_test_audio_get_play_position(void)
{
    static const int results[5] = {0, 1, -1, INT_MIN, INT_MAX};
    static const unsigned int statuses[7] = {0, 1, 2, 3, 0x80000000u, 0x80000001u, 0xffffffffu};
    int r, s, t, n, i, result, want_calls;
    for (a792_selected = 0; a792_selected < 2; ++a792_selected)
        for (a792_mutate = 0; a792_mutate < 2; ++a792_mutate)
            for (r = 0; r < 5; ++r)
                for (s = 0; s < 7; ++s)
                    for (t = 0; t < 5; ++t)
                        for (n = 0; n < 2; ++n)
                        {
                            memset(a792_buffers, 0x59, sizeof(a792_buffers));
                            memset(a792_tables, 0x6a, sizeof(a792_tables));
                            a792_tables[0].get_status = a792_status0;
                            a792_tables[1].get_status = a792_status1;
                            a792_tables[0].get_position = a792_position0;
                            a792_tables[1].get_position = a792_position1;
                            for (i = 0; i < 2; ++i)
                                a792_buffers[i].value.vt = &a792_tables[i];
                            for (i = 0; i < 3; ++i)
                                a792_output[i] = 0x12345678u + (unsigned int)i;
                            memcpy(a792_before_buffers, a792_buffers, sizeof(a792_buffers));
                            memcpy(a792_expected_buffers, a792_buffers, sizeof(a792_buffers));
                            memcpy(a792_before_tables, a792_tables, sizeof(a792_tables));
                            memcpy(a792_before_output, a792_output, sizeof(a792_output));
                            memcpy(a792_expected_output, a792_output, sizeof(a792_output));
                            if (a792_mutate)
                            {
                                a792_expected_buffers[a792_selected].value.vt = &a792_tables[1 - a792_selected];
                                a792_expected_buffers[a792_selected].payload[1] = 0xabcdef01u;
                            }
                            a792_first_result = results[r];
                            a792_second_result = results[t];
                            a792_status_word = statuses[s];
                            a792_position_word =
                                (unsigned int)r * 0x1fffffffu + (unsigned int)s * 31337u + (unsigned int)t;
                            a792_wanted_output = n ? &a792_output[1] : 0;
                            want_calls = 1 + (a792_first_result == 0 && (a792_status_word & 1) && n);
                            if (want_calls == 2)
                                a792_expected_output[1] = a792_position_word;
                            a792_calls = 0;
                            a792_status_pointer = 0;
                            result = op_audio_get_play_position(&a792_buffers[a792_selected].value, a792_wanted_output);
                            A792_CHECK(result == (a792_first_result ? -1 : (a792_status_word & 1) ? 1 : 0));
                            A792_CHECK(a792_calls == want_calls);
                            A792_CHECK(memcmp(a792_buffers, a792_expected_buffers, sizeof(a792_buffers)) == 0);
                            A792_CHECK(memcmp(a792_tables, a792_before_tables, sizeof(a792_tables)) == 0);
                            A792_CHECK(memcmp(a792_output, a792_expected_output, sizeof(a792_output)) == 0);
                        }
    printf("audio_get_play_position: %d checks, %d failures\n", a792_checks, a792_failures);
    return a792_failures != 0;
}

#undef A792_CHECK
