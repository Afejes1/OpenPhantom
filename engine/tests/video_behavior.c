#include "../src/video_helpers.h"
static int video_route;
void *op_video_sound_driver, *op_video_open_miles;
unsigned int(__stdcall *op_video_dd_type)(unsigned int);
int(__stdcall *op_video_sound_system)(void *, void *);
void *(__stdcall *op_video_bink_open)(const char *, unsigned int);
int(__stdcall *op_video_bink_close)(void *);
int(__stdcall *op_video_bink_volume)(void *, unsigned int);

#include "../src/video_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int vp0_checks, vp0_failures;
static void vp0_check(int ok, int line)
{
    ++vp0_checks;
    if (!ok)
    {
        ++vp0_failures;
        printf("line%d failed\n", line);
    }
}
#define VP0_CHECK(x) vp0_check(!!(x), __LINE__)
typedef struct VP0_OWNED_VIDEO
{
    unsigned int before;
    OP_VIDEO value;
    unsigned int after;
} VP0_OWNED_VIDEO;
typedef struct VP0_OWNED_DESC
{
    unsigned int before;
    OP_VIDEO_DESC value;
    unsigned int after;
} VP0_OWNED_DESC;
static VP0_OWNED_VIDEO vp0_videos[2], vp0_expected_videos[2];
static VP0_OWNED_DESC vp0_descriptions[2], vp0_expected_descriptions[2];
static char vp0_names[2][24], vp0_expected_names[2][24];
static unsigned char vp0_tokens[4][16], vp0_expected_tokens[4][16];
static void *vp0_expected_driver, *vp0_expected_miles, *vp0_driver_result, *vp0_handle_result, *vp0_captured_handle;
static OP_VIDEO *vp0_allocation_result, *vp0_active_video;
static OP_VIDEO_DESC *vp0_active_description;
static const char *vp0_active_name;
static unsigned int vp0_selected, vp0_description_index, vp0_argument3_value, vp0_argument4_value, vp0_word_value,
    vp0_surface_result, vp0_captured_surface;
