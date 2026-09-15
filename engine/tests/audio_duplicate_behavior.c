#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
static int t951_checks, t951_failures;
static void t951_check(int ok, int line)
{
    ++t951_checks;
    if (!ok)
    {
        ++t951_failures;
        printf("line%d failed\n", line);
    }
}
#define T951_CHECK(x) t951_check(!!(x), __LINE__)
typedef struct T951_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[2], after;
} T951_DEVICE;
typedef struct T951_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[2], after;
} T951_BUFFER;
typedef struct T951_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[2], after;
} T951_SPATIAL;
typedef struct T951_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[2], after;
} T951_LISTENER;
typedef struct T951_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} T951_VECTOR;
static T951_DEVICE t951_devices[2], t951_expected_devices[2];
static T951_BUFFER t951_buffers[3], t951_expected_buffers[3];
static T951_SPATIAL t951_spatials[2], t951_expected_spatials[2];
static T951_LISTENER t951_listeners[3], t951_expected_listeners[3];
static T951_VECTOR t951_vectors[3], t951_expected_vectors[3];
static OP_AUDIO_DEVICE_VT t951_dvt[2], t951_expected_dvt[2];
static OP_AUDIO_BUFFER_VT t951_bvt[3], t951_expected_bvt[3];
static OP_AUDIO_SPATIAL_VT t951_svt[2], t951_expected_svt[2];
static OP_AUDIO_LISTENER_VT t951_lvt[3], t951_expected_lvt[3];
static int t951_volume_values[2] = {-10000, -1};
static const unsigned int t951_words[8] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                           0x3eaaaaabu, 0x40000000u, 0x00800000u, 0xc2800000u};
static unsigned int t951_wanted_gate, t951_pattern, t951_selected;
static OP_AUDIO_LISTENER *t951_wanted_listener;
static OP_AUDIO_BUFFER *t951_wanted_primary, *t951_source_buffer;
static int t951_wanted_volume;
static int t951_phase_index, t951_phase_count, t951_phases[3], t951_mutation, t951_return_code, t951_write_output,
    t951_null_output, t951_operation;
