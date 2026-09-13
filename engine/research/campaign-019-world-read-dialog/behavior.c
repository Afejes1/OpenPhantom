#include "api.h"
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
typedef char dialog_header_count[(offsetof(OP_HEADER, dialog_count) == 0x2f0) ? 1 : -1];
typedef char dialog_header_size[(offsetof(OP_HEADER, dialog_payload_size) == 0x2f4) ? 1 : -1];
typedef char dialog_world_count[(offsetof(OP_WORLD, dialog_count) == 0xcd4) ? 1 : -1];
typedef char dialog_world_records[(offsetof(OP_WORLD, dialog_records) == 0xcd8) ? 1 : -1];
typedef char dialog_record_link[(offsetof(OP_DIALOG_RECORD, link) == 12) ? 1 : -1];
static int dialog_checks, dialog_failures, dialog_calls, dialog_result, dialog_mode;
static void *dialog_destination;
static int dialog_size, dialog_elements, dialog_visible_count;
static OP_STREAM *dialog_stream;
static OP_WORLD *dialog_world;
static OP_HEADER *dialog_header;
static OP_DIALOG_RECORD *dialog_alternate;
static void dialog_check(int x)
{
    ++dialog_checks;
    if (!x)
        ++dialog_failures;
}
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    ++dialog_calls;
    dialog_destination = destination;
    dialog_size = size;
    dialog_elements = count;
    dialog_stream = stream;
    dialog_visible_count = (int)dialog_world->dialog_count;
    if (dialog_mode == 1)
    {
        dialog_world->dialog_count = 77;
        dialog_world->dialog_records = dialog_alternate;
        dialog_header->dialog_count = 3;
        dialog_header->dialog_payload_size = 0x12345678U;
    }
    if (dialog_mode == 2)
    {
        dialog_world->dialog_records = dialog_alternate;
        dialog_header->dialog_count = 2;
        dialog_header->dialog_payload_size = 0x87654321U;
    }
    return dialog_result;
}
typedef struct DIALOG_GUARDED_HEADER
{
    unsigned int before;
    OP_HEADER value;
    unsigned int after;
} DIALOG_GUARDED_HEADER;
typedef struct DIALOG_GUARDED_WORLD
{
    unsigned int before;
    OP_WORLD value;
    unsigned int after;
} DIALOG_GUARDED_WORLD;
static void dialog_initialize(DIALOG_GUARDED_HEADER *h, DIALOG_GUARDED_WORLD *w, OP_DIALOG_RECORD *records,
                              OP_STREAM *stream)
{
    memset(h, 0x35, sizeof(*h));
    memset(w, 0x46, sizeof(*w));
    memset(stream, 0x57, sizeof(*stream));
    h->before = 0x11223344U;
    h->after = 0x55667788U;
    w->before = 0x89abcdefU;
    w->after = 0x76543210U;
    stream->handle = records;
    stream->mode = 7;
    w->value.dialog_count = 9;
    w->value.dialog_records = records;
    dialog_world = &w->value;
    dialog_header = &h->value;
}
static void dialog_test_failure(void)
{
    DIALOG_GUARDED_HEADER h, eh;
    DIALOG_GUARDED_WORLD w, ew;
    OP_STREAM stream;
    OP_DIALOG_RECORD records[3], alternate[3], er[3], ea[3];
    int result;
    memset(records, 0x61, sizeof(records));
    memset(alternate, 0x62, sizeof(alternate));
    dialog_initialize(&h, &w, records, &stream);
    h.value.dialog_count = 1;
    h.value.dialog_payload_size = 15;
    eh = h;
    ew = w;
    ew.value.dialog_count = 77;
    ew.value.dialog_records = alternate;
    eh.value.dialog_count = 3;
    eh.value.dialog_payload_size = 0x12345678U;
    memcpy(er, records, sizeof(er));
    memcpy(ea, alternate, sizeof(ea));
    dialog_calls = 0;
    dialog_result = 0;
    dialog_mode = 1;
    dialog_alternate = alternate;
    result = op_world_read_dialog(&stream, &w.value, &h.value);
    dialog_check(result == 0);
    dialog_check(dialog_calls == 1);
    dialog_check(dialog_destination == records);
    dialog_check(dialog_size == 15 && dialog_elements == 1);
    dialog_check(dialog_stream == &stream);
    dialog_check(dialog_visible_count == 0);
    dialog_check(memcmp(&h, &eh, sizeof(h)) == 0);
    dialog_check(memcmp(&w, &ew, sizeof(w)) == 0);
    dialog_check(memcmp(records, er, sizeof(records)) == 0);
    dialog_check(memcmp(alternate, ea, sizeof(alternate)) == 0);
}
static void dialog_test_retarget(void)
{
    DIALOG_GUARDED_HEADER h, eh;
    DIALOG_GUARDED_WORLD w, ew;
    OP_STREAM stream;
    OP_DIALOG_RECORD records[3], alternate[3], er[3], ea[3];
    int result;
    memset(records, 0x63, sizeof(records));
    memset(alternate, 0x64, sizeof(alternate));
    dialog_initialize(&h, &w, records, &stream);
    h.value.dialog_count = 1;
    h.value.dialog_payload_size = 16;
    eh = h;
    eh.value.dialog_count = 2;
    eh.value.dialog_payload_size = 0x87654321U;
    ew = w;
    ew.value.dialog_count = 2;
    ew.value.dialog_records = alternate;
    memcpy(er, records, sizeof(er));
    memcpy(ea, alternate, sizeof(ea));
    ea[0].link = 0;
    ea[1].link = 0;
    dialog_calls = 0;
    dialog_result = -7;
    dialog_mode = 2;
    dialog_alternate = alternate;
    result = op_world_read_dialog(&stream, &w.value, &h.value);
    dialog_check(result == 1);
    dialog_check(dialog_destination == records);
    dialog_check(dialog_visible_count == 0);
    dialog_check(memcmp(&h, &eh, sizeof(h)) == 0);
    dialog_check(memcmp(&w, &ew, sizeof(w)) == 0);
    dialog_check(memcmp(records, er, sizeof(records)) == 0);
    dialog_check(memcmp(alternate, ea, sizeof(alternate)) == 0);
}
static void dialog_test_highbit(void)
{
    DIALOG_GUARDED_HEADER h, eh;
    DIALOG_GUARDED_WORLD w, ew;
    OP_STREAM stream;
    OP_DIALOG_RECORD records[1], er[1];
    int result;
    memset(records, 0x65, sizeof(records));
    dialog_initialize(&h, &w, records, &stream);
    h.value.dialog_count = 0x80000000U;
    h.value.dialog_payload_size = 0x80000000U;
    eh = h;
    ew = w;
    ew.value.dialog_count = 0x80000000U;
    memcpy(er, records, sizeof(er));
    dialog_calls = 0;
    dialog_result = 1;
    dialog_mode = 0;
    result = op_world_read_dialog(&stream, &w.value, &h.value);
    dialog_check(result == 1);
    dialog_check(dialog_size == INT_MIN);
    dialog_check((unsigned int)dialog_size == 0x80000000U);
    dialog_check(memcmp(&h, &eh, sizeof(h)) == 0);
    dialog_check(memcmp(&w, &ew, sizeof(w)) == 0);
    dialog_check(memcmp(records, er, sizeof(records)) == 0);
}
typedef char dialog_record_extent[(sizeof(OP_DIALOG_RECORD) == 16) ? 1 : -1];
typedef struct DIALOG_RECORD_GUARD
{
    unsigned int before;
    OP_DIALOG_RECORD values[3];
    unsigned int after;
} DIALOG_RECORD_GUARD;
typedef struct DIALOG_STREAM_GUARD
{
    unsigned int before;
    OP_STREAM value;
    unsigned int after;
} DIALOG_STREAM_GUARD;

