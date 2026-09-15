#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
static int t953_checks, t953_failures;
static void t953_check(int ok, int line)
{
    ++t953_checks;
    if (!ok)
    {
        ++t953_failures;
        printf("line%d failed\n", line);
    }
}
#define T953_CHECK(x) t953_check(!!(x), __LINE__)
typedef struct T953_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[2], after;
} T953_DEVICE;
typedef struct T953_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[2], after;
} T953_BUFFER;
typedef struct T953_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[2], after;
} T953_SPATIAL;
typedef struct T953_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[2], after;
} T953_LISTENER;
typedef struct T953_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} T953_VECTOR;
static T953_DEVICE t953_devices[2], t953_expected_devices[2];
static T953_BUFFER t953_buffers[3], t953_expected_buffers[3];
static T953_SPATIAL t953_spatials[2], t953_expected_spatials[2];
static T953_LISTENER t953_listeners[3], t953_expected_listeners[3];
static T953_VECTOR t953_vectors[3], t953_expected_vectors[3];
static OP_AUDIO_DEVICE_VT t953_dvt[2], t953_expected_dvt[2];
static OP_AUDIO_BUFFER_VT t953_bvt[3], t953_expected_bvt[3];
static OP_AUDIO_SPATIAL_VT t953_svt[2], t953_expected_svt[2];
static OP_AUDIO_LISTENER_VT t953_lvt[3], t953_expected_lvt[3];
static int t953_volume_values[2] = {-10000, -1};
static const unsigned int t953_words[8] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                           0x3eaaaaabu, 0x40000000u, 0x00800000u, 0xc2800000u};
static unsigned int t953_wanted_gate, t953_pattern, t953_selected;
static OP_AUDIO_LISTENER *t953_wanted_listener;
static OP_AUDIO_BUFFER *t953_wanted_primary, *t953_source_buffer;
static int t953_wanted_volume;
static int t953_phase_index, t953_phase_count, t953_phases[3], t953_mutation, t953_return_code, t953_write_output,
    t953_null_output, t953_operation;
