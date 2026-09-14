#include "api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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
OP_SKILLS_STATE op_skills_state;
int op_skills_active_index, op_skills_previous_index;

#include "save_write_chunk_support.c"
#include "save_write_support.c"
#include "save_write_raw_support.c"
void *op_save_output_stream, *op_save_input_stream;
unsigned char op_save_error_context[116];
const char op_save_failure_text[] = "authored-write-failure";
static OP_SKILLS_STATE expected_state, payload;
static int expected_active, expected_previous, stage, mutate, context, header_result, file_result;
static unsigned int tokens[3];
static void *expected_input, *expected_output;
static unsigned char expected_context[116];
static void verify(void)
{
    CHECK(memcmp(&op_skills_state, &expected_state, sizeof(expected_state)) == 0);
    CHECK(op_skills_active_index == expected_active);
    CHECK(op_skills_previous_index == expected_previous);
    CHECK(op_save_input_stream == expected_input);
    CHECK(op_save_output_stream == expected_output);
    CHECK(memcmp(op_save_error_context, expected_context, sizeof(expected_context)) == 0);
}
int op_save_header(int actual, int bytes, unsigned short kind)
{
    CHECK(stage == 0);
    if (stage)
        exit(2);
    CHECK(actual == context);
    CHECK(bytes == 0xd8 && kind == 2);
    verify();
    if (mutate)
    {
        op_skills_state.words[0] = expected_state.words[0] = -731;
        op_save_output_stream = expected_output = &tokens[2];
        op_skills_active_index = expected_active = -17;
    }
    stage = 1;
    return header_result;
}
int op_save_file_write(const void *data, unsigned int bytes, unsigned int count, void *stream)
{
    CHECK(stage == 1);
    if (stage != 1)
        exit(2);
    CHECK(data == &op_skills_state && bytes == 0xd8 && count == 1);
    CHECK(stream == expected_output);
    verify();
    if (mutate)
    {
        op_skills_state.words[1] = expected_state.words[1] = 571;
        op_save_input_stream = expected_input = &tokens[2];
        op_skills_previous_index = expected_previous = 313;
    }
    stage = 2;
    return file_result;
}
int op_save_file_read(void *data, unsigned int bytes, unsigned int count, void *stream)
{
    CHECK(stage == 0);
    if (stage)
        exit(2);
    CHECK(data == &op_skills_state && bytes == 0xd8 && count == 1);
    CHECK(stream == expected_input);
    verify();
    if (data != &op_skills_state || bytes != 0xd8)
        exit(2);
    memcpy(data, &payload, sizeof(payload));
    expected_state = payload;
    if (mutate)
    {
        op_skills_active_index = expected_active = -91;
        op_skills_previous_index = expected_previous = 719;
        op_save_output_stream = expected_output = &tokens[2];
    }
    stage = 1;
    return file_result;
}
void op_save_error_begin(void *p)
{
    CHECK(stage == 2 && p == op_save_error_context);
    if (stage != 2 || p != op_save_error_context)
        exit(2);
    verify();
    op_save_error_context[4] = expected_context[4] = 0x41;
    stage = 3;
}
int op_save_error_notify(int code, int value, int zero, const char *text, int flags)
{
    CHECK(stage == 3);
    if (stage != 3)
        exit(2);
    CHECK(code == 17 && value == -1 && zero == 0 && text == op_save_failure_text && flags == 3840);
    verify();
    op_skills_state.words[3] = expected_state.words[3] = -117;
    stage = 4;
    return INT_MIN;
}
void op_save_error_end(void *p)
{
    CHECK(stage == 4 && p == op_save_error_context);
    if (stage != 4 || p != op_save_error_context)
        exit(2);
    verify();
    op_save_error_context[9] = expected_context[9] = 0x72;
    stage = 5;
}
int main(void)
{
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, h, f, p, result, expected_result, expected_stage;
    for (a = 0; a < 7; ++a)
        for (h = 0; h < 5; ++h)
            for (f = 0; f < 7; ++f)
                for (p = 0; p < 4; ++p)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        memset(&op_skills_state, 0x31 + p, sizeof(op_skills_state));
                        expected_state = op_skills_state;
                        memset(&payload, 0x61 + p, sizeof(payload));
                        op_skills_active_index = expected_active = values[(a + 1) % 7];
                        op_skills_previous_index = expected_previous = values[(a + 3) % 7];
                        op_save_input_stream = expected_input = &tokens[p % 3];
                        op_save_output_stream = expected_output = &tokens[(p + 1) % 3];
                        memset(op_save_error_context, 0x57, sizeof(op_save_error_context));
                        memcpy(expected_context, op_save_error_context, sizeof(expected_context));
                        context = values[a];
                        header_result = values[h];
                        file_result = values[f];
                        stage = 0;
                        expected_result = header_result == 1 || file_result == 0;
                        expected_stage = header_result == 1 ? 1 : file_result == 0 ? 5 : 2;
                        result = op_skills_save(context);
                        CHECK(result == expected_result);
                        CHECK(stage == expected_stage);
                        verify();
                    }
    printf("skills_save: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
