#include "api.h"
#include <stdio.h>
#include <string.h>
typedef struct LOCAL_HEADER
{
    char tag[8];
    unsigned int payload;
} LOCAL_HEADER;
typedef struct GUARDED_OUTPUT
{
    unsigned int before, value, after;
} GUARDED_OUTPUT;
typedef char local_header_size[sizeof(LOCAL_HEADER) == 12 ? 1 : -1];
typedef char local_payload_offset[offsetof(LOCAL_HEADER, payload) == 8 ? 1 : -1];
static char tag_storage[23][8];
static char unknown_tag[8];
const char *op_chunk_tags[23];
static int checks, failures, read_calls, compare_calls, read_result, selected, mutate_table;
static unsigned int callback_payload;
static OP_STREAM *expected_stream;
static void *expected_local;
static GUARDED_OUTPUT *active_output;
static void check(int value)
{
    ++checks;
    if (!value)
        ++failures;
}
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream)
{
    LOCAL_HEADER header;
    expected_local = destination;
    ++read_calls;
    check(size == 12);
    check(count == 1);
    check(stream == expected_stream);
    check(active_output->value == 0);
    active_output->value = 0xabcdef01U;
    if (read_result != 0)
    {
        memcpy(header.tag, selected < 23 ? tag_storage[selected] : unknown_tag, 8);
        header.payload = callback_payload;
        memcpy(destination, &header, 12);
    }
    return read_result;
}
int op_compare_tag(const char *left, const char *right, unsigned int limit)
{
    int call = compare_calls++;
    check(limit == 8);
    check(left == (const char *)expected_local);
    check(call >= 0 && call < 23);
    if (call < 0 || call >= 23)
        return 1;
    check(right == op_chunk_tags[call]);
    if (mutate_table && call == 0)
        op_chunk_tags[1] = tag_storage[selected];
    active_output->value = 0x12345678U;
    return strncmp(left, right, limit);
}
static void initialize_tags(void)
{
    int i, j;
    for (i = 0; i < 23; ++i)
    {
        for (j = 0; j < 8; ++j)
            tag_storage[i][j] = (char)(1 + i * 8 + j);
        op_chunk_tags[i] = tag_storage[i];
    }
    for (j = 0; j < 8; ++j)
        unknown_tag[j] = (char)(0xe0 + j);
}
static void run_case(int wanted, int result_value)
{
    GUARDED_OUTPUT output;
    OP_STREAM stream;
    OP_STREAM stream_before;
    const char *table_before[23];
    char tags_before[23][8];
    int result, table_index;
    initialize_tags();
    stream.handle = &unknown_tag;
    stream.mode = 4;
    stream_before = stream;
    for (table_index = 0; table_index < 23; ++table_index)
        table_before[table_index] = op_chunk_tags[table_index];
    memcpy(tags_before, tag_storage, sizeof(tags_before));
    output.before = 0x11223344U;
    output.value = 0x55667788U;
    output.after = 0x99aabbccU;
    active_output = &output;
    expected_stream = &stream;
    selected = wanted;
    read_result = result_value;
    callback_payload = 0x80000000U + (unsigned int)wanted;
    read_calls = compare_calls = 0;
    mutate_table = 0;
    result = op_world_read_chunk_header(&stream, &output.value);
    check(read_calls == 1);
    check(memcmp(&stream, &stream_before, sizeof(stream)) == 0);
    check(memcmp(table_before, op_chunk_tags, sizeof(table_before)) == 0);
    check(memcmp(tags_before, tag_storage, sizeof(tags_before)) == 0);
    check(output.before == 0x11223344U && output.after == 0x99aabbccU);
    if (result_value == 0)
    {
        check(result == -1);
        check(compare_calls == 0);
        check(output.value == 0xabcdef01U);
    }
    else
    {
        check(result == (wanted < 23 ? wanted : 23));
        check(compare_calls == (wanted < 23 ? wanted + 1 : 23));
        check(output.value == callback_payload);
    }
}
int main(void)
{
    int i;
    for (i = 0; i < 23; ++i)
        run_case(i, i == 7 ? -9 : 1);
    run_case(23, 1);
    run_case(0, 0);
    initialize_tags();
    mutate_table = 1;
    selected = 5;
    read_result = 1;
    callback_payload = 77;
    {
        GUARDED_OUTPUT output = {1, 2, 3};
        OP_STREAM stream = {unknown_tag, 4};
        OP_STREAM before_stream = stream;
        active_output = &output;
        expected_stream = &stream;
        read_calls = compare_calls = 0;
        check(op_world_read_chunk_header(&stream, &output.value) == 1);
        check(compare_calls == 2);
        check(output.value == 77);
        check(read_calls == 1);
        check(op_chunk_tags[1] == tag_storage[5]);
        check(output.before == 1 && output.after == 3);
        check(memcmp(&stream, &before_stream, sizeof(stream)) == 0);
    }
    /* strncmp stops at a shared NUL even when the remaining tag bytes differ. */
    initialize_tags();
    memset(tag_storage[0], 'A', 8);
    memset(tag_storage[1], 'B', 8);
    tag_storage[0][0] = tag_storage[1][0] = 'Q';
    tag_storage[0][1] = tag_storage[1][1] = 0;
    selected = 1;
    mutate_table = 0;
    read_result = -1;
    callback_payload = 0xffffffffU;
    {
        GUARDED_OUTPUT output = {1, 2, 3};
        OP_STREAM stream = {unknown_tag, 4};
        active_output = &output;
        expected_stream = &stream;
        read_calls = compare_calls = 0;
        check(op_world_read_chunk_header(&stream, &output.value) == 0);
        check(read_calls == 1 && compare_calls == 1);
        check(output.value == 0xffffffffU);
        check(output.before == 1 && output.after == 3);
    }
    printf("chunk header: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}