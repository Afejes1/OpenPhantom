#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
static int t952_checks, t952_failures;
static void t952_check(int ok, int line)
{
    ++t952_checks;
    if (!ok)
    {
        ++t952_failures;
        printf("line%d failed\n", line);
    }
}
#define T952_CHECK(x) t952_check(!!(x), __LINE__)
typedef struct T952_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[2], after;
} T952_DEVICE;
typedef struct T952_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[2], after;
} T952_BUFFER;
typedef struct T952_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[2], after;
} T952_SPATIAL;
typedef struct T952_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[2], after;
} T952_LISTENER;
typedef struct T952_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} T952_VECTOR;
static T952_DEVICE t952_devices[2], t952_expected_devices[2];
static T952_BUFFER t952_buffers[3], t952_expected_buffers[3];
static T952_SPATIAL t952_spatials[2], t952_expected_spatials[2];
static T952_LISTENER t952_listeners[3], t952_expected_listeners[3];
static T952_VECTOR t952_vectors[3], t952_expected_vectors[3];
static OP_AUDIO_DEVICE_VT t952_dvt[2], t952_expected_dvt[2];
static OP_AUDIO_BUFFER_VT t952_bvt[3], t952_expected_bvt[3];
static OP_AUDIO_SPATIAL_VT t952_svt[2], t952_expected_svt[2];
static OP_AUDIO_LISTENER_VT t952_lvt[3], t952_expected_lvt[3];
static int t952_volume_values[2] = {-10000, -1};
static const unsigned int t952_words[8] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                           0x3eaaaaabu, 0x40000000u, 0x00800000u, 0xc2800000u};
static unsigned int t952_wanted_gate, t952_pattern, t952_selected;
static OP_AUDIO_LISTENER *t952_wanted_listener;
static OP_AUDIO_BUFFER *t952_wanted_primary, *t952_source_buffer;
static int t952_wanted_volume;
static int t952_phase_index, t952_phase_count, t952_phases[3], t952_mutation, t952_return_code, t952_write_output,
    t952_null_output, t952_operation;