static void t951_verify(void)
{
    T951_CHECK(!memcmp(t951_devices, t951_expected_devices, sizeof(t951_devices)));
    T951_CHECK(!memcmp(t951_buffers, t951_expected_buffers, sizeof(t951_buffers)));
    T951_CHECK(!memcmp(t951_spatials, t951_expected_spatials, sizeof(t951_spatials)));
    T951_CHECK(!memcmp(t951_listeners, t951_expected_listeners, sizeof(t951_listeners)));
    T951_CHECK(!memcmp(t951_vectors, t951_expected_vectors, sizeof(t951_vectors)));
    T951_CHECK(!memcmp(t951_dvt, t951_expected_dvt, sizeof(t951_dvt)));
    T951_CHECK(!memcmp(t951_bvt, t951_expected_bvt, sizeof(t951_bvt)));
    T951_CHECK(!memcmp(t951_svt, t951_expected_svt, sizeof(t951_svt)));
    T951_CHECK(!memcmp(t951_lvt, t951_expected_lvt, sizeof(t951_lvt)));
    T951_CHECK(op_audio_gate_word == t951_wanted_gate && op_audio_primary == t951_wanted_primary &&
               op_audio_primary_volume == t951_wanted_volume && op_audio_listener == t951_wanted_listener);
    T951_CHECK(t951_volume_values[0] == -10000 && t951_volume_values[1] == -1);
}
static void t951_phase(int code)
{
    T951_CHECK(t951_phase_index < t951_phase_count);
    if (t951_phase_index >= t951_phase_count)
        exit(2);
    T951_CHECK(t951_phases[t951_phase_index] == code);
    t951_verify();
    ++t951_phase_index;
}
static unsigned int t951_bits(float v)
{
    unsigned int u;
    memcpy(&u, &v, 4);
    return u;
}
static void t951_vector_set(unsigned int at, unsigned int shift)
{
    unsigned int i;
    for (i = 0; i < 3; ++i)
    {
        unsigned int u = t951_words[(t951_pattern + shift + i) % 8];
        memcpy((unsigned char *)&t951_vectors[at].value + i * 4, &u, 4);
        memcpy((unsigned char *)&t951_expected_vectors[at].value + i * 4, &u, 4);
    }
}
static int __stdcall t951_query(OP_AUDIO_BUFFER *o, const void *iid, OP_AUDIO_SPATIAL **out)
{
    t951_phase(10);
    T951_CHECK(o == &t951_buffers[t951_selected].value && iid == op_audio_spatial_iid && out != NULL);
    if (!out)
        exit(2);
    if (t951_write_output)
        *out = t951_null_output ? NULL : &t951_spatials[t951_selected].value;
    if (t951_mutation)
    {
        t951_buffers[t951_selected].payload[0] = t951_expected_buffers[t951_selected].payload[0] = 0x731u;
        op_audio_gate_word = t951_wanted_gate = (unsigned int)&t951_devices[1].value;
    }
    t951_verify();
    return t951_return_code;
}
static int __stdcall t951_duplicate(OP_AUDIO_DEVICE *o, OP_AUDIO_BUFFER *source, OP_AUDIO_BUFFER **out)
{
    t951_phase(11);
    T951_CHECK((unsigned int)o == t951_wanted_gate && source == t951_source_buffer && out != NULL);
    if (!out)
        exit(2);
    if (t951_write_output)
        *out = t951_null_output ? NULL : &t951_buffers[2].value;
    if (t951_mutation)
    {
        t951_devices[t951_selected].payload[1] = t951_expected_devices[t951_selected].payload[1] = 0x732u;
        op_audio_gate_word = t951_wanted_gate = (unsigned int)&t951_devices[1 - t951_selected].value;
    }
    t951_verify();
    return t951_return_code;
}
static int __stdcall t951_volume(OP_AUDIO_BUFFER *o, int level)
{
    t951_phase(20);
    T951_CHECK(o == t951_wanted_primary && level == t951_wanted_volume);
    if (t951_mutation)
    {
        op_audio_primary = t951_wanted_primary = &t951_buffers[1].value;
        op_audio_gate_word = t951_wanted_gate = (unsigned int)&t951_devices[1].value;
        op_audio_primary_volume = t951_wanted_volume = t951_volume_values[1];
        t951_buffers[0].payload[0] = t951_expected_buffers[0].payload[0] = 0x733u;
    }
    t951_verify();
    return t951_return_code;
}
static unsigned int __stdcall t951_release_buffer(OP_AUDIO_BUFFER *o)
{
    t951_phase(21);
    T951_CHECK(o == t951_wanted_primary);
    if (t951_mutation)
    {
        op_audio_primary = t951_wanted_primary = &t951_buffers[2].value;
        op_audio_gate_word = t951_wanted_gate = (unsigned int)&t951_devices[0].value;
        t951_buffers[2].payload[1] = t951_expected_buffers[2].payload[1] = 0x734u;
    }
    t951_verify();
    t951_wanted_primary = NULL;
    return (unsigned int)t951_return_code;
}
static unsigned int __stdcall t951_release_device(OP_AUDIO_DEVICE *o)
{
    t951_phase(22);
    T951_CHECK((unsigned int)o == t951_wanted_gate);
    if (t951_mutation)
    {
        op_audio_gate_word = t951_wanted_gate = (unsigned int)&t951_devices[1].value;
        op_audio_listener = t951_wanted_listener = &t951_listeners[2].value;
        t951_devices[0].payload[0] = t951_expected_devices[0].payload[0] = 0x735u;
    }
    t951_verify();
    t951_wanted_gate = 0;
    return (unsigned int)t951_return_code;
}
static int __stdcall t951_position(OP_AUDIO_LISTENER *o, float x, float y, float z, unsigned int deferred)
{
    t951_phase(30);
    T951_CHECK(o == t951_wanted_listener && deferred == 1);
    T951_CHECK(t951_bits(x) == t951_words[t951_pattern % 8] && t951_bits(y) == t951_words[(t951_pattern + 1) % 8] &&
               t951_bits(z) == t951_words[(t951_pattern + 2) % 8]);
    if (t951_mutation)
    {
        op_audio_listener = t951_wanted_listener = &t951_listeners[1].value;
        t951_vector_set(1, 4);
        t951_vector_set(2, 5);
        t951_listeners[0].payload[0] = t951_expected_listeners[0].payload[0] = 0x736u;
    }
    t951_verify();
    return t951_return_code;
}
static int __stdcall t951_orientation(OP_AUDIO_LISTENER *o, float x, float y, float z, float ux, float uy, float uz,
                                      unsigned int deferred)
{
    unsigned int front = t951_mutation ? 4u : 1u, up = t951_mutation ? 5u : 2u;
    t951_phase(31);
    T951_CHECK(o == t951_wanted_listener && deferred == 1);
    T951_CHECK(t951_bits(x) == t951_words[(t951_pattern + front) % 8] &&
               t951_bits(y) == t951_words[(t951_pattern + front + 1) % 8] &&
               t951_bits(z) == t951_words[(t951_pattern + front + 2) % 8]);
    T951_CHECK(t951_bits(ux) == t951_words[(t951_pattern + up) % 8] &&
               t951_bits(uy) == t951_words[(t951_pattern + up + 1) % 8] &&
               t951_bits(uz) == t951_words[(t951_pattern + up + 2) % 8]);
    if (t951_mutation)
    {
        op_audio_listener = t951_wanted_listener = &t951_listeners[2].value;
        t951_listeners[1].payload[1] = t951_expected_listeners[1].payload[1] = 0x737u;
        t951_vector_set(0, 6);
    }
    t951_verify();
    return t951_return_code;
}
static int __stdcall t951_commit(OP_AUDIO_LISTENER *o)
{
    t951_phase(32);
    T951_CHECK(o == t951_wanted_listener);
    if (t951_mutation)
    {
        op_audio_listener = t951_wanted_listener = NULL;
        t951_listeners[2].payload[0] = t951_expected_listeners[2].payload[0] = 0x738u;
    }
    t951_verify();
    return t951_return_code;
}
static void t951_setup(void)
{
    unsigned int i;
    memset(t951_devices, 0, sizeof(t951_devices));
    memset(t951_buffers, 0, sizeof(t951_buffers));
    memset(t951_spatials, 0, sizeof(t951_spatials));
    memset(t951_listeners, 0, sizeof(t951_listeners));
    memset(t951_vectors, 0, sizeof(t951_vectors));
    memset(t951_dvt, 0, sizeof(t951_dvt));
    memset(t951_bvt, 0, sizeof(t951_bvt));
    memset(t951_svt, 0, sizeof(t951_svt));
    memset(t951_lvt, 0, sizeof(t951_lvt));
    for (i = 0; i < 2; ++i)
    {
        t951_devices[i].before = 0x1234u + t951_pattern + i;
        t951_devices[i].after = 0x8765u + i;
        t951_devices[i].value.vt = &t951_dvt[i];
        t951_dvt[i].release = t951_release_device;
        t951_dvt[i].duplicate = t951_duplicate;
        t951_spatials[i].before = 0x4321u + i;
        t951_spatials[i].after = 0x9876u + t951_pattern;
        t951_spatials[i].value.vt = &t951_svt[i];
    }
    for (i = 0; i < 3; ++i)
    {
        t951_buffers[i].before = 0x3210u + t951_pattern + i;
        t951_buffers[i].after = 0x7654u + i;
        t951_buffers[i].value.vt = &t951_bvt[i];
        t951_bvt[i].query = t951_query;
        t951_bvt[i].release = t951_release_buffer;
        t951_bvt[i].volume = t951_volume;
        t951_listeners[i].before = 0x7410u + i;
        t951_listeners[i].after = 0x1470u + t951_pattern;
        t951_listeners[i].value.vt = &t951_lvt[i];
        t951_lvt[i].position = t951_position;
        t951_lvt[i].orientation = t951_orientation;
        t951_lvt[i].commit = t951_commit;
        t951_vectors[i].before = 0xabc0u + i;
        t951_vectors[i].after = 0xdef0u + t951_pattern;
        t951_vector_set(i, i);
    }
    memcpy(t951_expected_devices, t951_devices, sizeof(t951_devices));
    memcpy(t951_expected_buffers, t951_buffers, sizeof(t951_buffers));
    memcpy(t951_expected_spatials, t951_spatials, sizeof(t951_spatials));
    memcpy(t951_expected_listeners, t951_listeners, sizeof(t951_listeners));
    memcpy(t951_expected_vectors, t951_vectors, sizeof(t951_vectors));
    memcpy(t951_expected_dvt, t951_dvt, sizeof(t951_dvt));
    memcpy(t951_expected_bvt, t951_bvt, sizeof(t951_bvt));
    memcpy(t951_expected_svt, t951_svt, sizeof(t951_svt));
    memcpy(t951_expected_lvt, t951_lvt, sizeof(t951_lvt));
    op_audio_gate_word = t951_wanted_gate = (unsigned int)&t951_devices[t951_selected].value;
    op_audio_primary = t951_wanted_primary = &t951_buffers[0].value;
    op_audio_primary_volume = t951_wanted_volume = t951_volume_values[0];
    op_audio_listener = t951_wanted_listener = &t951_listeners[0].value;
    t951_phase_index = t951_phase_count = 0;
    t951_source_buffer = &t951_buffers[t951_selected].value;
}
static int op_test_audio_duplicate(void)
{
    unsigned int saved_gate = op_audio_gate_word;
    OP_AUDIO_LISTENER *saved_listener = op_audio_listener;
    OP_AUDIO_BUFFER *saved_primary = op_audio_primary;
    int saved_volume = op_audio_primary_volume;

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
                                OP_AUDIO_BUFFER *result, *expected;
                                t951_pattern = p;
                                t951_selected = s;
                                t951_mutation = (int)m;
                                t951_return_code = returns[r];
                                t951_null_output = (int)n;
                                t951_operation = 1;
                                t951_setup();
                                t951_write_output = (t951_return_code == 0) || w;
                                if (!g)
                                    op_audio_gate_word = t951_wanted_gate = 0;
                                if (n)
                                    t951_source_buffer = NULL;
                                t951_phase_count = g ? 1 : 0;
                                t951_phases[0] = 11;
                                expected =
                                    g && t951_return_code == 0 && !t951_null_output ? &t951_buffers[2].value : NULL;
                                result = op_audio_duplicate(t951_source_buffer);
                                T951_CHECK(result == expected);
                                T951_CHECK(t951_phase_index == t951_phase_count);
                                t951_verify();
                            }
    op_audio_gate_word = saved_gate;
    op_audio_listener = saved_listener;
    op_audio_primary = saved_primary;
    op_audio_primary_volume = saved_volume;
    printf("audio_duplicate: %d checks, %d failures\n", t951_checks, t951_failures);
    return t951_failures != 0;
}

#undef T951_CHECK
