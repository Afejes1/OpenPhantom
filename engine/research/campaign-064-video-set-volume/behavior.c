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
typedef struct OWNED_VIDEO
{
    unsigned int before;
    OP_VIDEO value;
    unsigned int after;
} OWNED_VIDEO;
typedef struct OWNED_DESC
{
    unsigned int before;
    OP_VIDEO_DESC value;
    unsigned int after;
} OWNED_DESC;
static OWNED_VIDEO videos[2], expected_videos[2];
static OWNED_DESC descriptions[2], expected_descriptions[2];
static char names[2][24], expected_names[2][24];
static unsigned char tokens[4][16], expected_tokens[4][16];
void *op_video_sound_driver, *op_video_open_miles;
static void *expected_driver, *expected_miles, *driver_result, *handle_result, *captured_handle;
static OP_VIDEO *allocation_result, *active_video;
static OP_VIDEO_DESC *active_description;
static const char *active_name;
static unsigned int selected, description_index, argument3_value, argument4_value, word_value, surface_result,
    captured_surface;
static int calls, expected_count, codes[8], mutate, kind;
static const unsigned int words[] = {0, 1, 5, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static void verify(void)
{
    CHECK(memcmp(videos, expected_videos, sizeof(videos)) == 0);
    CHECK(memcmp(descriptions, expected_descriptions, sizeof(descriptions)) == 0);
    CHECK(memcmp(names, expected_names, sizeof(names)) == 0);
    CHECK(memcmp(tokens, expected_tokens, sizeof(tokens)) == 0);
    CHECK(op_video_sound_driver == expected_driver);
    CHECK(op_video_open_miles == expected_miles);
}
static void observe(int code)
{
    CHECK(calls < expected_count && calls < 8);
    if (calls >= expected_count || calls >= 8)
        exit(2);
    CHECK(codes[calls] == code);
    verify();
    ++calls;
}
static void change(void)
{
    if (mutate)
    {
        op_video_sound_driver = expected_driver = 0;
        op_video_open_miles = expected_miles = &tokens[3][0];
        videos[selected].value.handle = expected_videos[selected].value.handle = &tokens[2][0];
        videos[selected].value.argument4 = expected_videos[selected].value.argument4 = 0x87654321u;
        descriptions[description_index].value.mode = expected_descriptions[description_index].value.mode = 7;
        descriptions[description_index].value.format[2] = expected_descriptions[description_index].value.format[2] = 5;
        names[selected][2] = expected_names[selected][2] = 'Q';
    }
}
void *op_sound_get_driver(void)
{
    observe(1);
    change();
    expected_driver = driver_result;
    return driver_result;
}
static int __stdcall sound_system(void *miles, void *driver)
{
    observe(2);
    CHECK(miles == expected_miles);
    CHECK(driver == driver_result);
    change();
    return (int)word_value;
}
void *op_allocate(unsigned int size)
{
    observe(3);
    CHECK(size == 24);
    change();
    return allocation_result;
}
static void *__stdcall bink_open(const char *name, unsigned int flags)
{
    observe(4);
    CHECK(name == active_name);
    CHECK(flags == 0x00400000);
    change();
    expected_videos[selected].value.handle = handle_result;
    return handle_result;
}
#ifdef OP_TEST_OPEN
unsigned int op_video_surface_type(OP_VIDEO_DESC *description)
{
    observe(5);
    CHECK(description == active_description);
    change();
    expected_videos[selected].value.surface_type = surface_result;
    expected_videos[selected].value.description = active_description;
    expected_videos[selected].value.argument3 = argument3_value;
    expected_videos[selected].value.argument4 = argument4_value;
    expected_videos[selected].value.done = 0;
    return surface_result;
}
#endif
void op_release(void *memory)
{
    observe(6);
    CHECK(memory == active_video);
    change();
}
static int __stdcall bink_close(void *handle)
{
    observe(7);
    CHECK(handle == captured_handle);
    change();
    expected_videos[selected].value.handle = 0;
    return (int)word_value;
}
static int __stdcall bink_volume(void *handle, unsigned int volume)
{
    observe(8);
    CHECK(handle == captured_handle);
    CHECK(volume == word_value);
    change();
    return (int)~word_value;
}
static unsigned int __stdcall dd_type(unsigned int surface)
{
    observe(9);
    CHECK(surface == captured_surface);
    change();
    return surface_result;
}
unsigned int(__stdcall *op_video_dd_type)(unsigned int) = dd_type;
int(__stdcall *op_video_sound_system)(void *, void *) = sound_system;
void *(__stdcall *op_video_bink_open)(const char *, unsigned int) = bink_open;
int(__stdcall *op_video_bink_close)(void *) = bink_close;
int(__stdcall *op_video_bink_volume)(void *, unsigned int) = bink_volume;
static void snapshot(void)
{
    memcpy(expected_videos, videos, sizeof(videos));
    memcpy(expected_descriptions, descriptions, sizeof(descriptions));
    memcpy(expected_names, names, sizeof(names));
    memcpy(expected_tokens, tokens, sizeof(tokens));
    expected_driver = op_video_sound_driver;
    expected_miles = op_video_open_miles;
}
static void seed(unsigned int profile)
{
    unsigned int i;
    memset(videos, 0x57, sizeof(videos));
    memset(descriptions, 0x68, sizeof(descriptions));
    memset(names, 0, sizeof(names));
    strcpy(names[0], "fixture-a");
    strcpy(names[1], "fixture-b");
    memset(tokens, 0x79, sizeof(tokens));
    for (i = 0; i < 2; ++i)
    {
        videos[i].value.handle = &tokens[i][0];
        videos[i].value.description = &descriptions[i].value;
    }
    selected = profile % 2;
    description_index = (profile / 2) % 2;
    active_video = &videos[selected].value;
    active_description = &descriptions[description_index].value;
    active_name = names[selected];
    op_video_sound_driver = &tokens[0][0];
    op_video_open_miles = &tokens[1][0];
    driver_result = &tokens[0][0];
    handle_result = &tokens[1][0];
    allocation_result = active_video;
    argument3_value = words[profile % 6];
    argument4_value = words[(profile + 1) % 6];
    word_value = words[(profile + 2) % 6];
    surface_result = words[(profile + 3) % 6];
    captured_handle = active_video->handle;
    captured_surface = active_description->surface;
    calls = 0;
    expected_count = 0;
    mutate = 0;
    kind = 0;
    snapshot();
}
static void finish(void)
{
    CHECK(calls == expected_count);
    verify();
    CHECK(op_video_dd_type == dd_type);
    CHECK(op_video_sound_system == sound_system);
    CHECK(op_video_bink_open == bink_open);
    CHECK(op_video_bink_close == bink_close);
    CHECK(op_video_bink_volume == bink_volume);
}
int main(void)
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
                    seed(p);
                    mutate = change_state;
                    op_video_sound_driver = gate ? &tokens[gate - 1][0] : 0;
                    snapshot();
                    expected_count = gate != 0;
                    codes[0] = 8;
                    op_video_set_volume(null_input ? 0 : active_video, word_value);
                    finish();
                }
    printf("video_set_volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
