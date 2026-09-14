#include "api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int checks, failures;
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
unsigned int op_audio_gate_word;
OP_AUDIO_LISTENER *op_audio_listener;
typedef struct OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} OWNED_DEVICE;
typedef struct OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} OWNED_BUFFER;
typedef struct OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} OWNED_SPATIAL;
typedef struct OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} OWNED_LISTENER;
typedef struct OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} OWNED_VECTOR;
static OWNED_DEVICE devices[2], expected_devices[2];
static OWNED_BUFFER buffers[2], expected_buffers[2];
static OWNED_SPATIAL spatials[2], expected_spatials[2];
static OWNED_LISTENER listeners[2], expected_listeners[2];
static OWNED_VECTOR vector, expected_vector;
static OP_AUDIO_DEVICE_VT dvt[2], expected_dvt[2];
static OP_AUDIO_BUFFER_VT bvt[2], expected_bvt[2];
static OP_AUDIO_SPATIAL_VT svt[2], expected_svt[2];
static OP_AUDIO_LISTENER_VT lvt[2], expected_lvt[2];
static unsigned int selected, word, minimum_bits, maximum_bits, captured[3], result_word, expected_gate;
static OP_AUDIO_LISTENER *expected_listener;
static void *expected_this;
static int calls, expected_count, expected_codes[2], mutate;
static unsigned int bits(float value)
{
    unsigned int word;
    memcpy(&word, &value, 4);
    return word;
}
static float number(unsigned int word)
{
    float value;
    memcpy(&value, &word, 4);
    return value;
}
static void verify(void)
{
    CHECK(memcmp(devices, expected_devices, sizeof(devices)) == 0);
    CHECK(memcmp(buffers, expected_buffers, sizeof(buffers)) == 0);
    CHECK(memcmp(spatials, expected_spatials, sizeof(spatials)) == 0);
    CHECK(memcmp(listeners, expected_listeners, sizeof(listeners)) == 0);
    CHECK(memcmp(&vector, &expected_vector, sizeof(vector)) == 0);
    CHECK(memcmp(dvt, expected_dvt, sizeof(dvt)) == 0);
    CHECK(memcmp(bvt, expected_bvt, sizeof(bvt)) == 0);
    CHECK(memcmp(svt, expected_svt, sizeof(svt)) == 0);
    CHECK(memcmp(lvt, expected_lvt, sizeof(lvt)) == 0);
    CHECK(op_audio_gate_word == expected_gate);
    CHECK(op_audio_listener == expected_listener);
}
static void observe(int code, unsigned int table, void *object)
{
    CHECK(calls < expected_count && calls < 2);
    if (calls >= expected_count || calls >= 2)
        exit(2);
    CHECK(code == expected_codes[calls]);
    CHECK(table == (calls && mutate ? 1 - selected : selected));
    CHECK(object == expected_this);
    verify();
    ++calls;
}
static void change(void)
{
    if (mutate && calls == 1)
    {
        op_audio_gate_word = expected_gate = 0;
        devices[selected].value.vt = expected_devices[selected].value.vt = &dvt[1 - selected];
        buffers[selected].value.vt = expected_buffers[selected].value.vt = &bvt[1 - selected];
        spatials[selected].value.vt = expected_spatials[selected].value.vt = &svt[1 - selected];
        listeners[selected].value.vt = expected_listeners[selected].value.vt = &lvt[1 - selected];
        op_audio_listener = expected_listener = &listeners[1 - selected].value;
        vector.value.x = expected_vector.value.x = 7;
        vector.value.y = expected_vector.value.y = 9;
        vector.value.z = expected_vector.value.z = 11;
    }
}
static int __stdcall coinit0(void *reserved)
{
    observe(1, 0, 0);
    CHECK(reserved == 0);
    change();
    return (int)result_word;
}
static void __stdcall couninit0(void)
{
    observe(2, 0, 0);
    change();
}
static int __stdcall coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    observe(3, 0, o);
    CHECK(window == word && flags == 2);
    change();
    return (int)result_word;
}
static int __stdcall frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    observe(4, 0, o);
    CHECK(frequency == word);
    change();
    return (int)result_word;
}
static int __stdcall maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    observe(5, 0, o);
    CHECK(bits(value) == maximum_bits && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    observe(6, 0, o);
    CHECK(bits(value) == minimum_bits && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    observe(7, 0, o);
    CHECK(bits(x) == captured[0] && bits(y) == captured[1] && bits(z) == captured[2] && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    observe(8, 0, o);
    CHECK(bits(x) == captured[0] && bits(y) == captured[1] && bits(z) == captured[2] && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall commit0(OP_AUDIO_LISTENER *o)
{
    observe(9, 0, o);
    change();
    return (int)result_word;
}
static int __stdcall mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    observe(10, 0, o);
    CHECK(mode == word && flags == 0);
    change();
    return (int)result_word;
}
static int __stdcall stop0(OP_AUDIO_BUFFER *o)
{
    observe(11, 0, o);
    change();
    return (int)result_word;
}
static unsigned int __stdcall release_buffer0(OP_AUDIO_BUFFER *o)
{
    observe(12, 0, o);
    change();
    return (unsigned int)result_word;
}
static unsigned int __stdcall release_spatial0(OP_AUDIO_SPATIAL *o)
{
    observe(13, 0, o);
    change();
    return (unsigned int)result_word;
}
static int __stdcall current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    observe(14, 0, o);
    CHECK(position == 0);
    change();
    return (int)result_word;
}
static int __stdcall coinit1(void *reserved)
{
    observe(1, 1, 0);
    CHECK(reserved == 0);
    change();
    return (int)result_word;
}
static void __stdcall couninit1(void)
{
    observe(2, 1, 0);
    change();
}
static int __stdcall coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    observe(3, 1, o);
    CHECK(window == word && flags == 2);
    change();
    return (int)result_word;
}
static int __stdcall frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    observe(4, 1, o);
    CHECK(frequency == word);
    change();
    return (int)result_word;
}
static int __stdcall maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    observe(5, 1, o);
    CHECK(bits(value) == maximum_bits && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    observe(6, 1, o);
    CHECK(bits(value) == minimum_bits && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    observe(7, 1, o);
    CHECK(bits(x) == captured[0] && bits(y) == captured[1] && bits(z) == captured[2] && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    observe(8, 1, o);
    CHECK(bits(x) == captured[0] && bits(y) == captured[1] && bits(z) == captured[2] && flags == 1);
    change();
    return (int)result_word;
}
static int __stdcall commit1(OP_AUDIO_LISTENER *o)
{
    observe(9, 1, o);
    change();
    return (int)result_word;
}
static int __stdcall mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    observe(10, 1, o);
    CHECK(mode == word && flags == 0);
    change();
    return (int)result_word;
}
static int __stdcall stop1(OP_AUDIO_BUFFER *o)
{
    observe(11, 1, o);
    change();
    return (int)result_word;
}
static unsigned int __stdcall release_buffer1(OP_AUDIO_BUFFER *o)
{
    observe(12, 1, o);
    change();
    return (unsigned int)result_word;
}
static unsigned int __stdcall release_spatial1(OP_AUDIO_SPATIAL *o)
{
    observe(13, 1, o);
    change();
    return (unsigned int)result_word;
}
static int __stdcall current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    observe(14, 1, o);
    CHECK(position == 0);
    change();
    return (int)result_word;
}
int(__stdcall *op_audio_co_initialize)(void *);
void(__stdcall *op_audio_co_uninitialize)(void);
static void seed(void)
{
    unsigned int i;
    memset(devices, 0x57, sizeof(devices));
    memset(buffers, 0x68, sizeof(buffers));
    memset(spatials, 0x79, sizeof(spatials));
    memset(listeners, 0x2a, sizeof(listeners));
    memset(&vector, 0x3b, sizeof(vector));
    memset(dvt, 0x4c, sizeof(dvt));
    memset(bvt, 0x5d, sizeof(bvt));
    memset(svt, 0x6e, sizeof(svt));
    memset(lvt, 0x7f, sizeof(lvt));
    dvt[0].cooperative = coop0;
    bvt[0].release = release_buffer0;
    bvt[0].current_position = current0;
    bvt[0].frequency = frequency0;
    bvt[0].stop = stop0;
    svt[0].maximum = maximum0;
    svt[0].minimum = minimum0;
    svt[0].position = position0;
    svt[0].velocity = velocity0;
    svt[0].mode = mode0;
    svt[0].release = release_spatial0;
    lvt[0].commit = commit0;
    dvt[1].cooperative = coop1;
    bvt[1].release = release_buffer1;
    bvt[1].current_position = current1;
    bvt[1].frequency = frequency1;
    bvt[1].stop = stop1;
    svt[1].maximum = maximum1;
    svt[1].minimum = minimum1;
    svt[1].position = position1;
    svt[1].velocity = velocity1;
    svt[1].mode = mode1;
    svt[1].release = release_spatial1;
    lvt[1].commit = commit1;
    for (i = 0; i < 2; ++i)
    {
        devices[i].value.vt = &dvt[i];
        buffers[i].value.vt = &bvt[i];
        spatials[i].value.vt = &svt[i];
        listeners[i].value.vt = &lvt[i];
    }
    vector.value.x = number(captured[0]);
    vector.value.y = number(captured[1]);
    vector.value.z = number(captured[2]);
    memcpy(expected_devices, devices, sizeof(devices));
    memcpy(expected_buffers, buffers, sizeof(buffers));
    memcpy(expected_spatials, spatials, sizeof(spatials));
    memcpy(expected_listeners, listeners, sizeof(listeners));
    memcpy(&expected_vector, &vector, sizeof(vector));
    memcpy(expected_dvt, dvt, sizeof(dvt));
    memcpy(expected_bvt, bvt, sizeof(bvt));
    memcpy(expected_svt, svt, sizeof(svt));
    memcpy(expected_lvt, lvt, sizeof(lvt));
    expected_gate = op_audio_gate_word;
    expected_listener = op_audio_listener;
    op_audio_co_initialize = selected ? coinit1 : coinit0;
    op_audio_co_uninitialize = selected ? couninit1 : couninit0;
    calls = 0;
}
int main(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (selected = 0; selected < 2; ++selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (mutate = 0; mutate < 2; ++mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &listeners[selected].value;
                            word = results[pattern % 4];
                            result_word = results[ri];
                            minimum_bits = values[pattern];
                            maximum_bits = values[(pattern + 3) % 8];
                            captured[0] = values[pattern];
                            captured[1] = values[(pattern + 1) % 8];
                            captured[2] = values[(pattern + 2) % 8];
                            seed();
                            expected_this = &devices[selected].value;
                            expected_count = 1;
                            expected_codes[0] = 3;
                            expected_codes[1] = 0;
                            op_audio_set_cooperative(&devices[selected].value, word);
                            CHECK(calls == expected_count);
                            verify();
                            CHECK(op_audio_co_initialize == (selected ? coinit1 : coinit0));
                            CHECK(op_audio_co_uninitialize == (selected ? couninit1 : couninit0));
                        }
    printf("audio_set_cooperative: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
