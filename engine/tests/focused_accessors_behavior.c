/* Authored fixtures; one shared shield table and original bodies with static-name prefixes. */
#include "../src/focused_accessors.h"
#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_SHIELD op_shields[32];

typedef struct model_load_state_GUARDED
{
    unsigned int before;
    OP_MODEL model;
    unsigned int after;
} model_load_state_GUARDED;
static model_load_state_GUARDED model_load_state_state, model_load_state_expected;
static int model_load_state_checks, model_load_state_failures;
static void model_load_state_check(int ok)
{
    ++model_load_state_checks;
    if (!ok)
        ++model_load_state_failures;
}
static int model_load_state_main(void)
{
    static const int values[] = {0, 1, -1, INT_MIN, INT_MAX, 0x12345678};
    unsigned int i;
    for (i = 0; i < sizeof(values) / sizeof(values[0]); ++i)
    {
        memset(&model_load_state_state, 0x6d, sizeof(model_load_state_state));
        model_load_state_state.before = 0x11223344U;
        model_load_state_state.after = 0x55667788U;
        model_load_state_state.model.load_state = values[i];
        model_load_state_expected = model_load_state_state;
        model_load_state_check(op_model_load_state(&model_load_state_state.model) == values[i]);
        model_load_state_check(
            memcmp(&model_load_state_state, &model_load_state_expected, sizeof(model_load_state_state)) == 0);
        model_load_state_check(op_model_load_state(&model_load_state_state.model) == values[i]);
        model_load_state_check(
            memcmp(&model_load_state_state, &model_load_state_expected, sizeof(model_load_state_state)) == 0);
    }
    printf("model load model_load_state_state: %d checks, %d failures\n",
           model_load_state_checks, model_load_state_failures);
    return model_load_state_failures != 0;
}

struct OP_FONT
{
    unsigned int marker;
    unsigned char payload[28];
};
OP_FONT *op_system_font;
static OP_FONT get_system_font_objects[3], get_system_font_expected[3];
static int get_system_font_checks, get_system_font_failures;
static void get_system_font_check(int ok)
{
    ++get_system_font_checks;
    if (!ok)
        ++get_system_font_failures;
}
static int get_system_font_main(void)
{
    int i;
    OP_FONT *p;
    memset(get_system_font_objects, 0x6d, sizeof(get_system_font_objects));
    memcpy(get_system_font_expected, get_system_font_objects, sizeof(get_system_font_objects));
    for (i = 0; i < 4; ++i)
    {
        p = i == 3 ? 0 : &get_system_font_objects[i];
        op_system_font = p;
        get_system_font_check(op_get_system_font() == p);
        get_system_font_check(op_system_font == p);
        get_system_font_check(
            memcmp(get_system_font_objects, get_system_font_expected, sizeof(get_system_font_objects)) == 0);
        get_system_font_check(op_get_system_font() == p);
        get_system_font_check(op_system_font == p);
        get_system_font_check(
            memcmp(get_system_font_objects, get_system_font_expected, sizeof(get_system_font_objects)) == 0);
    }
    printf("system font getter: %d checks, %d failures\n", get_system_font_checks,
           get_system_font_failures);
    return get_system_font_failures != 0;
}

static OP_SHIELD shield_radius_expected[32];
static unsigned char shield_radius_owners[32];
static int shield_radius_checks, shield_radius_failures;
static void shield_radius_check(int ok)
{
    ++shield_radius_checks;
    if (!ok)
        ++shield_radius_failures;
}
static void shield_radius_set_radius(int slot, unsigned int bits)
{
    memcpy(&op_shields[slot].radius, &bits, 4);
}
static void shield_radius_verify(int slot, unsigned int expected_bits)
{
    float answer;
    unsigned int bits;
    memcpy(shield_radius_expected, op_shields, sizeof(shield_radius_expected));
    answer = op_shield_radius(slot);
    memcpy(&bits, &answer, 4);
    shield_radius_check(bits == expected_bits);
    shield_radius_check(memcmp(shield_radius_expected, op_shields, sizeof(shield_radius_expected)) == 0);
}
static int shield_radius_main(void)
{
    int i, j;
    unsigned int saved;
    static const unsigned int radii[] = {0U,          0x80000000U, 0x3f800000U, 0xc0200000U,
                                         0x7f7fffffU, 0x00800000U, 0x7fc12345U};
    saved = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_64 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    memset(op_shields, 0x6d, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].owner = 0;
        shield_radius_set_radius(i, 0x7fc12345U);
    }
    shield_radius_verify(-1, 0);
    shield_radius_verify(INT_MIN, 0);
    shield_radius_verify(32, 0);
    shield_radius_verify(INT_MAX, 0);
    for (i = 0; i < 32; ++i)
        shield_radius_verify(i, 0);
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].owner = &shield_radius_owners[i];
        for (j = 0; j < 7; ++j)
        {
            shield_radius_set_radius(i, radii[j]);
            shield_radius_verify(i, radii[j]);
            shield_radius_verify(i, radii[j]);
        }
        op_shields[i].owner = 0;
        shield_radius_verify(i, 0);
    }
    _controlfp(saved, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("shield radius: %d checks, %d failures\n", shield_radius_checks,
           shield_radius_failures);
    return shield_radius_failures != 0;
}

