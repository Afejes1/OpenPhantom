#include "api.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static OP_LIGHT_WORLD world;
static OP_LIGHT_RECORD records[3];
static int checks;
static int failures;
static int calls;
static OP_LIGHT_WORLD *expected_world;
static OP_LIGHT_RECORD *expected_record;
static int callback_mode;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static int owned_record(OP_LIGHT_RECORD *record)
{
    return record == &records[0] || record == &records[1] ||
           record == &records[2];
}

void op_light_deactivate_record(OP_LIGHT_WORLD *value, int bank,
                                OP_LIGHT_RECORD *record)
{
    ++calls;
    check(value == expected_world, "callback world");
    check(bank == 1, "callback bank");
    check(owned_record(record), "callback owned record");
    if (!owned_record(record))
        return;
    check(record == expected_record, "callback exact record");
    if (callback_mode == 1)
        record->active = 0;
    else if (callback_mode == 2) {
        world.count = -7;
        world.records = &records[2];
        memset(record, 0x39, sizeof(*record));
    }
}

static void reset_fixture(void)
{
    memset(&world, 0x5a, sizeof(world));
    memset(records, 0x6b, sizeof(records));
    world.count = 3;
    world.records = records;
    calls = 0;
    expected_world = &world;
    expected_record = 0;
    callback_mode = 0;
}

static void guard_case(OP_LIGHT_WORLD *value, int index, int count)
{
    OP_LIGHT_WORLD before_world;
    OP_LIGHT_RECORD before_records[3];

    reset_fixture();
    world.count = count;
    memcpy(&before_world, &world, sizeof(world));
    memcpy(before_records, records, sizeof(records));
    op_light_disable_world(value, index);
    check(calls == 0, "guard has no callback");
    check(memcmp(&before_world, &world, sizeof(world)) == 0,
          "guard leaves world unchanged");
    check(memcmp(before_records, records, sizeof(records)) == 0,
          "guard leaves records unchanged");
}

static void active_case(int index, int active, int mode)
{
    OP_LIGHT_WORLD before_world;
    OP_LIGHT_RECORD before_records[3];

    reset_fixture();
    records[index].active = active;
    expected_record = &records[index];
    callback_mode = mode;
    memcpy(&before_world, &world, sizeof(world));
    memcpy(before_records, records, sizeof(records));
    op_light_disable_world(&world, index);
    check(calls == (active != 0), "active callback count");
    if (active == 0) {
        check(memcmp(&before_world, &world, sizeof(world)) == 0,
              "inactive world unchanged");
        check(memcmp(before_records, records, sizeof(records)) == 0,
              "inactive records unchanged");
    } else if (mode == 0) {
        check(memcmp(&before_world, &world, sizeof(world)) == 0,
              "callback may leave world unchanged");
        check(memcmp(before_records, records, sizeof(records)) == 0,
              "callback may leave active unchanged");
    } else if (mode == 1) {
        before_records[index].active = 0;
        check(memcmp(&before_world, &world, sizeof(world)) == 0,
              "clear callback leaves world unchanged");
        check(memcmp(before_records, records, sizeof(records)) == 0,
              "callback clear is sole record change");
    } else {
        before_world.count = -7;
        before_world.records = &records[2];
        memset(&before_records[index], 0x39, sizeof(before_records[index]));
        check(memcmp(&before_world, &world, sizeof(world)) == 0,
              "callback full world mutation retained");
        check(memcmp(before_records, records, sizeof(records)) == 0,
              "callback full record mutation and neighbors retained");
    }
}

int main(void)
{
    check(sizeof(OP_LIGHT_RECORD) == 128, "record size");
    check(offsetof(OP_LIGHT_RECORD, active) == 0x74, "active offset");
    check(offsetof(OP_LIGHT_WORLD, count) == 0x1c4, "count offset");
    check(offsetof(OP_LIGHT_WORLD, records) == 0x1cc, "records offset");

    reset_fixture();
    guard_case(0, 0, 3);
    guard_case(&world, -1, 3);
    guard_case(&world, INT_MIN, 3);
    guard_case(&world, 0, 0);
    guard_case(&world, 0, -1);
    guard_case(&world, 3, 3);
    guard_case(&world, 4, 3);

    active_case(0, 0, 0);
    active_case(0, 1, 0);
    active_case(2, -1, 1);
    active_case(1, 7, 2);

    printf("Light disable world: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
