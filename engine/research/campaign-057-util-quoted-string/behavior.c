#include "api.h"
#include <stdio.h>
#include <limits.h>
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

typedef struct OWNED_TEXT
{
    unsigned int before;
    char data[96];
    unsigned int after;
} OWNED_TEXT;
static OWNED_TEXT input, output, expected_input, expected_output;
static unsigned int length, prefix, source_offset, destination_offset, size, copy_count;
static int output_present, mutate, stage;
static void verify(void)
{
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    CHECK(memcmp(&output, &expected_output, sizeof(output)) == 0);
}
static unsigned int bounded_count(void)
{
    unsigned int cap = size - 1;
    return length < cap ? length : cap;
}
static void initialize(void)
{
    memset(&input, 0x57, sizeof(input));
    memset(&output, 0x68, sizeof(output));
    stage = 0;
}
static void snapshot(void)
{
    memcpy(&expected_input, &input, sizeof(input));
    memcpy(&expected_output, &output, sizeof(output));
    if (output_present)
        expected_output.data[destination_offset] = 0;
    copy_count = bounded_count();
}

static int scenario;
char *op_find_character(const char *text, int character)
{
    unsigned int i;
    CHECK(character == 34);
    CHECK(stage == 0 || stage == 1);
    verify();
    if (stage == 0)
    {
        CHECK(text == input.data + source_offset);
        stage = 1;
        if (scenario == 0)
            return 0;
        if (mutate && length)
        {
            input.data[source_offset + prefix + 1] = 'z';
            expected_input.data[source_offset + prefix + 1] = 'z';
        }
        return input.data + source_offset + prefix;
    }
    CHECK(text == input.data + source_offset + prefix + 1);
    stage = 2;
    if (scenario == 1)
        return 0;
    if (mutate && length > 1)
    {
        input.data[source_offset + prefix + 2] = 'q';
        expected_input.data[source_offset + prefix + 2] = 'q';
    }
    if (output_present)
    {
        for (i = 0; i < copy_count; ++i)
            expected_output.data[destination_offset + i] = expected_input.data[source_offset + prefix + 1 + i];
        expected_output.data[destination_offset + copy_count] = 0;
    }
    return input.data + source_offset + prefix + 1 + length;
}
int main(void)
{
    static const unsigned int lengths[] = {0, 1, 3, 4, 15, 31};
    static const unsigned int sizes[] = {0, 1, 2, 4, 8, 32, 64, 0xffffffffu};
    unsigned int li, si, i, start;
    char *result;
    for (li = 0; li < 6; ++li)
        for (prefix = 0; prefix < 4; ++prefix)
            for (source_offset = 0; source_offset < 4; ++source_offset)
                for (destination_offset = 0; destination_offset < 4; ++destination_offset)
                    for (si = 0; si < 8; ++si)
                        for (scenario = 0; scenario < 3; ++scenario)
                            for (output_present = 0; output_present < 2; ++output_present)
                                for (mutate = 0; mutate < 2; ++mutate)
                                {
                                    length = lengths[li];
                                    size = sizes[si];
                                    initialize();
                                    for (i = 0; i < prefix; ++i)
                                        input.data[source_offset + i] = 'p';
                                    start = source_offset + prefix;
                                    if (scenario)
                                        input.data[start++] = '"';
                                    for (i = 0; i < length; ++i)
                                        input.data[start + i] = (char)('A' + i % 26);
                                    if (scenario == 2)
                                    {
                                        input.data[start + length] = '"';
                                        memcpy(input.data + start + length + 1, "tail", sizeof("tail"));
                                    }
                                    else
                                        input.data[start + length] = 0;
                                    snapshot();
                                    result = op_util_quoted_string(
                                        input.data + source_offset,
                                        output_present ? output.data + destination_offset : 0, size);
                                    CHECK(result == (scenario == 2 ? input.data + start + length + 1 : 0));
                                    CHECK(stage == (scenario ? 2 : 1));
                                    verify();
                                }
    printf("util_quoted_string: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
