#include "api.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

typedef char op_event_size_is_20[(sizeof(OP_SOUND_RESOURCE_EVENT) == 20) ? 1 : -1];
typedef char op_file_open_at_30[(offsetof(OP_SOUND_FILE_API, open) == 0x30) ? 1 : -1];
typedef char op_file_close_at_34[(offsetof(OP_SOUND_FILE_API, close) == 0x34) ? 1 : -1];
typedef char op_file_read_at_38[(offsetof(OP_SOUND_FILE_API, read) == 0x38) ? 1 : -1];
typedef char op_file_size_at_50[(offsetof(OP_SOUND_FILE_API, size) == 0x50) ? 1 : -1];

typedef struct GUARDED_EVENT {
    unsigned int before;
    OP_SOUND_RESOURCE_EVENT event;
    unsigned int after;
} GUARDED_EVENT;

enum { SCENARIO_OPEN_FAIL, SCENARIO_ALLOC_FAIL, SCENARIO_SUCCESS };

OP_SOUND_FILE_API *op_sound_file_api;
const char op_sound_read_mode[] = "rb";
unsigned int op_sound_allocation_tag;

static OP_SOUND_FILE_API api_a;
static OP_SOUND_FILE_API api_b;
static GUARDED_EVENT *active_event;
static int scenario;
static int checks;
static int failures;
static int events[12];
static int event_count;
static unsigned int supplied_size;
static unsigned int sized_resource;
static unsigned char allocation[16];
static unsigned char payload[8];
static const char name_a[] = "first";
static const char name_b[] = "second";
static const char name_c[] = "third";
static const char name_d[] = "fourth";

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static void note(int id)
{
    if (event_count >= 0 && event_count < 12)
        events[event_count] = id;
    else
        check(0, "event log bound");
    ++event_count;
}

static int __cdecl open_a(const char *name, const char *mode)
{
    note(1);
    check(name == active_event->event.name, "open receives live name");
    check(mode == op_sound_read_mode && strcmp(mode, "rb") == 0,
          "open receives exact read mode");
    active_event->event.name = name_b;
    op_sound_file_api = &api_b;
    return scenario == SCENARIO_OPEN_FAIL ? 0 : 37;
}

static unsigned int __cdecl size_b(const char *name)
{
    note(2);
    check(name == name_b, "size reloads callback-mutated name");
    op_sound_allocation_tag = 0x778899aaU;
    return supplied_size;
}

static int __cdecl read_a(int handle, void *buffer, unsigned int size)
{
    note(4);
    check(handle == 37, "read receives open handle");
    check(buffer == (scenario == SCENARIO_ALLOC_FAIL ? 0 : allocation),
          "read receives allocation result including null");
    check(size == supplied_size, "read receives unsigned file size");
    active_event->event.name = name_c;
    active_event->event.resource_type = 0x12345678U;
    op_sound_file_api = &api_b;
    return -19;
}

static int __cdecl close_b(int handle)
{
    note(5);
    check(handle == 37, "close receives open handle");
    active_event->event.name = name_d;
    active_event->event.resource_type = 0x87654321U;
    return -23;
}

void *op_sound_allocate(unsigned int tag, unsigned int size)
{
    note(3);
    check(tag == 0x778899aaU, "allocator reloads tag mutated by size callback");
    check(size == supplied_size, "allocator receives exact unsigned size");
    op_sound_file_api = &api_a;
    return scenario == SCENARIO_ALLOC_FAIL ? 0 : allocation;
}

int op_sound_register_loaded(unsigned int type, const char *name, void *buffer)
{
    note(6);
    check(type == 0x87654321U, "register reloads type mutated by close");
    check(name == name_d, "register reloads name mutated by close");
    check(buffer == allocation, "register receives allocation");
    return -1;
}

void op_sound_free_resource(void *data)
{
    note(7);
    check(data == active_event->event.data, "free receives event data");
}

unsigned int op_sound_resource_size(void *data)
{
    note(8);
    check(data == active_event->event.data, "resource size receives event data");
    return sized_resource;
}

static void initialize_apis(void)
{
    memset(&api_a, 0xa5, sizeof(api_a));
    memset(&api_b, 0x6b, sizeof(api_b));
    api_a.open = open_a;
    api_a.read = read_a;
    api_b.close = close_b;
    api_b.size = size_b;
}

