#include "../src/audio_helpers.h"
unsigned int op_audio_gate_word;
OP_AUDIO_LISTENER *op_audio_listener;
int(__stdcall *op_audio_co_initialize)(void *);
void(__stdcall *op_audio_co_uninitialize)(void);

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au0_checks, au0_failures;
static void au0_check(int ok, int line)
{
    ++au0_checks;
    if (!ok)
    {
        ++au0_failures;
        printf("line%d failed\n", line);
    }
}
#define AU0_CHECK(x) au0_check(!!(x), __LINE__)
typedef struct AU0_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU0_OWNED_DEVICE;
typedef struct AU0_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU0_OWNED_BUFFER;
typedef struct AU0_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU0_OWNED_SPATIAL;
typedef struct AU0_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU0_OWNED_LISTENER;
typedef struct AU0_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU0_OWNED_VECTOR;
static AU0_OWNED_DEVICE au0_devices[2], au0_expected_devices[2];
static AU0_OWNED_BUFFER au0_buffers[2], au0_expected_buffers[2];
static AU0_OWNED_SPATIAL au0_spatials[2], au0_expected_spatials[2];
static AU0_OWNED_LISTENER au0_listeners[2], au0_expected_listeners[2];
static AU0_OWNED_VECTOR au0_vector, au0_expected_vector;
static OP_AUDIO_DEVICE_VT au0_dvt[2], au0_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au0_bvt[2], au0_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au0_svt[2], au0_expected_svt[2];
static OP_AUDIO_LISTENER_VT au0_lvt[2], au0_expected_lvt[2];
static unsigned int au0_selected, au0_word, au0_minimum_bits, au0_maximum_bits, au0_captured[3], au0_result_word,
    au0_expected_gate;
