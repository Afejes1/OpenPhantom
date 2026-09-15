#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
static int t950_checks, t950_failures;
static void t950_check(int ok, int line)
{
    ++t950_checks;
    if (!ok)
    {
        ++t950_failures;
        printf("line%d failed\n", line);
    }
}
#define T950_CHECK(x) t950_check(!!(x), __LINE__)
OP_AUDIO_BUFFER *op_audio_primary;
int op_audio_primary_volume;
const unsigned char op_audio_spatial_iid[16] = {9, 6, 1, 4, 2, 3, 5, 7, 8, 0, 1, 2, 3, 4, 5, 6};
typedef struct T950_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[2], after;
} T950_DEVICE;
typedef struct T950_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[2], after;
} T950_BUFFER;
typedef struct T950_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[2], after;
} T950_SPATIAL;
typedef struct T950_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[2], after;
} T950_LISTENER;
typedef struct T950_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} T950_VECTOR;
static T950_DEVICE t950_devices[2], t950_expected_devices[2];
static T950_BUFFER t950_buffers[3], t950_expected_buffers[3];
static T950_SPATIAL t950_spatials[2], t950_expected_spatials[2];
static T950_LISTENER t950_listeners[3], t950_expected_listeners[3];
static T950_VECTOR t950_vectors[3], t950_expected_vectors[3];
static OP_AUDIO_DEVICE_VT t950_dvt[2], t950_expected_dvt[2];
static OP_AUDIO_BUFFER_VT t950_bvt[3], t950_expected_bvt[3];
static OP_AUDIO_SPATIAL_VT t950_svt[2], t950_expected_svt[2];
static OP_AUDIO_LISTENER_VT t950_lvt[3], t950_expected_lvt[3];
static int t950_volume_values[2] = {-10000, -1};
static const unsigned int t950_words[8] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                           0x3eaaaaabu, 0x40000000u, 0x00800000u, 0xc2800000u};
static unsigned int t950_wanted_gate, t950_pattern, t950_selected;
static OP_AUDIO_LISTENER *t950_wanted_listener;
static OP_AUDIO_BUFFER *t950_wanted_primary, *t950_source_buffer;
static int t950_wanted_volume;
static int t950_phase_index, t950_phase_count, t950_phases[3], t950_mutation, t950_return_code, t950_write_output,
    t950_null_output, t950_operation;
