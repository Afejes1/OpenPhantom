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
int op_shield_count;
static OP_SHIELD expected[32];
static int expected_count, selected, calls, mutate_future;
static unsigned char objects[10][16], expected_objects[10][16];
static OP_ATTACHED_ACTOR actor, expected_actor;
static void **field(OP_SHIELD *shield, int n)
{
    switch (n)
    {
    case 0:
        return &shield->buffer_1c;
    case 1:
        return &shield->buffer_40;
    case 2:
        return &shield->buffer_20;
    case 3:
        return &shield->buffer_24;
    case 4:
        return &shield->buffer_28;
    case 5:
        return &shield->buffer_2c;
    case 6:
        return &shield->buffer_30;
    default:
        return &shield->buffer_34;
    }
}
static void verify_state(void)
{
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    CHECK(op_shield_count == expected_count);
    CHECK(memcmp(objects, expected_objects, sizeof(objects)) == 0);
    CHECK(memcmp(&actor, &expected_actor, sizeof(actor)) == 0);
}
void op_shield_free(void *memory)
{
    int call = calls++;
    CHECK(call >= 0 && call < 8);
    if (call < 0 || call >= 8)
        return;
    CHECK(memory == *field(&expected[selected], call));
    verify_state();
    op_shields[selected].visible = 100 + call;
    expected[selected].visible = 100 + call;
    op_shields[selected].active = 0;
    expected[selected].active = 0;
    *field(&op_shields[selected], call) = objects[9];
    if (call == 0 && mutate_future)
    {
        op_shields[selected].buffer_40 = objects[8];
        expected[selected].buffer_40 = objects[8];
    }
    objects[0][5] = 0x26;
    expected_objects[0][5] = 0x26;
    /* Predict the candidate's next stores; the next callback checks them. */
    *field(&expected[selected], call) = 0;
    if (call == 0)
        expected[selected].count_18 = 0;
    if (call == 1)
        expected[selected].count_3c = 0;
}
void op_release_sprite(void **sprite)
{
    CHECK(calls++ == 8);
    CHECK(sprite == &op_shields[selected].sprite);
    verify_state();
    *sprite = objects[9];
    expected[selected].sprite = objects[9];
    op_shield_count = 17;
    expected_count = 16;
    op_shields[selected].attached = &actor;
    expected[selected].attached = 0;
    op_shields[selected].active = -7;
    expected[selected].active = 0;
    op_shields[selected].no_save = -9;
    expected[selected].no_save = -9;
}
static void run_case(int slot, int active, int profile)
{
    int i, j, result, valid;
    memset(op_shields, 0x39, sizeof(op_shields));
    memset(objects, 0x51, sizeof(objects));
    memset(&actor, 0x68, sizeof(actor));
    for (i = 0; i < 32; ++i)
    {
        op_shields[i].active = 1;
        op_shields[i].attached = &actor;
        op_shields[i].sprite = objects[8];
        for (j = 0; j < 8; ++j)
            *field(&op_shields[i], j) = profile == 2 || (profile == 1 && j % 2) ? 0 : objects[j];
    }
    if (slot >= 0 && slot < 32)
        op_shields[slot].active = active ? INT_MIN : 0;
    memcpy(expected, op_shields, sizeof(expected));
    memcpy(expected_objects, objects, sizeof(objects));
    expected_actor = actor;
    op_shield_count = -7;
    expected_count = -7;
    selected = slot;
    calls = 0;
    mutate_future = profile == 0;
    valid = slot >= 0 && slot < 32 && active;
    result = op_shield_destroy(slot);
    CHECK(result == valid);
    CHECK(calls == (valid ? 9 : 0));
    verify_state();
}
int main(void)
{
    static const int invalid[4] = {INT_MIN, -1, 32, INT_MAX};
    int i, j;
    for (i = 0; i < 4; ++i)
        run_case(invalid[i], 1, 0);
    for (i = 0; i < 32; ++i)
    {
        run_case(i, 0, 0);
        for (j = 0; j < 3; ++j)
            run_case(i, 1, j);
    }
    printf("shield destructor: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