static int vp0_calls, vp0_expected_count, vp0_codes[8], vp0_mutate, vp0_kind;
static const unsigned int vp0_words[] = {0, 1, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static void vp0_verify(void)
{
    VP0_CHECK(memcmp(vp0_videos, vp0_expected_videos, sizeof(vp0_videos)) == 0);
    VP0_CHECK(memcmp(vp0_descriptions, vp0_expected_descriptions, sizeof(vp0_descriptions)) == 0);
    VP0_CHECK(memcmp(vp0_names, vp0_expected_names, sizeof(vp0_names)) == 0);
    VP0_CHECK(memcmp(vp0_tokens, vp0_expected_tokens, sizeof(vp0_tokens)) == 0);
    VP0_CHECK(op_video_sound_driver == vp0_expected_driver);
    VP0_CHECK(op_video_open_miles == vp0_expected_miles);
}
static void vp0_observe(int code)
{
    VP0_CHECK(vp0_calls < vp0_expected_count && vp0_calls < 8);
    if (vp0_calls >= vp0_expected_count || vp0_calls >= 8)
        exit(2);
    VP0_CHECK(vp0_codes[vp0_calls] == code);
    vp0_verify();
    ++vp0_calls;
}
static void vp0_change(void)
{
    if (vp0_mutate)
    {
        op_video_sound_driver = vp0_expected_driver = 0;
        op_video_open_miles = vp0_expected_miles = &vp0_tokens[3][0];
        vp0_videos[vp0_selected].value.handle = vp0_expected_videos[vp0_selected].value.handle = &vp0_tokens[2][0];
        vp0_videos[vp0_selected].value.argument4 = vp0_expected_videos[vp0_selected].value.argument4 = 0x87654321u;
        vp0_descriptions[vp0_description_index].value.mode =
            vp0_expected_descriptions[vp0_description_index].value.mode = 7;
        vp0_descriptions[vp0_description_index].value.format[2] =
            vp0_expected_descriptions[vp0_description_index].value.format[2] = 5;
        vp0_names[vp0_selected][2] = vp0_expected_names[vp0_selected][2] = 'Q';
    }
}
static void *vp0_driver(void)
{
    vp0_observe(1);
    vp0_change();
    vp0_expected_driver = vp0_driver_result;
    return vp0_driver_result;
}
static int __stdcall vp0_sound_system(void *miles, void *driver)
{
    vp0_observe(2);
    VP0_CHECK(miles == vp0_expected_miles);
    VP0_CHECK(driver == vp0_driver_result);
    vp0_change();
    return (int)vp0_word_value;
}
static void *vp0_allocate(unsigned int size)
{
    vp0_observe(3);
    VP0_CHECK(size == 24);
    vp0_change();
    return vp0_allocation_result;
}
static void *__stdcall vp0_bink_open(const char *name, unsigned int flags)
{
    vp0_observe(4);
    VP0_CHECK(name == vp0_active_name);
    VP0_CHECK(flags == 0x00400000);
    vp0_change();
    vp0_expected_videos[vp0_selected].value.handle = vp0_handle_result;
    return vp0_handle_result;
}
static unsigned int vp0_surface(OP_VIDEO_DESC *description)
{
    vp0_observe(5);
    VP0_CHECK(description == vp0_active_description);
    vp0_change();
    vp0_expected_videos[vp0_selected].value.surface_type = vp0_surface_result;
    vp0_expected_videos[vp0_selected].value.description = vp0_active_description;
    vp0_expected_videos[vp0_selected].value.argument3 = vp0_argument3_value;
    vp0_expected_videos[vp0_selected].value.argument4 = vp0_argument4_value;
    vp0_expected_videos[vp0_selected].value.done = 0;
    return vp0_surface_result;
}
static void vp0_release(void *memory)
{
    vp0_observe(6);
    VP0_CHECK(memory == vp0_active_video);
    vp0_change();
}
static int __stdcall vp0_bink_close(void *handle)
{
    vp0_observe(7);
    VP0_CHECK(handle == vp0_captured_handle);
    vp0_change();
    vp0_expected_videos[vp0_selected].value.handle = 0;
    return (int)vp0_word_value;
}
static int __stdcall vp0_bink_volume(void *handle, unsigned int volume)
{
    vp0_observe(8);
    VP0_CHECK(handle == vp0_captured_handle);
    VP0_CHECK(volume == vp0_word_value);
    vp0_change();
    return (int)~vp0_word_value;
}
static unsigned int __stdcall vp0_dd_type(unsigned int surface)
{
    vp0_observe(9);
    VP0_CHECK(surface == vp0_captured_surface);
    vp0_change();
    return vp0_surface_result;
}
static void vp0_snapshot(void)
{
    memcpy(vp0_expected_videos, vp0_videos, sizeof(vp0_videos));
    memcpy(vp0_expected_descriptions, vp0_descriptions, sizeof(vp0_descriptions));
    memcpy(vp0_expected_names, vp0_names, sizeof(vp0_names));
    memcpy(vp0_expected_tokens, vp0_tokens, sizeof(vp0_tokens));
    vp0_expected_driver = op_video_sound_driver;
    vp0_expected_miles = op_video_open_miles;
}
static void vp0_seed(unsigned int profile)
{
    unsigned int i;
    op_video_dd_type = vp0_dd_type;
    op_video_sound_system = vp0_sound_system;
    op_video_bink_open = vp0_bink_open;
    op_video_bink_close = vp0_bink_close;
    op_video_bink_volume = vp0_bink_volume;
    memset(vp0_videos, 0x57, sizeof(vp0_videos));
    memset(vp0_descriptions, 0x68, sizeof(vp0_descriptions));
    memset(vp0_names, 0, sizeof(vp0_names));
    strcpy(vp0_names[0], "fixture-a");
    strcpy(vp0_names[1], "fixture-b");
    memset(vp0_tokens, 0x79, sizeof(vp0_tokens));
    for (i = 0; i < 2; ++i)
    {
        vp0_videos[i].value.handle = &vp0_tokens[i][0];
        vp0_videos[i].value.description = &vp0_descriptions[i].value;
    }
    vp0_selected = profile % 2;
    vp0_description_index = (profile / 2) % 2;
    vp0_active_video = &vp0_videos[vp0_selected].value;
    vp0_active_description = &vp0_descriptions[vp0_description_index].value;
    vp0_active_name = vp0_names[vp0_selected];
    op_video_sound_driver = &vp0_tokens[0][0];
    op_video_open_miles = &vp0_tokens[1][0];
    vp0_driver_result = &vp0_tokens[0][0];
    vp0_handle_result = &vp0_tokens[1][0];
    vp0_allocation_result = vp0_active_video;
    vp0_argument3_value = vp0_words[profile % 6];
    vp0_argument4_value = vp0_words[(profile + 1) % 6];
    vp0_word_value = vp0_words[(profile + 2) % 6];
    vp0_surface_result = vp0_words[(profile + 3) % 6];
    vp0_captured_handle = vp0_active_video->handle;
    vp0_captured_surface = vp0_active_description->surface;
    vp0_calls = 0;
    vp0_expected_count = 0;
    vp0_mutate = 0;
    vp0_kind = 0;
    vp0_snapshot();
}
static void vp0_finish(void)
{
    VP0_CHECK(vp0_calls == vp0_expected_count);
    vp0_verify();
    VP0_CHECK(op_video_dd_type == vp0_dd_type);
    VP0_CHECK(op_video_sound_system == vp0_sound_system);
    VP0_CHECK(op_video_bink_open == vp0_bink_open);
    VP0_CHECK(op_video_bink_close == vp0_bink_close);
    VP0_CHECK(op_video_bink_volume == vp0_bink_volume);
}
static int op_test_video_open(void)
{
    unsigned int p, initial, get_result, allocate_ok, open_ok;
    int change_state;
    OP_VIDEO *result;
    for (p = 0; p < 24; ++p)
        for (initial = 0; initial < 3; ++initial)
            for (get_result = 0; get_result < 3; ++get_result)
                for (allocate_ok = 0; allocate_ok < 2; ++allocate_ok)
                    for (open_ok = 0; open_ok < 2; ++open_ok)
                        for (change_state = 0; change_state < 2; ++change_state)
                        {
                            vp0_seed(p);
                            vp0_mutate = change_state;
                            op_video_sound_driver = initial ? &vp0_tokens[initial - 1][0] : 0;
                            vp0_driver_result = get_result ? &vp0_tokens[get_result - 1][0] : 0;
                            vp0_allocation_result = allocate_ok ? vp0_active_video : 0;
                            vp0_handle_result = open_ok ? &vp0_tokens[2][0] : 0;
                            vp0_snapshot();
                            if (!initial)
                            {
                                vp0_codes[vp0_expected_count++] = 1;
                                if (get_result)
                                    vp0_codes[vp0_expected_count++] = 2;
                            }
                            vp0_codes[vp0_expected_count++] = 3;
                            if (allocate_ok)
                            {
                                vp0_codes[vp0_expected_count++] = 4;
                                vp0_codes[vp0_expected_count++] = open_ok ? 5 : 6;
                            }
                            result = op_video_open(vp0_active_name, vp0_active_description, vp0_argument3_value,
                                                   vp0_argument4_value);
                            VP0_CHECK(result == (allocate_ok && open_ok ? vp0_active_video : 0));
                            vp0_finish();
                        }
    printf("video_open: %d checks, %d failures\n", vp0_checks, vp0_failures);
    return vp0_failures != 0;
}

#undef VP0_CHECK

#include "../src/video_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int vp1_checks, vp1_failures;
static void vp1_check(int ok, int line)
{
    ++vp1_checks;
    if (!ok)
    {
        ++vp1_failures;
        printf("line%d failed\n", line);
    }
}
#define VP1_CHECK(x) vp1_check(!!(x), __LINE__)
typedef struct VP1_OWNED_VIDEO
{
    unsigned int before;
    OP_VIDEO value;
    unsigned int after;
} VP1_OWNED_VIDEO;
typedef struct VP1_OWNED_DESC
{
    unsigned int before;
    OP_VIDEO_DESC value;
    unsigned int after;
} VP1_OWNED_DESC;
static VP1_OWNED_VIDEO vp1_videos[2], vp1_expected_videos[2];
static VP1_OWNED_DESC vp1_descriptions[2], vp1_expected_descriptions[2];
static char vp1_names[2][24], vp1_expected_names[2][24];
static unsigned char vp1_tokens[4][16], vp1_expected_tokens[4][16];
static void *vp1_expected_driver, *vp1_expected_miles, *vp1_driver_result, *vp1_handle_result, *vp1_captured_handle;
static OP_VIDEO *vp1_allocation_result, *vp1_active_video;
static OP_VIDEO_DESC *vp1_active_description;
static const char *vp1_active_name;
static unsigned int vp1_selected, vp1_description_index, vp1_argument3_value, vp1_argument4_value, vp1_word_value,
    vp1_surface_result, vp1_captured_surface;
static int vp1_calls, vp1_expected_count, vp1_codes[8], vp1_mutate, vp1_kind;
static const unsigned int vp1_words[] = {0, 1, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static void vp1_verify(void)
{
    VP1_CHECK(memcmp(vp1_videos, vp1_expected_videos, sizeof(vp1_videos)) == 0);
    VP1_CHECK(memcmp(vp1_descriptions, vp1_expected_descriptions, sizeof(vp1_descriptions)) == 0);
    VP1_CHECK(memcmp(vp1_names, vp1_expected_names, sizeof(vp1_names)) == 0);
    VP1_CHECK(memcmp(vp1_tokens, vp1_expected_tokens, sizeof(vp1_tokens)) == 0);
    VP1_CHECK(op_video_sound_driver == vp1_expected_driver);
    VP1_CHECK(op_video_open_miles == vp1_expected_miles);
}
static void vp1_observe(int code)
{
    VP1_CHECK(vp1_calls < vp1_expected_count && vp1_calls < 8);
    if (vp1_calls >= vp1_expected_count || vp1_calls >= 8)
        exit(2);
    VP1_CHECK(vp1_codes[vp1_calls] == code);
    vp1_verify();
    ++vp1_calls;
}
static void vp1_change(void)
{
    if (vp1_mutate)
    {
        op_video_sound_driver = vp1_expected_driver = 0;
        op_video_open_miles = vp1_expected_miles = &vp1_tokens[3][0];
        vp1_videos[vp1_selected].value.handle = vp1_expected_videos[vp1_selected].value.handle = &vp1_tokens[2][0];
        vp1_videos[vp1_selected].value.argument4 = vp1_expected_videos[vp1_selected].value.argument4 = 0x87654321u;
        vp1_descriptions[vp1_description_index].value.mode =
            vp1_expected_descriptions[vp1_description_index].value.mode = 7;
        vp1_descriptions[vp1_description_index].value.format[2] =
            vp1_expected_descriptions[vp1_description_index].value.format[2] = 5;
        vp1_names[vp1_selected][2] = vp1_expected_names[vp1_selected][2] = 'Q';
    }
}
static void *vp1_driver(void)
{
    vp1_observe(1);
    vp1_change();
    vp1_expected_driver = vp1_driver_result;
    return vp1_driver_result;
}
static int __stdcall vp1_sound_system(void *miles, void *driver)
{
    vp1_observe(2);
    VP1_CHECK(miles == vp1_expected_miles);
    VP1_CHECK(driver == vp1_driver_result);
    vp1_change();
    return (int)vp1_word_value;
}
static void *vp1_allocate(unsigned int size)
{
    vp1_observe(3);
    VP1_CHECK(size == 24);
    vp1_change();
    return vp1_allocation_result;
}
static void *__stdcall vp1_bink_open(const char *name, unsigned int flags)
{
    vp1_observe(4);
    VP1_CHECK(name == vp1_active_name);
    VP1_CHECK(flags == 0x00400000);
    vp1_change();
    vp1_expected_videos[vp1_selected].value.handle = vp1_handle_result;
    return vp1_handle_result;
}
static void vp1_release(void *memory)
{
    vp1_observe(6);
    VP1_CHECK(memory == vp1_active_video);
    vp1_change();
}
static int __stdcall vp1_bink_close(void *handle)
{
    vp1_observe(7);
    VP1_CHECK(handle == vp1_captured_handle);
    vp1_change();
    vp1_expected_videos[vp1_selected].value.handle = 0;
    return (int)vp1_word_value;
}
static int __stdcall vp1_bink_volume(void *handle, unsigned int volume)
{
    vp1_observe(8);
    VP1_CHECK(handle == vp1_captured_handle);
    VP1_CHECK(volume == vp1_word_value);
    vp1_change();
    return (int)~vp1_word_value;
}
static unsigned int __stdcall vp1_dd_type(unsigned int surface)
{
    vp1_observe(9);
    VP1_CHECK(surface == vp1_captured_surface);
    vp1_change();
    return vp1_surface_result;
}
static void vp1_snapshot(void)
{
    memcpy(vp1_expected_videos, vp1_videos, sizeof(vp1_videos));
    memcpy(vp1_expected_descriptions, vp1_descriptions, sizeof(vp1_descriptions));
    memcpy(vp1_expected_names, vp1_names, sizeof(vp1_names));
    memcpy(vp1_expected_tokens, vp1_tokens, sizeof(vp1_tokens));
    vp1_expected_driver = op_video_sound_driver;
    vp1_expected_miles = op_video_open_miles;
}
static void vp1_seed(unsigned int profile)
{
    unsigned int i;
    op_video_dd_type = vp1_dd_type;
    op_video_sound_system = vp1_sound_system;
    op_video_bink_open = vp1_bink_open;
    op_video_bink_close = vp1_bink_close;
    op_video_bink_volume = vp1_bink_volume;
    memset(vp1_videos, 0x57, sizeof(vp1_videos));
    memset(vp1_descriptions, 0x68, sizeof(vp1_descriptions));
    memset(vp1_names, 0, sizeof(vp1_names));
    strcpy(vp1_names[0], "fixture-a");
    strcpy(vp1_names[1], "fixture-b");
    memset(vp1_tokens, 0x79, sizeof(vp1_tokens));
    for (i = 0; i < 2; ++i)
    {
        vp1_videos[i].value.handle = &vp1_tokens[i][0];
        vp1_videos[i].value.description = &vp1_descriptions[i].value;
    }
    vp1_selected = profile % 2;
    vp1_description_index = (profile / 2) % 2;
    vp1_active_video = &vp1_videos[vp1_selected].value;
    vp1_active_description = &vp1_descriptions[vp1_description_index].value;
    vp1_active_name = vp1_names[vp1_selected];
    op_video_sound_driver = &vp1_tokens[0][0];
    op_video_open_miles = &vp1_tokens[1][0];
    vp1_driver_result = &vp1_tokens[0][0];
    vp1_handle_result = &vp1_tokens[1][0];
    vp1_allocation_result = vp1_active_video;
    vp1_argument3_value = vp1_words[profile % 6];
    vp1_argument4_value = vp1_words[(profile + 1) % 6];
    vp1_word_value = vp1_words[(profile + 2) % 6];
    vp1_surface_result = vp1_words[(profile + 3) % 6];
    vp1_captured_handle = vp1_active_video->handle;
    vp1_captured_surface = vp1_active_description->surface;
    vp1_calls = 0;
    vp1_expected_count = 0;
    vp1_mutate = 0;
    vp1_kind = 0;
    vp1_snapshot();
}
static void vp1_finish(void)
{
    VP1_CHECK(vp1_calls == vp1_expected_count);
    vp1_verify();
    VP1_CHECK(op_video_dd_type == vp1_dd_type);
    VP1_CHECK(op_video_sound_system == vp1_sound_system);
    VP1_CHECK(op_video_bink_open == vp1_bink_open);
    VP1_CHECK(op_video_bink_close == vp1_bink_close);
    VP1_CHECK(op_video_bink_volume == vp1_bink_volume);
}
static int op_test_video_close(void)
{
    unsigned int p, null_input, has_handle;
    int change_state;
    for (p = 0; p < 24; ++p)
        for (null_input = 0; null_input < 2; ++null_input)
            for (has_handle = 0; has_handle < 2; ++has_handle)
                for (change_state = 0; change_state < 2; ++change_state)
                {
                    vp1_seed(p);
                    vp1_mutate = change_state;
                    vp1_active_video->handle = has_handle ? &vp1_tokens[1][0] : 0;
                    vp1_captured_handle = vp1_active_video->handle;
                    vp1_snapshot();
                    if (!null_input)
                    {
                        if (has_handle)
                            vp1_codes[vp1_expected_count++] = 7;
                        vp1_codes[vp1_expected_count++] = 6;
                    }
                    VP1_CHECK(op_video_close(null_input ? 0 : vp1_active_video) == (!null_input && has_handle));
                    vp1_finish();
                }
    printf("video_close: %d checks, %d failures\n", vp1_checks, vp1_failures);
    return vp1_failures != 0;
}

#undef VP1_CHECK

#include "../src/video_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int vp2_checks, vp2_failures;
static void vp2_check(int ok, int line)
{
    ++vp2_checks;
    if (!ok)
    {
        ++vp2_failures;
        printf("line%d failed\n", line);
    }
}
#define VP2_CHECK(x) vp2_check(!!(x), __LINE__)
typedef struct VP2_OWNED_VIDEO
{
    unsigned int before;
    OP_VIDEO value;
    unsigned int after;
} VP2_OWNED_VIDEO;
typedef struct VP2_OWNED_DESC
{
    unsigned int before;
    OP_VIDEO_DESC value;
    unsigned int after;
} VP2_OWNED_DESC;
static VP2_OWNED_VIDEO vp2_videos[2], vp2_expected_videos[2];
static VP2_OWNED_DESC vp2_descriptions[2], vp2_expected_descriptions[2];
static char vp2_names[2][24], vp2_expected_names[2][24];
static unsigned char vp2_tokens[4][16], vp2_expected_tokens[4][16];
static void *vp2_expected_driver, *vp2_expected_miles, *vp2_driver_result, *vp2_handle_result, *vp2_captured_handle;
static OP_VIDEO *vp2_allocation_result, *vp2_active_video;
static OP_VIDEO_DESC *vp2_active_description;
static const char *vp2_active_name;
static unsigned int vp2_selected, vp2_description_index, vp2_argument3_value, vp2_argument4_value, vp2_word_value,
    vp2_surface_result, vp2_captured_surface;
static int vp2_calls, vp2_expected_count, vp2_codes[8], vp2_mutate, vp2_kind;
static const unsigned int vp2_words[] = {0, 1, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static void vp2_verify(void)
{
    VP2_CHECK(memcmp(vp2_videos, vp2_expected_videos, sizeof(vp2_videos)) == 0);
    VP2_CHECK(memcmp(vp2_descriptions, vp2_expected_descriptions, sizeof(vp2_descriptions)) == 0);
    VP2_CHECK(memcmp(vp2_names, vp2_expected_names, sizeof(vp2_names)) == 0);
    VP2_CHECK(memcmp(vp2_tokens, vp2_expected_tokens, sizeof(vp2_tokens)) == 0);
    VP2_CHECK(op_video_sound_driver == vp2_expected_driver);
    VP2_CHECK(op_video_open_miles == vp2_expected_miles);
}
static void vp2_observe(int code)
{
    VP2_CHECK(vp2_calls < vp2_expected_count && vp2_calls < 8);
    if (vp2_calls >= vp2_expected_count || vp2_calls >= 8)
        exit(2);
    VP2_CHECK(vp2_codes[vp2_calls] == code);
    vp2_verify();
    ++vp2_calls;
}
static void vp2_change(void)
{
    if (vp2_mutate)
    {
        op_video_sound_driver = vp2_expected_driver = 0;
        op_video_open_miles = vp2_expected_miles = &vp2_tokens[3][0];
        vp2_videos[vp2_selected].value.handle = vp2_expected_videos[vp2_selected].value.handle = &vp2_tokens[2][0];
        vp2_videos[vp2_selected].value.argument4 = vp2_expected_videos[vp2_selected].value.argument4 = 0x87654321u;
        vp2_descriptions[vp2_description_index].value.mode =
            vp2_expected_descriptions[vp2_description_index].value.mode = 7;
        vp2_descriptions[vp2_description_index].value.format[2] =
            vp2_expected_descriptions[vp2_description_index].value.format[2] = 5;
        vp2_names[vp2_selected][2] = vp2_expected_names[vp2_selected][2] = 'Q';
    }
}
static void *vp2_driver(void)
{
    vp2_observe(1);
    vp2_change();
    vp2_expected_driver = vp2_driver_result;
    return vp2_driver_result;
}
static int __stdcall vp2_sound_system(void *miles, void *driver)
{
    vp2_observe(2);
    VP2_CHECK(miles == vp2_expected_miles);
    VP2_CHECK(driver == vp2_driver_result);
    vp2_change();
    return (int)vp2_word_value;
}
static void *vp2_allocate(unsigned int size)
{
    vp2_observe(3);
    VP2_CHECK(size == 24);
    vp2_change();
    return vp2_allocation_result;
}
static void *__stdcall vp2_bink_open(const char *name, unsigned int flags)
{
    vp2_observe(4);
    VP2_CHECK(name == vp2_active_name);
    VP2_CHECK(flags == 0x00400000);
    vp2_change();
    vp2_expected_videos[vp2_selected].value.handle = vp2_handle_result;
    return vp2_handle_result;
}
static void vp2_release(void *memory)
{
    vp2_observe(6);
    VP2_CHECK(memory == vp2_active_video);
    vp2_change();
}
static int __stdcall vp2_bink_close(void *handle)
{
    vp2_observe(7);
    VP2_CHECK(handle == vp2_captured_handle);
    vp2_change();
    vp2_expected_videos[vp2_selected].value.handle = 0;
    return (int)vp2_word_value;
}
static int __stdcall vp2_bink_volume(void *handle, unsigned int volume)
{
    vp2_observe(8);
    VP2_CHECK(handle == vp2_captured_handle);
    VP2_CHECK(volume == vp2_word_value);
    vp2_change();
    return (int)~vp2_word_value;
}
static unsigned int __stdcall vp2_dd_type(unsigned int surface)
{
    vp2_observe(9);
    VP2_CHECK(surface == vp2_captured_surface);
    vp2_change();
    return vp2_surface_result;
}
static void vp2_snapshot(void)
{
    memcpy(vp2_expected_videos, vp2_videos, sizeof(vp2_videos));
    memcpy(vp2_expected_descriptions, vp2_descriptions, sizeof(vp2_descriptions));
    memcpy(vp2_expected_names, vp2_names, sizeof(vp2_names));
    memcpy(vp2_expected_tokens, vp2_tokens, sizeof(vp2_tokens));
    vp2_expected_driver = op_video_sound_driver;
    vp2_expected_miles = op_video_open_miles;
}
static void vp2_seed(unsigned int profile)
{
    unsigned int i;
    op_video_dd_type = vp2_dd_type;
    op_video_sound_system = vp2_sound_system;
    op_video_bink_open = vp2_bink_open;
    op_video_bink_close = vp2_bink_close;
    op_video_bink_volume = vp2_bink_volume;
    memset(vp2_videos, 0x57, sizeof(vp2_videos));
    memset(vp2_descriptions, 0x68, sizeof(vp2_descriptions));
    memset(vp2_names, 0, sizeof(vp2_names));
    strcpy(vp2_names[0], "fixture-a");
    strcpy(vp2_names[1], "fixture-b");
    memset(vp2_tokens, 0x79, sizeof(vp2_tokens));
    for (i = 0; i < 2; ++i)
    {
        vp2_videos[i].value.handle = &vp2_tokens[i][0];
        vp2_videos[i].value.description = &vp2_descriptions[i].value;
    }
    vp2_selected = profile % 2;
    vp2_description_index = (profile / 2) % 2;
    vp2_active_video = &vp2_videos[vp2_selected].value;
    vp2_active_description = &vp2_descriptions[vp2_description_index].value;
    vp2_active_name = vp2_names[vp2_selected];
    op_video_sound_driver = &vp2_tokens[0][0];
    op_video_open_miles = &vp2_tokens[1][0];
    vp2_driver_result = &vp2_tokens[0][0];
    vp2_handle_result = &vp2_tokens[1][0];
    vp2_allocation_result = vp2_active_video;
    vp2_argument3_value = vp2_words[profile % 6];
    vp2_argument4_value = vp2_words[(profile + 1) % 6];
    vp2_word_value = vp2_words[(profile + 2) % 6];
    vp2_surface_result = vp2_words[(profile + 3) % 6];
    vp2_captured_handle = vp2_active_video->handle;
    vp2_captured_surface = vp2_active_description->surface;
    vp2_calls = 0;
    vp2_expected_count = 0;
    vp2_mutate = 0;
    vp2_kind = 0;
    vp2_snapshot();
}
static void vp2_finish(void)
{
    VP2_CHECK(vp2_calls == vp2_expected_count);
    vp2_verify();
    VP2_CHECK(op_video_dd_type == vp2_dd_type);
    VP2_CHECK(op_video_sound_system == vp2_sound_system);
    VP2_CHECK(op_video_bink_open == vp2_bink_open);
    VP2_CHECK(op_video_bink_close == vp2_bink_close);
    VP2_CHECK(op_video_bink_volume == vp2_bink_volume);
}
static int op_test_video_set_volume(void)
{
    unsigned int p, gate, null_input;
    int change_state;
    for (p = 0; p < 24; ++p)
        for (gate = 0; gate < 3; ++gate)
            for (null_input = 0; null_input < 2; ++null_input)
                for (change_state = 0; change_state < 2; ++change_state)
                {
                    if (gate && null_input)
                        continue;
                    vp2_seed(p);
                    vp2_mutate = change_state;
                    op_video_sound_driver = gate ? &vp2_tokens[gate - 1][0] : 0;
                    vp2_snapshot();
                    vp2_expected_count = gate != 0;
                    vp2_codes[0] = 8;
                    op_video_set_volume(null_input ? 0 : vp2_active_video, vp2_word_value);
                    vp2_finish();
                }
    printf("video_set_volume: %d checks, %d failures\n", vp2_checks, vp2_failures);
    return vp2_failures != 0;
}

#undef VP2_CHECK

static void *video_allocate(unsigned int bytes)
{
    if (video_route == 1)
        return vp0_allocate(bytes);
    if (video_route == 2)
        return vp1_allocate(bytes);
    return vp2_allocate(bytes);
}
static void video_release(void *memory)
{
    if (video_route == 1)
    {
        vp0_release(memory);
        return;
    }
    if (video_route == 2)
    {
        vp1_release(memory);
        return;
    }
    vp2_release(memory);
}
void *op_sound_get_driver(void)
{
    if (video_route == 1)
        return vp0_driver();
    if (video_route == 2)
        return vp1_driver();
    return vp2_driver();
}
unsigned int op_video_surface_type(OP_VIDEO_DESC *description)
{
    return vp0_surface(description);
}
static int op_test_video_connected(void)
{
    unsigned int p, initial, get_result, allocate_ok, open_ok;
    int mutation, enabled;
    OP_VIDEO *result;
    int before_checks = vp0_checks, before_failures = vp0_failures;
    video_route = 1;
    for (p = 0; p < 24; ++p)
        for (initial = 0; initial < 3; ++initial)
            for (get_result = 0; get_result < 3; ++get_result)
                for (allocate_ok = 0; allocate_ok < 2; ++allocate_ok)
                    for (open_ok = 0; open_ok < 2; ++open_ok)
                        for (mutation = 0; mutation < 2; ++mutation)
                        {
                            vp0_seed(p);
                            vp0_mutate = mutation;
                            op_video_sound_driver = initial ? &vp0_tokens[initial - 1][0] : 0;
                            vp0_driver_result = get_result ? &vp0_tokens[get_result - 1][0] : 0;
                            vp0_allocation_result = allocate_ok ? vp0_active_video : 0;
                            vp0_handle_result = open_ok ? &vp0_tokens[2][0] : 0;
                            vp0_snapshot();
                            if (!initial)
                            {
                                vp0_codes[vp0_expected_count++] = 1;
                                if (get_result)
                                    vp0_codes[vp0_expected_count++] = 2;
                            }
                            vp0_codes[vp0_expected_count++] = 3;
                            if (allocate_ok)
                            {
                                vp0_codes[vp0_expected_count++] = 4;
                                vp0_codes[vp0_expected_count++] = open_ok ? 5 : 6;
                            }
                            result = op_video_open(vp0_active_name, vp0_active_description, vp0_argument3_value,
                                                   vp0_argument4_value);
                            vp0_check(result == (allocate_ok && open_ok ? vp0_active_video : 0), __LINE__);
                            vp0_finish();
                            if (result)
                            {
                                enabled = op_video_sound_driver != 0;
                                vp0_captured_handle = result->handle;
                                vp0_calls = 0;
                                vp0_expected_count = enabled;
                                vp0_codes[0] = 8;
                                op_video_set_volume(result, vp0_word_value);
                                vp0_finish();
                                vp0_captured_handle = result->handle;
                                vp0_calls = 0;
                                vp0_expected_count = 2;
                                vp0_codes[0] = 7;
                                vp0_codes[1] = 6;
                                vp0_check(op_video_close(result) == 1, __LINE__);
                                vp0_finish();
                            }
                            else
                            {
                                vp0_calls = 0;
                                vp0_expected_count = 0;
                                vp0_check(op_video_close(0) == 0, __LINE__);
                                vp0_finish();
                            }
                        }
    video_route = 0;
    printf("video connected: %d checks, %d failures\n", vp0_checks - before_checks, vp0_failures - before_failures);
    return vp0_failures != before_failures;
}