static void t952_verify(void)
{
    T952_CHECK(!memcmp(t952_devices, t952_expected_devices, sizeof(t952_devices)));
    T952_CHECK(!memcmp(t952_buffers, t952_expected_buffers, sizeof(t952_buffers)));
    T952_CHECK(!memcmp(t952_spatials, t952_expected_spatials, sizeof(t952_spatials)));
    T952_CHECK(!memcmp(t952_listeners, t952_expected_listeners, sizeof(t952_listeners)));
    T952_CHECK(!memcmp(t952_vectors, t952_expected_vectors, sizeof(t952_vectors)));
    T952_CHECK(!memcmp(t952_dvt, t952_expected_dvt, sizeof(t952_dvt)));
    T952_CHECK(!memcmp(t952_bvt, t952_expected_bvt, sizeof(t952_bvt)));
    T952_CHECK(!memcmp(t952_svt, t952_expected_svt, sizeof(t952_svt)));
    T952_CHECK(!memcmp(t952_lvt, t952_expected_lvt, sizeof(t952_lvt)));
    T952_CHECK(op_audio_gate_word == t952_wanted_gate && op_audio_primary == t952_wanted_primary &&
               op_audio_primary_volume == t952_wanted_volume && op_audio_listener == t952_wanted_listener);
    T952_CHECK(t952_volume_values[0] == -10000 && t952_volume_values[1] == -1);
}
static void t952_phase(int code)
{
    T952_CHECK(t952_phase_index < t952_phase_count);
    if (t952_phase_index >= t952_phase_count)
        exit(2);
    T952_CHECK(t952_phases[t952_phase_index] == code);
    t952_verify();
    ++t952_phase_index;
}
static unsigned int t952_bits(float v)
{
    unsigned int u;
    memcpy(&u, &v, 4);
    return u;
}
static void t952_vector_set(unsigned int at, unsigned int shift)
{
    unsigned int i;
    for (i = 0; i < 3; ++i)
    {
        unsigned int u = t952_words[(t952_pattern + shift + i) % 8];
        memcpy((unsigned char *)&t952_vectors[at].value + i * 4, &u, 4);
        memcpy((unsigned char *)&t952_expected_vectors[at].value + i * 4, &u, 4);
    }
}
static int __stdcall t952_query(OP_AUDIO_BUFFER *o, const void *iid, OP_AUDIO_SPATIAL **out)
{
    t952_phase(10);
    T952_CHECK(o == &t952_buffers[t952_selected].value && iid == op_audio_spatial_iid && out != NULL);
    if (!out)
        exit(2);
    if (t952_write_output)
        *out = t952_null_output ? NULL : &t952_spatials[t952_selected].value;
    if (t952_mutation)
    {
        t952_buffers[t952_selected].payload[0] = t952_expected_buffers[t952_selected].payload[0] = 0x731u;
        op_audio_gate_word = t952_wanted_gate = (unsigned int)&t952_devices[1].value;
    }
    t952_verify();
    return t952_return_code;
}
static int __stdcall t952_duplicate(OP_AUDIO_DEVICE *o, OP_AUDIO_BUFFER *source, OP_AUDIO_BUFFER **out)
{
    t952_phase(11);
    T952_CHECK((unsigned int)o == t952_wanted_gate && source == t952_source_buffer && out != NULL);
    if (!out)
        exit(2);
    if (t952_write_output)
        *out = t952_null_output ? NULL : &t952_buffers[2].value;
    if (t952_mutation)
    {
        t952_devices[t952_selected].payload[1] = t952_expected_devices[t952_selected].payload[1] = 0x732u;
        op_audio_gate_word = t952_wanted_gate = (unsigned int)&t952_devices[1 - t952_selected].value;
    }
    t952_verify();
    return t952_return_code;
}
static int __stdcall t952_volume(OP_AUDIO_BUFFER *o, int level)
{
    t952_phase(20);
    T952_CHECK(o == t952_wanted_primary && level == t952_wanted_volume);
    if (t952_mutation)
    {
        op_audio_primary = t952_wanted_primary = &t952_buffers[1].value;
        op_audio_gate_word = t952_wanted_gate = (unsigned int)&t952_devices[1].value;
        op_audio_primary_volume = t952_wanted_volume = t952_volume_values[1];
        t952_buffers[0].payload[0] = t952_expected_buffers[0].payload[0] = 0x733u;
    }
    t952_verify();
    return t952_return_code;
}
static unsigned int __stdcall t952_release_buffer(OP_AUDIO_BUFFER *o)
{
    t952_phase(21);
    T952_CHECK(o == t952_wanted_primary);
    if (t952_mutation)
    {
        op_audio_primary = t952_wanted_primary = &t952_buffers[2].value;
        op_audio_gate_word = t952_wanted_gate = (unsigned int)&t952_devices[0].value;
        t952_buffers[2].payload[1] = t952_expected_buffers[2].payload[1] = 0x734u;
    }
    t952_verify();
    t952_wanted_primary = NULL;
    return (unsigned int)t952_return_code;
}
static unsigned int __stdcall t952_release_device(OP_AUDIO_DEVICE *o)
{
    t952_phase(22);
    T952_CHECK((unsigned int)o == t952_wanted_gate);
    if (t952_mutation)
    {
        op_audio_gate_word = t952_wanted_gate = (unsigned int)&t952_devices[1].value;
        op_audio_listener = t952_wanted_listener = &t952_listeners[2].value;
        t952_devices[0].payload[0] = t952_expected_devices[0].payload[0] = 0x735u;
    }
    t952_verify();
    t952_wanted_gate = 0;
    return (unsigned int)t952_return_code;
}
static int __stdcall t952_position(OP_AUDIO_LISTENER *o, float x, float y, float z, unsigned int deferred)
{
    t952_phase(30);
    T952_CHECK(o == t952_wanted_listener && deferred == 1);
    T952_CHECK(t952_bits(x) == t952_words[t952_pattern % 8] && t952_bits(y) == t952_words[(t952_pattern + 1) % 8] &&
               t952_bits(z) == t952_words[(t952_pattern + 2) % 8]);
    if (t952_mutation)
    {
        op_audio_listener = t952_wanted_listener = &t952_listeners[1].value;
        t952_vector_set(1, 4);
        t952_vector_set(2, 5);
        t952_listeners[0].payload[0] = t952_expected_listeners[0].payload[0] = 0x736u;
    }
    t952_verify();
    return t952_return_code;
}
static int __stdcall t952_orientation(OP_AUDIO_LISTENER *o, float x, float y, float z, float ux, float uy, float uz,
                                      unsigned int deferred)
{
    unsigned int front = t952_mutation ? 4u : 1u, up = t952_mutation ? 5u : 2u;
    t952_phase(31);
    T952_CHECK(o == t952_wanted_listener && deferred == 1);
    T952_CHECK(t952_bits(x) == t952_words[(t952_pattern + front) % 8] &&
               t952_bits(y) == t952_words[(t952_pattern + front + 1) % 8] &&
               t952_bits(z) == t952_words[(t952_pattern + front + 2) % 8]);
    T952_CHECK(t952_bits(ux) == t952_words[(t952_pattern + up) % 8] &&
               t952_bits(uy) == t952_words[(t952_pattern + up + 1) % 8] &&
               t952_bits(uz) == t952_words[(t952_pattern + up + 2) % 8]);
    if (t952_mutation)
    {
        op_audio_listener = t952_wanted_listener = &t952_listeners[2].value;
        t952_listeners[1].payload[1] = t952_expected_listeners[1].payload[1] = 0x737u;
        t952_vector_set(0, 6);
    }
    t952_verify();
    return t952_return_code;
}
static int __stdcall t952_commit(OP_AUDIO_LISTENER *o)
{
    t952_phase(32);
    T952_CHECK(o == t952_wanted_listener);
    if (t952_mutation)
    {
        op_audio_listener = t952_wanted_listener = NULL;
        t952_listeners[2].payload[0] = t952_expected_listeners[2].payload[0] = 0x738u;
    }
    t952_verify();
    return t952_return_code;
}
static void t952_setup(void)
{
    unsigned int i;
    memset(t952_devices, 0, sizeof(t952_devices));
    memset(t952_buffers, 0, sizeof(t952_buffers));
    memset(t952_spatials, 0, sizeof(t952_spatials));
    memset(t952_listeners, 0, sizeof(t952_listeners));
    memset(t952_vectors, 0, sizeof(t952_vectors));
    memset(t952_dvt, 0, sizeof(t952_dvt));
    memset(t952_bvt, 0, sizeof(t952_bvt));
    memset(t952_svt, 0, sizeof(t952_svt));
    memset(t952_lvt, 0, sizeof(t952_lvt));
    for (i = 0; i < 2; ++i)
    {
        t952_devices[i].before = 0x1234u + t952_pattern + i;
        t952_devices[i].after = 0x8765u + i;
        t952_devices[i].value.vt = &t952_dvt[i];
        t952_dvt[i].release = t952_release_device;
        t952_dvt[i].duplicate = t952_duplicate;
        t952_spatials[i].before = 0x4321u + i;
        t952_spatials[i].after = 0x9876u + t952_pattern;
        t952_spatials[i].value.vt = &t952_svt[i];
    }
    for (i = 0; i < 3; ++i)
    {
        t952_buffers[i].before = 0x3210u + t952_pattern + i;
        t952_buffers[i].after = 0x7654u + i;
        t952_buffers[i].value.vt = &t952_bvt[i];
        t952_bvt[i].query = t952_query;
        t952_bvt[i].release = t952_release_buffer;
        t952_bvt[i].volume = t952_volume;
        t952_listeners[i].before = 0x7410u + i;
        t952_listeners[i].after = 0x1470u + t952_pattern;
        t952_listeners[i].value.vt = &t952_lvt[i];
        t952_lvt[i].position = t952_position;
        t952_lvt[i].orientation = t952_orientation;
        t952_lvt[i].commit = t952_commit;
        t952_vectors[i].before = 0xabc0u + i;
        t952_vectors[i].after = 0xdef0u + t952_pattern;
        t952_vector_set(i, i);
    }
    memcpy(t952_expected_devices, t952_devices, sizeof(t952_devices));
    memcpy(t952_expected_buffers, t952_buffers, sizeof(t952_buffers));
    memcpy(t952_expected_spatials, t952_spatials, sizeof(t952_spatials));
    memcpy(t952_expected_listeners, t952_listeners, sizeof(t952_listeners));
    memcpy(t952_expected_vectors, t952_vectors, sizeof(t952_vectors));
    memcpy(t952_expected_dvt, t952_dvt, sizeof(t952_dvt));
    memcpy(t952_expected_bvt, t952_bvt, sizeof(t952_bvt));
    memcpy(t952_expected_svt, t952_svt, sizeof(t952_svt));
    memcpy(t952_expected_lvt, t952_lvt, sizeof(t952_lvt));
    op_audio_gate_word = t952_wanted_gate = (unsigned int)&t952_devices[t952_selected].value;
    op_audio_primary = t952_wanted_primary = &t952_buffers[0].value;
    op_audio_primary_volume = t952_wanted_volume = t952_volume_values[0];
    op_audio_listener = t952_wanted_listener = &t952_listeners[0].value;
    t952_phase_index = t952_phase_count = 0;
    t952_source_buffer = &t952_buffers[t952_selected].value;
}
static int op_test_audio_shutdown(void)
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
                                t952_pattern = p;
                                t952_selected = s;
                                t952_mutation = (int)m;
                                t952_return_code = returns[r];
                                t952_null_output = (int)n;
                                t952_operation = 2;
                                t952_setup();
                                t952_write_output = (int)w;
                                if (!g)
                                    op_audio_gate_word = t952_wanted_gate = 0;
                                if (n)
                                    op_audio_primary = t952_wanted_primary = NULL;
                                t952_phase_count = g ? (n ? 1 : 3) : 0;
                                if (n)
                                    t952_phases[0] = 22;
                                else
                                {
                                    t952_phases[0] = 20;
                                    t952_phases[1] = 21;
                                    t952_phases[2] = 22;
                                }
                                op_audio_shutdown();
                                T952_CHECK(t952_phase_index == t952_phase_count);
                                t952_verify();
                            }
    op_audio_gate_word = saved_gate;
    op_audio_listener = saved_listener;
    op_audio_primary = saved_primary;
    op_audio_primary_volume = saved_volume;
    printf("audio_shutdown: %d checks, %d failures\n", t952_checks, t952_failures);
    return t952_failures != 0;
}

#undef T952_CHECK