static void t950_verify(void)
{
    T950_CHECK(!memcmp(t950_devices, t950_expected_devices, sizeof(t950_devices)));
    T950_CHECK(!memcmp(t950_buffers, t950_expected_buffers, sizeof(t950_buffers)));
    T950_CHECK(!memcmp(t950_spatials, t950_expected_spatials, sizeof(t950_spatials)));
    T950_CHECK(!memcmp(t950_listeners, t950_expected_listeners, sizeof(t950_listeners)));
    T950_CHECK(!memcmp(t950_vectors, t950_expected_vectors, sizeof(t950_vectors)));
    T950_CHECK(!memcmp(t950_dvt, t950_expected_dvt, sizeof(t950_dvt)));
    T950_CHECK(!memcmp(t950_bvt, t950_expected_bvt, sizeof(t950_bvt)));
    T950_CHECK(!memcmp(t950_svt, t950_expected_svt, sizeof(t950_svt)));
    T950_CHECK(!memcmp(t950_lvt, t950_expected_lvt, sizeof(t950_lvt)));
    T950_CHECK(op_audio_gate_word == t950_wanted_gate && op_audio_primary == t950_wanted_primary &&
               op_audio_primary_volume == t950_wanted_volume && op_audio_listener == t950_wanted_listener);
    T950_CHECK(t950_volume_values[0] == -10000 && t950_volume_values[1] == -1);
}
static void t950_phase(int code)
{
    T950_CHECK(t950_phase_index < t950_phase_count);
    if (t950_phase_index >= t950_phase_count)
        exit(2);
    T950_CHECK(t950_phases[t950_phase_index] == code);
    t950_verify();
    ++t950_phase_index;
}
static unsigned int t950_bits(float v)
{
    unsigned int u;
    memcpy(&u, &v, 4);
    return u;
}
static void t950_vector_set(unsigned int at, unsigned int shift)
{
    unsigned int i;
    for (i = 0; i < 3; ++i)
    {
        unsigned int u = t950_words[(t950_pattern + shift + i) % 8];
        memcpy((unsigned char *)&t950_vectors[at].value + i * 4, &u, 4);
        memcpy((unsigned char *)&t950_expected_vectors[at].value + i * 4, &u, 4);
    }
}
static int __stdcall t950_query(OP_AUDIO_BUFFER *o, const void *iid, OP_AUDIO_SPATIAL **out)
{
    t950_phase(10);
    T950_CHECK(o == &t950_buffers[t950_selected].value && iid == op_audio_spatial_iid && out != NULL);
    if (!out)
        exit(2);
    if (t950_write_output)
        *out = t950_null_output ? NULL : &t950_spatials[t950_selected].value;
    if (t950_mutation)
    {
        t950_buffers[t950_selected].payload[0] = t950_expected_buffers[t950_selected].payload[0] = 0x731u;
        op_audio_gate_word = t950_wanted_gate = (unsigned int)&t950_devices[1].value;
    }
    t950_verify();
    return t950_return_code;
}
static int __stdcall t950_duplicate(OP_AUDIO_DEVICE *o, OP_AUDIO_BUFFER *source, OP_AUDIO_BUFFER **out)
{
    t950_phase(11);
    T950_CHECK((unsigned int)o == t950_wanted_gate && source == t950_source_buffer && out != NULL);
    if (!out)
        exit(2);
    if (t950_write_output)
        *out = t950_null_output ? NULL : &t950_buffers[2].value;
    if (t950_mutation)
    {
        t950_devices[t950_selected].payload[1] = t950_expected_devices[t950_selected].payload[1] = 0x732u;
        op_audio_gate_word = t950_wanted_gate = (unsigned int)&t950_devices[1 - t950_selected].value;
    }
    t950_verify();
    return t950_return_code;
}
static int __stdcall t950_volume(OP_AUDIO_BUFFER *o, int level)
{
    t950_phase(20);
    T950_CHECK(o == t950_wanted_primary && level == t950_wanted_volume);
    if (t950_mutation)
    {
        op_audio_primary = t950_wanted_primary = &t950_buffers[1].value;
        op_audio_gate_word = t950_wanted_gate = (unsigned int)&t950_devices[1].value;
        op_audio_primary_volume = t950_wanted_volume = t950_volume_values[1];
        t950_buffers[0].payload[0] = t950_expected_buffers[0].payload[0] = 0x733u;
    }
    t950_verify();
    return t950_return_code;
}
static unsigned int __stdcall t950_release_buffer(OP_AUDIO_BUFFER *o)
{
    t950_phase(21);
    T950_CHECK(o == t950_wanted_primary);
    if (t950_mutation)
    {
        op_audio_primary = t950_wanted_primary = &t950_buffers[2].value;
        op_audio_gate_word = t950_wanted_gate = (unsigned int)&t950_devices[0].value;
        t950_buffers[2].payload[1] = t950_expected_buffers[2].payload[1] = 0x734u;
    }
    t950_verify();
    t950_wanted_primary = NULL;
    return (unsigned int)t950_return_code;
}
static unsigned int __stdcall t950_release_device(OP_AUDIO_DEVICE *o)
{
    t950_phase(22);
    T950_CHECK((unsigned int)o == t950_wanted_gate);
    if (t950_mutation)
    {
        op_audio_gate_word = t950_wanted_gate = (unsigned int)&t950_devices[1].value;
        op_audio_listener = t950_wanted_listener = &t950_listeners[2].value;
        t950_devices[0].payload[0] = t950_expected_devices[0].payload[0] = 0x735u;
    }
    t950_verify();
    t950_wanted_gate = 0;
    return (unsigned int)t950_return_code;
}
static int __stdcall t950_position(OP_AUDIO_LISTENER *o, float x, float y, float z, unsigned int deferred)
{
    t950_phase(30);
    T950_CHECK(o == t950_wanted_listener && deferred == 1);
    T950_CHECK(t950_bits(x) == t950_words[t950_pattern % 8] && t950_bits(y) == t950_words[(t950_pattern + 1) % 8] &&
               t950_bits(z) == t950_words[(t950_pattern + 2) % 8]);
    if (t950_mutation)
    {
        op_audio_listener = t950_wanted_listener = &t950_listeners[1].value;
        t950_vector_set(1, 4);
        t950_vector_set(2, 5);
        t950_listeners[0].payload[0] = t950_expected_listeners[0].payload[0] = 0x736u;
    }
    t950_verify();
    return t950_return_code;
}
static int __stdcall t950_orientation(OP_AUDIO_LISTENER *o, float x, float y, float z, float ux, float uy, float uz,
                                      unsigned int deferred)
{
    unsigned int front = t950_mutation ? 4u : 1u, up = t950_mutation ? 5u : 2u;
    t950_phase(31);
    T950_CHECK(o == t950_wanted_listener && deferred == 1);
    T950_CHECK(t950_bits(x) == t950_words[(t950_pattern + front) % 8] &&
               t950_bits(y) == t950_words[(t950_pattern + front + 1) % 8] &&
               t950_bits(z) == t950_words[(t950_pattern + front + 2) % 8]);
    T950_CHECK(t950_bits(ux) == t950_words[(t950_pattern + up) % 8] &&
               t950_bits(uy) == t950_words[(t950_pattern + up + 1) % 8] &&
               t950_bits(uz) == t950_words[(t950_pattern + up + 2) % 8]);
    if (t950_mutation)
    {
        op_audio_listener = t950_wanted_listener = &t950_listeners[2].value;
        t950_listeners[1].payload[1] = t950_expected_listeners[1].payload[1] = 0x737u;
        t950_vector_set(0, 6);
    }
    t950_verify();
    return t950_return_code;
}
static int __stdcall t950_commit(OP_AUDIO_LISTENER *o)
{
    t950_phase(32);
    T950_CHECK(o == t950_wanted_listener);
    if (t950_mutation)
    {
        op_audio_listener = t950_wanted_listener = NULL;
        t950_listeners[2].payload[0] = t950_expected_listeners[2].payload[0] = 0x738u;
    }
    t950_verify();
    return t950_return_code;
}
static void t950_setup(void)
{
    unsigned int i;
    memset(t950_devices, 0, sizeof(t950_devices));
    memset(t950_buffers, 0, sizeof(t950_buffers));
    memset(t950_spatials, 0, sizeof(t950_spatials));
    memset(t950_listeners, 0, sizeof(t950_listeners));
    memset(t950_vectors, 0, sizeof(t950_vectors));
    memset(t950_dvt, 0, sizeof(t950_dvt));
    memset(t950_bvt, 0, sizeof(t950_bvt));
    memset(t950_svt, 0, sizeof(t950_svt));
    memset(t950_lvt, 0, sizeof(t950_lvt));
    for (i = 0; i < 2; ++i)
    {
        t950_devices[i].before = 0x1234u + t950_pattern + i;
        t950_devices[i].after = 0x8765u + i;
        t950_devices[i].value.vt = &t950_dvt[i];
        t950_dvt[i].release = t950_release_device;
        t950_dvt[i].duplicate = t950_duplicate;
        t950_spatials[i].before = 0x4321u + i;
        t950_spatials[i].after = 0x9876u + t950_pattern;
        t950_spatials[i].value.vt = &t950_svt[i];
    }
    for (i = 0; i < 3; ++i)
    {
        t950_buffers[i].before = 0x3210u + t950_pattern + i;
        t950_buffers[i].after = 0x7654u + i;
        t950_buffers[i].value.vt = &t950_bvt[i];
        t950_bvt[i].query = t950_query;
        t950_bvt[i].release = t950_release_buffer;
        t950_bvt[i].volume = t950_volume;
        t950_listeners[i].before = 0x7410u + i;
        t950_listeners[i].after = 0x1470u + t950_pattern;
        t950_listeners[i].value.vt = &t950_lvt[i];
        t950_lvt[i].position = t950_position;
        t950_lvt[i].orientation = t950_orientation;
        t950_lvt[i].commit = t950_commit;
        t950_vectors[i].before = 0xabc0u + i;
        t950_vectors[i].after = 0xdef0u + t950_pattern;
        t950_vector_set(i, i);
    }
    memcpy(t950_expected_devices, t950_devices, sizeof(t950_devices));
    memcpy(t950_expected_buffers, t950_buffers, sizeof(t950_buffers));
    memcpy(t950_expected_spatials, t950_spatials, sizeof(t950_spatials));
    memcpy(t950_expected_listeners, t950_listeners, sizeof(t950_listeners));
    memcpy(t950_expected_vectors, t950_vectors, sizeof(t950_vectors));
    memcpy(t950_expected_dvt, t950_dvt, sizeof(t950_dvt));
    memcpy(t950_expected_bvt, t950_bvt, sizeof(t950_bvt));
    memcpy(t950_expected_svt, t950_svt, sizeof(t950_svt));
    memcpy(t950_expected_lvt, t950_lvt, sizeof(t950_lvt));
    op_audio_gate_word = t950_wanted_gate = (unsigned int)&t950_devices[t950_selected].value;
    op_audio_primary = t950_wanted_primary = &t950_buffers[0].value;
    op_audio_primary_volume = t950_wanted_volume = t950_volume_values[0];
    op_audio_listener = t950_wanted_listener = &t950_listeners[0].value;
    t950_phase_index = t950_phase_count = 0;
    t950_source_buffer = &t950_buffers[t950_selected].value;
}
static int op_test_audio_get_spatial(void)
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
                                OP_AUDIO_SPATIAL *result, *expected;
                                t950_pattern = p;
                                t950_selected = s;
                                t950_mutation = (int)m;
                                t950_return_code = returns[r];
                                t950_null_output = (int)n;
                                t950_operation = 0;
                                t950_setup();
                                t950_write_output = (t950_return_code >= 0) || w;
                                t950_phase_count = 1;
                                t950_phases[0] = 10;
                                if (!g)
                                    op_audio_gate_word = t950_wanted_gate = 0;
                                expected = t950_return_code >= 0 && !t950_null_output
                                               ? &t950_spatials[t950_selected].value
                                               : NULL;
                                result = op_audio_get_spatial(&t950_buffers[t950_selected].value);
                                T950_CHECK(result == expected);
                                T950_CHECK(t950_phase_index == t950_phase_count);
                                t950_verify();
                            }
    op_audio_gate_word = saved_gate;
    op_audio_listener = saved_listener;
    op_audio_primary = saved_primary;
    op_audio_primary_volume = saved_volume;
    printf("audio_get_spatial: %d checks, %d failures\n", t950_checks, t950_failures);
    return t950_failures != 0;
}

#undef T950_CHECK
