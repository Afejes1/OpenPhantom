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

static int separator;
static char delimiters[] = " ,;";
static char expected_delimiters[sizeof(delimiters)];
unsigned int op_span_delimiters(const char *text, const char *set)
{
    CHECK(stage == 0 && text == input.data + source_offset && set == delimiters);
    verify();
    CHECK(memcmp(delimiters, expected_delimiters, sizeof(delimiters)) == 0);
    stage = 1;
    return prefix;
}
char *op_find_delimiter(const char *text, const char *set)
{
    CHECK(stage == 1 && text == input.data + source_offset + prefix && set == delimiters);
    verify();
    CHECK(memcmp(delimiters, expected_delimiters, sizeof(delimiters)) == 0);
    if (mutate && length)
    {
        input.data[source_offset + prefix] = 'z';
        expected_input.data[source_offset + prefix] = 'z';
    }
    stage = 2;
    return separator ? input.data + source_offset + prefix + length : 0;
}
char *op_copy_keyframe_name(char *out, const char *text, unsigned int count)
{
    unsigned int i;
    CHECK(stage == 2 && output_present && out == output.data + destination_offset &&
          text == input.data + source_offset + prefix && count == copy_count);
    verify();
    for (i = 0; i < copy_count; ++i)
    {
        output.data[destination_offset + i] = expected_input.data[source_offset + prefix + i];
        expected_output.data[destination_offset + i] = expected_input.data[source_offset + prefix + i];
    }
    output.data[destination_offset + copy_count] = 'Z';
    expected_output.data[destination_offset + copy_count] = 0;
    stage = 3;
    return input.data;
}
int main(void)
{
    static const unsigned int lengths[] = {0, 1, 3, 4, 15, 31};
    static const unsigned int sizes[] = {0, 1, 2, 4, 8, 32, 64, 0xffffffffu};
    unsigned int li, si, i;
    char *result;
    for (li = 0; li < 6; ++li)
        for (prefix = 0; prefix < 4; ++prefix)
            for (source_offset = 0; source_offset < 4; ++source_offset)
                for (destination_offset = 0; destination_offset < 4; ++destination_offset)
                    for (si = 0; si < 8; ++si)
                        for (separator = 0; separator < 2; ++separator)
                            for (output_present = 0; output_present < 2; ++output_present)
                                for (mutate = 0; mutate < 2; ++mutate)
                                {
                                    length = lengths[li];
                                    size = sizes[si];
                                    if (length == 0 && separator)
                                        continue;
                                    initialize();
                                    for (i = 0; i < prefix; ++i)
                                        input.data[source_offset + i] = delimiters[i % 3];
                                    for (i = 0; i < length; ++i)
                                        input.data[source_offset + prefix + i] = (char)('A' + i % 26);
                                    if (separator)
                                    {
                                        input.data[source_offset + prefix + length] = ';';
                                        memcpy(input.data + source_offset + prefix + length + 1, "tail",
                                               sizeof("tail"));
                                    }
                                    else
                                        input.data[source_offset + prefix + length] = 0;
                                    memcpy(expected_delimiters, delimiters, sizeof(delimiters));
                                    snapshot();
                                    result = op_util_tokenize(input.data + source_offset,
                                                              output_present ? output.data + destination_offset : 0,
                                                              size, delimiters);
                                    CHECK(result == (separator ? input.data + source_offset + prefix + length : 0));
                                    CHECK(stage == (output_present ? 3 : 2));
                                    verify();
                                    CHECK(memcmp(delimiters, expected_delimiters, sizeof(delimiters)) == 0);
                                }
    printf("util_tokenize: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
