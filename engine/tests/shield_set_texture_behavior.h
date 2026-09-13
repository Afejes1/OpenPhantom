#include "../src/focused_accessors.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sl_shield_set_texture_checks, sl_shield_set_texture_failures;
static void sl_shield_set_texture_check_at(int value, int line)
{
    ++sl_shield_set_texture_checks;
    if (!value)
    {
        ++sl_shield_set_texture_failures;
        printf("failed at %d\n", line);
    }
}
#define SL_SHIELD_SET_TEXTURE_CHECK(x) sl_shield_set_texture_check_at(!!(x), __LINE__)

static OP_SHIELD sl_shield_set_texture_expected[32];
typedef struct sl_shield_set_texture_NAME
{
    unsigned int before;
    char text[16];
    unsigned int after;
} sl_shield_set_texture_NAME;
static sl_shield_set_texture_NAME sl_shield_set_texture_input, sl_shield_set_texture_expected_input;
static unsigned char sl_shield_set_texture_objects[4][8], sl_shield_set_texture_expected_objects[4][8];
static int sl_shield_set_texture_selected, sl_shield_set_texture_events, sl_shield_set_texture_mutate;
static void *sl_shield_set_texture_acquired;
static void sl_shield_set_texture_verify_state(void)
{
    SL_SHIELD_SET_TEXTURE_CHECK(
        memcmp(op_shields, sl_shield_set_texture_expected, sizeof(sl_shield_set_texture_expected)) == 0);
    SL_SHIELD_SET_TEXTURE_CHECK(memcmp(&sl_shield_set_texture_input, &sl_shield_set_texture_expected_input,
                                       sizeof(sl_shield_set_texture_input)) == 0);
    SL_SHIELD_SET_TEXTURE_CHECK(memcmp(sl_shield_set_texture_objects, sl_shield_set_texture_expected_objects,
                                       sizeof(sl_shield_set_texture_objects)) == 0);
}
static void sl_shield_set_texture_op_release_sprite(void **sprite)
{
    SL_SHIELD_SET_TEXTURE_CHECK(sl_shield_set_texture_events++ == 0);
    SL_SHIELD_SET_TEXTURE_CHECK(sprite == &op_shields[sl_shield_set_texture_selected].sprite);
    SL_SHIELD_SET_TEXTURE_CHECK(*sprite == sl_shield_set_texture_objects[0]);
    sl_shield_set_texture_verify_state();
    *sprite = sl_shield_set_texture_objects[1];
    sl_shield_set_texture_expected[sl_shield_set_texture_selected].sprite = sl_shield_set_texture_objects[1];
    if (sl_shield_set_texture_mutate)
    {
        op_shields[sl_shield_set_texture_selected].active = 0;
        sl_shield_set_texture_expected[sl_shield_set_texture_selected].active = 0;
        op_shields[sl_shield_set_texture_selected].visible = -5;
        sl_shield_set_texture_expected[sl_shield_set_texture_selected].visible = -5;
        memcpy(sl_shield_set_texture_input.text, "change", 7);
        memcpy(sl_shield_set_texture_expected_input.text, "change", 7);
    }
}
static void *sl_shield_set_texture_op_acquire_sprite(char *name)
{
    SL_SHIELD_SET_TEXTURE_CHECK(sl_shield_set_texture_events++ == 1);
    SL_SHIELD_SET_TEXTURE_CHECK(name == sl_shield_set_texture_input.text);
    sl_shield_set_texture_verify_state();
    op_shields[sl_shield_set_texture_selected].sprite = sl_shield_set_texture_objects[2];
    sl_shield_set_texture_expected[sl_shield_set_texture_selected].sprite = sl_shield_set_texture_objects[2];
    if (sl_shield_set_texture_mutate)
    {
        memcpy(sl_shield_set_texture_input.text, "latest", 7);
        memcpy(sl_shield_set_texture_expected_input.text, "latest", 7);
    }
    return sl_shield_set_texture_acquired;
}
static void sl_shield_set_texture_run_case(int slot, int active, int success, int change_name, const char *name)
{
    int i, result, valid;
    size_t length;
    memset(op_shields, 0x63, sizeof(op_shields));
    memset(&sl_shield_set_texture_input, 0x42, sizeof(sl_shield_set_texture_input));
    memset(sl_shield_set_texture_objects, 0x27, sizeof(sl_shield_set_texture_objects));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = 1;
        op_shields[i].sprite = sl_shield_set_texture_objects[0];
    }
    length = strlen(name) + 1;
    memcpy(sl_shield_set_texture_input.text, name, length);
    valid = slot >= 0 && slot < 32 && active;
    if (slot >= 0 && slot < 32)
        op_shields[slot].active = active ? -7 : 0;
    memcpy(sl_shield_set_texture_expected, op_shields, sizeof(sl_shield_set_texture_expected));
    sl_shield_set_texture_expected_input = sl_shield_set_texture_input;
    memcpy(sl_shield_set_texture_expected_objects, sl_shield_set_texture_objects,
           sizeof(sl_shield_set_texture_objects));
    sl_shield_set_texture_selected = slot;
    sl_shield_set_texture_events = 0;
    sl_shield_set_texture_mutate = change_name;
    sl_shield_set_texture_acquired = success ? sl_shield_set_texture_objects[3] : 0;
    result = op_shield_set_texture(slot, sl_shield_set_texture_input.text);
    if (valid)
    {
        sl_shield_set_texture_expected[slot].sprite = sl_shield_set_texture_acquired;
        if (success)
            memcpy(sl_shield_set_texture_expected[slot].name, sl_shield_set_texture_expected_input.text,
                   strlen(sl_shield_set_texture_expected_input.text) + 1);
    }
    SL_SHIELD_SET_TEXTURE_CHECK(result == (valid && success));
    SL_SHIELD_SET_TEXTURE_CHECK(sl_shield_set_texture_events == (valid ? 2 : 0));
    sl_shield_set_texture_verify_state();
}
static int sl_shield_set_texture_main(void)
{
    static const int invalid[4] = {INT_MIN, -1, 32, INT_MAX};
    int i;
    for (i = 0; i < 4; ++i)
        sl_shield_set_texture_run_case(invalid[i], 1, 1, 0, "one");
    for (i = 0; i < 32; ++i)
    {
        sl_shield_set_texture_run_case(i, 0, 1, 0, "one");
        sl_shield_set_texture_run_case(i, 1, 0, 0, "two");
        sl_shield_set_texture_run_case(i, 1, 1, 0, "");
        sl_shield_set_texture_run_case(i, 1, 1, 0, "seven77");
        sl_shield_set_texture_run_case(i, 1, 1, 1, "one");
        sl_shield_set_texture_run_case(i, 1, 0, 1, "one");
    }
    printf("shield texture: %d checks, %d failures\n", sl_shield_set_texture_checks, sl_shield_set_texture_failures);
    return sl_shield_set_texture_failures != 0;
}

#undef SL_SHIELD_SET_TEXTURE_CHECK