static void dialog_test_guarded(void)
{
    DIALOG_GUARDED_HEADER header;
    DIALOG_GUARDED_HEADER expected_header;
    DIALOG_GUARDED_WORLD world;
    DIALOG_GUARDED_WORLD expected_world;
    DIALOG_RECORD_GUARD records;
    DIALOG_RECORD_GUARD expected_records;
    DIALOG_STREAM_GUARD stream;
    DIALOG_STREAM_GUARD expected_stream;
    int scenario;
    int index;
    int result;

    for (scenario = 0; scenario < 4; ++scenario)
    {
        memset(&records, 0x71, sizeof(records));
        memset(&stream, 0x72, sizeof(stream));
        for (index = 0; index < 3; ++index)
            records.values[index].link = &records.values[index];
        dialog_initialize(&header, &world, records.values, &stream.value);
        header.value.dialog_count = scenario == 0 ? 0U : (scenario == 2 ? 0xffffffffU : 3U);
        header.value.dialog_payload_size = scenario == 0 ? 0U : (scenario == 2 ? 0xffffffffU : 7U);
        expected_header = header;
        expected_world = world;
        expected_world.value.dialog_count = scenario == 3 ? 0U : header.value.dialog_count;
        expected_records = records;
        expected_stream = stream;
        if (scenario == 1)
        {
            for (index = 0; index < 3; ++index)
                expected_records.values[index].link = 0;
        }
        dialog_calls = 0;
        dialog_mode = 0;
        dialog_result = scenario == 3 ? 0 : -9;
        result = op_world_read_dialog(&stream.value, &world.value, &header.value);
        dialog_check(result == (scenario == 3 ? 0 : 1));
        dialog_check(dialog_calls == 1);
        dialog_check(dialog_destination == records.values);
        dialog_check(dialog_size == (scenario == 0 ? 0 : (scenario == 2 ? -1 : 7)));
        dialog_check(dialog_elements == 1);
        dialog_check(dialog_stream == &stream.value);
        dialog_check(dialog_visible_count == 0);
        dialog_check(memcmp(&header, &expected_header, sizeof(header)) == 0);
        dialog_check(memcmp(&world, &expected_world, sizeof(world)) == 0);
        dialog_check(memcmp(&records, &expected_records, sizeof(records)) == 0);
        dialog_check(memcmp(&stream, &expected_stream, sizeof(stream)) == 0);
    }
}

int main(void)
{
    dialog_test_guarded();
    dialog_test_failure();
    dialog_test_retarget();
    dialog_test_highbit();
    printf("world read dialog: %d checks, %d failures\n", dialog_checks, dialog_failures);
    return dialog_failures != 0;
}
