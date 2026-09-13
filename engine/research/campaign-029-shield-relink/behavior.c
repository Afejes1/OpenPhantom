#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_SHIELD op_shields[32];
void *op_object_pool;
static OP_SHIELD expected[32];
static OP_ACTOR actors[4], expected_actors[4];
static int pools[2], iterator_token;
static int multi;
static int selected, match, mutation, opened, resets, next_calls, closed, position, active;
void *op_iterator_open(void *pool)
{
    CHECK(opened++ == 0);
    CHECK(pool == &pools[0]);
    if (mutation)
        op_object_pool = &pools[1];
    return &iterator_token;
}
void op_iterator_reset(void *iterator)
{
    CHECK(opened == 1);
    CHECK(iterator == &iterator_token);
    if (multi)
    {
        CHECK(resets < 2);
        if (resets == 0)
        {
            CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
            op_shields[10].active = expected[10].active = 0;
            op_shields[30].active = expected[30].active = 1;
        }
        else
        {
            expected_actors[1].slot = 2;
            expected[2].actor = &actors[1];
            expected[2].saved_actor = -1;
            CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
            CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
        }
        resets++;
        position = 0;
        return;
    }
    CHECK(resets++ == 0);
    position = 0;
    if (mutation)
    {
        op_shields[selected].saved_actor = expected[selected].saved_actor = -500;
        op_shields[selected].active = expected[selected].active = 0;
    }
}
OP_ACTOR *op_iterator_next(void *iterator)
{
    CHECK(iterator == &iterator_token);
    CHECK(multi ? (resets == 1 || resets == 2) : resets == 1);
    CHECK(closed == 0);
    next_calls++;
    CHECK(position <= 4);
    if (position == 4)
    {
        position++;
        return 0;
    }
    return &actors[position++];
}
void op_iterator_close(void *iterator)
{
    CHECK(iterator == &iterator_token);
    CHECK(opened == 1);
    CHECK(closed++ == 0);
    if (multi)
    {
        expected_actors[3].slot = 30;
        expected[30].actor = &actors[3];
        expected[30].saved_actor = -1;
        CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
        CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
        return;
    }
    if (active && match >= 0)
    {
        expected_actors[match].slot = selected;
        expected[selected].actor = &actors[match];
        expected[selected].saved_actor = -1;
    }
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
}
int main(void)
{
    int i;
    for (selected = 0; selected < 32; selected++)
        for (match = -3; match < 4; match++)
            for (mutation = 0; mutation < 2; mutation++)
                for (active = 0; active < 2; active++)
                {
                    memset(op_shields, 0x45, sizeof(op_shields));
                    for (i = 0; i < 32; i++)
                    {
                        op_shields[i].active = 0;
                        op_shields[i].actor = 0;
                    }
                    memset(actors, 0, sizeof(actors));
                    for (i = 0; i < 4; i++)
                        actors[i].slot = 1000 + i;
                    op_shields[selected].active = active ? -1 : 0;
                    op_shields[selected].saved_actor = match < 0 ? (match == -3   ? INT_MIN
                                                                    : match == -2 ? INT_MAX
                                                                                  : -7)
                                                                 : 1000 + match;
                    memcpy(expected, op_shields, sizeof(expected));
                    memcpy(expected_actors, actors, sizeof(actors));
                    op_object_pool = &pools[0];
                    opened = resets = next_calls = closed = position = 0;
                    op_shield_relink();
                    CHECK(opened == 1);
                    CHECK(resets == active);
                    CHECK(next_calls == (active ? (match < 0 ? 5 : match + 1) : 0));
                    CHECK(closed == 1);
                    CHECK(op_object_pool == &pools[mutation ? 1 : 0]);
                    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
                    CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
                }
    multi = 1;
    mutation = 0;
    active = 1;
    opened = resets = next_calls = closed = position = 0;
    memset(op_shields, 0, sizeof(op_shields));
    memset(actors, 0, sizeof(actors));
    for (i = 0; i < 4; i++)
        actors[i].slot = 1000 + i;
    op_shields[2].active = 1;
    op_shields[2].saved_actor = 1001;
    op_shields[10].active = 1;
    op_shields[10].saved_actor = 1002;
    op_shields[30].saved_actor = 1003;
    memcpy(expected, op_shields, sizeof(expected));
    memcpy(expected_actors, actors, sizeof(actors));
    op_object_pool = &pools[0];
    op_shield_relink();
    CHECK(opened == 1);
    CHECK(resets == 2);
    CHECK(next_calls == 6);
    CHECK(closed == 1);
    CHECK(op_shields[10].saved_actor == 1002);
    CHECK(op_shields[10].actor == 0);
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    CHECK(memcmp(actors, expected_actors, sizeof(actors)) == 0);
    printf("shield relink: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
