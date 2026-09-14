#include "api.h"
#include <stdio.h>
#include <string.h>
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
typedef struct OWNED_BUFFER
{
    unsigned int before;
    unsigned char data[64];
    unsigned int after;
} OWNED_BUFFER;
static OWNED_BUFFER buffers[3], expected_buffers[3];
void *op_save_output_stream, *op_save_input_stream;
unsigned char op_save_error_context[116];
const char op_save_failure_text[] = "authored-write-failure";
static unsigned char expected_context[116];
static unsigned int stream_tokens[3];
static void *expected_output, *expected_input;
static const int answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int sizes[] = {0, 1, 8, 63, 0xffffffffu};
static int row, answer, size_case, stream_case, mutate, stage;
static void seed(void)
{
    memset(buffers, 0x57, sizeof(buffers));
    memcpy(expected_buffers, buffers, sizeof(buffers));
    memset(op_save_error_context, 0x63, sizeof(op_save_error_context));
    memcpy(expected_context, op_save_error_context, sizeof(expected_context));
    op_save_input_stream = expected_input = stream_case ? &stream_tokens[stream_case - 1] : 0;
    op_save_output_stream = expected_output = stream_case ? &stream_tokens[2 - stream_case] : 0;
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(buffers, expected_buffers, sizeof(buffers)) == 0);
    CHECK(memcmp(op_save_error_context, expected_context, sizeof(expected_context)) == 0);
    CHECK(op_save_input_stream == expected_input && op_save_output_stream == expected_output);
}
static void change(void)
{
    if (mutate)
    {
        buffers[row].data[3] = expected_buffers[row].data[3] = 0x29;
        op_save_output_stream = expected_output = &stream_tokens[2];
        op_save_input_stream = expected_input = &stream_tokens[0];
        op_save_error_context[9] = expected_context[9] = 0x35;
    }
}

static int header_answer, kind_case, context_case;
static const unsigned short kinds[] = {0, 1, 0x7fff, 0x8000, 0xffff};
static const int contexts[] = {0, -1, INT_MIN, INT_MAX};
int op_save_header(int context, int bytes, unsigned short kind)
{
    CHECK(stage == 0 && context == contexts[context_case] && (unsigned int)bytes == sizes[size_case] &&
          kind == kinds[kind_case]);
    verify();
    change();
    ++stage;
    return answers[header_answer];
}
int op_save_write(const void *data, unsigned int bytes)
{
    CHECK(stage == 1 && data == buffers[row].data && bytes == sizes[size_case]);
    verify();
    buffers[row].data[4] = expected_buffers[row].data[4] = 0x43;
    ++stage;
    return answers[answer];
}
int main(void)
{
    for (row = 0; row < 3; ++row)
        for (answer = 0; answer < 5; ++answer)
            for (size_case = 0; size_case < 5; ++size_case)
                for (stream_case = 0; stream_case < 3; ++stream_case)
                    for (mutate = 0; mutate < 2; ++mutate)
                        for (header_answer = 0; header_answer < 5; ++header_answer)
                            for (kind_case = 0; kind_case < 5; ++kind_case)
                                for (context_case = 0; context_case < 4; ++context_case)
                                {
                                    seed();
                                    CHECK(op_save_write_chunk(contexts[context_case], buffers[row].data,
                                                              sizes[size_case], kinds[kind_case]) ==
                                          ((answers[header_answer] == 1 || answers[answer] == 0) ? 1 : 0));
                                    CHECK(stage == (answers[header_answer] == 1 ? 1 : 2));
                                    verify();
                                }
    printf("save_write_chunk: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
