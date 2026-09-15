#include "api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
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
OP_AUDIO_BUFFER *op_audio_primary;
int op_audio_primary_volume;
const unsigned char op_audio_spatial_iid[16] = {9, 6, 1, 4, 2, 3, 5, 7, 8, 0, 1, 2, 3, 4, 5, 6};
typedef struct DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[2], after;
} DEVICE;
typedef struct BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[2], after;
} BUFFER;
typedef struct SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[2], after;
} SPATIAL;
typedef struct LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[2], after;
} LISTENER;
typedef struct VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} VECTOR;
static DEVICE devices[2], expected_devices[2];
static BUFFER buffers[3], expected_buffers[3];
static SPATIAL spatials[2], expected_spatials[2];
static LISTENER listeners[3], expected_listeners[3];
static VECTOR vectors[3], expected_vectors[3];
static OP_AUDIO_DEVICE_VT dvt[2], expected_dvt[2];
static OP_AUDIO_BUFFER_VT bvt[3], expected_bvt[3];
static OP_AUDIO_SPATIAL_VT svt[2], expected_svt[2];
static OP_AUDIO_LISTENER_VT lvt[3], expected_lvt[3];
static int volume_values[2] = {-10000, -1};
static const unsigned int words[8] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                      0x3eaaaaabu, 0x40000000u, 0x00800000u, 0xc2800000u};
static unsigned int wanted_gate, pattern, selected;
static OP_AUDIO_LISTENER *wanted_listener;
static OP_AUDIO_BUFFER *wanted_primary, *source_buffer;
static int wanted_volume;
static int phase_index, phase_count, phases[3], mutation, return_code, write_output, null_output, operation;
static void verify(void)
{
    CHECK(!memcmp(devices, expected_devices, sizeof(devices)));
    CHECK(!memcmp(buffers, expected_buffers, sizeof(buffers)));
    CHECK(!memcmp(spatials, expected_spatials, sizeof(spatials)));
    CHECK(!memcmp(listeners, expected_listeners, sizeof(listeners)));
    CHECK(!memcmp(vectors, expected_vectors, sizeof(vectors)));
    CHECK(!memcmp(dvt, expected_dvt, sizeof(dvt)));
    CHECK(!memcmp(bvt, expected_bvt, sizeof(bvt)));
    CHECK(!memcmp(svt, expected_svt, sizeof(svt)));
    CHECK(!memcmp(lvt, expected_lvt, sizeof(lvt)));
    CHECK(op_audio_gate_word == wanted_gate && op_audio_primary == wanted_primary &&
          op_audio_primary_volume == wanted_volume && op_audio_listener == wanted_listener);
    CHECK(volume_values[0] == -10000 && volume_values[1] == -1);
}
static void phase(int code)
{
    CHECK(phase_index < phase_count);
    if (phase_index >= phase_count)
        exit(2);
    CHECK(phases[phase_index] == code);
    verify();
    ++phase_index;
}
static unsigned int bits(float v)
{
    unsigned int u;
    memcpy(&u, &v, 4);
    return u;
}
static void vector_set(unsigned int at, unsigned int shift)
{
    unsigned int i;
    for (i = 0; i < 3; ++i)
    {
        unsigned int u = words[(pattern + shift + i) % 8];
        memcpy((unsigned char *)&vectors[at].value + i * 4, &u, 4);
        memcpy((unsigned char *)&expected_vectors[at].value + i * 4, &u, 4);
    }
}
static int __stdcall query(OP_AUDIO_BUFFER *o, const void *iid, OP_AUDIO_SPATIAL **out)
{
    phase(10);
    CHECK(o == &buffers[selected].value && iid == op_audio_spatial_iid && out != NULL);
    if (!out)
        exit(2);
    if (write_output)
        *out = null_output ? NULL : &spatials[selected].value;
    if (mutation)
    {
        buffers[selected].payload[0] = expected_buffers[selected].payload[0] = 0x731u;
        op_audio_gate_word = wanted_gate = (unsigned int)&devices[1].value;
    }
    verify();
    return return_code;
}
static int __stdcall duplicate(OP_AUDIO_DEVICE *o, OP_AUDIO_BUFFER *source, OP_AUDIO_BUFFER **out)
{
    phase(11);
    CHECK((unsigned int)o == wanted_gate && source == source_buffer && out != NULL);
    if (!out)
        exit(2);
    if (write_output)
        *out = null_output ? NULL : &buffers[2].value;
    if (mutation)
    {
        devices[selected].payload[1] = expected_devices[selected].payload[1] = 0x732u;
        op_audio_gate_word = wanted_gate = (unsigned int)&devices[1 - selected].value;
    }
    verify();
    return return_code;
}
static int __stdcall volume(OP_AUDIO_BUFFER *o, int level)
{
    phase(20);
    CHECK(o == wanted_primary && level == wanted_volume);
    if (mutation)
    {
        op_audio_primary = wanted_primary = &buffers[1].value;
        op_audio_gate_word = wanted_gate = (unsigned int)&devices[1].value;
        op_audio_primary_volume = wanted_volume = volume_values[1];
        buffers[0].payload[0] = expected_buffers[0].payload[0] = 0x733u;
    }
    verify();
    return return_code;
}
static unsigned int __stdcall release_buffer(OP_AUDIO_BUFFER *o)
{
    phase(21);
    CHECK(o == wanted_primary);
    if (mutation)
    {
        op_audio_primary = wanted_primary = &buffers[2].value;
        op_audio_gate_word = wanted_gate = (unsigned int)&devices[0].value;
        buffers[2].payload[1] = expected_buffers[2].payload[1] = 0x734u;
    }
    verify();
    wanted_primary = NULL;
    return (unsigned int)return_code;
}
static unsigned int __stdcall release_device(OP_AUDIO_DEVICE *o)
{
    phase(22);
    CHECK((unsigned int)o == wanted_gate);
    if (mutation)
    {
        op_audio_gate_word = wanted_gate = (unsigned int)&devices[1].value;
        op_audio_listener = wanted_listener = &listeners[2].value;
        devices[0].payload[0] = expected_devices[0].payload[0] = 0x735u;
    }
    verify();
    wanted_gate = 0;
    return (unsigned int)return_code;
}
static int __stdcall position(OP_AUDIO_LISTENER *o, float x, float y, float z, unsigned int deferred)
{
    phase(30);
    CHECK(o == wanted_listener && deferred == 1);
    CHECK(bits(x) == words[pattern % 8] && bits(y) == words[(pattern + 1) % 8] && bits(z) == words[(pattern + 2) % 8]);
    if (mutation)
    {
        op_audio_listener = wanted_listener = &listeners[1].value;
        vector_set(1, 4);
        vector_set(2, 5);
        listeners[0].payload[0] = expected_listeners[0].payload[0] = 0x736u;
    }
    verify();
    return return_code;
}
static int __stdcall orientation(OP_AUDIO_LISTENER *o, float x, float y, float z, float ux, float uy, float uz,
                                 unsigned int deferred)
{
    unsigned int front = mutation ? 4u : 1u, up = mutation ? 5u : 2u;
    phase(31);
    CHECK(o == wanted_listener && deferred == 1);
    CHECK(bits(x) == words[(pattern + front) % 8] && bits(y) == words[(pattern + front + 1) % 8] &&
          bits(z) == words[(pattern + front + 2) % 8]);
    CHECK(bits(ux) == words[(pattern + up) % 8] && bits(uy) == words[(pattern + up + 1) % 8] &&
          bits(uz) == words[(pattern + up + 2) % 8]);
    if (mutation)
    {
        op_audio_listener = wanted_listener = &listeners[2].value;
        listeners[1].payload[1] = expected_listeners[1].payload[1] = 0x737u;
        vector_set(0, 6);
    }
    verify();
    return return_code;
}
static int __stdcall commit(OP_AUDIO_LISTENER *o)
{
    phase(32);
    CHECK(o == wanted_listener);
    if (mutation)
    {
        op_audio_listener = wanted_listener = NULL;
        listeners[2].payload[0] = expected_listeners[2].payload[0] = 0x738u;
    }
    verify();
    return return_code;
}
static void setup(void)
{
    unsigned int i;
    memset(devices, 0, sizeof(devices));
    memset(buffers, 0, sizeof(buffers));
    memset(spatials, 0, sizeof(spatials));
    memset(listeners, 0, sizeof(listeners));
    memset(vectors, 0, sizeof(vectors));
    memset(dvt, 0, sizeof(dvt));
    memset(bvt, 0, sizeof(bvt));
    memset(svt, 0, sizeof(svt));
    memset(lvt, 0, sizeof(lvt));
    for (i = 0; i < 2; ++i)
    {
        devices[i].before = 0x1234u + pattern + i;
        devices[i].after = 0x8765u + i;
        devices[i].value.vt = &dvt[i];
        dvt[i].release = release_device;
        dvt[i].duplicate = duplicate;
        spatials[i].before = 0x4321u + i;
        spatials[i].after = 0x9876u + pattern;
        spatials[i].value.vt = &svt[i];
    }
    for (i = 0; i < 3; ++i)
    {
        buffers[i].before = 0x3210u + pattern + i;
        buffers[i].after = 0x7654u + i;
        buffers[i].value.vt = &bvt[i];
        bvt[i].query = query;
        bvt[i].release = release_buffer;
        bvt[i].volume = volume;
        listeners[i].before = 0x7410u + i;
        listeners[i].after = 0x1470u + pattern;
        listeners[i].value.vt = &lvt[i];
        lvt[i].position = position;
        lvt[i].orientation = orientation;
        lvt[i].commit = commit;
        vectors[i].before = 0xabc0u + i;
        vectors[i].after = 0xdef0u + pattern;
        vector_set(i, i);
    }
    memcpy(expected_devices, devices, sizeof(devices));
    memcpy(expected_buffers, buffers, sizeof(buffers));
    memcpy(expected_spatials, spatials, sizeof(spatials));
    memcpy(expected_listeners, listeners, sizeof(listeners));
    memcpy(expected_vectors, vectors, sizeof(vectors));
    memcpy(expected_dvt, dvt, sizeof(dvt));
    memcpy(expected_bvt, bvt, sizeof(bvt));
    memcpy(expected_svt, svt, sizeof(svt));
    memcpy(expected_lvt, lvt, sizeof(lvt));
    op_audio_gate_word = wanted_gate = (unsigned int)&devices[selected].value;
    op_audio_primary = wanted_primary = &buffers[0].value;
    op_audio_primary_volume = wanted_volume = volume_values[0];
    op_audio_listener = wanted_listener = &listeners[0].value;
    phase_index = phase_count = 0;
    source_buffer = &buffers[selected].value;
}
int main(void)
{
    static const int returns[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    unsigned int m, r, s, p, g, n, w;
    for (p = 0; p < 8; ++p)
        for (s = 0; s < 2; ++s)
            for (m = 0; m < 2; ++m)
                for (r = 0; r < 5; ++r)
                    for (g = 0; g < 2; ++g)
                        for (n = 0; n < 2; ++n)
                            for (w = 0; w < 2; ++w)
                            {
                                OP_AUDIO_SPATIAL *result, *expected;
                                pattern = p;
                                selected = s;
                                mutation = (int)m;
                                return_code = returns[r];
                                null_output = (int)n;
                                operation = 0;
                                setup();
                                write_output = (return_code >= 0) || w;
                                phase_count = 1;
                                phases[0] = 10;
                                if (!g)
                                    op_audio_gate_word = wanted_gate = 0;
                                expected = return_code >= 0 && !null_output ? &spatials[selected].value : NULL;
                                result = op_audio_get_spatial(&buffers[selected].value);
                                CHECK(result == expected);
                                CHECK(phase_index == phase_count);
                                verify();
                            }
    printf("audio_get_spatial: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