static OP_SHIELD shield_set_colour_expected[32];
static unsigned char shield_set_colour_owners[32];
static int shield_set_colour_checks, shield_set_colour_failures;
static void shield_set_colour_check(int ok)
{
    ++shield_set_colour_checks;
    if (!ok)
        ++shield_set_colour_failures;
}
static void shield_set_colour_seed(void)
{
    int i;
    memset(op_shields, 0x6d, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
        op_shields[i].owner = &shield_set_colour_owners[i];
}
static void shield_set_colour_verify(int slot, int accepted, unsigned char r, unsigned char g, unsigned char b,
                                     unsigned char a)
{
    unsigned char *bytes;
    memcpy(shield_set_colour_expected, op_shields, sizeof(shield_set_colour_expected));
    if (accepted)
    {
        bytes = (unsigned char *)&shield_set_colour_expected[slot];
        bytes[0x38] = r;
        bytes[0x39] = g;
        bytes[0x3a] = b;
        bytes[0x3b] = a;
    }
    shield_set_colour_check(op_shield_set_colour(slot, r, g, b, a) == accepted);
    shield_set_colour_check(memcmp(shield_set_colour_expected, op_shields, sizeof(shield_set_colour_expected)) == 0);
}
static int shield_set_colour_main(void)
{
    int i, j;
    static const unsigned char values[8] = {0, 1, 127, 128, 254, 255, 0x55, 0xaa};
    shield_set_colour_seed();
    shield_set_colour_verify(-1, 0, 1, 2, 3, 4);
    shield_set_colour_verify(INT_MIN, 0, 255, 128, 0, 1);
    shield_set_colour_verify(32, 0, 1, 2, 3, 4);
    shield_set_colour_verify(INT_MAX, 0, 1, 2, 3, 4);
    for (i = 0; i < 32; ++i)
    {
        shield_set_colour_seed();
        op_shields[i].owner = 0;
        shield_set_colour_verify(i, 0, 255, 255, 255, 255);
        op_shields[i].owner = &shield_set_colour_owners[i];
        for (j = 0; j < 8; ++j)
        {
            shield_set_colour_verify(i, 1, values[j], values[(j + 1) % 8], values[(j + 2) % 8], values[(j + 3) % 8]);
            shield_set_colour_verify(i, 1, values[j], values[(j + 1) % 8], values[(j + 2) % 8], values[(j + 3) % 8]);
        }
    }
    printf("shield colour: %d checks, %d failures\n", shield_set_colour_checks,
           shield_set_colour_failures);
    return shield_set_colour_failures != 0;
}

static OP_SHIELD shield_set_visible_expected[32];
static unsigned char shield_set_visible_owners[32];
static int shield_set_visible_checks, shield_set_visible_failures;
static void shield_set_visible_check(int ok)
{
    ++shield_set_visible_checks;
    if (!ok)
        ++shield_set_visible_failures;
}
static void shield_set_visible_seed(void)
{
    int i;
    memset(op_shields, 0x6d, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
        op_shields[i].owner = &shield_set_visible_owners[i];
}
static void shield_set_visible_verify(int slot, int accepted, int value)
{
    memcpy(shield_set_visible_expected, op_shields, sizeof(shield_set_visible_expected));
    if (accepted)
        memcpy((unsigned char *)&shield_set_visible_expected[slot] + 4, &value, 4);
    shield_set_visible_check(op_shield_set_visible(slot, value) == accepted);
    shield_set_visible_check(memcmp(shield_set_visible_expected, op_shields, sizeof(shield_set_visible_expected)) == 0);
}
static int shield_set_visible_main(void)
{
    int i, j;
    static const int values[6] = {0, 1, -1, INT_MIN, INT_MAX, 0x12345678};
    shield_set_visible_seed();
    shield_set_visible_verify(-1, 0, -1);
    shield_set_visible_verify(INT_MIN, 0, INT_MAX);
    shield_set_visible_verify(32, 0, 1);
    shield_set_visible_verify(INT_MAX, 0, INT_MIN);
    for (i = 0; i < 32; ++i)
    {
        shield_set_visible_seed();
        op_shields[i].owner = 0;
        shield_set_visible_verify(i, 0, INT_MIN);
        op_shields[i].owner = &shield_set_visible_owners[i];
        for (j = 0; j < 6; ++j)
        {
            shield_set_visible_verify(i, 1, values[j]);
            shield_set_visible_verify(i, 1, values[j]);
        }
    }
    printf("shield word setter: %d checks, %d failures\n",
           shield_set_visible_checks, shield_set_visible_failures);
    return shield_set_visible_failures != 0;
}

static OP_SHIELD shield_set_visibility_bypass_expected[32];
static unsigned char shield_set_visibility_bypass_owners[32];
static int shield_set_visibility_bypass_checks, shield_set_visibility_bypass_failures;
static void shield_set_visibility_bypass_check(int ok)
{
    ++shield_set_visibility_bypass_checks;
    if (!ok)
        ++shield_set_visibility_bypass_failures;
}
static void shield_set_visibility_bypass_seed(void)
{
    int i;
    memset(op_shields, 0x6d, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
        op_shields[i].owner = &shield_set_visibility_bypass_owners[i];
}
static void shield_set_visibility_bypass_verify(int slot, int accepted, int value)
{
    memcpy(shield_set_visibility_bypass_expected, op_shields, sizeof(shield_set_visibility_bypass_expected));
    if (accepted)
        memcpy((unsigned char *)&shield_set_visibility_bypass_expected[slot] + 12, &value, 4);
    shield_set_visibility_bypass_check(op_shield_set_visibility_bypass(slot, value) == accepted);
    shield_set_visibility_bypass_check(
        memcmp(shield_set_visibility_bypass_expected, op_shields, sizeof(shield_set_visibility_bypass_expected)) == 0);
}
static int shield_set_visibility_bypass_main(void)
{
    int i, j;
    static const int values[6] = {0, 1, -1, INT_MIN, INT_MAX, 0x12345678};
    shield_set_visibility_bypass_seed();
    shield_set_visibility_bypass_verify(-1, 0, -1);
    shield_set_visibility_bypass_verify(INT_MIN, 0, INT_MAX);
    shield_set_visibility_bypass_verify(32, 0, 1);
    shield_set_visibility_bypass_verify(INT_MAX, 0, INT_MIN);
    for (i = 0; i < 32; ++i)
    {
        shield_set_visibility_bypass_seed();
        op_shields[i].owner = 0;
        shield_set_visibility_bypass_verify(i, 0, INT_MIN);
        op_shields[i].owner = &shield_set_visibility_bypass_owners[i];
        for (j = 0; j < 6; ++j)
        {
            shield_set_visibility_bypass_verify(i, 1, values[j]);
            shield_set_visibility_bypass_verify(i, 1, values[j]);
        }
    }
    printf("shield word setter: %d checks, %d failures\n",
           shield_set_visibility_bypass_checks, shield_set_visibility_bypass_failures);
    return shield_set_visibility_bypass_failures != 0;
}

static int accessor_sequence_checks, accessor_sequence_failures;
static OP_SHIELD accessor_sequence_expected[32];
static unsigned char accessor_sequence_owners[32];
static void accessor_sequence_check(int ok)
{
    ++accessor_sequence_checks;
    if (!ok)
        ++accessor_sequence_failures;
}
static void accessor_sequence(void)
{
    int i, result, value;
    float radius;
    unsigned char *bytes;
    memset(op_shields, 0x6d, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].owner = &accessor_sequence_owners[i];
        op_shields[i].radius = 1.25f;
    }
    memcpy(accessor_sequence_expected, op_shields, sizeof(op_shields));
    for (i = 0; i < 32; ++i)
    {
        bytes = (unsigned char *)&accessor_sequence_expected[i];
        bytes[0x38] = 128;
        bytes[0x39] = 0;
        bytes[0x3a] = 255;
        bytes[0x3b] = 127;
        result = op_shield_set_colour(i, 128, 0, 255, 127);
        accessor_sequence_check(result == 1);
        accessor_sequence_check(memcmp(op_shields, accessor_sequence_expected, sizeof(op_shields)) == 0);
        value = INT_MIN;
        memcpy(bytes + 4, &value, 4);
        result = op_shield_set_visible(i, value);
        accessor_sequence_check(result == 1);
        accessor_sequence_check(memcmp(op_shields, accessor_sequence_expected, sizeof(op_shields)) == 0);
        value = -1;
        memcpy(bytes + 12, &value, 4);
        result = op_shield_set_visibility_bypass(i, value);
        accessor_sequence_check(result == 1);
        accessor_sequence_check(memcmp(op_shields, accessor_sequence_expected, sizeof(op_shields)) == 0);
        radius = op_shield_radius(i);
        accessor_sequence_check(radius == 1.25f);
        accessor_sequence_check(memcmp(op_shields, accessor_sequence_expected, sizeof(op_shields)) == 0);
    }
}
static int op_test_focused_accessors(void)
{
    int failures, checks;
    failures = 0;
    checks = 0;
    failures += model_load_state_main();
    checks += model_load_state_checks;
    failures += get_system_font_main();
    checks += get_system_font_checks;
    failures += shield_radius_main();
    checks += shield_radius_checks;
    failures += shield_set_colour_main();
    checks += shield_set_colour_checks;
    failures += shield_set_visible_main();
    checks += shield_set_visible_checks;
    failures += shield_set_visibility_bypass_main();
    checks += shield_set_visibility_bypass_checks;
    accessor_sequence();
    failures += accessor_sequence_failures;
    checks += accessor_sequence_checks;
    printf("focused accessors integrated: %d checks, %d failures\n", checks, failures);
    return failures;
}