static OP_AUDIO_LISTENER *au0_expected_listener;
static void *au0_expected_this;
static int au0_calls, au0_expected_count, au0_expected_codes[2], au0_mutate;
static unsigned int au0_bits(float value)
{
    unsigned int au0_bits_word;
    memcpy(&au0_bits_word, &value, 4);
    return au0_bits_word;
}
static float au0_number(unsigned int au0_number_word)
{
    float value;
    memcpy(&value, &au0_number_word, 4);
    return value;
}
static void au0_verify(void)
{
    AU0_CHECK(memcmp(au0_devices, au0_expected_devices, sizeof(au0_devices)) == 0);
    AU0_CHECK(memcmp(au0_buffers, au0_expected_buffers, sizeof(au0_buffers)) == 0);
    AU0_CHECK(memcmp(au0_spatials, au0_expected_spatials, sizeof(au0_spatials)) == 0);
    AU0_CHECK(memcmp(au0_listeners, au0_expected_listeners, sizeof(au0_listeners)) == 0);
    AU0_CHECK(memcmp(&au0_vector, &au0_expected_vector, sizeof(au0_vector)) == 0);
    AU0_CHECK(memcmp(au0_dvt, au0_expected_dvt, sizeof(au0_dvt)) == 0);
    AU0_CHECK(memcmp(au0_bvt, au0_expected_bvt, sizeof(au0_bvt)) == 0);
    AU0_CHECK(memcmp(au0_svt, au0_expected_svt, sizeof(au0_svt)) == 0);
    AU0_CHECK(memcmp(au0_lvt, au0_expected_lvt, sizeof(au0_lvt)) == 0);
    AU0_CHECK(op_audio_gate_word == au0_expected_gate);
    AU0_CHECK(op_audio_listener == au0_expected_listener);
}
static void au0_observe(int code, unsigned int table, void *object)
{
    AU0_CHECK(au0_calls < au0_expected_count && au0_calls < 2);
    if (au0_calls >= au0_expected_count || au0_calls >= 2)
        exit(2);
    AU0_CHECK(code == au0_expected_codes[au0_calls]);
    AU0_CHECK(table == (au0_calls && au0_mutate ? 1 - au0_selected : au0_selected));
    AU0_CHECK(object == au0_expected_this);
    au0_verify();
    ++au0_calls;
}
static void au0_change(void)
{
    if (au0_mutate && au0_calls == 1)
    {
        op_audio_gate_word = au0_expected_gate = 0;
        au0_devices[au0_selected].value.vt = au0_expected_devices[au0_selected].value.vt = &au0_dvt[1 - au0_selected];
        au0_buffers[au0_selected].value.vt = au0_expected_buffers[au0_selected].value.vt = &au0_bvt[1 - au0_selected];
        au0_spatials[au0_selected].value.vt = au0_expected_spatials[au0_selected].value.vt = &au0_svt[1 - au0_selected];
        au0_listeners[au0_selected].value.vt = au0_expected_listeners[au0_selected].value.vt =
            &au0_lvt[1 - au0_selected];
        op_audio_listener = au0_expected_listener = &au0_listeners[1 - au0_selected].value;
        au0_vector.value.x = au0_expected_vector.value.x = 7;
        au0_vector.value.y = au0_expected_vector.value.y = 9;
        au0_vector.value.z = au0_expected_vector.value.z = 11;
    }
}
static int __stdcall au0_coinit0(void *reserved)
{
    au0_observe(1, 0, 0);
    AU0_CHECK(reserved == 0);
    au0_change();
    return (int)au0_result_word;
}
static void __stdcall au0_couninit0(void)
{
    au0_observe(2, 0, 0);
    au0_change();
}
static int __stdcall au0_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au0_observe(3, 0, o);
    AU0_CHECK(window == au0_word && flags == 2);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au0_observe(4, 0, o);
    AU0_CHECK(frequency == au0_word);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au0_observe(5, 0, o);
    AU0_CHECK(au0_bits(value) == au0_maximum_bits && flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au0_observe(6, 0, o);
    AU0_CHECK(au0_bits(value) == au0_minimum_bits && flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au0_observe(7, 0, o);
    AU0_CHECK(au0_bits(x) == au0_captured[0] && au0_bits(y) == au0_captured[1] && au0_bits(z) == au0_captured[2] &&
              flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au0_observe(8, 0, o);
    AU0_CHECK(au0_bits(x) == au0_captured[0] && au0_bits(y) == au0_captured[1] && au0_bits(z) == au0_captured[2] &&
              flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_commit0(OP_AUDIO_LISTENER *o)
{
    au0_observe(9, 0, o);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au0_observe(10, 0, o);
    AU0_CHECK(mode == au0_word && flags == 0);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_stop0(OP_AUDIO_BUFFER *o)
{
    au0_observe(11, 0, o);
    au0_change();
    return (int)au0_result_word;
}
static unsigned int __stdcall au0_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au0_observe(12, 0, o);
    au0_change();
    return (unsigned int)au0_result_word;
}
static unsigned int __stdcall au0_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au0_observe(13, 0, o);
    au0_change();
    return (unsigned int)au0_result_word;
}
static int __stdcall au0_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au0_observe(14, 0, o);
    AU0_CHECK(position == 0);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_coinit1(void *reserved)
{
    au0_observe(1, 1, 0);
    AU0_CHECK(reserved == 0);
    au0_change();
    return (int)au0_result_word;
}
static void __stdcall au0_couninit1(void)
{
    au0_observe(2, 1, 0);
    au0_change();
}
static int __stdcall au0_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au0_observe(3, 1, o);
    AU0_CHECK(window == au0_word && flags == 2);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au0_observe(4, 1, o);
    AU0_CHECK(frequency == au0_word);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au0_observe(5, 1, o);
    AU0_CHECK(au0_bits(value) == au0_maximum_bits && flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au0_observe(6, 1, o);
    AU0_CHECK(au0_bits(value) == au0_minimum_bits && flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au0_observe(7, 1, o);
    AU0_CHECK(au0_bits(x) == au0_captured[0] && au0_bits(y) == au0_captured[1] && au0_bits(z) == au0_captured[2] &&
              flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au0_observe(8, 1, o);
    AU0_CHECK(au0_bits(x) == au0_captured[0] && au0_bits(y) == au0_captured[1] && au0_bits(z) == au0_captured[2] &&
              flags == 1);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_commit1(OP_AUDIO_LISTENER *o)
{
    au0_observe(9, 1, o);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au0_observe(10, 1, o);
    AU0_CHECK(mode == au0_word && flags == 0);
    au0_change();
    return (int)au0_result_word;
}
static int __stdcall au0_stop1(OP_AUDIO_BUFFER *o)
{
    au0_observe(11, 1, o);
    au0_change();
    return (int)au0_result_word;
}
static unsigned int __stdcall au0_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au0_observe(12, 1, o);
    au0_change();
    return (unsigned int)au0_result_word;
}
static unsigned int __stdcall au0_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au0_observe(13, 1, o);
    au0_change();
    return (unsigned int)au0_result_word;
}
static int __stdcall au0_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au0_observe(14, 1, o);
    AU0_CHECK(position == 0);
    au0_change();
    return (int)au0_result_word;
}
static void au0_seed(void)
{
    unsigned int i;
    memset(au0_devices, 0x57, sizeof(au0_devices));
    memset(au0_buffers, 0x68, sizeof(au0_buffers));
    memset(au0_spatials, 0x79, sizeof(au0_spatials));
    memset(au0_listeners, 0x2a, sizeof(au0_listeners));
    memset(&au0_vector, 0x3b, sizeof(au0_vector));
    memset(au0_dvt, 0x4c, sizeof(au0_dvt));
    memset(au0_bvt, 0x5d, sizeof(au0_bvt));
    memset(au0_svt, 0x6e, sizeof(au0_svt));
    memset(au0_lvt, 0x7f, sizeof(au0_lvt));
    au0_dvt[0].cooperative = au0_coop0;
    au0_bvt[0].release = au0_release_buffer0;
    au0_bvt[0].current_position = au0_current0;
    au0_bvt[0].frequency = au0_frequency0;
    au0_bvt[0].stop = au0_stop0;
    au0_svt[0].maximum = au0_maximum0;
    au0_svt[0].minimum = au0_minimum0;
    au0_svt[0].position = au0_position0;
    au0_svt[0].velocity = au0_velocity0;
    au0_svt[0].mode = au0_mode0;
    au0_svt[0].release = au0_release_spatial0;
    au0_lvt[0].commit = au0_commit0;
    au0_dvt[1].cooperative = au0_coop1;
    au0_bvt[1].release = au0_release_buffer1;
    au0_bvt[1].current_position = au0_current1;
    au0_bvt[1].frequency = au0_frequency1;
    au0_bvt[1].stop = au0_stop1;
    au0_svt[1].maximum = au0_maximum1;
    au0_svt[1].minimum = au0_minimum1;
    au0_svt[1].position = au0_position1;
    au0_svt[1].velocity = au0_velocity1;
    au0_svt[1].mode = au0_mode1;
    au0_svt[1].release = au0_release_spatial1;
    au0_lvt[1].commit = au0_commit1;
    for (i = 0; i < 2; ++i)
    {
        au0_devices[i].value.vt = &au0_dvt[i];
        au0_buffers[i].value.vt = &au0_bvt[i];
        au0_spatials[i].value.vt = &au0_svt[i];
        au0_listeners[i].value.vt = &au0_lvt[i];
    }
    au0_vector.value.x = au0_number(au0_captured[0]);
    au0_vector.value.y = au0_number(au0_captured[1]);
    au0_vector.value.z = au0_number(au0_captured[2]);
    memcpy(au0_expected_devices, au0_devices, sizeof(au0_devices));
    memcpy(au0_expected_buffers, au0_buffers, sizeof(au0_buffers));
    memcpy(au0_expected_spatials, au0_spatials, sizeof(au0_spatials));
    memcpy(au0_expected_listeners, au0_listeners, sizeof(au0_listeners));
    memcpy(&au0_expected_vector, &au0_vector, sizeof(au0_vector));
    memcpy(au0_expected_dvt, au0_dvt, sizeof(au0_dvt));
    memcpy(au0_expected_bvt, au0_bvt, sizeof(au0_bvt));
    memcpy(au0_expected_svt, au0_svt, sizeof(au0_svt));
    memcpy(au0_expected_lvt, au0_lvt, sizeof(au0_lvt));
    au0_expected_gate = op_audio_gate_word;
    au0_expected_listener = op_audio_listener;
    op_audio_co_initialize = au0_selected ? au0_coinit1 : au0_coinit0;
    op_audio_co_uninitialize = au0_selected ? au0_couninit1 : au0_couninit0;
    au0_calls = 0;
}
static int op_test_audio_com_initialize(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au0_selected = 0; au0_selected < 2; ++au0_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au0_mutate = 0; au0_mutate < 2; ++au0_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au0_listeners[au0_selected].value;
                            au0_word = results[pattern % 4];
                            au0_result_word = results[ri];
                            au0_minimum_bits = values[pattern];
                            au0_maximum_bits = values[(pattern + 3) % 8];
                            au0_captured[0] = values[pattern];
                            au0_captured[1] = values[(pattern + 1) % 8];
                            au0_captured[2] = values[(pattern + 2) % 8];
                            au0_seed();
                            au0_expected_this = 0;
                            au0_expected_count = 1;
                            au0_expected_codes[0] = 1;
                            au0_expected_codes[1] = 0;
                            AU0_CHECK((unsigned int)op_audio_com_initialize() == au0_result_word);
                            AU0_CHECK(au0_calls == au0_expected_count);
                            au0_verify();
                            AU0_CHECK(op_audio_co_initialize == (au0_selected ? au0_coinit1 : au0_coinit0));
                            AU0_CHECK(op_audio_co_uninitialize == (au0_selected ? au0_couninit1 : au0_couninit0));
                        }
    printf("audio_com_initialize: %d checks, %d failures\n", au0_checks, au0_failures);
    return au0_failures != 0;
}

#undef AU0_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au1_checks, au1_failures;
static void au1_check(int ok, int line)
{
    ++au1_checks;
    if (!ok)
    {
        ++au1_failures;
        printf("line%d failed\n", line);
    }
}
#define AU1_CHECK(x) au1_check(!!(x), __LINE__)
typedef struct AU1_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU1_OWNED_DEVICE;
typedef struct AU1_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU1_OWNED_BUFFER;
typedef struct AU1_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU1_OWNED_SPATIAL;
typedef struct AU1_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU1_OWNED_LISTENER;
typedef struct AU1_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU1_OWNED_VECTOR;
static AU1_OWNED_DEVICE au1_devices[2], au1_expected_devices[2];
static AU1_OWNED_BUFFER au1_buffers[2], au1_expected_buffers[2];
static AU1_OWNED_SPATIAL au1_spatials[2], au1_expected_spatials[2];
static AU1_OWNED_LISTENER au1_listeners[2], au1_expected_listeners[2];
static AU1_OWNED_VECTOR au1_vector, au1_expected_vector;
static OP_AUDIO_DEVICE_VT au1_dvt[2], au1_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au1_bvt[2], au1_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au1_svt[2], au1_expected_svt[2];
static OP_AUDIO_LISTENER_VT au1_lvt[2], au1_expected_lvt[2];
static unsigned int au1_selected, au1_word, au1_minimum_bits, au1_maximum_bits, au1_captured[3], au1_result_word,
    au1_expected_gate;
static OP_AUDIO_LISTENER *au1_expected_listener;
static void *au1_expected_this;
static int au1_calls, au1_expected_count, au1_expected_codes[2], au1_mutate;
static unsigned int au1_bits(float value)
{
    unsigned int au1_bits_word;
    memcpy(&au1_bits_word, &value, 4);
    return au1_bits_word;
}
static float au1_number(unsigned int au1_number_word)
{
    float value;
    memcpy(&value, &au1_number_word, 4);
    return value;
}
static void au1_verify(void)
{
    AU1_CHECK(memcmp(au1_devices, au1_expected_devices, sizeof(au1_devices)) == 0);
    AU1_CHECK(memcmp(au1_buffers, au1_expected_buffers, sizeof(au1_buffers)) == 0);
    AU1_CHECK(memcmp(au1_spatials, au1_expected_spatials, sizeof(au1_spatials)) == 0);
    AU1_CHECK(memcmp(au1_listeners, au1_expected_listeners, sizeof(au1_listeners)) == 0);
    AU1_CHECK(memcmp(&au1_vector, &au1_expected_vector, sizeof(au1_vector)) == 0);
    AU1_CHECK(memcmp(au1_dvt, au1_expected_dvt, sizeof(au1_dvt)) == 0);
    AU1_CHECK(memcmp(au1_bvt, au1_expected_bvt, sizeof(au1_bvt)) == 0);
    AU1_CHECK(memcmp(au1_svt, au1_expected_svt, sizeof(au1_svt)) == 0);
    AU1_CHECK(memcmp(au1_lvt, au1_expected_lvt, sizeof(au1_lvt)) == 0);
    AU1_CHECK(op_audio_gate_word == au1_expected_gate);
    AU1_CHECK(op_audio_listener == au1_expected_listener);
}
static void au1_observe(int code, unsigned int table, void *object)
{
    AU1_CHECK(au1_calls < au1_expected_count && au1_calls < 2);
    if (au1_calls >= au1_expected_count || au1_calls >= 2)
        exit(2);
    AU1_CHECK(code == au1_expected_codes[au1_calls]);
    AU1_CHECK(table == (au1_calls && au1_mutate ? 1 - au1_selected : au1_selected));
    AU1_CHECK(object == au1_expected_this);
    au1_verify();
    ++au1_calls;
}
static void au1_change(void)
{
    if (au1_mutate && au1_calls == 1)
    {
        op_audio_gate_word = au1_expected_gate = 0;
        au1_devices[au1_selected].value.vt = au1_expected_devices[au1_selected].value.vt = &au1_dvt[1 - au1_selected];
        au1_buffers[au1_selected].value.vt = au1_expected_buffers[au1_selected].value.vt = &au1_bvt[1 - au1_selected];
        au1_spatials[au1_selected].value.vt = au1_expected_spatials[au1_selected].value.vt = &au1_svt[1 - au1_selected];
        au1_listeners[au1_selected].value.vt = au1_expected_listeners[au1_selected].value.vt =
            &au1_lvt[1 - au1_selected];
        op_audio_listener = au1_expected_listener = &au1_listeners[1 - au1_selected].value;
        au1_vector.value.x = au1_expected_vector.value.x = 7;
        au1_vector.value.y = au1_expected_vector.value.y = 9;
        au1_vector.value.z = au1_expected_vector.value.z = 11;
    }
}
static int __stdcall au1_coinit0(void *reserved)
{
    au1_observe(1, 0, 0);
    AU1_CHECK(reserved == 0);
    au1_change();
    return (int)au1_result_word;
}
static void __stdcall au1_couninit0(void)
{
    au1_observe(2, 0, 0);
    au1_change();
}
static int __stdcall au1_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au1_observe(3, 0, o);
    AU1_CHECK(window == au1_word && flags == 2);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au1_observe(4, 0, o);
    AU1_CHECK(frequency == au1_word);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au1_observe(5, 0, o);
    AU1_CHECK(au1_bits(value) == au1_maximum_bits && flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au1_observe(6, 0, o);
    AU1_CHECK(au1_bits(value) == au1_minimum_bits && flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au1_observe(7, 0, o);
    AU1_CHECK(au1_bits(x) == au1_captured[0] && au1_bits(y) == au1_captured[1] && au1_bits(z) == au1_captured[2] &&
              flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au1_observe(8, 0, o);
    AU1_CHECK(au1_bits(x) == au1_captured[0] && au1_bits(y) == au1_captured[1] && au1_bits(z) == au1_captured[2] &&
              flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_commit0(OP_AUDIO_LISTENER *o)
{
    au1_observe(9, 0, o);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au1_observe(10, 0, o);
    AU1_CHECK(mode == au1_word && flags == 0);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_stop0(OP_AUDIO_BUFFER *o)
{
    au1_observe(11, 0, o);
    au1_change();
    return (int)au1_result_word;
}
static unsigned int __stdcall au1_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au1_observe(12, 0, o);
    au1_change();
    return (unsigned int)au1_result_word;
}
static unsigned int __stdcall au1_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au1_observe(13, 0, o);
    au1_change();
    return (unsigned int)au1_result_word;
}
static int __stdcall au1_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au1_observe(14, 0, o);
    AU1_CHECK(position == 0);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_coinit1(void *reserved)
{
    au1_observe(1, 1, 0);
    AU1_CHECK(reserved == 0);
    au1_change();
    return (int)au1_result_word;
}
static void __stdcall au1_couninit1(void)
{
    au1_observe(2, 1, 0);
    au1_change();
}
static int __stdcall au1_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au1_observe(3, 1, o);
    AU1_CHECK(window == au1_word && flags == 2);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au1_observe(4, 1, o);
    AU1_CHECK(frequency == au1_word);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au1_observe(5, 1, o);
    AU1_CHECK(au1_bits(value) == au1_maximum_bits && flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au1_observe(6, 1, o);
    AU1_CHECK(au1_bits(value) == au1_minimum_bits && flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au1_observe(7, 1, o);
    AU1_CHECK(au1_bits(x) == au1_captured[0] && au1_bits(y) == au1_captured[1] && au1_bits(z) == au1_captured[2] &&
              flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au1_observe(8, 1, o);
    AU1_CHECK(au1_bits(x) == au1_captured[0] && au1_bits(y) == au1_captured[1] && au1_bits(z) == au1_captured[2] &&
              flags == 1);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_commit1(OP_AUDIO_LISTENER *o)
{
    au1_observe(9, 1, o);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au1_observe(10, 1, o);
    AU1_CHECK(mode == au1_word && flags == 0);
    au1_change();
    return (int)au1_result_word;
}
static int __stdcall au1_stop1(OP_AUDIO_BUFFER *o)
{
    au1_observe(11, 1, o);
    au1_change();
    return (int)au1_result_word;
}
static unsigned int __stdcall au1_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au1_observe(12, 1, o);
    au1_change();
    return (unsigned int)au1_result_word;
}
static unsigned int __stdcall au1_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au1_observe(13, 1, o);
    au1_change();
    return (unsigned int)au1_result_word;
}
static int __stdcall au1_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au1_observe(14, 1, o);
    AU1_CHECK(position == 0);
    au1_change();
    return (int)au1_result_word;
}
static void au1_seed(void)
{
    unsigned int i;
    memset(au1_devices, 0x57, sizeof(au1_devices));
    memset(au1_buffers, 0x68, sizeof(au1_buffers));
    memset(au1_spatials, 0x79, sizeof(au1_spatials));
    memset(au1_listeners, 0x2a, sizeof(au1_listeners));
    memset(&au1_vector, 0x3b, sizeof(au1_vector));
    memset(au1_dvt, 0x4c, sizeof(au1_dvt));
    memset(au1_bvt, 0x5d, sizeof(au1_bvt));
    memset(au1_svt, 0x6e, sizeof(au1_svt));
    memset(au1_lvt, 0x7f, sizeof(au1_lvt));
    au1_dvt[0].cooperative = au1_coop0;
    au1_bvt[0].release = au1_release_buffer0;
    au1_bvt[0].current_position = au1_current0;
    au1_bvt[0].frequency = au1_frequency0;
    au1_bvt[0].stop = au1_stop0;
    au1_svt[0].maximum = au1_maximum0;
    au1_svt[0].minimum = au1_minimum0;
    au1_svt[0].position = au1_position0;
    au1_svt[0].velocity = au1_velocity0;
    au1_svt[0].mode = au1_mode0;
    au1_svt[0].release = au1_release_spatial0;
    au1_lvt[0].commit = au1_commit0;
    au1_dvt[1].cooperative = au1_coop1;
    au1_bvt[1].release = au1_release_buffer1;
    au1_bvt[1].current_position = au1_current1;
    au1_bvt[1].frequency = au1_frequency1;
    au1_bvt[1].stop = au1_stop1;
    au1_svt[1].maximum = au1_maximum1;
    au1_svt[1].minimum = au1_minimum1;
    au1_svt[1].position = au1_position1;
    au1_svt[1].velocity = au1_velocity1;
    au1_svt[1].mode = au1_mode1;
    au1_svt[1].release = au1_release_spatial1;
    au1_lvt[1].commit = au1_commit1;
    for (i = 0; i < 2; ++i)
    {
        au1_devices[i].value.vt = &au1_dvt[i];
        au1_buffers[i].value.vt = &au1_bvt[i];
        au1_spatials[i].value.vt = &au1_svt[i];
        au1_listeners[i].value.vt = &au1_lvt[i];
    }
    au1_vector.value.x = au1_number(au1_captured[0]);
    au1_vector.value.y = au1_number(au1_captured[1]);
    au1_vector.value.z = au1_number(au1_captured[2]);
    memcpy(au1_expected_devices, au1_devices, sizeof(au1_devices));
    memcpy(au1_expected_buffers, au1_buffers, sizeof(au1_buffers));
    memcpy(au1_expected_spatials, au1_spatials, sizeof(au1_spatials));
    memcpy(au1_expected_listeners, au1_listeners, sizeof(au1_listeners));
    memcpy(&au1_expected_vector, &au1_vector, sizeof(au1_vector));
    memcpy(au1_expected_dvt, au1_dvt, sizeof(au1_dvt));
    memcpy(au1_expected_bvt, au1_bvt, sizeof(au1_bvt));
    memcpy(au1_expected_svt, au1_svt, sizeof(au1_svt));
    memcpy(au1_expected_lvt, au1_lvt, sizeof(au1_lvt));
    au1_expected_gate = op_audio_gate_word;
    au1_expected_listener = op_audio_listener;
    op_audio_co_initialize = au1_selected ? au1_coinit1 : au1_coinit0;
    op_audio_co_uninitialize = au1_selected ? au1_couninit1 : au1_couninit0;
    au1_calls = 0;
}
static int op_test_audio_com_uninitialize(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au1_selected = 0; au1_selected < 2; ++au1_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au1_mutate = 0; au1_mutate < 2; ++au1_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au1_listeners[au1_selected].value;
                            au1_word = results[pattern % 4];
                            au1_result_word = results[ri];
                            au1_minimum_bits = values[pattern];
                            au1_maximum_bits = values[(pattern + 3) % 8];
                            au1_captured[0] = values[pattern];
                            au1_captured[1] = values[(pattern + 1) % 8];
                            au1_captured[2] = values[(pattern + 2) % 8];
                            au1_seed();
                            au1_expected_this = 0;
                            au1_expected_count = 1;
                            au1_expected_codes[0] = 2;
                            au1_expected_codes[1] = 0;
                            op_audio_com_uninitialize();
                            AU1_CHECK(au1_calls == au1_expected_count);
                            au1_verify();
                            AU1_CHECK(op_audio_co_initialize == (au1_selected ? au1_coinit1 : au1_coinit0));
                            AU1_CHECK(op_audio_co_uninitialize == (au1_selected ? au1_couninit1 : au1_couninit0));
                        }
    printf("audio_com_uninitialize: %d checks, %d failures\n", au1_checks, au1_failures);
    return au1_failures != 0;
}

#undef AU1_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au2_checks, au2_failures;
static void au2_check(int ok, int line)
{
    ++au2_checks;
    if (!ok)
    {
        ++au2_failures;
        printf("line%d failed\n", line);
    }
}
#define AU2_CHECK(x) au2_check(!!(x), __LINE__)
typedef struct AU2_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU2_OWNED_DEVICE;
typedef struct AU2_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU2_OWNED_BUFFER;
typedef struct AU2_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU2_OWNED_SPATIAL;
typedef struct AU2_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU2_OWNED_LISTENER;
typedef struct AU2_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU2_OWNED_VECTOR;
static AU2_OWNED_DEVICE au2_devices[2], au2_expected_devices[2];
static AU2_OWNED_BUFFER au2_buffers[2], au2_expected_buffers[2];
static AU2_OWNED_SPATIAL au2_spatials[2], au2_expected_spatials[2];
static AU2_OWNED_LISTENER au2_listeners[2], au2_expected_listeners[2];
static AU2_OWNED_VECTOR au2_vector, au2_expected_vector;
static OP_AUDIO_DEVICE_VT au2_dvt[2], au2_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au2_bvt[2], au2_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au2_svt[2], au2_expected_svt[2];
static OP_AUDIO_LISTENER_VT au2_lvt[2], au2_expected_lvt[2];
static unsigned int au2_selected, au2_word, au2_minimum_bits, au2_maximum_bits, au2_captured[3], au2_result_word,
    au2_expected_gate;
static OP_AUDIO_LISTENER *au2_expected_listener;
static void *au2_expected_this;
static int au2_calls, au2_expected_count, au2_expected_codes[2], au2_mutate;
static unsigned int au2_bits(float value)
{
    unsigned int au2_bits_word;
    memcpy(&au2_bits_word, &value, 4);
    return au2_bits_word;
}
static float au2_number(unsigned int au2_number_word)
{
    float value;
    memcpy(&value, &au2_number_word, 4);
    return value;
}
static void au2_verify(void)
{
    AU2_CHECK(memcmp(au2_devices, au2_expected_devices, sizeof(au2_devices)) == 0);
    AU2_CHECK(memcmp(au2_buffers, au2_expected_buffers, sizeof(au2_buffers)) == 0);
    AU2_CHECK(memcmp(au2_spatials, au2_expected_spatials, sizeof(au2_spatials)) == 0);
    AU2_CHECK(memcmp(au2_listeners, au2_expected_listeners, sizeof(au2_listeners)) == 0);
    AU2_CHECK(memcmp(&au2_vector, &au2_expected_vector, sizeof(au2_vector)) == 0);
    AU2_CHECK(memcmp(au2_dvt, au2_expected_dvt, sizeof(au2_dvt)) == 0);
    AU2_CHECK(memcmp(au2_bvt, au2_expected_bvt, sizeof(au2_bvt)) == 0);
    AU2_CHECK(memcmp(au2_svt, au2_expected_svt, sizeof(au2_svt)) == 0);
    AU2_CHECK(memcmp(au2_lvt, au2_expected_lvt, sizeof(au2_lvt)) == 0);
    AU2_CHECK(op_audio_gate_word == au2_expected_gate);
    AU2_CHECK(op_audio_listener == au2_expected_listener);
}
static void au2_observe(int code, unsigned int table, void *object)
{
    AU2_CHECK(au2_calls < au2_expected_count && au2_calls < 2);
    if (au2_calls >= au2_expected_count || au2_calls >= 2)
        exit(2);
    AU2_CHECK(code == au2_expected_codes[au2_calls]);
    AU2_CHECK(table == (au2_calls && au2_mutate ? 1 - au2_selected : au2_selected));
    AU2_CHECK(object == au2_expected_this);
    au2_verify();
    ++au2_calls;
}
static void au2_change(void)
{
    if (au2_mutate && au2_calls == 1)
    {
        op_audio_gate_word = au2_expected_gate = 0;
        au2_devices[au2_selected].value.vt = au2_expected_devices[au2_selected].value.vt = &au2_dvt[1 - au2_selected];
        au2_buffers[au2_selected].value.vt = au2_expected_buffers[au2_selected].value.vt = &au2_bvt[1 - au2_selected];
        au2_spatials[au2_selected].value.vt = au2_expected_spatials[au2_selected].value.vt = &au2_svt[1 - au2_selected];
        au2_listeners[au2_selected].value.vt = au2_expected_listeners[au2_selected].value.vt =
            &au2_lvt[1 - au2_selected];
        op_audio_listener = au2_expected_listener = &au2_listeners[1 - au2_selected].value;
        au2_vector.value.x = au2_expected_vector.value.x = 7;
        au2_vector.value.y = au2_expected_vector.value.y = 9;
        au2_vector.value.z = au2_expected_vector.value.z = 11;
    }
}
static int __stdcall au2_coinit0(void *reserved)
{
    au2_observe(1, 0, 0);
    AU2_CHECK(reserved == 0);
    au2_change();
    return (int)au2_result_word;
}
static void __stdcall au2_couninit0(void)
{
    au2_observe(2, 0, 0);
    au2_change();
}
static int __stdcall au2_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au2_observe(3, 0, o);
    AU2_CHECK(window == au2_word && flags == 2);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au2_observe(4, 0, o);
    AU2_CHECK(frequency == au2_word);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au2_observe(5, 0, o);
    AU2_CHECK(au2_bits(value) == au2_maximum_bits && flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au2_observe(6, 0, o);
    AU2_CHECK(au2_bits(value) == au2_minimum_bits && flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au2_observe(7, 0, o);
    AU2_CHECK(au2_bits(x) == au2_captured[0] && au2_bits(y) == au2_captured[1] && au2_bits(z) == au2_captured[2] &&
              flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au2_observe(8, 0, o);
    AU2_CHECK(au2_bits(x) == au2_captured[0] && au2_bits(y) == au2_captured[1] && au2_bits(z) == au2_captured[2] &&
              flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_commit0(OP_AUDIO_LISTENER *o)
{
    au2_observe(9, 0, o);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au2_observe(10, 0, o);
    AU2_CHECK(mode == au2_word && flags == 0);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_stop0(OP_AUDIO_BUFFER *o)
{
    au2_observe(11, 0, o);
    au2_change();
    return (int)au2_result_word;
}
static unsigned int __stdcall au2_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au2_observe(12, 0, o);
    au2_change();
    return (unsigned int)au2_result_word;
}
static unsigned int __stdcall au2_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au2_observe(13, 0, o);
    au2_change();
    return (unsigned int)au2_result_word;
}
static int __stdcall au2_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au2_observe(14, 0, o);
    AU2_CHECK(position == 0);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_coinit1(void *reserved)
{
    au2_observe(1, 1, 0);
    AU2_CHECK(reserved == 0);
    au2_change();
    return (int)au2_result_word;
}
static void __stdcall au2_couninit1(void)
{
    au2_observe(2, 1, 0);
    au2_change();
}
static int __stdcall au2_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au2_observe(3, 1, o);
    AU2_CHECK(window == au2_word && flags == 2);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au2_observe(4, 1, o);
    AU2_CHECK(frequency == au2_word);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au2_observe(5, 1, o);
    AU2_CHECK(au2_bits(value) == au2_maximum_bits && flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au2_observe(6, 1, o);
    AU2_CHECK(au2_bits(value) == au2_minimum_bits && flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au2_observe(7, 1, o);
    AU2_CHECK(au2_bits(x) == au2_captured[0] && au2_bits(y) == au2_captured[1] && au2_bits(z) == au2_captured[2] &&
              flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au2_observe(8, 1, o);
    AU2_CHECK(au2_bits(x) == au2_captured[0] && au2_bits(y) == au2_captured[1] && au2_bits(z) == au2_captured[2] &&
              flags == 1);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_commit1(OP_AUDIO_LISTENER *o)
{
    au2_observe(9, 1, o);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au2_observe(10, 1, o);
    AU2_CHECK(mode == au2_word && flags == 0);
    au2_change();
    return (int)au2_result_word;
}
static int __stdcall au2_stop1(OP_AUDIO_BUFFER *o)
{
    au2_observe(11, 1, o);
    au2_change();
    return (int)au2_result_word;
}
static unsigned int __stdcall au2_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au2_observe(12, 1, o);
    au2_change();
    return (unsigned int)au2_result_word;
}
static unsigned int __stdcall au2_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au2_observe(13, 1, o);
    au2_change();
    return (unsigned int)au2_result_word;
}
static int __stdcall au2_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au2_observe(14, 1, o);
    AU2_CHECK(position == 0);
    au2_change();
    return (int)au2_result_word;
}
static void au2_seed(void)
{
    unsigned int i;
    memset(au2_devices, 0x57, sizeof(au2_devices));
    memset(au2_buffers, 0x68, sizeof(au2_buffers));
    memset(au2_spatials, 0x79, sizeof(au2_spatials));
    memset(au2_listeners, 0x2a, sizeof(au2_listeners));
    memset(&au2_vector, 0x3b, sizeof(au2_vector));
    memset(au2_dvt, 0x4c, sizeof(au2_dvt));
    memset(au2_bvt, 0x5d, sizeof(au2_bvt));
    memset(au2_svt, 0x6e, sizeof(au2_svt));
    memset(au2_lvt, 0x7f, sizeof(au2_lvt));
    au2_dvt[0].cooperative = au2_coop0;
    au2_bvt[0].release = au2_release_buffer0;
    au2_bvt[0].current_position = au2_current0;
    au2_bvt[0].frequency = au2_frequency0;
    au2_bvt[0].stop = au2_stop0;
    au2_svt[0].maximum = au2_maximum0;
    au2_svt[0].minimum = au2_minimum0;
    au2_svt[0].position = au2_position0;
    au2_svt[0].velocity = au2_velocity0;
    au2_svt[0].mode = au2_mode0;
    au2_svt[0].release = au2_release_spatial0;
    au2_lvt[0].commit = au2_commit0;
    au2_dvt[1].cooperative = au2_coop1;
    au2_bvt[1].release = au2_release_buffer1;
    au2_bvt[1].current_position = au2_current1;
    au2_bvt[1].frequency = au2_frequency1;
    au2_bvt[1].stop = au2_stop1;
    au2_svt[1].maximum = au2_maximum1;
    au2_svt[1].minimum = au2_minimum1;
    au2_svt[1].position = au2_position1;
    au2_svt[1].velocity = au2_velocity1;
    au2_svt[1].mode = au2_mode1;
    au2_svt[1].release = au2_release_spatial1;
    au2_lvt[1].commit = au2_commit1;
    for (i = 0; i < 2; ++i)
    {
        au2_devices[i].value.vt = &au2_dvt[i];
        au2_buffers[i].value.vt = &au2_bvt[i];
        au2_spatials[i].value.vt = &au2_svt[i];
        au2_listeners[i].value.vt = &au2_lvt[i];
    }
    au2_vector.value.x = au2_number(au2_captured[0]);
    au2_vector.value.y = au2_number(au2_captured[1]);
    au2_vector.value.z = au2_number(au2_captured[2]);
    memcpy(au2_expected_devices, au2_devices, sizeof(au2_devices));
    memcpy(au2_expected_buffers, au2_buffers, sizeof(au2_buffers));
    memcpy(au2_expected_spatials, au2_spatials, sizeof(au2_spatials));
    memcpy(au2_expected_listeners, au2_listeners, sizeof(au2_listeners));
    memcpy(&au2_expected_vector, &au2_vector, sizeof(au2_vector));
    memcpy(au2_expected_dvt, au2_dvt, sizeof(au2_dvt));
    memcpy(au2_expected_bvt, au2_bvt, sizeof(au2_bvt));
    memcpy(au2_expected_svt, au2_svt, sizeof(au2_svt));
    memcpy(au2_expected_lvt, au2_lvt, sizeof(au2_lvt));
    au2_expected_gate = op_audio_gate_word;
    au2_expected_listener = op_audio_listener;
    op_audio_co_initialize = au2_selected ? au2_coinit1 : au2_coinit0;
    op_audio_co_uninitialize = au2_selected ? au2_couninit1 : au2_couninit0;
    au2_calls = 0;
}
static int op_test_audio_set_cooperative(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au2_selected = 0; au2_selected < 2; ++au2_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au2_mutate = 0; au2_mutate < 2; ++au2_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au2_listeners[au2_selected].value;
                            au2_word = results[pattern % 4];
                            au2_result_word = results[ri];
                            au2_minimum_bits = values[pattern];
                            au2_maximum_bits = values[(pattern + 3) % 8];
                            au2_captured[0] = values[pattern];
                            au2_captured[1] = values[(pattern + 1) % 8];
                            au2_captured[2] = values[(pattern + 2) % 8];
                            au2_seed();
                            au2_expected_this = &au2_devices[au2_selected].value;
                            au2_expected_count = 1;
                            au2_expected_codes[0] = 3;
                            au2_expected_codes[1] = 0;
                            op_audio_set_cooperative(&au2_devices[au2_selected].value, au2_word);
                            AU2_CHECK(au2_calls == au2_expected_count);
                            au2_verify();
                            AU2_CHECK(op_audio_co_initialize == (au2_selected ? au2_coinit1 : au2_coinit0));
                            AU2_CHECK(op_audio_co_uninitialize == (au2_selected ? au2_couninit1 : au2_couninit0));
                        }
    printf("audio_set_cooperative: %d checks, %d failures\n", au2_checks, au2_failures);
    return au2_failures != 0;
}

#undef AU2_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au3_checks, au3_failures;
static void au3_check(int ok, int line)
{
    ++au3_checks;
    if (!ok)
    {
        ++au3_failures;
        printf("line%d failed\n", line);
    }
}
#define AU3_CHECK(x) au3_check(!!(x), __LINE__)
typedef struct AU3_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU3_OWNED_DEVICE;
typedef struct AU3_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU3_OWNED_BUFFER;
typedef struct AU3_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU3_OWNED_SPATIAL;
typedef struct AU3_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU3_OWNED_LISTENER;
typedef struct AU3_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU3_OWNED_VECTOR;
static AU3_OWNED_DEVICE au3_devices[2], au3_expected_devices[2];
static AU3_OWNED_BUFFER au3_buffers[2], au3_expected_buffers[2];
static AU3_OWNED_SPATIAL au3_spatials[2], au3_expected_spatials[2];
static AU3_OWNED_LISTENER au3_listeners[2], au3_expected_listeners[2];
static AU3_OWNED_VECTOR au3_vector, au3_expected_vector;
static OP_AUDIO_DEVICE_VT au3_dvt[2], au3_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au3_bvt[2], au3_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au3_svt[2], au3_expected_svt[2];
static OP_AUDIO_LISTENER_VT au3_lvt[2], au3_expected_lvt[2];
static unsigned int au3_selected, au3_word, au3_minimum_bits, au3_maximum_bits, au3_captured[3], au3_result_word,
    au3_expected_gate;
static OP_AUDIO_LISTENER *au3_expected_listener;
static void *au3_expected_this;
static int au3_calls, au3_expected_count, au3_expected_codes[2], au3_mutate;
static unsigned int au3_bits(float value)
{
    unsigned int au3_bits_word;
    memcpy(&au3_bits_word, &value, 4);
    return au3_bits_word;
}
static float au3_number(unsigned int au3_number_word)
{
    float value;
    memcpy(&value, &au3_number_word, 4);
    return value;
}
static void au3_verify(void)
{
    AU3_CHECK(memcmp(au3_devices, au3_expected_devices, sizeof(au3_devices)) == 0);
    AU3_CHECK(memcmp(au3_buffers, au3_expected_buffers, sizeof(au3_buffers)) == 0);
    AU3_CHECK(memcmp(au3_spatials, au3_expected_spatials, sizeof(au3_spatials)) == 0);
    AU3_CHECK(memcmp(au3_listeners, au3_expected_listeners, sizeof(au3_listeners)) == 0);
    AU3_CHECK(memcmp(&au3_vector, &au3_expected_vector, sizeof(au3_vector)) == 0);
    AU3_CHECK(memcmp(au3_dvt, au3_expected_dvt, sizeof(au3_dvt)) == 0);
    AU3_CHECK(memcmp(au3_bvt, au3_expected_bvt, sizeof(au3_bvt)) == 0);
    AU3_CHECK(memcmp(au3_svt, au3_expected_svt, sizeof(au3_svt)) == 0);
    AU3_CHECK(memcmp(au3_lvt, au3_expected_lvt, sizeof(au3_lvt)) == 0);
    AU3_CHECK(op_audio_gate_word == au3_expected_gate);
    AU3_CHECK(op_audio_listener == au3_expected_listener);
}
static void au3_observe(int code, unsigned int table, void *object)
{
    AU3_CHECK(au3_calls < au3_expected_count && au3_calls < 2);
    if (au3_calls >= au3_expected_count || au3_calls >= 2)
        exit(2);
    AU3_CHECK(code == au3_expected_codes[au3_calls]);
    AU3_CHECK(table == (au3_calls && au3_mutate ? 1 - au3_selected : au3_selected));
    AU3_CHECK(object == au3_expected_this);
    au3_verify();
    ++au3_calls;
}
static void au3_change(void)
{
    if (au3_mutate && au3_calls == 1)
    {
        op_audio_gate_word = au3_expected_gate = 0;
        au3_devices[au3_selected].value.vt = au3_expected_devices[au3_selected].value.vt = &au3_dvt[1 - au3_selected];
        au3_buffers[au3_selected].value.vt = au3_expected_buffers[au3_selected].value.vt = &au3_bvt[1 - au3_selected];
        au3_spatials[au3_selected].value.vt = au3_expected_spatials[au3_selected].value.vt = &au3_svt[1 - au3_selected];
        au3_listeners[au3_selected].value.vt = au3_expected_listeners[au3_selected].value.vt =
            &au3_lvt[1 - au3_selected];
        op_audio_listener = au3_expected_listener = &au3_listeners[1 - au3_selected].value;
        au3_vector.value.x = au3_expected_vector.value.x = 7;
        au3_vector.value.y = au3_expected_vector.value.y = 9;
        au3_vector.value.z = au3_expected_vector.value.z = 11;
    }
}
static int __stdcall au3_coinit0(void *reserved)
{
    au3_observe(1, 0, 0);
    AU3_CHECK(reserved == 0);
    au3_change();
    return (int)au3_result_word;
}
static void __stdcall au3_couninit0(void)
{
    au3_observe(2, 0, 0);
    au3_change();
}
static int __stdcall au3_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au3_observe(3, 0, o);
    AU3_CHECK(window == au3_word && flags == 2);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au3_observe(4, 0, o);
    AU3_CHECK(frequency == au3_word);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au3_observe(5, 0, o);
    AU3_CHECK(au3_bits(value) == au3_maximum_bits && flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au3_observe(6, 0, o);
    AU3_CHECK(au3_bits(value) == au3_minimum_bits && flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au3_observe(7, 0, o);
    AU3_CHECK(au3_bits(x) == au3_captured[0] && au3_bits(y) == au3_captured[1] && au3_bits(z) == au3_captured[2] &&
              flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au3_observe(8, 0, o);
    AU3_CHECK(au3_bits(x) == au3_captured[0] && au3_bits(y) == au3_captured[1] && au3_bits(z) == au3_captured[2] &&
              flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_commit0(OP_AUDIO_LISTENER *o)
{
    au3_observe(9, 0, o);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au3_observe(10, 0, o);
    AU3_CHECK(mode == au3_word && flags == 0);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_stop0(OP_AUDIO_BUFFER *o)
{
    au3_observe(11, 0, o);
    au3_change();
    return (int)au3_result_word;
}
static unsigned int __stdcall au3_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au3_observe(12, 0, o);
    au3_change();
    return (unsigned int)au3_result_word;
}
static unsigned int __stdcall au3_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au3_observe(13, 0, o);
    au3_change();
    return (unsigned int)au3_result_word;
}
static int __stdcall au3_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au3_observe(14, 0, o);
    AU3_CHECK(position == 0);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_coinit1(void *reserved)
{
    au3_observe(1, 1, 0);
    AU3_CHECK(reserved == 0);
    au3_change();
    return (int)au3_result_word;
}
static void __stdcall au3_couninit1(void)
{
    au3_observe(2, 1, 0);
    au3_change();
}
static int __stdcall au3_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au3_observe(3, 1, o);
    AU3_CHECK(window == au3_word && flags == 2);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au3_observe(4, 1, o);
    AU3_CHECK(frequency == au3_word);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au3_observe(5, 1, o);
    AU3_CHECK(au3_bits(value) == au3_maximum_bits && flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au3_observe(6, 1, o);
    AU3_CHECK(au3_bits(value) == au3_minimum_bits && flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au3_observe(7, 1, o);
    AU3_CHECK(au3_bits(x) == au3_captured[0] && au3_bits(y) == au3_captured[1] && au3_bits(z) == au3_captured[2] &&
              flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au3_observe(8, 1, o);
    AU3_CHECK(au3_bits(x) == au3_captured[0] && au3_bits(y) == au3_captured[1] && au3_bits(z) == au3_captured[2] &&
              flags == 1);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_commit1(OP_AUDIO_LISTENER *o)
{
    au3_observe(9, 1, o);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au3_observe(10, 1, o);
    AU3_CHECK(mode == au3_word && flags == 0);
    au3_change();
    return (int)au3_result_word;
}
static int __stdcall au3_stop1(OP_AUDIO_BUFFER *o)
{
    au3_observe(11, 1, o);
    au3_change();
    return (int)au3_result_word;
}
static unsigned int __stdcall au3_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au3_observe(12, 1, o);
    au3_change();
    return (unsigned int)au3_result_word;
}
static unsigned int __stdcall au3_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au3_observe(13, 1, o);
    au3_change();
    return (unsigned int)au3_result_word;
}
static int __stdcall au3_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au3_observe(14, 1, o);
    AU3_CHECK(position == 0);
    au3_change();
    return (int)au3_result_word;
}
static void au3_seed(void)
{
    unsigned int i;
    memset(au3_devices, 0x57, sizeof(au3_devices));
    memset(au3_buffers, 0x68, sizeof(au3_buffers));
    memset(au3_spatials, 0x79, sizeof(au3_spatials));
    memset(au3_listeners, 0x2a, sizeof(au3_listeners));
    memset(&au3_vector, 0x3b, sizeof(au3_vector));
    memset(au3_dvt, 0x4c, sizeof(au3_dvt));
    memset(au3_bvt, 0x5d, sizeof(au3_bvt));
    memset(au3_svt, 0x6e, sizeof(au3_svt));
    memset(au3_lvt, 0x7f, sizeof(au3_lvt));
    au3_dvt[0].cooperative = au3_coop0;
    au3_bvt[0].release = au3_release_buffer0;
    au3_bvt[0].current_position = au3_current0;
    au3_bvt[0].frequency = au3_frequency0;
    au3_bvt[0].stop = au3_stop0;
    au3_svt[0].maximum = au3_maximum0;
    au3_svt[0].minimum = au3_minimum0;
    au3_svt[0].position = au3_position0;
    au3_svt[0].velocity = au3_velocity0;
    au3_svt[0].mode = au3_mode0;
    au3_svt[0].release = au3_release_spatial0;
    au3_lvt[0].commit = au3_commit0;
    au3_dvt[1].cooperative = au3_coop1;
    au3_bvt[1].release = au3_release_buffer1;
    au3_bvt[1].current_position = au3_current1;
    au3_bvt[1].frequency = au3_frequency1;
    au3_bvt[1].stop = au3_stop1;
    au3_svt[1].maximum = au3_maximum1;
    au3_svt[1].minimum = au3_minimum1;
    au3_svt[1].position = au3_position1;
    au3_svt[1].velocity = au3_velocity1;
    au3_svt[1].mode = au3_mode1;
    au3_svt[1].release = au3_release_spatial1;
    au3_lvt[1].commit = au3_commit1;
    for (i = 0; i < 2; ++i)
    {
        au3_devices[i].value.vt = &au3_dvt[i];
        au3_buffers[i].value.vt = &au3_bvt[i];
        au3_spatials[i].value.vt = &au3_svt[i];
        au3_listeners[i].value.vt = &au3_lvt[i];
    }
    au3_vector.value.x = au3_number(au3_captured[0]);
    au3_vector.value.y = au3_number(au3_captured[1]);
    au3_vector.value.z = au3_number(au3_captured[2]);
    memcpy(au3_expected_devices, au3_devices, sizeof(au3_devices));
    memcpy(au3_expected_buffers, au3_buffers, sizeof(au3_buffers));
    memcpy(au3_expected_spatials, au3_spatials, sizeof(au3_spatials));
    memcpy(au3_expected_listeners, au3_listeners, sizeof(au3_listeners));
    memcpy(&au3_expected_vector, &au3_vector, sizeof(au3_vector));
    memcpy(au3_expected_dvt, au3_dvt, sizeof(au3_dvt));
    memcpy(au3_expected_bvt, au3_bvt, sizeof(au3_bvt));
    memcpy(au3_expected_svt, au3_svt, sizeof(au3_svt));
    memcpy(au3_expected_lvt, au3_lvt, sizeof(au3_lvt));
    au3_expected_gate = op_audio_gate_word;
    au3_expected_listener = op_audio_listener;
    op_audio_co_initialize = au3_selected ? au3_coinit1 : au3_coinit0;
    op_audio_co_uninitialize = au3_selected ? au3_couninit1 : au3_couninit0;
    au3_calls = 0;
}
static int op_test_audio_set_frequency(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au3_selected = 0; au3_selected < 2; ++au3_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au3_mutate = 0; au3_mutate < 2; ++au3_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            if (gate && null_pointer)
                                continue;
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au3_listeners[au3_selected].value;
                            au3_word = results[pattern % 4];
                            au3_result_word = results[ri];
                            au3_minimum_bits = values[pattern];
                            au3_maximum_bits = values[(pattern + 3) % 8];
                            au3_captured[0] = values[pattern];
                            au3_captured[1] = values[(pattern + 1) % 8];
                            au3_captured[2] = values[(pattern + 2) % 8];
                            au3_seed();
                            au3_expected_this = null_pointer ? 0 : &au3_buffers[au3_selected].value;
                            au3_expected_count = gate != 0;
                            au3_expected_codes[0] = 4;
                            au3_expected_codes[1] = 0;
                            op_audio_set_frequency(null_pointer ? 0 : &au3_buffers[au3_selected].value, au3_word);
                            AU3_CHECK(au3_calls == au3_expected_count);
                            au3_verify();
                            AU3_CHECK(op_audio_co_initialize == (au3_selected ? au3_coinit1 : au3_coinit0));
                            AU3_CHECK(op_audio_co_uninitialize == (au3_selected ? au3_couninit1 : au3_couninit0));
                        }
    printf("audio_set_frequency: %d checks, %d failures\n", au3_checks, au3_failures);
    return au3_failures != 0;
}

#undef AU3_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au4_checks, au4_failures;
static void au4_check(int ok, int line)
{
    ++au4_checks;
    if (!ok)
    {
        ++au4_failures;
        printf("line%d failed\n", line);
    }
}
#define AU4_CHECK(x) au4_check(!!(x), __LINE__)
typedef struct AU4_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU4_OWNED_DEVICE;
typedef struct AU4_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU4_OWNED_BUFFER;
typedef struct AU4_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU4_OWNED_SPATIAL;
typedef struct AU4_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU4_OWNED_LISTENER;
typedef struct AU4_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU4_OWNED_VECTOR;
static AU4_OWNED_DEVICE au4_devices[2], au4_expected_devices[2];
static AU4_OWNED_BUFFER au4_buffers[2], au4_expected_buffers[2];
static AU4_OWNED_SPATIAL au4_spatials[2], au4_expected_spatials[2];
static AU4_OWNED_LISTENER au4_listeners[2], au4_expected_listeners[2];
static AU4_OWNED_VECTOR au4_vector, au4_expected_vector;
static OP_AUDIO_DEVICE_VT au4_dvt[2], au4_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au4_bvt[2], au4_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au4_svt[2], au4_expected_svt[2];
static OP_AUDIO_LISTENER_VT au4_lvt[2], au4_expected_lvt[2];
static unsigned int au4_selected, au4_word, au4_minimum_bits, au4_maximum_bits, au4_captured[3], au4_result_word,
    au4_expected_gate;
static OP_AUDIO_LISTENER *au4_expected_listener;
static void *au4_expected_this;
static int au4_calls, au4_expected_count, au4_expected_codes[2], au4_mutate;
static unsigned int au4_bits(float value)
{
    unsigned int au4_bits_word;
    memcpy(&au4_bits_word, &value, 4);
    return au4_bits_word;
}
static float au4_number(unsigned int au4_number_word)
{
    float value;
    memcpy(&value, &au4_number_word, 4);
    return value;
}
static void au4_verify(void)
{
    AU4_CHECK(memcmp(au4_devices, au4_expected_devices, sizeof(au4_devices)) == 0);
    AU4_CHECK(memcmp(au4_buffers, au4_expected_buffers, sizeof(au4_buffers)) == 0);
    AU4_CHECK(memcmp(au4_spatials, au4_expected_spatials, sizeof(au4_spatials)) == 0);
    AU4_CHECK(memcmp(au4_listeners, au4_expected_listeners, sizeof(au4_listeners)) == 0);
    AU4_CHECK(memcmp(&au4_vector, &au4_expected_vector, sizeof(au4_vector)) == 0);
    AU4_CHECK(memcmp(au4_dvt, au4_expected_dvt, sizeof(au4_dvt)) == 0);
    AU4_CHECK(memcmp(au4_bvt, au4_expected_bvt, sizeof(au4_bvt)) == 0);
    AU4_CHECK(memcmp(au4_svt, au4_expected_svt, sizeof(au4_svt)) == 0);
    AU4_CHECK(memcmp(au4_lvt, au4_expected_lvt, sizeof(au4_lvt)) == 0);
    AU4_CHECK(op_audio_gate_word == au4_expected_gate);
    AU4_CHECK(op_audio_listener == au4_expected_listener);
}
static void au4_observe(int code, unsigned int table, void *object)
{
    AU4_CHECK(au4_calls < au4_expected_count && au4_calls < 2);
    if (au4_calls >= au4_expected_count || au4_calls >= 2)
        exit(2);
    AU4_CHECK(code == au4_expected_codes[au4_calls]);
    AU4_CHECK(table == (au4_calls && au4_mutate ? 1 - au4_selected : au4_selected));
    AU4_CHECK(object == au4_expected_this);
    au4_verify();
    ++au4_calls;
}
static void au4_change(void)
{
    if (au4_mutate && au4_calls == 1)
    {
        op_audio_gate_word = au4_expected_gate = 0;
        au4_devices[au4_selected].value.vt = au4_expected_devices[au4_selected].value.vt = &au4_dvt[1 - au4_selected];
        au4_buffers[au4_selected].value.vt = au4_expected_buffers[au4_selected].value.vt = &au4_bvt[1 - au4_selected];
        au4_spatials[au4_selected].value.vt = au4_expected_spatials[au4_selected].value.vt = &au4_svt[1 - au4_selected];
        au4_listeners[au4_selected].value.vt = au4_expected_listeners[au4_selected].value.vt =
            &au4_lvt[1 - au4_selected];
        op_audio_listener = au4_expected_listener = &au4_listeners[1 - au4_selected].value;
        au4_vector.value.x = au4_expected_vector.value.x = 7;
        au4_vector.value.y = au4_expected_vector.value.y = 9;
        au4_vector.value.z = au4_expected_vector.value.z = 11;
    }
}
static int __stdcall au4_coinit0(void *reserved)
{
    au4_observe(1, 0, 0);
    AU4_CHECK(reserved == 0);
    au4_change();
    return (int)au4_result_word;
}
static void __stdcall au4_couninit0(void)
{
    au4_observe(2, 0, 0);
    au4_change();
}
static int __stdcall au4_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au4_observe(3, 0, o);
    AU4_CHECK(window == au4_word && flags == 2);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au4_observe(4, 0, o);
    AU4_CHECK(frequency == au4_word);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au4_observe(5, 0, o);
    AU4_CHECK(au4_bits(value) == au4_maximum_bits && flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au4_observe(6, 0, o);
    AU4_CHECK(au4_bits(value) == au4_minimum_bits && flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au4_observe(7, 0, o);
    AU4_CHECK(au4_bits(x) == au4_captured[0] && au4_bits(y) == au4_captured[1] && au4_bits(z) == au4_captured[2] &&
              flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au4_observe(8, 0, o);
    AU4_CHECK(au4_bits(x) == au4_captured[0] && au4_bits(y) == au4_captured[1] && au4_bits(z) == au4_captured[2] &&
              flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_commit0(OP_AUDIO_LISTENER *o)
{
    au4_observe(9, 0, o);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au4_observe(10, 0, o);
    AU4_CHECK(mode == au4_word && flags == 0);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_stop0(OP_AUDIO_BUFFER *o)
{
    au4_observe(11, 0, o);
    au4_change();
    return (int)au4_result_word;
}
static unsigned int __stdcall au4_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au4_observe(12, 0, o);
    au4_change();
    return (unsigned int)au4_result_word;
}
static unsigned int __stdcall au4_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au4_observe(13, 0, o);
    au4_change();
    return (unsigned int)au4_result_word;
}
static int __stdcall au4_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au4_observe(14, 0, o);
    AU4_CHECK(position == 0);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_coinit1(void *reserved)
{
    au4_observe(1, 1, 0);
    AU4_CHECK(reserved == 0);
    au4_change();
    return (int)au4_result_word;
}
static void __stdcall au4_couninit1(void)
{
    au4_observe(2, 1, 0);
    au4_change();
}
static int __stdcall au4_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au4_observe(3, 1, o);
    AU4_CHECK(window == au4_word && flags == 2);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au4_observe(4, 1, o);
    AU4_CHECK(frequency == au4_word);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au4_observe(5, 1, o);
    AU4_CHECK(au4_bits(value) == au4_maximum_bits && flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au4_observe(6, 1, o);
    AU4_CHECK(au4_bits(value) == au4_minimum_bits && flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au4_observe(7, 1, o);
    AU4_CHECK(au4_bits(x) == au4_captured[0] && au4_bits(y) == au4_captured[1] && au4_bits(z) == au4_captured[2] &&
              flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au4_observe(8, 1, o);
    AU4_CHECK(au4_bits(x) == au4_captured[0] && au4_bits(y) == au4_captured[1] && au4_bits(z) == au4_captured[2] &&
              flags == 1);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_commit1(OP_AUDIO_LISTENER *o)
{
    au4_observe(9, 1, o);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au4_observe(10, 1, o);
    AU4_CHECK(mode == au4_word && flags == 0);
    au4_change();
    return (int)au4_result_word;
}
static int __stdcall au4_stop1(OP_AUDIO_BUFFER *o)
{
    au4_observe(11, 1, o);
    au4_change();
    return (int)au4_result_word;
}
static unsigned int __stdcall au4_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au4_observe(12, 1, o);
    au4_change();
    return (unsigned int)au4_result_word;
}
static unsigned int __stdcall au4_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au4_observe(13, 1, o);
    au4_change();
    return (unsigned int)au4_result_word;
}
static int __stdcall au4_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au4_observe(14, 1, o);
    AU4_CHECK(position == 0);
    au4_change();
    return (int)au4_result_word;
}
static void au4_seed(void)
{
    unsigned int i;
    memset(au4_devices, 0x57, sizeof(au4_devices));
    memset(au4_buffers, 0x68, sizeof(au4_buffers));
    memset(au4_spatials, 0x79, sizeof(au4_spatials));
    memset(au4_listeners, 0x2a, sizeof(au4_listeners));
    memset(&au4_vector, 0x3b, sizeof(au4_vector));
    memset(au4_dvt, 0x4c, sizeof(au4_dvt));
    memset(au4_bvt, 0x5d, sizeof(au4_bvt));
    memset(au4_svt, 0x6e, sizeof(au4_svt));
    memset(au4_lvt, 0x7f, sizeof(au4_lvt));
    au4_dvt[0].cooperative = au4_coop0;
    au4_bvt[0].release = au4_release_buffer0;
    au4_bvt[0].current_position = au4_current0;
    au4_bvt[0].frequency = au4_frequency0;
    au4_bvt[0].stop = au4_stop0;
    au4_svt[0].maximum = au4_maximum0;
    au4_svt[0].minimum = au4_minimum0;
    au4_svt[0].position = au4_position0;
    au4_svt[0].velocity = au4_velocity0;
    au4_svt[0].mode = au4_mode0;
    au4_svt[0].release = au4_release_spatial0;
    au4_lvt[0].commit = au4_commit0;
    au4_dvt[1].cooperative = au4_coop1;
    au4_bvt[1].release = au4_release_buffer1;
    au4_bvt[1].current_position = au4_current1;
    au4_bvt[1].frequency = au4_frequency1;
    au4_bvt[1].stop = au4_stop1;
    au4_svt[1].maximum = au4_maximum1;
    au4_svt[1].minimum = au4_minimum1;
    au4_svt[1].position = au4_position1;
    au4_svt[1].velocity = au4_velocity1;
    au4_svt[1].mode = au4_mode1;
    au4_svt[1].release = au4_release_spatial1;
    au4_lvt[1].commit = au4_commit1;
    for (i = 0; i < 2; ++i)
    {
        au4_devices[i].value.vt = &au4_dvt[i];
        au4_buffers[i].value.vt = &au4_bvt[i];
        au4_spatials[i].value.vt = &au4_svt[i];
        au4_listeners[i].value.vt = &au4_lvt[i];
    }
    au4_vector.value.x = au4_number(au4_captured[0]);
    au4_vector.value.y = au4_number(au4_captured[1]);
    au4_vector.value.z = au4_number(au4_captured[2]);
    memcpy(au4_expected_devices, au4_devices, sizeof(au4_devices));
    memcpy(au4_expected_buffers, au4_buffers, sizeof(au4_buffers));
    memcpy(au4_expected_spatials, au4_spatials, sizeof(au4_spatials));
    memcpy(au4_expected_listeners, au4_listeners, sizeof(au4_listeners));
    memcpy(&au4_expected_vector, &au4_vector, sizeof(au4_vector));
    memcpy(au4_expected_dvt, au4_dvt, sizeof(au4_dvt));
    memcpy(au4_expected_bvt, au4_bvt, sizeof(au4_bvt));
    memcpy(au4_expected_svt, au4_svt, sizeof(au4_svt));
    memcpy(au4_expected_lvt, au4_lvt, sizeof(au4_lvt));
    au4_expected_gate = op_audio_gate_word;
    au4_expected_listener = op_audio_listener;
    op_audio_co_initialize = au4_selected ? au4_coinit1 : au4_coinit0;
    op_audio_co_uninitialize = au4_selected ? au4_couninit1 : au4_couninit0;
    au4_calls = 0;
}
static int op_test_audio3d_set_distance_bounds(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au4_selected = 0; au4_selected < 2; ++au4_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au4_mutate = 0; au4_mutate < 2; ++au4_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au4_listeners[au4_selected].value;
                            au4_word = results[pattern % 4];
                            au4_result_word = results[ri];
                            au4_minimum_bits = values[pattern];
                            au4_maximum_bits = values[(pattern + 3) % 8];
                            au4_captured[0] = values[pattern];
                            au4_captured[1] = values[(pattern + 1) % 8];
                            au4_captured[2] = values[(pattern + 2) % 8];
                            au4_seed();
                            au4_expected_this = &au4_spatials[au4_selected].value;
                            au4_expected_count = 2;
                            au4_expected_codes[0] = 5;
                            au4_expected_codes[1] = 6;
                            op_audio3d_set_distance_bounds(&au4_spatials[au4_selected].value,
                                                           au4_number(au4_minimum_bits), au4_number(au4_maximum_bits));
                            AU4_CHECK(au4_calls == au4_expected_count);
                            au4_verify();
                            AU4_CHECK(op_audio_co_initialize == (au4_selected ? au4_coinit1 : au4_coinit0));
                            AU4_CHECK(op_audio_co_uninitialize == (au4_selected ? au4_couninit1 : au4_couninit0));
                        }
    printf("audio3d_set_distance_bounds: %d checks, %d failures\n", au4_checks, au4_failures);
    return au4_failures != 0;
}

#undef AU4_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au5_checks, au5_failures;
static void au5_check(int ok, int line)
{
    ++au5_checks;
    if (!ok)
    {
        ++au5_failures;
        printf("line%d failed\n", line);
    }
}
#define AU5_CHECK(x) au5_check(!!(x), __LINE__)
typedef struct AU5_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU5_OWNED_DEVICE;
typedef struct AU5_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU5_OWNED_BUFFER;
typedef struct AU5_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU5_OWNED_SPATIAL;
typedef struct AU5_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU5_OWNED_LISTENER;
typedef struct AU5_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU5_OWNED_VECTOR;
static AU5_OWNED_DEVICE au5_devices[2], au5_expected_devices[2];
static AU5_OWNED_BUFFER au5_buffers[2], au5_expected_buffers[2];
static AU5_OWNED_SPATIAL au5_spatials[2], au5_expected_spatials[2];
static AU5_OWNED_LISTENER au5_listeners[2], au5_expected_listeners[2];
static AU5_OWNED_VECTOR au5_vector, au5_expected_vector;
static OP_AUDIO_DEVICE_VT au5_dvt[2], au5_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au5_bvt[2], au5_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au5_svt[2], au5_expected_svt[2];
static OP_AUDIO_LISTENER_VT au5_lvt[2], au5_expected_lvt[2];
static unsigned int au5_selected, au5_word, au5_minimum_bits, au5_maximum_bits, au5_captured[3], au5_result_word,
    au5_expected_gate;
static OP_AUDIO_LISTENER *au5_expected_listener;
static void *au5_expected_this;
static int au5_calls, au5_expected_count, au5_expected_codes[2], au5_mutate;
static unsigned int au5_bits(float value)
{
    unsigned int au5_bits_word;
    memcpy(&au5_bits_word, &value, 4);
    return au5_bits_word;
}
static float au5_number(unsigned int au5_number_word)
{
    float value;
    memcpy(&value, &au5_number_word, 4);
    return value;
}
static void au5_verify(void)
{
    AU5_CHECK(memcmp(au5_devices, au5_expected_devices, sizeof(au5_devices)) == 0);
    AU5_CHECK(memcmp(au5_buffers, au5_expected_buffers, sizeof(au5_buffers)) == 0);
    AU5_CHECK(memcmp(au5_spatials, au5_expected_spatials, sizeof(au5_spatials)) == 0);
    AU5_CHECK(memcmp(au5_listeners, au5_expected_listeners, sizeof(au5_listeners)) == 0);
    AU5_CHECK(memcmp(&au5_vector, &au5_expected_vector, sizeof(au5_vector)) == 0);
    AU5_CHECK(memcmp(au5_dvt, au5_expected_dvt, sizeof(au5_dvt)) == 0);
    AU5_CHECK(memcmp(au5_bvt, au5_expected_bvt, sizeof(au5_bvt)) == 0);
    AU5_CHECK(memcmp(au5_svt, au5_expected_svt, sizeof(au5_svt)) == 0);
    AU5_CHECK(memcmp(au5_lvt, au5_expected_lvt, sizeof(au5_lvt)) == 0);
    AU5_CHECK(op_audio_gate_word == au5_expected_gate);
    AU5_CHECK(op_audio_listener == au5_expected_listener);
}
static void au5_observe(int code, unsigned int table, void *object)
{
    AU5_CHECK(au5_calls < au5_expected_count && au5_calls < 2);
    if (au5_calls >= au5_expected_count || au5_calls >= 2)
        exit(2);
    AU5_CHECK(code == au5_expected_codes[au5_calls]);
    AU5_CHECK(table == (au5_calls && au5_mutate ? 1 - au5_selected : au5_selected));
    AU5_CHECK(object == au5_expected_this);
    au5_verify();
    ++au5_calls;
}
static void au5_change(void)
{
    if (au5_mutate && au5_calls == 1)
    {
        op_audio_gate_word = au5_expected_gate = 0;
        au5_devices[au5_selected].value.vt = au5_expected_devices[au5_selected].value.vt = &au5_dvt[1 - au5_selected];
        au5_buffers[au5_selected].value.vt = au5_expected_buffers[au5_selected].value.vt = &au5_bvt[1 - au5_selected];
        au5_spatials[au5_selected].value.vt = au5_expected_spatials[au5_selected].value.vt = &au5_svt[1 - au5_selected];
        au5_listeners[au5_selected].value.vt = au5_expected_listeners[au5_selected].value.vt =
            &au5_lvt[1 - au5_selected];
        op_audio_listener = au5_expected_listener = &au5_listeners[1 - au5_selected].value;
        au5_vector.value.x = au5_expected_vector.value.x = 7;
        au5_vector.value.y = au5_expected_vector.value.y = 9;
        au5_vector.value.z = au5_expected_vector.value.z = 11;
    }
}
static int __stdcall au5_coinit0(void *reserved)
{
    au5_observe(1, 0, 0);
    AU5_CHECK(reserved == 0);
    au5_change();
    return (int)au5_result_word;
}
static void __stdcall au5_couninit0(void)
{
    au5_observe(2, 0, 0);
    au5_change();
}
static int __stdcall au5_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au5_observe(3, 0, o);
    AU5_CHECK(window == au5_word && flags == 2);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au5_observe(4, 0, o);
    AU5_CHECK(frequency == au5_word);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au5_observe(5, 0, o);
    AU5_CHECK(au5_bits(value) == au5_maximum_bits && flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au5_observe(6, 0, o);
    AU5_CHECK(au5_bits(value) == au5_minimum_bits && flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au5_observe(7, 0, o);
    AU5_CHECK(au5_bits(x) == au5_captured[0] && au5_bits(y) == au5_captured[1] && au5_bits(z) == au5_captured[2] &&
              flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au5_observe(8, 0, o);
    AU5_CHECK(au5_bits(x) == au5_captured[0] && au5_bits(y) == au5_captured[1] && au5_bits(z) == au5_captured[2] &&
              flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_commit0(OP_AUDIO_LISTENER *o)
{
    au5_observe(9, 0, o);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au5_observe(10, 0, o);
    AU5_CHECK(mode == au5_word && flags == 0);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_stop0(OP_AUDIO_BUFFER *o)
{
    au5_observe(11, 0, o);
    au5_change();
    return (int)au5_result_word;
}
static unsigned int __stdcall au5_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au5_observe(12, 0, o);
    au5_change();
    return (unsigned int)au5_result_word;
}
static unsigned int __stdcall au5_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au5_observe(13, 0, o);
    au5_change();
    return (unsigned int)au5_result_word;
}
static int __stdcall au5_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au5_observe(14, 0, o);
    AU5_CHECK(position == 0);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_coinit1(void *reserved)
{
    au5_observe(1, 1, 0);
    AU5_CHECK(reserved == 0);
    au5_change();
    return (int)au5_result_word;
}
static void __stdcall au5_couninit1(void)
{
    au5_observe(2, 1, 0);
    au5_change();
}
static int __stdcall au5_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au5_observe(3, 1, o);
    AU5_CHECK(window == au5_word && flags == 2);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au5_observe(4, 1, o);
    AU5_CHECK(frequency == au5_word);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au5_observe(5, 1, o);
    AU5_CHECK(au5_bits(value) == au5_maximum_bits && flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au5_observe(6, 1, o);
    AU5_CHECK(au5_bits(value) == au5_minimum_bits && flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au5_observe(7, 1, o);
    AU5_CHECK(au5_bits(x) == au5_captured[0] && au5_bits(y) == au5_captured[1] && au5_bits(z) == au5_captured[2] &&
              flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au5_observe(8, 1, o);
    AU5_CHECK(au5_bits(x) == au5_captured[0] && au5_bits(y) == au5_captured[1] && au5_bits(z) == au5_captured[2] &&
              flags == 1);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_commit1(OP_AUDIO_LISTENER *o)
{
    au5_observe(9, 1, o);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au5_observe(10, 1, o);
    AU5_CHECK(mode == au5_word && flags == 0);
    au5_change();
    return (int)au5_result_word;
}
static int __stdcall au5_stop1(OP_AUDIO_BUFFER *o)
{
    au5_observe(11, 1, o);
    au5_change();
    return (int)au5_result_word;
}
static unsigned int __stdcall au5_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au5_observe(12, 1, o);
    au5_change();
    return (unsigned int)au5_result_word;
}
static unsigned int __stdcall au5_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au5_observe(13, 1, o);
    au5_change();
    return (unsigned int)au5_result_word;
}
static int __stdcall au5_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au5_observe(14, 1, o);
    AU5_CHECK(position == 0);
    au5_change();
    return (int)au5_result_word;
}
static void au5_seed(void)
{
    unsigned int i;
    memset(au5_devices, 0x57, sizeof(au5_devices));
    memset(au5_buffers, 0x68, sizeof(au5_buffers));
    memset(au5_spatials, 0x79, sizeof(au5_spatials));
    memset(au5_listeners, 0x2a, sizeof(au5_listeners));
    memset(&au5_vector, 0x3b, sizeof(au5_vector));
    memset(au5_dvt, 0x4c, sizeof(au5_dvt));
    memset(au5_bvt, 0x5d, sizeof(au5_bvt));
    memset(au5_svt, 0x6e, sizeof(au5_svt));
    memset(au5_lvt, 0x7f, sizeof(au5_lvt));
    au5_dvt[0].cooperative = au5_coop0;
    au5_bvt[0].release = au5_release_buffer0;
    au5_bvt[0].current_position = au5_current0;
    au5_bvt[0].frequency = au5_frequency0;
    au5_bvt[0].stop = au5_stop0;
    au5_svt[0].maximum = au5_maximum0;
    au5_svt[0].minimum = au5_minimum0;
    au5_svt[0].position = au5_position0;
    au5_svt[0].velocity = au5_velocity0;
    au5_svt[0].mode = au5_mode0;
    au5_svt[0].release = au5_release_spatial0;
    au5_lvt[0].commit = au5_commit0;
    au5_dvt[1].cooperative = au5_coop1;
    au5_bvt[1].release = au5_release_buffer1;
    au5_bvt[1].current_position = au5_current1;
    au5_bvt[1].frequency = au5_frequency1;
    au5_bvt[1].stop = au5_stop1;
    au5_svt[1].maximum = au5_maximum1;
    au5_svt[1].minimum = au5_minimum1;
    au5_svt[1].position = au5_position1;
    au5_svt[1].velocity = au5_velocity1;
    au5_svt[1].mode = au5_mode1;
    au5_svt[1].release = au5_release_spatial1;
    au5_lvt[1].commit = au5_commit1;
    for (i = 0; i < 2; ++i)
    {
        au5_devices[i].value.vt = &au5_dvt[i];
        au5_buffers[i].value.vt = &au5_bvt[i];
        au5_spatials[i].value.vt = &au5_svt[i];
        au5_listeners[i].value.vt = &au5_lvt[i];
    }
    au5_vector.value.x = au5_number(au5_captured[0]);
    au5_vector.value.y = au5_number(au5_captured[1]);
    au5_vector.value.z = au5_number(au5_captured[2]);
    memcpy(au5_expected_devices, au5_devices, sizeof(au5_devices));
    memcpy(au5_expected_buffers, au5_buffers, sizeof(au5_buffers));
    memcpy(au5_expected_spatials, au5_spatials, sizeof(au5_spatials));
    memcpy(au5_expected_listeners, au5_listeners, sizeof(au5_listeners));
    memcpy(&au5_expected_vector, &au5_vector, sizeof(au5_vector));
    memcpy(au5_expected_dvt, au5_dvt, sizeof(au5_dvt));
    memcpy(au5_expected_bvt, au5_bvt, sizeof(au5_bvt));
    memcpy(au5_expected_svt, au5_svt, sizeof(au5_svt));
    memcpy(au5_expected_lvt, au5_lvt, sizeof(au5_lvt));
    au5_expected_gate = op_audio_gate_word;
    au5_expected_listener = op_audio_listener;
    op_audio_co_initialize = au5_selected ? au5_coinit1 : au5_coinit0;
    op_audio_co_uninitialize = au5_selected ? au5_couninit1 : au5_couninit0;
    au5_calls = 0;
}
static int op_test_audio_commit_listener(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au5_selected = 0; au5_selected < 2; ++au5_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au5_mutate = 0; au5_mutate < 2; ++au5_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au5_listeners[au5_selected].value;
                            au5_word = results[pattern % 4];
                            au5_result_word = results[ri];
                            au5_minimum_bits = values[pattern];
                            au5_maximum_bits = values[(pattern + 3) % 8];
                            au5_captured[0] = values[pattern];
                            au5_captured[1] = values[(pattern + 1) % 8];
                            au5_captured[2] = values[(pattern + 2) % 8];
                            au5_seed();
                            au5_expected_this = op_audio_listener;
                            au5_expected_count = !null_pointer;
                            au5_expected_codes[0] = 9;
                            au5_expected_codes[1] = 0;
                            op_audio_commit_listener();
                            AU5_CHECK(au5_calls == au5_expected_count);
                            au5_verify();
                            AU5_CHECK(op_audio_co_initialize == (au5_selected ? au5_coinit1 : au5_coinit0));
                            AU5_CHECK(op_audio_co_uninitialize == (au5_selected ? au5_couninit1 : au5_couninit0));
                        }
    printf("audio_commit_listener: %d checks, %d failures\n", au5_checks, au5_failures);
    return au5_failures != 0;
}

#undef AU5_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au6_checks, au6_failures;
static void au6_check(int ok, int line)
{
    ++au6_checks;
    if (!ok)
    {
        ++au6_failures;
        printf("line%d failed\n", line);
    }
}
#define AU6_CHECK(x) au6_check(!!(x), __LINE__)
typedef struct AU6_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU6_OWNED_DEVICE;
typedef struct AU6_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU6_OWNED_BUFFER;
typedef struct AU6_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU6_OWNED_SPATIAL;
typedef struct AU6_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU6_OWNED_LISTENER;
typedef struct AU6_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU6_OWNED_VECTOR;
static AU6_OWNED_DEVICE au6_devices[2], au6_expected_devices[2];
static AU6_OWNED_BUFFER au6_buffers[2], au6_expected_buffers[2];
static AU6_OWNED_SPATIAL au6_spatials[2], au6_expected_spatials[2];
static AU6_OWNED_LISTENER au6_listeners[2], au6_expected_listeners[2];
static AU6_OWNED_VECTOR au6_vector, au6_expected_vector;
static OP_AUDIO_DEVICE_VT au6_dvt[2], au6_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au6_bvt[2], au6_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au6_svt[2], au6_expected_svt[2];
static OP_AUDIO_LISTENER_VT au6_lvt[2], au6_expected_lvt[2];
static unsigned int au6_selected, au6_word, au6_minimum_bits, au6_maximum_bits, au6_captured[3], au6_result_word,
    au6_expected_gate;
static OP_AUDIO_LISTENER *au6_expected_listener;
static void *au6_expected_this;
static int au6_calls, au6_expected_count, au6_expected_codes[2], au6_mutate;
static unsigned int au6_bits(float value)
{
    unsigned int au6_bits_word;
    memcpy(&au6_bits_word, &value, 4);
    return au6_bits_word;
}
static float au6_number(unsigned int au6_number_word)
{
    float value;
    memcpy(&value, &au6_number_word, 4);
    return value;
}
static void au6_verify(void)
{
    AU6_CHECK(memcmp(au6_devices, au6_expected_devices, sizeof(au6_devices)) == 0);
    AU6_CHECK(memcmp(au6_buffers, au6_expected_buffers, sizeof(au6_buffers)) == 0);
    AU6_CHECK(memcmp(au6_spatials, au6_expected_spatials, sizeof(au6_spatials)) == 0);
    AU6_CHECK(memcmp(au6_listeners, au6_expected_listeners, sizeof(au6_listeners)) == 0);
    AU6_CHECK(memcmp(&au6_vector, &au6_expected_vector, sizeof(au6_vector)) == 0);
    AU6_CHECK(memcmp(au6_dvt, au6_expected_dvt, sizeof(au6_dvt)) == 0);
    AU6_CHECK(memcmp(au6_bvt, au6_expected_bvt, sizeof(au6_bvt)) == 0);
    AU6_CHECK(memcmp(au6_svt, au6_expected_svt, sizeof(au6_svt)) == 0);
    AU6_CHECK(memcmp(au6_lvt, au6_expected_lvt, sizeof(au6_lvt)) == 0);
    AU6_CHECK(op_audio_gate_word == au6_expected_gate);
    AU6_CHECK(op_audio_listener == au6_expected_listener);
}
static void au6_observe(int code, unsigned int table, void *object)
{
    AU6_CHECK(au6_calls < au6_expected_count && au6_calls < 2);
    if (au6_calls >= au6_expected_count || au6_calls >= 2)
        exit(2);
    AU6_CHECK(code == au6_expected_codes[au6_calls]);
    AU6_CHECK(table == (au6_calls && au6_mutate ? 1 - au6_selected : au6_selected));
    AU6_CHECK(object == au6_expected_this);
    au6_verify();
    ++au6_calls;
}
static void au6_change(void)
{
    if (au6_mutate && au6_calls == 1)
    {
        op_audio_gate_word = au6_expected_gate = 0;
        au6_devices[au6_selected].value.vt = au6_expected_devices[au6_selected].value.vt = &au6_dvt[1 - au6_selected];
        au6_buffers[au6_selected].value.vt = au6_expected_buffers[au6_selected].value.vt = &au6_bvt[1 - au6_selected];
        au6_spatials[au6_selected].value.vt = au6_expected_spatials[au6_selected].value.vt = &au6_svt[1 - au6_selected];
        au6_listeners[au6_selected].value.vt = au6_expected_listeners[au6_selected].value.vt =
            &au6_lvt[1 - au6_selected];
        op_audio_listener = au6_expected_listener = &au6_listeners[1 - au6_selected].value;
        au6_vector.value.x = au6_expected_vector.value.x = 7;
        au6_vector.value.y = au6_expected_vector.value.y = 9;
        au6_vector.value.z = au6_expected_vector.value.z = 11;
    }
}
static int __stdcall au6_coinit0(void *reserved)
{
    au6_observe(1, 0, 0);
    AU6_CHECK(reserved == 0);
    au6_change();
    return (int)au6_result_word;
}
static void __stdcall au6_couninit0(void)
{
    au6_observe(2, 0, 0);
    au6_change();
}
static int __stdcall au6_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au6_observe(3, 0, o);
    AU6_CHECK(window == au6_word && flags == 2);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au6_observe(4, 0, o);
    AU6_CHECK(frequency == au6_word);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au6_observe(5, 0, o);
    AU6_CHECK(au6_bits(value) == au6_maximum_bits && flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au6_observe(6, 0, o);
    AU6_CHECK(au6_bits(value) == au6_minimum_bits && flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au6_observe(7, 0, o);
    AU6_CHECK(au6_bits(x) == au6_captured[0] && au6_bits(y) == au6_captured[1] && au6_bits(z) == au6_captured[2] &&
              flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au6_observe(8, 0, o);
    AU6_CHECK(au6_bits(x) == au6_captured[0] && au6_bits(y) == au6_captured[1] && au6_bits(z) == au6_captured[2] &&
              flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_commit0(OP_AUDIO_LISTENER *o)
{
    au6_observe(9, 0, o);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au6_observe(10, 0, o);
    AU6_CHECK(mode == au6_word && flags == 0);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_stop0(OP_AUDIO_BUFFER *o)
{
    au6_observe(11, 0, o);
    au6_change();
    return (int)au6_result_word;
}
static unsigned int __stdcall au6_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au6_observe(12, 0, o);
    au6_change();
    return (unsigned int)au6_result_word;
}
static unsigned int __stdcall au6_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au6_observe(13, 0, o);
    au6_change();
    return (unsigned int)au6_result_word;
}
static int __stdcall au6_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au6_observe(14, 0, o);
    AU6_CHECK(position == 0);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_coinit1(void *reserved)
{
    au6_observe(1, 1, 0);
    AU6_CHECK(reserved == 0);
    au6_change();
    return (int)au6_result_word;
}
static void __stdcall au6_couninit1(void)
{
    au6_observe(2, 1, 0);
    au6_change();
}
static int __stdcall au6_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au6_observe(3, 1, o);
    AU6_CHECK(window == au6_word && flags == 2);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au6_observe(4, 1, o);
    AU6_CHECK(frequency == au6_word);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au6_observe(5, 1, o);
    AU6_CHECK(au6_bits(value) == au6_maximum_bits && flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au6_observe(6, 1, o);
    AU6_CHECK(au6_bits(value) == au6_minimum_bits && flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au6_observe(7, 1, o);
    AU6_CHECK(au6_bits(x) == au6_captured[0] && au6_bits(y) == au6_captured[1] && au6_bits(z) == au6_captured[2] &&
              flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au6_observe(8, 1, o);
    AU6_CHECK(au6_bits(x) == au6_captured[0] && au6_bits(y) == au6_captured[1] && au6_bits(z) == au6_captured[2] &&
              flags == 1);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_commit1(OP_AUDIO_LISTENER *o)
{
    au6_observe(9, 1, o);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au6_observe(10, 1, o);
    AU6_CHECK(mode == au6_word && flags == 0);
    au6_change();
    return (int)au6_result_word;
}
static int __stdcall au6_stop1(OP_AUDIO_BUFFER *o)
{
    au6_observe(11, 1, o);
    au6_change();
    return (int)au6_result_word;
}
static unsigned int __stdcall au6_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au6_observe(12, 1, o);
    au6_change();
    return (unsigned int)au6_result_word;
}
static unsigned int __stdcall au6_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au6_observe(13, 1, o);
    au6_change();
    return (unsigned int)au6_result_word;
}
static int __stdcall au6_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au6_observe(14, 1, o);
    AU6_CHECK(position == 0);
    au6_change();
    return (int)au6_result_word;
}
static void au6_seed(void)
{
    unsigned int i;
    memset(au6_devices, 0x57, sizeof(au6_devices));
    memset(au6_buffers, 0x68, sizeof(au6_buffers));
    memset(au6_spatials, 0x79, sizeof(au6_spatials));
    memset(au6_listeners, 0x2a, sizeof(au6_listeners));
    memset(&au6_vector, 0x3b, sizeof(au6_vector));
    memset(au6_dvt, 0x4c, sizeof(au6_dvt));
    memset(au6_bvt, 0x5d, sizeof(au6_bvt));
    memset(au6_svt, 0x6e, sizeof(au6_svt));
    memset(au6_lvt, 0x7f, sizeof(au6_lvt));
    au6_dvt[0].cooperative = au6_coop0;
    au6_bvt[0].release = au6_release_buffer0;
    au6_bvt[0].current_position = au6_current0;
    au6_bvt[0].frequency = au6_frequency0;
    au6_bvt[0].stop = au6_stop0;
    au6_svt[0].maximum = au6_maximum0;
    au6_svt[0].minimum = au6_minimum0;
    au6_svt[0].position = au6_position0;
    au6_svt[0].velocity = au6_velocity0;
    au6_svt[0].mode = au6_mode0;
    au6_svt[0].release = au6_release_spatial0;
    au6_lvt[0].commit = au6_commit0;
    au6_dvt[1].cooperative = au6_coop1;
    au6_bvt[1].release = au6_release_buffer1;
    au6_bvt[1].current_position = au6_current1;
    au6_bvt[1].frequency = au6_frequency1;
    au6_bvt[1].stop = au6_stop1;
    au6_svt[1].maximum = au6_maximum1;
    au6_svt[1].minimum = au6_minimum1;
    au6_svt[1].position = au6_position1;
    au6_svt[1].velocity = au6_velocity1;
    au6_svt[1].mode = au6_mode1;
    au6_svt[1].release = au6_release_spatial1;
    au6_lvt[1].commit = au6_commit1;
    for (i = 0; i < 2; ++i)
    {
        au6_devices[i].value.vt = &au6_dvt[i];
        au6_buffers[i].value.vt = &au6_bvt[i];
        au6_spatials[i].value.vt = &au6_svt[i];
        au6_listeners[i].value.vt = &au6_lvt[i];
    }
    au6_vector.value.x = au6_number(au6_captured[0]);
    au6_vector.value.y = au6_number(au6_captured[1]);
    au6_vector.value.z = au6_number(au6_captured[2]);
    memcpy(au6_expected_devices, au6_devices, sizeof(au6_devices));
    memcpy(au6_expected_buffers, au6_buffers, sizeof(au6_buffers));
    memcpy(au6_expected_spatials, au6_spatials, sizeof(au6_spatials));
    memcpy(au6_expected_listeners, au6_listeners, sizeof(au6_listeners));
    memcpy(&au6_expected_vector, &au6_vector, sizeof(au6_vector));
    memcpy(au6_expected_dvt, au6_dvt, sizeof(au6_dvt));
    memcpy(au6_expected_bvt, au6_bvt, sizeof(au6_bvt));
    memcpy(au6_expected_svt, au6_svt, sizeof(au6_svt));
    memcpy(au6_expected_lvt, au6_lvt, sizeof(au6_lvt));
    au6_expected_gate = op_audio_gate_word;
    au6_expected_listener = op_audio_listener;
    op_audio_co_initialize = au6_selected ? au6_coinit1 : au6_coinit0;
    op_audio_co_uninitialize = au6_selected ? au6_couninit1 : au6_couninit0;
    au6_calls = 0;
}
static int op_test_audio3d_set_mode(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au6_selected = 0; au6_selected < 2; ++au6_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au6_mutate = 0; au6_mutate < 2; ++au6_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au6_listeners[au6_selected].value;
                            au6_word = results[pattern % 4];
                            au6_result_word = results[ri];
                            au6_minimum_bits = values[pattern];
                            au6_maximum_bits = values[(pattern + 3) % 8];
                            au6_captured[0] = values[pattern];
                            au6_captured[1] = values[(pattern + 1) % 8];
                            au6_captured[2] = values[(pattern + 2) % 8];
                            au6_seed();
                            au6_expected_this = &au6_spatials[au6_selected].value;
                            au6_expected_count = 1;
                            au6_expected_codes[0] = 10;
                            au6_expected_codes[1] = 0;
                            op_audio3d_set_mode(&au6_spatials[au6_selected].value, au6_word);
                            AU6_CHECK(au6_calls == au6_expected_count);
                            au6_verify();
                            AU6_CHECK(op_audio_co_initialize == (au6_selected ? au6_coinit1 : au6_coinit0));
                            AU6_CHECK(op_audio_co_uninitialize == (au6_selected ? au6_couninit1 : au6_couninit0));
                        }
    printf("audio3d_set_mode: %d checks, %d failures\n", au6_checks, au6_failures);
    return au6_failures != 0;
}

#undef AU6_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au7_checks, au7_failures;
static void au7_check(int ok, int line)
{
    ++au7_checks;
    if (!ok)
    {
        ++au7_failures;
        printf("line%d failed\n", line);
    }
}
#define AU7_CHECK(x) au7_check(!!(x), __LINE__)
typedef struct AU7_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU7_OWNED_DEVICE;
typedef struct AU7_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU7_OWNED_BUFFER;
typedef struct AU7_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU7_OWNED_SPATIAL;
typedef struct AU7_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU7_OWNED_LISTENER;
typedef struct AU7_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU7_OWNED_VECTOR;
static AU7_OWNED_DEVICE au7_devices[2], au7_expected_devices[2];
static AU7_OWNED_BUFFER au7_buffers[2], au7_expected_buffers[2];
static AU7_OWNED_SPATIAL au7_spatials[2], au7_expected_spatials[2];
static AU7_OWNED_LISTENER au7_listeners[2], au7_expected_listeners[2];
static AU7_OWNED_VECTOR au7_vector, au7_expected_vector;
static OP_AUDIO_DEVICE_VT au7_dvt[2], au7_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au7_bvt[2], au7_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au7_svt[2], au7_expected_svt[2];
static OP_AUDIO_LISTENER_VT au7_lvt[2], au7_expected_lvt[2];
static unsigned int au7_selected, au7_word, au7_minimum_bits, au7_maximum_bits, au7_captured[3], au7_result_word,
    au7_expected_gate;
static OP_AUDIO_LISTENER *au7_expected_listener;
static void *au7_expected_this;
static int au7_calls, au7_expected_count, au7_expected_codes[2], au7_mutate;
static unsigned int au7_bits(float value)
{
    unsigned int au7_bits_word;
    memcpy(&au7_bits_word, &value, 4);
    return au7_bits_word;
}
static float au7_number(unsigned int au7_number_word)
{
    float value;
    memcpy(&value, &au7_number_word, 4);
    return value;
}
static void au7_verify(void)
{
    AU7_CHECK(memcmp(au7_devices, au7_expected_devices, sizeof(au7_devices)) == 0);
    AU7_CHECK(memcmp(au7_buffers, au7_expected_buffers, sizeof(au7_buffers)) == 0);
    AU7_CHECK(memcmp(au7_spatials, au7_expected_spatials, sizeof(au7_spatials)) == 0);
    AU7_CHECK(memcmp(au7_listeners, au7_expected_listeners, sizeof(au7_listeners)) == 0);
    AU7_CHECK(memcmp(&au7_vector, &au7_expected_vector, sizeof(au7_vector)) == 0);
    AU7_CHECK(memcmp(au7_dvt, au7_expected_dvt, sizeof(au7_dvt)) == 0);
    AU7_CHECK(memcmp(au7_bvt, au7_expected_bvt, sizeof(au7_bvt)) == 0);
    AU7_CHECK(memcmp(au7_svt, au7_expected_svt, sizeof(au7_svt)) == 0);
    AU7_CHECK(memcmp(au7_lvt, au7_expected_lvt, sizeof(au7_lvt)) == 0);
    AU7_CHECK(op_audio_gate_word == au7_expected_gate);
    AU7_CHECK(op_audio_listener == au7_expected_listener);
}
static void au7_observe(int code, unsigned int table, void *object)
{
    AU7_CHECK(au7_calls < au7_expected_count && au7_calls < 2);
    if (au7_calls >= au7_expected_count || au7_calls >= 2)
        exit(2);
    AU7_CHECK(code == au7_expected_codes[au7_calls]);
    AU7_CHECK(table == (au7_calls && au7_mutate ? 1 - au7_selected : au7_selected));
    AU7_CHECK(object == au7_expected_this);
    au7_verify();
    ++au7_calls;
}
static void au7_change(void)
{
    if (au7_mutate && au7_calls == 1)
    {
        op_audio_gate_word = au7_expected_gate = 0;
        au7_devices[au7_selected].value.vt = au7_expected_devices[au7_selected].value.vt = &au7_dvt[1 - au7_selected];
        au7_buffers[au7_selected].value.vt = au7_expected_buffers[au7_selected].value.vt = &au7_bvt[1 - au7_selected];
        au7_spatials[au7_selected].value.vt = au7_expected_spatials[au7_selected].value.vt = &au7_svt[1 - au7_selected];
        au7_listeners[au7_selected].value.vt = au7_expected_listeners[au7_selected].value.vt =
            &au7_lvt[1 - au7_selected];
        op_audio_listener = au7_expected_listener = &au7_listeners[1 - au7_selected].value;
        au7_vector.value.x = au7_expected_vector.value.x = 7;
        au7_vector.value.y = au7_expected_vector.value.y = 9;
        au7_vector.value.z = au7_expected_vector.value.z = 11;
    }
}
static int __stdcall au7_coinit0(void *reserved)
{
    au7_observe(1, 0, 0);
    AU7_CHECK(reserved == 0);
    au7_change();
    return (int)au7_result_word;
}
static void __stdcall au7_couninit0(void)
{
    au7_observe(2, 0, 0);
    au7_change();
}
static int __stdcall au7_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au7_observe(3, 0, o);
    AU7_CHECK(window == au7_word && flags == 2);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au7_observe(4, 0, o);
    AU7_CHECK(frequency == au7_word);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au7_observe(5, 0, o);
    AU7_CHECK(au7_bits(value) == au7_maximum_bits && flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au7_observe(6, 0, o);
    AU7_CHECK(au7_bits(value) == au7_minimum_bits && flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au7_observe(7, 0, o);
    AU7_CHECK(au7_bits(x) == au7_captured[0] && au7_bits(y) == au7_captured[1] && au7_bits(z) == au7_captured[2] &&
              flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au7_observe(8, 0, o);
    AU7_CHECK(au7_bits(x) == au7_captured[0] && au7_bits(y) == au7_captured[1] && au7_bits(z) == au7_captured[2] &&
              flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_commit0(OP_AUDIO_LISTENER *o)
{
    au7_observe(9, 0, o);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au7_observe(10, 0, o);
    AU7_CHECK(mode == au7_word && flags == 0);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_stop0(OP_AUDIO_BUFFER *o)
{
    au7_observe(11, 0, o);
    au7_change();
    return (int)au7_result_word;
}
static unsigned int __stdcall au7_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au7_observe(12, 0, o);
    au7_change();
    return (unsigned int)au7_result_word;
}
static unsigned int __stdcall au7_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au7_observe(13, 0, o);
    au7_change();
    return (unsigned int)au7_result_word;
}
static int __stdcall au7_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au7_observe(14, 0, o);
    AU7_CHECK(position == 0);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_coinit1(void *reserved)
{
    au7_observe(1, 1, 0);
    AU7_CHECK(reserved == 0);
    au7_change();
    return (int)au7_result_word;
}
static void __stdcall au7_couninit1(void)
{
    au7_observe(2, 1, 0);
    au7_change();
}
static int __stdcall au7_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au7_observe(3, 1, o);
    AU7_CHECK(window == au7_word && flags == 2);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au7_observe(4, 1, o);
    AU7_CHECK(frequency == au7_word);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au7_observe(5, 1, o);
    AU7_CHECK(au7_bits(value) == au7_maximum_bits && flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au7_observe(6, 1, o);
    AU7_CHECK(au7_bits(value) == au7_minimum_bits && flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au7_observe(7, 1, o);
    AU7_CHECK(au7_bits(x) == au7_captured[0] && au7_bits(y) == au7_captured[1] && au7_bits(z) == au7_captured[2] &&
              flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au7_observe(8, 1, o);
    AU7_CHECK(au7_bits(x) == au7_captured[0] && au7_bits(y) == au7_captured[1] && au7_bits(z) == au7_captured[2] &&
              flags == 1);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_commit1(OP_AUDIO_LISTENER *o)
{
    au7_observe(9, 1, o);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au7_observe(10, 1, o);
    AU7_CHECK(mode == au7_word && flags == 0);
    au7_change();
    return (int)au7_result_word;
}
static int __stdcall au7_stop1(OP_AUDIO_BUFFER *o)
{
    au7_observe(11, 1, o);
    au7_change();
    return (int)au7_result_word;
}
static unsigned int __stdcall au7_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au7_observe(12, 1, o);
    au7_change();
    return (unsigned int)au7_result_word;
}
static unsigned int __stdcall au7_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au7_observe(13, 1, o);
    au7_change();
    return (unsigned int)au7_result_word;
}
static int __stdcall au7_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au7_observe(14, 1, o);
    AU7_CHECK(position == 0);
    au7_change();
    return (int)au7_result_word;
}
static void au7_seed(void)
{
    unsigned int i;
    memset(au7_devices, 0x57, sizeof(au7_devices));
    memset(au7_buffers, 0x68, sizeof(au7_buffers));
    memset(au7_spatials, 0x79, sizeof(au7_spatials));
    memset(au7_listeners, 0x2a, sizeof(au7_listeners));
    memset(&au7_vector, 0x3b, sizeof(au7_vector));
    memset(au7_dvt, 0x4c, sizeof(au7_dvt));
    memset(au7_bvt, 0x5d, sizeof(au7_bvt));
    memset(au7_svt, 0x6e, sizeof(au7_svt));
    memset(au7_lvt, 0x7f, sizeof(au7_lvt));
    au7_dvt[0].cooperative = au7_coop0;
    au7_bvt[0].release = au7_release_buffer0;
    au7_bvt[0].current_position = au7_current0;
    au7_bvt[0].frequency = au7_frequency0;
    au7_bvt[0].stop = au7_stop0;
    au7_svt[0].maximum = au7_maximum0;
    au7_svt[0].minimum = au7_minimum0;
    au7_svt[0].position = au7_position0;
    au7_svt[0].velocity = au7_velocity0;
    au7_svt[0].mode = au7_mode0;
    au7_svt[0].release = au7_release_spatial0;
    au7_lvt[0].commit = au7_commit0;
    au7_dvt[1].cooperative = au7_coop1;
    au7_bvt[1].release = au7_release_buffer1;
    au7_bvt[1].current_position = au7_current1;
    au7_bvt[1].frequency = au7_frequency1;
    au7_bvt[1].stop = au7_stop1;
    au7_svt[1].maximum = au7_maximum1;
    au7_svt[1].minimum = au7_minimum1;
    au7_svt[1].position = au7_position1;
    au7_svt[1].velocity = au7_velocity1;
    au7_svt[1].mode = au7_mode1;
    au7_svt[1].release = au7_release_spatial1;
    au7_lvt[1].commit = au7_commit1;
    for (i = 0; i < 2; ++i)
    {
        au7_devices[i].value.vt = &au7_dvt[i];
        au7_buffers[i].value.vt = &au7_bvt[i];
        au7_spatials[i].value.vt = &au7_svt[i];
        au7_listeners[i].value.vt = &au7_lvt[i];
    }
    au7_vector.value.x = au7_number(au7_captured[0]);
    au7_vector.value.y = au7_number(au7_captured[1]);
    au7_vector.value.z = au7_number(au7_captured[2]);
    memcpy(au7_expected_devices, au7_devices, sizeof(au7_devices));
    memcpy(au7_expected_buffers, au7_buffers, sizeof(au7_buffers));
    memcpy(au7_expected_spatials, au7_spatials, sizeof(au7_spatials));
    memcpy(au7_expected_listeners, au7_listeners, sizeof(au7_listeners));
    memcpy(&au7_expected_vector, &au7_vector, sizeof(au7_vector));
    memcpy(au7_expected_dvt, au7_dvt, sizeof(au7_dvt));
    memcpy(au7_expected_bvt, au7_bvt, sizeof(au7_bvt));
    memcpy(au7_expected_svt, au7_svt, sizeof(au7_svt));
    memcpy(au7_expected_lvt, au7_lvt, sizeof(au7_lvt));
    au7_expected_gate = op_audio_gate_word;
    au7_expected_listener = op_audio_listener;
    op_audio_co_initialize = au7_selected ? au7_coinit1 : au7_coinit0;
    op_audio_co_uninitialize = au7_selected ? au7_couninit1 : au7_couninit0;
    au7_calls = 0;
}
static int op_test_audio_stop(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au7_selected = 0; au7_selected < 2; ++au7_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au7_mutate = 0; au7_mutate < 2; ++au7_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au7_listeners[au7_selected].value;
                            au7_word = results[pattern % 4];
                            au7_result_word = results[ri];
                            au7_minimum_bits = values[pattern];
                            au7_maximum_bits = values[(pattern + 3) % 8];
                            au7_captured[0] = values[pattern];
                            au7_captured[1] = values[(pattern + 1) % 8];
                            au7_captured[2] = values[(pattern + 2) % 8];
                            au7_seed();
                            au7_expected_this = null_pointer ? 0 : &au7_buffers[au7_selected].value;
                            au7_expected_count = !null_pointer && gate ? 2 : 0;
                            au7_expected_codes[0] = 11;
                            au7_expected_codes[1] = 14;
                            AU7_CHECK(op_audio_stop(null_pointer ? 0 : &au7_buffers[au7_selected].value) ==
                                      (null_pointer ? 1 : (gate ? 1 : 0)));
                            AU7_CHECK(au7_calls == au7_expected_count);
                            au7_verify();
                            AU7_CHECK(op_audio_co_initialize == (au7_selected ? au7_coinit1 : au7_coinit0));
                            AU7_CHECK(op_audio_co_uninitialize == (au7_selected ? au7_couninit1 : au7_couninit0));
                        }
    printf("audio_stop: %d checks, %d failures\n", au7_checks, au7_failures);
    return au7_failures != 0;
}

#undef AU7_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au8_checks, au8_failures;
static void au8_check(int ok, int line)
{
    ++au8_checks;
    if (!ok)
    {
        ++au8_failures;
        printf("line%d failed\n", line);
    }
}
#define AU8_CHECK(x) au8_check(!!(x), __LINE__)
typedef struct AU8_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU8_OWNED_DEVICE;
typedef struct AU8_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU8_OWNED_BUFFER;
typedef struct AU8_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU8_OWNED_SPATIAL;
typedef struct AU8_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU8_OWNED_LISTENER;
typedef struct AU8_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU8_OWNED_VECTOR;
static AU8_OWNED_DEVICE au8_devices[2], au8_expected_devices[2];
static AU8_OWNED_BUFFER au8_buffers[2], au8_expected_buffers[2];
static AU8_OWNED_SPATIAL au8_spatials[2], au8_expected_spatials[2];
static AU8_OWNED_LISTENER au8_listeners[2], au8_expected_listeners[2];
static AU8_OWNED_VECTOR au8_vector, au8_expected_vector;
static OP_AUDIO_DEVICE_VT au8_dvt[2], au8_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au8_bvt[2], au8_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au8_svt[2], au8_expected_svt[2];
static OP_AUDIO_LISTENER_VT au8_lvt[2], au8_expected_lvt[2];
static unsigned int au8_selected, au8_word, au8_minimum_bits, au8_maximum_bits, au8_captured[3], au8_result_word,
    au8_expected_gate;
static OP_AUDIO_LISTENER *au8_expected_listener;
static void *au8_expected_this;
static int au8_calls, au8_expected_count, au8_expected_codes[2], au8_mutate;
static unsigned int au8_bits(float value)
{
    unsigned int au8_bits_word;
    memcpy(&au8_bits_word, &value, 4);
    return au8_bits_word;
}
static float au8_number(unsigned int au8_number_word)
{
    float value;
    memcpy(&value, &au8_number_word, 4);
    return value;
}
static void au8_verify(void)
{
    AU8_CHECK(memcmp(au8_devices, au8_expected_devices, sizeof(au8_devices)) == 0);
    AU8_CHECK(memcmp(au8_buffers, au8_expected_buffers, sizeof(au8_buffers)) == 0);
    AU8_CHECK(memcmp(au8_spatials, au8_expected_spatials, sizeof(au8_spatials)) == 0);
    AU8_CHECK(memcmp(au8_listeners, au8_expected_listeners, sizeof(au8_listeners)) == 0);
    AU8_CHECK(memcmp(&au8_vector, &au8_expected_vector, sizeof(au8_vector)) == 0);
    AU8_CHECK(memcmp(au8_dvt, au8_expected_dvt, sizeof(au8_dvt)) == 0);
    AU8_CHECK(memcmp(au8_bvt, au8_expected_bvt, sizeof(au8_bvt)) == 0);
    AU8_CHECK(memcmp(au8_svt, au8_expected_svt, sizeof(au8_svt)) == 0);
    AU8_CHECK(memcmp(au8_lvt, au8_expected_lvt, sizeof(au8_lvt)) == 0);
    AU8_CHECK(op_audio_gate_word == au8_expected_gate);
    AU8_CHECK(op_audio_listener == au8_expected_listener);
}
static void au8_observe(int code, unsigned int table, void *object)
{
    AU8_CHECK(au8_calls < au8_expected_count && au8_calls < 2);
    if (au8_calls >= au8_expected_count || au8_calls >= 2)
        exit(2);
    AU8_CHECK(code == au8_expected_codes[au8_calls]);
    AU8_CHECK(table == (au8_calls && au8_mutate ? 1 - au8_selected : au8_selected));
    AU8_CHECK(object == au8_expected_this);
    au8_verify();
    ++au8_calls;
}
static void au8_change(void)
{
    if (au8_mutate && au8_calls == 1)
    {
        op_audio_gate_word = au8_expected_gate = 0;
        au8_devices[au8_selected].value.vt = au8_expected_devices[au8_selected].value.vt = &au8_dvt[1 - au8_selected];
        au8_buffers[au8_selected].value.vt = au8_expected_buffers[au8_selected].value.vt = &au8_bvt[1 - au8_selected];
        au8_spatials[au8_selected].value.vt = au8_expected_spatials[au8_selected].value.vt = &au8_svt[1 - au8_selected];
        au8_listeners[au8_selected].value.vt = au8_expected_listeners[au8_selected].value.vt =
            &au8_lvt[1 - au8_selected];
        op_audio_listener = au8_expected_listener = &au8_listeners[1 - au8_selected].value;
        au8_vector.value.x = au8_expected_vector.value.x = 7;
        au8_vector.value.y = au8_expected_vector.value.y = 9;
        au8_vector.value.z = au8_expected_vector.value.z = 11;
    }
}
static int __stdcall au8_coinit0(void *reserved)
{
    au8_observe(1, 0, 0);
    AU8_CHECK(reserved == 0);
    au8_change();
    return (int)au8_result_word;
}
static void __stdcall au8_couninit0(void)
{
    au8_observe(2, 0, 0);
    au8_change();
}
static int __stdcall au8_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au8_observe(3, 0, o);
    AU8_CHECK(window == au8_word && flags == 2);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au8_observe(4, 0, o);
    AU8_CHECK(frequency == au8_word);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au8_observe(5, 0, o);
    AU8_CHECK(au8_bits(value) == au8_maximum_bits && flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au8_observe(6, 0, o);
    AU8_CHECK(au8_bits(value) == au8_minimum_bits && flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au8_observe(7, 0, o);
    AU8_CHECK(au8_bits(x) == au8_captured[0] && au8_bits(y) == au8_captured[1] && au8_bits(z) == au8_captured[2] &&
              flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au8_observe(8, 0, o);
    AU8_CHECK(au8_bits(x) == au8_captured[0] && au8_bits(y) == au8_captured[1] && au8_bits(z) == au8_captured[2] &&
              flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_commit0(OP_AUDIO_LISTENER *o)
{
    au8_observe(9, 0, o);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au8_observe(10, 0, o);
    AU8_CHECK(mode == au8_word && flags == 0);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_stop0(OP_AUDIO_BUFFER *o)
{
    au8_observe(11, 0, o);
    au8_change();
    return (int)au8_result_word;
}
static unsigned int __stdcall au8_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au8_observe(12, 0, o);
    au8_change();
    return (unsigned int)au8_result_word;
}
static unsigned int __stdcall au8_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au8_observe(13, 0, o);
    au8_change();
    return (unsigned int)au8_result_word;
}
static int __stdcall au8_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au8_observe(14, 0, o);
    AU8_CHECK(position == 0);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_coinit1(void *reserved)
{
    au8_observe(1, 1, 0);
    AU8_CHECK(reserved == 0);
    au8_change();
    return (int)au8_result_word;
}
static void __stdcall au8_couninit1(void)
{
    au8_observe(2, 1, 0);
    au8_change();
}
static int __stdcall au8_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au8_observe(3, 1, o);
    AU8_CHECK(window == au8_word && flags == 2);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au8_observe(4, 1, o);
    AU8_CHECK(frequency == au8_word);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au8_observe(5, 1, o);
    AU8_CHECK(au8_bits(value) == au8_maximum_bits && flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au8_observe(6, 1, o);
    AU8_CHECK(au8_bits(value) == au8_minimum_bits && flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au8_observe(7, 1, o);
    AU8_CHECK(au8_bits(x) == au8_captured[0] && au8_bits(y) == au8_captured[1] && au8_bits(z) == au8_captured[2] &&
              flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au8_observe(8, 1, o);
    AU8_CHECK(au8_bits(x) == au8_captured[0] && au8_bits(y) == au8_captured[1] && au8_bits(z) == au8_captured[2] &&
              flags == 1);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_commit1(OP_AUDIO_LISTENER *o)
{
    au8_observe(9, 1, o);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au8_observe(10, 1, o);
    AU8_CHECK(mode == au8_word && flags == 0);
    au8_change();
    return (int)au8_result_word;
}
static int __stdcall au8_stop1(OP_AUDIO_BUFFER *o)
{
    au8_observe(11, 1, o);
    au8_change();
    return (int)au8_result_word;
}
static unsigned int __stdcall au8_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au8_observe(12, 1, o);
    au8_change();
    return (unsigned int)au8_result_word;
}
static unsigned int __stdcall au8_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au8_observe(13, 1, o);
    au8_change();
    return (unsigned int)au8_result_word;
}
static int __stdcall au8_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au8_observe(14, 1, o);
    AU8_CHECK(position == 0);
    au8_change();
    return (int)au8_result_word;
}
static void au8_seed(void)
{
    unsigned int i;
    memset(au8_devices, 0x57, sizeof(au8_devices));
    memset(au8_buffers, 0x68, sizeof(au8_buffers));
    memset(au8_spatials, 0x79, sizeof(au8_spatials));
    memset(au8_listeners, 0x2a, sizeof(au8_listeners));
    memset(&au8_vector, 0x3b, sizeof(au8_vector));
    memset(au8_dvt, 0x4c, sizeof(au8_dvt));
    memset(au8_bvt, 0x5d, sizeof(au8_bvt));
    memset(au8_svt, 0x6e, sizeof(au8_svt));
    memset(au8_lvt, 0x7f, sizeof(au8_lvt));
    au8_dvt[0].cooperative = au8_coop0;
    au8_bvt[0].release = au8_release_buffer0;
    au8_bvt[0].current_position = au8_current0;
    au8_bvt[0].frequency = au8_frequency0;
    au8_bvt[0].stop = au8_stop0;
    au8_svt[0].maximum = au8_maximum0;
    au8_svt[0].minimum = au8_minimum0;
    au8_svt[0].position = au8_position0;
    au8_svt[0].velocity = au8_velocity0;
    au8_svt[0].mode = au8_mode0;
    au8_svt[0].release = au8_release_spatial0;
    au8_lvt[0].commit = au8_commit0;
    au8_dvt[1].cooperative = au8_coop1;
    au8_bvt[1].release = au8_release_buffer1;
    au8_bvt[1].current_position = au8_current1;
    au8_bvt[1].frequency = au8_frequency1;
    au8_bvt[1].stop = au8_stop1;
    au8_svt[1].maximum = au8_maximum1;
    au8_svt[1].minimum = au8_minimum1;
    au8_svt[1].position = au8_position1;
    au8_svt[1].velocity = au8_velocity1;
    au8_svt[1].mode = au8_mode1;
    au8_svt[1].release = au8_release_spatial1;
    au8_lvt[1].commit = au8_commit1;
    for (i = 0; i < 2; ++i)
    {
        au8_devices[i].value.vt = &au8_dvt[i];
        au8_buffers[i].value.vt = &au8_bvt[i];
        au8_spatials[i].value.vt = &au8_svt[i];
        au8_listeners[i].value.vt = &au8_lvt[i];
    }
    au8_vector.value.x = au8_number(au8_captured[0]);
    au8_vector.value.y = au8_number(au8_captured[1]);
    au8_vector.value.z = au8_number(au8_captured[2]);
    memcpy(au8_expected_devices, au8_devices, sizeof(au8_devices));
    memcpy(au8_expected_buffers, au8_buffers, sizeof(au8_buffers));
    memcpy(au8_expected_spatials, au8_spatials, sizeof(au8_spatials));
    memcpy(au8_expected_listeners, au8_listeners, sizeof(au8_listeners));
    memcpy(&au8_expected_vector, &au8_vector, sizeof(au8_vector));
    memcpy(au8_expected_dvt, au8_dvt, sizeof(au8_dvt));
    memcpy(au8_expected_bvt, au8_bvt, sizeof(au8_bvt));
    memcpy(au8_expected_svt, au8_svt, sizeof(au8_svt));
    memcpy(au8_expected_lvt, au8_lvt, sizeof(au8_lvt));
    au8_expected_gate = op_audio_gate_word;
    au8_expected_listener = op_audio_listener;
    op_audio_co_initialize = au8_selected ? au8_coinit1 : au8_coinit0;
    op_audio_co_uninitialize = au8_selected ? au8_couninit1 : au8_couninit0;
    au8_calls = 0;
}
static int op_test_audio_release_buffer(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au8_selected = 0; au8_selected < 2; ++au8_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au8_mutate = 0; au8_mutate < 2; ++au8_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au8_listeners[au8_selected].value;
                            au8_word = results[pattern % 4];
                            au8_result_word = results[ri];
                            au8_minimum_bits = values[pattern];
                            au8_maximum_bits = values[(pattern + 3) % 8];
                            au8_captured[0] = values[pattern];
                            au8_captured[1] = values[(pattern + 1) % 8];
                            au8_captured[2] = values[(pattern + 2) % 8];
                            au8_seed();
                            au8_expected_this = null_pointer ? 0 : &au8_buffers[au8_selected].value;
                            au8_expected_count = !null_pointer && gate;
                            au8_expected_codes[0] = 12;
                            au8_expected_codes[1] = 0;
                            op_audio_release_buffer(null_pointer ? 0 : &au8_buffers[au8_selected].value);
                            AU8_CHECK(au8_calls == au8_expected_count);
                            au8_verify();
                            AU8_CHECK(op_audio_co_initialize == (au8_selected ? au8_coinit1 : au8_coinit0));
                            AU8_CHECK(op_audio_co_uninitialize == (au8_selected ? au8_couninit1 : au8_couninit0));
                        }
    printf("audio_release_buffer: %d checks, %d failures\n", au8_checks, au8_failures);
    return au8_failures != 0;
}

#undef AU8_CHECK

#include "../src/audio_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int au9_checks, au9_failures;
static void au9_check(int ok, int line)
{
    ++au9_checks;
    if (!ok)
    {
        ++au9_failures;
        printf("line%d failed\n", line);
    }
}
#define AU9_CHECK(x) au9_check(!!(x), __LINE__)
typedef struct AU9_OWNED_DEVICE
{
    unsigned int before;
    OP_AUDIO_DEVICE value;
    unsigned int payload[3], after;
} AU9_OWNED_DEVICE;
typedef struct AU9_OWNED_BUFFER
{
    unsigned int before;
    OP_AUDIO_BUFFER value;
    unsigned int payload[3], after;
} AU9_OWNED_BUFFER;
typedef struct AU9_OWNED_SPATIAL
{
    unsigned int before;
    OP_AUDIO_SPATIAL value;
    unsigned int payload[3], after;
} AU9_OWNED_SPATIAL;
typedef struct AU9_OWNED_LISTENER
{
    unsigned int before;
    OP_AUDIO_LISTENER value;
    unsigned int payload[3], after;
} AU9_OWNED_LISTENER;
typedef struct AU9_OWNED_VECTOR
{
    unsigned int before;
    OP_AUDIO_VECTOR value;
    unsigned int after;
} AU9_OWNED_VECTOR;
static AU9_OWNED_DEVICE au9_devices[2], au9_expected_devices[2];
static AU9_OWNED_BUFFER au9_buffers[2], au9_expected_buffers[2];
static AU9_OWNED_SPATIAL au9_spatials[2], au9_expected_spatials[2];
static AU9_OWNED_LISTENER au9_listeners[2], au9_expected_listeners[2];
static AU9_OWNED_VECTOR au9_vector, au9_expected_vector;
static OP_AUDIO_DEVICE_VT au9_dvt[2], au9_expected_dvt[2];
static OP_AUDIO_BUFFER_VT au9_bvt[2], au9_expected_bvt[2];
static OP_AUDIO_SPATIAL_VT au9_svt[2], au9_expected_svt[2];
static OP_AUDIO_LISTENER_VT au9_lvt[2], au9_expected_lvt[2];
static unsigned int au9_selected, au9_word, au9_minimum_bits, au9_maximum_bits, au9_captured[3], au9_result_word,
    au9_expected_gate;
static OP_AUDIO_LISTENER *au9_expected_listener;
static void *au9_expected_this;
static int au9_calls, au9_expected_count, au9_expected_codes[2], au9_mutate;
static unsigned int au9_bits(float value)
{
    unsigned int au9_bits_word;
    memcpy(&au9_bits_word, &value, 4);
    return au9_bits_word;
}
static float au9_number(unsigned int au9_number_word)
{
    float value;
    memcpy(&value, &au9_number_word, 4);
    return value;
}
static void au9_verify(void)
{
    AU9_CHECK(memcmp(au9_devices, au9_expected_devices, sizeof(au9_devices)) == 0);
    AU9_CHECK(memcmp(au9_buffers, au9_expected_buffers, sizeof(au9_buffers)) == 0);
    AU9_CHECK(memcmp(au9_spatials, au9_expected_spatials, sizeof(au9_spatials)) == 0);
    AU9_CHECK(memcmp(au9_listeners, au9_expected_listeners, sizeof(au9_listeners)) == 0);
    AU9_CHECK(memcmp(&au9_vector, &au9_expected_vector, sizeof(au9_vector)) == 0);
    AU9_CHECK(memcmp(au9_dvt, au9_expected_dvt, sizeof(au9_dvt)) == 0);
    AU9_CHECK(memcmp(au9_bvt, au9_expected_bvt, sizeof(au9_bvt)) == 0);
    AU9_CHECK(memcmp(au9_svt, au9_expected_svt, sizeof(au9_svt)) == 0);
    AU9_CHECK(memcmp(au9_lvt, au9_expected_lvt, sizeof(au9_lvt)) == 0);
    AU9_CHECK(op_audio_gate_word == au9_expected_gate);
    AU9_CHECK(op_audio_listener == au9_expected_listener);
}
static void au9_observe(int code, unsigned int table, void *object)
{
    AU9_CHECK(au9_calls < au9_expected_count && au9_calls < 2);
    if (au9_calls >= au9_expected_count || au9_calls >= 2)
        exit(2);
    AU9_CHECK(code == au9_expected_codes[au9_calls]);
    AU9_CHECK(table == (au9_calls && au9_mutate ? 1 - au9_selected : au9_selected));
    AU9_CHECK(object == au9_expected_this);
    au9_verify();
    ++au9_calls;
}
static void au9_change(void)
{
    if (au9_mutate && au9_calls == 1)
    {
        op_audio_gate_word = au9_expected_gate = 0;
        au9_devices[au9_selected].value.vt = au9_expected_devices[au9_selected].value.vt = &au9_dvt[1 - au9_selected];
        au9_buffers[au9_selected].value.vt = au9_expected_buffers[au9_selected].value.vt = &au9_bvt[1 - au9_selected];
        au9_spatials[au9_selected].value.vt = au9_expected_spatials[au9_selected].value.vt = &au9_svt[1 - au9_selected];
        au9_listeners[au9_selected].value.vt = au9_expected_listeners[au9_selected].value.vt =
            &au9_lvt[1 - au9_selected];
        op_audio_listener = au9_expected_listener = &au9_listeners[1 - au9_selected].value;
        au9_vector.value.x = au9_expected_vector.value.x = 7;
        au9_vector.value.y = au9_expected_vector.value.y = 9;
        au9_vector.value.z = au9_expected_vector.value.z = 11;
    }
}
static int __stdcall au9_coinit0(void *reserved)
{
    au9_observe(1, 0, 0);
    AU9_CHECK(reserved == 0);
    au9_change();
    return (int)au9_result_word;
}
static void __stdcall au9_couninit0(void)
{
    au9_observe(2, 0, 0);
    au9_change();
}
static int __stdcall au9_coop0(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au9_observe(3, 0, o);
    AU9_CHECK(window == au9_word && flags == 2);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_frequency0(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au9_observe(4, 0, o);
    AU9_CHECK(frequency == au9_word);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_maximum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au9_observe(5, 0, o);
    AU9_CHECK(au9_bits(value) == au9_maximum_bits && flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_minimum0(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au9_observe(6, 0, o);
    AU9_CHECK(au9_bits(value) == au9_minimum_bits && flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_position0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au9_observe(7, 0, o);
    AU9_CHECK(au9_bits(x) == au9_captured[0] && au9_bits(y) == au9_captured[1] && au9_bits(z) == au9_captured[2] &&
              flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_velocity0(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au9_observe(8, 0, o);
    AU9_CHECK(au9_bits(x) == au9_captured[0] && au9_bits(y) == au9_captured[1] && au9_bits(z) == au9_captured[2] &&
              flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_commit0(OP_AUDIO_LISTENER *o)
{
    au9_observe(9, 0, o);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_mode0(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au9_observe(10, 0, o);
    AU9_CHECK(mode == au9_word && flags == 0);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_stop0(OP_AUDIO_BUFFER *o)
{
    au9_observe(11, 0, o);
    au9_change();
    return (int)au9_result_word;
}
static unsigned int __stdcall au9_release_buffer0(OP_AUDIO_BUFFER *o)
{
    au9_observe(12, 0, o);
    au9_change();
    return (unsigned int)au9_result_word;
}
static unsigned int __stdcall au9_release_spatial0(OP_AUDIO_SPATIAL *o)
{
    au9_observe(13, 0, o);
    au9_change();
    return (unsigned int)au9_result_word;
}
static int __stdcall au9_current0(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au9_observe(14, 0, o);
    AU9_CHECK(position == 0);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_coinit1(void *reserved)
{
    au9_observe(1, 1, 0);
    AU9_CHECK(reserved == 0);
    au9_change();
    return (int)au9_result_word;
}
static void __stdcall au9_couninit1(void)
{
    au9_observe(2, 1, 0);
    au9_change();
}
static int __stdcall au9_coop1(OP_AUDIO_DEVICE *o, unsigned int window, unsigned int flags)
{
    au9_observe(3, 1, o);
    AU9_CHECK(window == au9_word && flags == 2);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_frequency1(OP_AUDIO_BUFFER *o, unsigned int frequency)
{
    au9_observe(4, 1, o);
    AU9_CHECK(frequency == au9_word);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_maximum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au9_observe(5, 1, o);
    AU9_CHECK(au9_bits(value) == au9_maximum_bits && flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_minimum1(OP_AUDIO_SPATIAL *o, float value, unsigned int flags)
{
    au9_observe(6, 1, o);
    AU9_CHECK(au9_bits(value) == au9_minimum_bits && flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_position1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au9_observe(7, 1, o);
    AU9_CHECK(au9_bits(x) == au9_captured[0] && au9_bits(y) == au9_captured[1] && au9_bits(z) == au9_captured[2] &&
              flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_velocity1(OP_AUDIO_SPATIAL *o, float x, float y, float z, unsigned int flags)
{
    au9_observe(8, 1, o);
    AU9_CHECK(au9_bits(x) == au9_captured[0] && au9_bits(y) == au9_captured[1] && au9_bits(z) == au9_captured[2] &&
              flags == 1);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_commit1(OP_AUDIO_LISTENER *o)
{
    au9_observe(9, 1, o);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_mode1(OP_AUDIO_SPATIAL *o, unsigned int mode, unsigned int flags)
{
    au9_observe(10, 1, o);
    AU9_CHECK(mode == au9_word && flags == 0);
    au9_change();
    return (int)au9_result_word;
}
static int __stdcall au9_stop1(OP_AUDIO_BUFFER *o)
{
    au9_observe(11, 1, o);
    au9_change();
    return (int)au9_result_word;
}
static unsigned int __stdcall au9_release_buffer1(OP_AUDIO_BUFFER *o)
{
    au9_observe(12, 1, o);
    au9_change();
    return (unsigned int)au9_result_word;
}
static unsigned int __stdcall au9_release_spatial1(OP_AUDIO_SPATIAL *o)
{
    au9_observe(13, 1, o);
    au9_change();
    return (unsigned int)au9_result_word;
}
static int __stdcall au9_current1(OP_AUDIO_BUFFER *o, unsigned int position)
{
    au9_observe(14, 1, o);
    AU9_CHECK(position == 0);
    au9_change();
    return (int)au9_result_word;
}
static void au9_seed(void)
{
    unsigned int i;
    memset(au9_devices, 0x57, sizeof(au9_devices));
    memset(au9_buffers, 0x68, sizeof(au9_buffers));
    memset(au9_spatials, 0x79, sizeof(au9_spatials));
    memset(au9_listeners, 0x2a, sizeof(au9_listeners));
    memset(&au9_vector, 0x3b, sizeof(au9_vector));
    memset(au9_dvt, 0x4c, sizeof(au9_dvt));
    memset(au9_bvt, 0x5d, sizeof(au9_bvt));
    memset(au9_svt, 0x6e, sizeof(au9_svt));
    memset(au9_lvt, 0x7f, sizeof(au9_lvt));
    au9_dvt[0].cooperative = au9_coop0;
    au9_bvt[0].release = au9_release_buffer0;
    au9_bvt[0].current_position = au9_current0;
    au9_bvt[0].frequency = au9_frequency0;
    au9_bvt[0].stop = au9_stop0;
    au9_svt[0].maximum = au9_maximum0;
    au9_svt[0].minimum = au9_minimum0;
    au9_svt[0].position = au9_position0;
    au9_svt[0].velocity = au9_velocity0;
    au9_svt[0].mode = au9_mode0;
    au9_svt[0].release = au9_release_spatial0;
    au9_lvt[0].commit = au9_commit0;
    au9_dvt[1].cooperative = au9_coop1;
    au9_bvt[1].release = au9_release_buffer1;
    au9_bvt[1].current_position = au9_current1;
    au9_bvt[1].frequency = au9_frequency1;
    au9_bvt[1].stop = au9_stop1;
    au9_svt[1].maximum = au9_maximum1;
    au9_svt[1].minimum = au9_minimum1;
    au9_svt[1].position = au9_position1;
    au9_svt[1].velocity = au9_velocity1;
    au9_svt[1].mode = au9_mode1;
    au9_svt[1].release = au9_release_spatial1;
    au9_lvt[1].commit = au9_commit1;
    for (i = 0; i < 2; ++i)
    {
        au9_devices[i].value.vt = &au9_dvt[i];
        au9_buffers[i].value.vt = &au9_bvt[i];
        au9_spatials[i].value.vt = &au9_svt[i];
        au9_listeners[i].value.vt = &au9_lvt[i];
    }
    au9_vector.value.x = au9_number(au9_captured[0]);
    au9_vector.value.y = au9_number(au9_captured[1]);
    au9_vector.value.z = au9_number(au9_captured[2]);
    memcpy(au9_expected_devices, au9_devices, sizeof(au9_devices));
    memcpy(au9_expected_buffers, au9_buffers, sizeof(au9_buffers));
    memcpy(au9_expected_spatials, au9_spatials, sizeof(au9_spatials));
    memcpy(au9_expected_listeners, au9_listeners, sizeof(au9_listeners));
    memcpy(&au9_expected_vector, &au9_vector, sizeof(au9_vector));
    memcpy(au9_expected_dvt, au9_dvt, sizeof(au9_dvt));
    memcpy(au9_expected_bvt, au9_bvt, sizeof(au9_bvt));
    memcpy(au9_expected_svt, au9_svt, sizeof(au9_svt));
    memcpy(au9_expected_lvt, au9_lvt, sizeof(au9_lvt));
    au9_expected_gate = op_audio_gate_word;
    au9_expected_listener = op_audio_listener;
    op_audio_co_initialize = au9_selected ? au9_coinit1 : au9_coinit0;
    op_audio_co_uninitialize = au9_selected ? au9_couninit1 : au9_couninit0;
    au9_calls = 0;
}
static int op_test_audio3d_release(void)
{
    unsigned int gi, ri, pattern, gate;
    int null_pointer;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    static const unsigned int values[] = {0,           0x80000000u, 0x3f800000u, 0xbf800000u,
                                          0x3f000000u, 0x40000000u, 0x49800000u, 0xc9800000u};
    for (au9_selected = 0; au9_selected < 2; ++au9_selected)
        for (gi = 0; gi < 4; ++gi)
            for (null_pointer = 0; null_pointer < 2; ++null_pointer)
                for (au9_mutate = 0; au9_mutate < 2; ++au9_mutate)
                    for (ri = 0; ri < 4; ++ri)
                        for (pattern = 0; pattern < 8; ++pattern)
                        {
                            gate = gates[gi];
                            if (gate && null_pointer)
                                continue;
                            op_audio_gate_word = gate;
                            op_audio_listener = null_pointer ? 0 : &au9_listeners[au9_selected].value;
                            au9_word = results[pattern % 4];
                            au9_result_word = results[ri];
                            au9_minimum_bits = values[pattern];
                            au9_maximum_bits = values[(pattern + 3) % 8];
                            au9_captured[0] = values[pattern];
                            au9_captured[1] = values[(pattern + 1) % 8];
                            au9_captured[2] = values[(pattern + 2) % 8];
                            au9_seed();
                            au9_expected_this = null_pointer ? 0 : &au9_spatials[au9_selected].value;
                            au9_expected_count = gate != 0;
                            au9_expected_codes[0] = 13;
                            au9_expected_codes[1] = 0;
                            op_audio3d_release(null_pointer ? 0 : &au9_spatials[au9_selected].value);
                            AU9_CHECK(au9_calls == au9_expected_count);
                            au9_verify();
                            AU9_CHECK(op_audio_co_initialize == (au9_selected ? au9_coinit1 : au9_coinit0));
                            AU9_CHECK(op_audio_co_uninitialize == (au9_selected ? au9_couninit1 : au9_couninit0));
                        }
    printf("audio3d_release: %d checks, %d failures\n", au9_checks, au9_failures);
    return au9_failures != 0;
}

#undef AU9_CHECK

/* The stop fixture's callback observers also verify a real multi-wrapper sequence.
 * Each step installs only its next expected calls; owned state is not reseeded. */
static void aucy_step(int count, int first, int second, void *object)
{
    au7_calls = 0;
    au7_expected_count = count;
    au7_expected_codes[0] = first;
    au7_expected_codes[1] = second;
    au7_expected_this = object;
}
static void aucy_done(void)
{
    au7_check(au7_calls == au7_expected_count, __LINE__);
    au7_verify();
}
static int op_test_audio_connected(void)
{
    unsigned int gi, ri, saved_selected;
    int before_checks = au7_checks, before_failures = au7_failures, enable;
    static const unsigned int gates[] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int results[] = {0, 1, 0x80004005u, 0xffffffffu};
    for (saved_selected = 0; saved_selected < 2; ++saved_selected)
        for (gi = 0; gi < 4; ++gi)
            for (ri = 0; ri < 4; ++ri)
            {
                au7_selected = saved_selected;
                op_audio_gate_word = gates[gi];
                op_audio_listener = &au7_listeners[au7_selected].value;
                au7_mutate = 0;
                au7_word = results[(ri + 1) % 4];
                au7_result_word = results[ri];
                au7_minimum_bits = 0x3f000000u;
                au7_maximum_bits = 0x40000000u;
                au7_captured[0] = 0;
                au7_captured[1] = 0x80000000u;
                au7_captured[2] = 0x3f800000u;
                au7_seed();
                aucy_step(1, 1, 0, 0);
                au7_check((unsigned int)op_audio_com_initialize() == au7_result_word, __LINE__);
                aucy_done();
                aucy_step(1, 3, 0, &au7_devices[au7_selected].value);
                op_audio_set_cooperative(&au7_devices[au7_selected].value, au7_word);
                aucy_done();
                enable = op_audio_gate_word != 0;
                aucy_step(enable, 4, 0, &au7_buffers[au7_selected].value);
                op_audio_set_frequency(&au7_buffers[au7_selected].value, au7_word);
                aucy_done();
                aucy_step(2, 5, 6, &au7_spatials[au7_selected].value);
                op_audio3d_set_distance_bounds(&au7_spatials[au7_selected].value, au7_number(au7_minimum_bits),
                                               au7_number(au7_maximum_bits));
                aucy_done();
                aucy_step(1, 10, 0, &au7_spatials[au7_selected].value);
                op_audio3d_set_mode(&au7_spatials[au7_selected].value, au7_word);
                aucy_done();
                aucy_step(1, 9, 0, op_audio_listener);
                op_audio_commit_listener();
                aucy_done();
                au7_mutate = 1;
                aucy_step(enable ? 2 : 0, 11, 14, &au7_buffers[au7_selected].value);
                au7_check(op_audio_stop(&au7_buffers[au7_selected].value) == enable, __LINE__);
                aucy_done();
                /* Stop's first callback cleared the gate; releases must now skip without reset. */
                au7_mutate = 0;
                aucy_step(0, 12, 0, &au7_buffers[au7_selected].value);
                op_audio_release_buffer(&au7_buffers[au7_selected].value);
                aucy_done();
                aucy_step(0, 13, 0, &au7_spatials[au7_selected].value);
                op_audio3d_release(&au7_spatials[au7_selected].value);
                aucy_done();
                /* Explicitly enable release of the live swapped objects; preserve their payloads. */
                op_audio_gate_word = au7_expected_gate = 1;
                if (enable)
                    au7_selected = 1 - saved_selected;
                aucy_step(1, 12, 0, &au7_buffers[saved_selected].value);
                op_audio_release_buffer(&au7_buffers[saved_selected].value);
                aucy_done();
                aucy_step(1, 13, 0, &au7_spatials[saved_selected].value);
                op_audio3d_release(&au7_spatials[saved_selected].value);
                aucy_done();
                au7_selected = saved_selected;
                aucy_step(1, 2, 0, 0);
                op_audio_com_uninitialize();
                aucy_done();
            }
    printf("audio connected: %d checks, %d failures\n", au7_checks - before_checks, au7_failures - before_failures);
    return au7_failures != before_failures;
}
