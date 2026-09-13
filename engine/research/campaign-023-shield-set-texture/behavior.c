#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)

OP_SHIELD op_shields[32];
static OP_SHIELD expected[32];
typedef struct NAME
{
    unsigned int before;
    char text[16];
    unsigned int after;
} NAME;
static NAME input, expected_input;
static unsigned char objects[4][8], expected_objects[4][8];
static int selected, events, mutate;
static void *acquired;
static void verify_state(void)
{
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    CHECK(memcmp(objects, expected_objects, sizeof(objects)) == 0);
}
void op_release_sprite(void **sprite)
{
    CHECK(events++ == 0);
    CHECK(sprite == &op_shields[selected].sprite);
    CHECK(*sprite == objects[0]);
    verify_state();
    *sprite = objects[1];
    expected[selected].sprite = objects[1];
    if (mutate)
    {
        op_shields[selected].active = 0;
        expected[selected].active = 0;
        op_shields[selected].visible = -5;
        expected[selected].visible = -5;
        memcpy(input.text, "change", 7);
        memcpy(expected_input.text, "change", 7);
    }
}
void *op_acquire_sprite(char *name)
{
    CHECK(events++ == 1);
    CHECK(name == input.text);
    verify_state();
    op_shields[selected].sprite = objects[2];
    expected[selected].sprite = objects[2];
    if (mutate)
    {
        memcpy(input.text, "latest", 7);
        memcpy(expected_input.text, "latest", 7);
    }
    return acquired;
}
static void run_case(int slot, int active, int success, int change_name, const char *name)
{
    int i, result, valid;
    size_t length;
    memset(op_shields, 0x63, sizeof(op_shields));
    memset(&input, 0x42, sizeof(input));
    memset(objects, 0x27, sizeof(objects));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = 1;
        op_shields[i].sprite = objects[0];
    }
    length = strlen(name) + 1;
    memcpy(input.text, name, length);
    valid = slot >= 0 && slot < 32 && active;
    if (slot >= 0 && slot < 32)
        op_shields[slot].active = active ? -7 : 0;
    memcpy(expected, op_shields, sizeof(expected));
    expected_input = input;
    memcpy(expected_objects, objects, sizeof(objects));
    selected = slot;
    events = 0;
    mutate = change_name;
    acquired = success ? objects[3] : 0;
    result = op_shield_set_texture(slot, input.text);
    if (valid)
    {
        expected[slot].sprite = acquired;
        if (success)
            memcpy(expected[slot].name, expected_input.text, strlen(expected_input.text) + 1);
    }
    CHECK(result == (valid && success));
    CHECK(events == (valid ? 2 : 0));
    verify_state();
}
int main(void)
{
    static const int invalid[4] = {INT_MIN, -1, 32, INT_MAX};
    int i;
    for (i = 0; i < 4; ++i)
        run_case(invalid[i], 1, 1, 0, "one");
    for (i = 0; i < 32; ++i)
    {
        run_case(i, 0, 1, 0, "one");
        run_case(i, 1, 0, 0, "two");
        run_case(i, 1, 1, 0, "");
        run_case(i, 1, 1, 0, "seven77");
        run_case(i, 1, 1, 1, "one");
        run_case(i, 1, 0, 1, "one");
    }
    printf("shield texture: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