static void reset_event(GUARDED_EVENT *guarded, int operation)
{
    guarded->before = 0x11111111U;
    guarded->event.operation = operation;
    guarded->event.name = name_a;
    guarded->event.resource_type = 0x42534e44U;
    guarded->event.data = payload;
    guarded->event.size = 0xabcdef01U;
    guarded->after = 0x22222222U;
    active_event = guarded;
    event_count = 0;
    memset(events, 0, sizeof(events));
    op_sound_file_api = &api_a;
    op_sound_allocation_tag = 0x11112222U;
}

static void unknown_case(int operation)
{
    GUARDED_EVENT guarded;
    GUARDED_EVENT before;
    reset_event(&guarded, operation);
    before = guarded;
    check(op_sound_resource_handler(&guarded.event) == 0, "unknown operation returns zero");
    check(event_count == 0, "unknown operation makes no callbacks");
    check(memcmp(&guarded, &before, sizeof(guarded)) == 0,
          "unknown operation preserves full event and guards");
}

static void open_case(int selected_scenario, unsigned int size)
{
    GUARDED_EVENT guarded;
    OP_SOUND_RESOURCE_EVENT expected;
    int result;

    scenario = selected_scenario;
    supplied_size = size;
    reset_event(&guarded, 1);
    expected = guarded.event;
    result = op_sound_resource_handler(&guarded.event);
    expected.operation = 1;
    expected.data = payload;
    expected.size = 0xabcdef01U;
    if (selected_scenario == SCENARIO_OPEN_FAIL) {
        expected.name = name_b;
        expected.resource_type = 0x42534e44U;
        check(result == 0, "open failure returns zero");
        check(event_count == 2 && events[0] == 1 && events[1] == 2,
              "open failure still queries size then stops");
    } else if (selected_scenario == SCENARIO_ALLOC_FAIL) {
        expected.name = name_d;
        expected.resource_type = 0x87654321U;
        check(result == 0, "allocation failure returns zero");
        check(event_count == 5 && events[0] == 1 && events[1] == 2 &&
              events[2] == 3 && events[3] == 4 && events[4] == 5,
              "allocation failure reads null and closes in order");
    } else {
        expected.name = name_d;
        expected.resource_type = 0x87654321U;
        check(result == 1, "success ignores failing registration return");
        check(event_count == 6 && events[0] == 1 && events[1] == 2 &&
              events[2] == 3 && events[3] == 4 && events[4] == 5 &&
              events[5] == 6, "success callback order");
    }
    check(op_sound_file_api == &api_b && op_sound_allocation_tag == 0x778899aaU,
          "operation one retains callback global state");
    check(memcmp(&guarded.event, &expected, sizeof(expected)) == 0,
          "operation one retains only callback mutations");
    check(guarded.before == 0x11111111U && guarded.after == 0x22222222U,
          "operation one preserves event neighbors");
}

static void free_case(void *data)
{
    GUARDED_EVENT guarded;
    GUARDED_EVENT before;
    reset_event(&guarded, 2);
    guarded.event.data = data;
    before = guarded;
    check(op_sound_resource_handler(&guarded.event) == 1, "free operation returns one");
    check(event_count == 1 && events[0] == 7, "free operation calls only free");
    check(memcmp(&guarded, &before, sizeof(guarded)) == 0,
          "free operation preserves full event and guards");
}

static void size_case(void *data, unsigned int value)
{
    GUARDED_EVENT guarded;
    OP_SOUND_RESOURCE_EVENT expected;
    reset_event(&guarded, 3);
    guarded.event.data = data;
    expected = guarded.event;
    expected.size = value;
    sized_resource = value;
    check(op_sound_resource_handler(&guarded.event) == 1, "size operation returns one");
    check(event_count == 1 && events[0] == 8, "size operation calls only size helper");
    check(memcmp(&guarded.event, &expected, sizeof(expected)) == 0,
          "size operation changes only event size");
    check(guarded.before == 0x11111111U && guarded.after == 0x22222222U,
          "size operation preserves neighbors");
}

int main(void)
{
    initialize_apis();
    unknown_case(0);
    unknown_case(4);
    unknown_case(-1);
    unknown_case(INT_MAX);
    open_case(SCENARIO_OPEN_FAIL, 0xffffffffU);
    open_case(SCENARIO_ALLOC_FAIL, 0U);
    open_case(SCENARIO_SUCCESS, 13U);
    open_case(SCENARIO_SUCCESS, 0U);
    open_case(SCENARIO_SUCCESS, 0xffffffffU);
    free_case(payload);
    free_case(0);
    size_case(payload, 0U);
    size_case(payload, 0xffffffffU);
    size_case(0, 13U);
    printf("Sound resource handler: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
