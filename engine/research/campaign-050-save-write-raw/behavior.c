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

int op_stream_write(const void *data, unsigned int bytes, unsigned int count, void *stream)
{
    CHECK(stage == 0 && data == buffers[row].data && bytes == sizes[size_case] && count == 1 &&
          stream == expected_output);
    verify();
    change();
    ++stage;
    return answers[answer];
}
void op_save_error_begin(void *context)
{
    CHECK(stage == 1 && answers[answer] == 0 && context == op_save_error_context);
    verify();
    op_save_error_context[48] = expected_context[48] = 0x41;
    ++stage;
}
int op_save_error_notify(int code, int value, int zero, const char *text, int flags)
{
    CHECK(stage == 2 && code == 17 && value == -1 && zero == 0 && text == op_save_failure_text && flags == 3840);
    verify();
    op_save_error_context[84] = expected_context[84] = 0x72;
    buffers[row].data[8] = expected_buffers[row].data[8] = 0x28;
    ++stage;
    return mutate ? INT_MIN : INT_MAX;
}
void op_save_error_end(void *context)
{
    CHECK(stage == 3 && context == op_save_error_context);
    verify();
    op_save_error_context[107] = expected_context[107] = 0x23;
    ++stage;
}
int main(void)
{
    for (row = 0; row < 3; ++row)
        for (answer = 0; answer < 5; ++answer)
            for (size_case = 0; size_case < 5; ++size_case)
                for (stream_case = 0; stream_case < 3; ++stream_case)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        seed();
                        CHECK(op_save_write_raw(buffers[row].data, sizes[size_case]) == answers[answer]);
                        CHECK(stage == (answers[answer] == 0 ? 4 : 1));
                        verify();
                    }
    printf("save_write_raw: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