static void t953_verify(void)
{
    T953_CHECK(!memcmp(t953_devices, t953_expected_devices, sizeof(t953_devices)));
    T953_CHECK(!memcmp(t953_buffers, t953_expected_buffers, sizeof(t953_buffers)));
    T953_CHECK(!memcmp(t953_spatials, t953_expected_spatials, sizeof(t953_spatials)));
    T953_CHECK(!memcmp(t953_listeners, t953_expected_listeners, sizeof(t953_listeners)));
    T953_CHECK(!memcmp(t953_vectors, t953_expected_vectors, sizeof(t953_vectors)));
    T953_CHECK(!memcmp(t953_dvt, t953_expected_dvt, sizeof(t953_dvt)));
    T953_CHECK(!memcmp(t953_bvt, t953_expected_bvt, sizeof(t953_bvt)));
    T953_CHECK(!memcmp(t953_svt, t953_expected_svt, sizeof(t953_svt)));
    T953_CHECK(!memcmp(t953_lvt, t953_expected_lvt, sizeof(t953_lvt)));
    T953_CHECK(op_audio_gate_word == t953_wanted_gate && op_audio_primary == t953_wanted_primary &&
               op_audio_primary_volume == t953_wanted_volume && op_audio_listener == t953_wanted_listener);
    T953_CHECK(t953_volume_values[0] == -10000 && t953_volume_values[1] == -1);
}
static void t953_phase(int code)
{
    T953_CHECK(t953_phase_index < t953_phase_count);
    if (t953_phase_index >= t953_phase_count)
        exit(2);
    T953_CHECK(t953_phases[t953_phase_index] == code);
    t953_verify();
    ++t953_phase_index;
}
static unsigned int t953_bits(float v)
{
    unsigned int u;
    memcpy(&u, &v, 4);
    return u;
}
static void t953_vector_set(unsigned int at, unsigned int shift)
{
    unsigned int i;
    for (i = 0; i < 3; ++i)
    {
        unsigned int u = t953_words[(t953_pattern + shift + i) % 8];
        memcpy((unsigned char *)&t953_vectors[at].value + i * 4, &u, 4);
        memcpy((unsigned char *)&t953_expected_vectors[at].value + i * 4, &u, 4);
    }
}
static int __stdcall t953_query(OP_AUDIO_BUFFER *o, const void *iid, OP_AUDIO_SPATIAL **out)
{
    t953_phase(10);
    T953_CHECK(o == &t953_buffers[t953_selected].value && iid == op_audio_spatial_iid && out != NULL);
    if (!out)
        exit(2);
    if (t953_write_output)
        *out = t953_null_output ? NULL : &t953_spatials[t953_selected].value;
    if (t953_mutation)
    {
        t953_buffers[t953_selected].payload[0] = t953_expected_buffers[t953_selected].payload[0] = 0x731u;
        op_audio_gate_word = t953_wanted_gate = (unsigned int)&t953_devices[1].value;
    }
    t953_verify();
    return t953_return_code;
}
static int __stdcall t953_duplicate(OP_AUDIO_DEVICE *o, OP_AUDIO_BUFFER *source, OP_AUDIO_BUFFER **out)
{
    t953_phase(11);
    T953_CHECK((unsigned int)o == t953_wanted_gate && source == t953_source_buffer && out != NULL);
    if (!out)
        exit(2);
    if (t953_write_output)
        *out = t953_null_output ? NULL : &t953_buffers[2].value;
    if (t953_mutation)
    {
        t953_devices[t953_selected].payload[1] = t953_expected_devices[t953_selected].payload[1] = 0x732u;
        op_audio_gate_word = t953_wanted_gate = (unsigned int)&t953_devices[1 - t953_selected].value;
    }
    t953_verify();
    return t953_return_code;
}
static int __stdcall t953_volume(OP_AUDIO_BUFFER *o, int level)
{
    t953_phase(20);
    T953_CHECK(o == t953_wanted_primary && level == t953_wanted_volume);
    if (t953_mutation)
    {
        op_audio_primary = t953_wanted_primary = &t953_buffers[1].value;
        op_audio_gate_word = t953_wanted_gate = (unsigned int)&t953_devices[1].value;
        op_audio_primary_volume = t953_wanted_volume = t953_volume_values[1];
        t953_buffers[0].payload[0] = t953_expected_buffers[0].payload[0] = 0x733u;
    }
    t953_verify();
    return t953_return_code;
}
static unsigned int __stdcall t953_release_buffer(OP_AUDIO_BUFFER *o)
{
    t953_phase(21);
    T953_CHECK(o == t953_wanted_primary);
    if (t953_mutation)
    {
        op_audio_primary = t953_wanted_primary = &t953_buffers[2].value;
        op_audio_gate_word = t953_wanted_gate = (unsigned int)&t953_devices[0].value;
        t953_buffers[2].payload[1] = t953_expected_buffers[2].payload[1] = 0x734u;
    }
    t953_verify();
    t953_wanted_primary = NULL;
    return (unsigned int)t953_return_code;
}
static unsigned int __stdcall t953_release_device(OP_AUDIO_DEVICE *o)
{
    t953_phase(22);
    T953_CHECK((unsigned int)o == t953_wanted_gate);
    if (t953_mutation)
    {
        op_audio_gate_word = t953_wanted_gate = (unsigned int)&t953_devices[1].value;
        op_audio_listener = t953_wanted_listener = &t953_listeners[2].value;
        t953_devices[0].payload[0] = t953_expected_devices[0].payload[0] = 0x735u;
    }
    t953_verify();
    t953_wanted_gate = 0;
    return (unsigned int)t953_return_code;
}
static int __stdcall t953_position(OP_AUDIO_LISTENER *o, float x, float y, float z, unsigned int deferred)
{
    t953_phase(30);
    T953_CHECK(o == t953_wanted_listener && deferred == 1);
    T953_CHECK(t953_bits(x) == t953_words[t953_pattern % 8] && t953_bits(y) == t953_words[(t953_pattern + 1) % 8] &&
               t953_bits(z) == t953_words[(t953_pattern + 2) % 8]);
    if (t953_mutation)
    {
        op_audio_listener = t953_wanted_listener = &t953_listeners[1].value;
        t953_vector_set(1, 4);
        t953_vector_set(2, 5);
        t953_listeners[0].payload[0] = t953_expected_listeners[0].payload[0] = 0x736u;
    }
    t953_verify();
    return t953_return_code;
}
static int __stdcall t953_orientation(OP_AUDIO_LISTENER *o, float x, float y, float z, float ux, float uy, float uz,
                                      unsigned int deferred)
{
    unsigned int front = t953_mutation ? 4u : 1u, up = t953_mutation ? 5u : 2u;
    t953_phase(31);
    T953_CHECK(o == t953_wanted_listener && deferred == 1);
    T953_CHECK(t953_bits(x) == t953_words[(t953_pattern + front) % 8] &&
               t953_bits(y) == t953_words[(t953_pattern + front + 1) % 8] &&
               t953_bits(z) == t953_words[(t953_pattern + front + 2) % 8]);
    T953_CHECK(t953_bits(ux) == t953_words[(t953_pattern + up) % 8] &&
               t953_bits(uy) == t953_words[(t953_pattern + up + 1) % 8] &&
               t953_bits(uz) == t953_words[(t953_pattern + up + 2) % 8]);
    if (t953_mutation)
    {
        op_audio_listener = t953_wanted_listener = &t953_listeners[2].value;
        t953_listeners[1].payload[1] = t953_expected_listeners[1].payload[1] = 0x737u;
        t953_vector_set(0, 6);
    }
    t953_verify();
    return t953_return_code;
}
static int __stdcall t953_commit(OP_AUDIO_LISTENER *o)
{
    t953_phase(32);
    T953_CHECK(o == t953_wanted_listener);
    if (t953_mutation)
    {
        op_audio_listener = t953_wanted_listener = NULL;
        t953_listeners[2].payload[0] = t953_expected_listeners[2].payload[0] = 0x738u;
    }
    t953_verify();
    return t953_return_code;
}
static void t953_setup(void)
{
    unsigned int i;
    memset(t953_devices, 0, sizeof(t953_devices));
    memset(t953_buffers, 0, sizeof(t953_buffers));
    memset(t953_spatials, 0, sizeof(t953_spatials));
    memset(t953_listeners, 0, sizeof(t953_listeners));
    memset(t953_vectors, 0, sizeof(t953_vectors));
    memset(t953_dvt, 0, sizeof(t953_dvt));
    memset(t953_bvt, 0, sizeof(t953_bvt));
    memset(t953_svt, 0, sizeof(t953_svt));
    memset(t953_lvt, 0, sizeof(t953_lvt));
    for (i = 0; i < 2; ++i)
    {
        t953_devices[i].before = 0x1234u + t953_pattern + i;
        t953_devices[i].after = 0x8765u + i;
        t953_devices[i].value.vt = &t953_dvt[i];
        t953_dvt[i].release = t953_release_device;
        t953_dvt[i].duplicate = t953_duplicate;
        t953_spatials[i].before = 0x4321u + i;
        t953_spatials[i].after = 0x9876u + t953_pattern;
        t953_spatials[i].value.vt = &t953_svt[i];
    }
    for (i = 0; i < 3; ++i)
    {
        t953_buffers[i].before = 0x3210u + t953_pattern + i;
        t953_buffers[i].after = 0x7654u + i;
        t953_buffers[i].value.vt = &t953_bvt[i];
        t953_bvt[i].query = t953_query;
        t953_bvt[i].release = t953_release_buffer;
        t953_bvt[i].volume = t953_volume;
        t953_listeners[i].before = 0x7410u + i;
        t953_listeners[i].after = 0x1470u + t953_pattern;
        t953_listeners[i].value.vt = &t953_lvt[i];
        t953_lvt[i].position = t953_position;
        t953_lvt[i].orientation = t953_orientation;
        t953_lvt[i].commit = t953_commit;
        t953_vectors[i].before = 0xabc0u + i;
        t953_vectors[i].after = 0xdef0u + t953_pattern;
        t953_vector_set(i, i);
    }
    memcpy(t953_expected_devices, t953_devices, sizeof(t953_devices));
    memcpy(t953_expected_buffers, t953_buffers, sizeof(t953_buffers));
    memcpy(t953_expected_spatials, t953_spatials, sizeof(t953_spatials));
    memcpy(t953_expected_listeners, t953_listeners, sizeof(t953_listeners));
    memcpy(t953_expected_vectors, t953_vectors, sizeof(t953_vectors));
    memcpy(t953_expected_dvt, t953_dvt, sizeof(t953_dvt));
    memcpy(t953_expected_bvt, t953_bvt, sizeof(t953_bvt));
    memcpy(t953_expected_svt, t953_svt, sizeof(t953_svt));
    memcpy(t953_expected_lvt, t953_lvt, sizeof(t953_lvt));
    op_audio_gate_word = t953_wanted_gate = (unsigned int)&t953_devices[t953_selected].value;
    op_audio_primary = t953_wanted_primary = &t953_buffers[0].value;
    op_audio_primary_volume = t953_wanted_volume = t953_volume_values[0];
    op_audio_listener = t953_wanted_listener = &t953_listeners[0].value;
    t953_phase_index = t953_phase_count = 0;
    t953_source_buffer = &t953_buffers[t953_selected].value;
}
static int op_test_audio_set_listener(void)
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
                                t953_pattern = p;
                                t953_selected = s;
                                t953_mutation = (int)m;
                                t953_return_code = returns[r];
                                t953_null_output = (int)n;
                                t953_operation = 3;
                                t953_setup();
                                t953_write_output = (int)w;
                                if (!g)
                                    op_audio_listener = t953_wanted_listener = NULL;
                                t953_phase_count = g ? 3 : 0;
                                t953_phases[0] = 30;
                                t953_phases[1] = 31;
                                t953_phases[2] = 32;
                                if (g)
                                    op_audio_set_listener(&t953_vectors[0].value, &t953_vectors[1].value,
                                                          &t953_vectors[2].value);
                                else
                                    op_audio_set_listener(NULL, NULL, NULL);
                                T953_CHECK(t953_phase_index == t953_phase_count);
                                t953_verify();
                            }
    op_audio_gate_word = saved_gate;
    op_audio_listener = saved_listener;
    op_audio_primary = saved_primary;
    op_audio_primary_volume = saved_volume;
    printf("audio_set_listener: %d checks, %d failures\n", t953_checks, t953_failures);
    return t953_failures != 0;
}

#undef T953_CHECK
