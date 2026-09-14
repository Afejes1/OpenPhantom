#include "../src/skills_state.h"
#include "../src/save_stream.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
static int s870_checks, s870_failures;
static void s870_check(int ok, int line)
{
    ++s870_checks;
    if (!ok)
    {
        ++s870_failures;
        printf("line%d failed\n", line);
    }
}
#define S870_CHECK(x) s870_check(!!(x), __LINE__)

static OP_SKILLS_STATE s870_expected_state, s870_payload;
static int s870_expected_active, s870_expected_previous, s870_stage, s870_mutate, s870_context, s870_header_result,
    s870_file_result;
static unsigned int s870_tokens[3];
static void *s870_expected_input, *s870_expected_output;
static unsigned char s870_expected_context[116];
static void s870_verify(void)
{
    S870_CHECK(memcmp(&op_skills_state, &s870_expected_state, sizeof(s870_expected_state)) == 0);
    S870_CHECK(op_skills_active_index == s870_expected_active);
    S870_CHECK(op_skills_previous_index == s870_expected_previous);
    S870_CHECK(op_save_input_stream == s870_expected_input);
    S870_CHECK(op_save_output_stream == s870_expected_output);
    S870_CHECK(memcmp(op_save_error_context, s870_expected_context, sizeof(s870_expected_context)) == 0);
}
static int s870_header(int actual, int bytes, unsigned short kind)
{
    S870_CHECK(s870_stage == 0);
    if (s870_stage)
        exit(2);
    S870_CHECK(actual == s870_context);
    S870_CHECK(bytes == 0xd8 && kind == 2);
    s870_verify();
    if (s870_mutate)
    {
        op_skills_state.words[0] = s870_expected_state.words[0] = -731;
        op_save_output_stream = s870_expected_output = &s870_tokens[2];
        op_skills_active_index = s870_expected_active = -17;
    }
    s870_stage = 1;
    return s870_header_result;
}
static int s870_file_write(const void *data, unsigned int bytes, unsigned int count, void *stream)
{
    S870_CHECK(s870_stage == 1);
    if (s870_stage != 1)
        exit(2);
    S870_CHECK(data == &op_skills_state && bytes == 0xd8 && count == 1);
    S870_CHECK(stream == s870_expected_output);
    s870_verify();
    if (s870_mutate)
    {
        op_skills_state.words[1] = s870_expected_state.words[1] = 571;
        op_save_input_stream = s870_expected_input = &s870_tokens[2];
        op_skills_previous_index = s870_expected_previous = 313;
    }
    s870_stage = 2;
    return s870_file_result;
}
static int s870_file_read(void *data, unsigned int bytes, unsigned int count, void *stream)
{
    S870_CHECK(s870_stage == 0);
    if (s870_stage)
        exit(2);
    S870_CHECK(data == &op_skills_state && bytes == 0xd8 && count == 1);
    S870_CHECK(stream == s870_expected_input);
    s870_verify();
    if (data != &op_skills_state || bytes != 0xd8)
        exit(2);
    memcpy(data, &s870_payload, sizeof(s870_payload));
    s870_expected_state = s870_payload;
    if (s870_mutate)
    {
        op_skills_active_index = s870_expected_active = -91;
        op_skills_previous_index = s870_expected_previous = 719;
        op_save_output_stream = s870_expected_output = &s870_tokens[2];
    }
    s870_stage = 1;
    return s870_file_result;
}
static void s870_error_begin(void *p)
{
    S870_CHECK(s870_stage == 2 && p == op_save_error_context);
    if (s870_stage != 2 || p != op_save_error_context)
        exit(2);
    s870_verify();
    op_save_error_context[4] = s870_expected_context[4] = 0x41;
    s870_stage = 3;
}
static int s870_error_notify(int code, int value, int zero, const char *text, int flags)
{
    S870_CHECK(s870_stage == 3);
    if (s870_stage != 3)
        exit(2);
    S870_CHECK(code == 17 && value == -1 && zero == 0 && text == op_save_failure_text && flags == 3840);
    s870_verify();
    op_skills_state.words[3] = s870_expected_state.words[3] = -117;
    s870_stage = 4;
    return INT_MIN;
}
static void s870_error_end(void *p)
{
    S870_CHECK(s870_stage == 4 && p == op_save_error_context);
    if (s870_stage != 4 || p != op_save_error_context)
        exit(2);
    s870_verify();
    op_save_error_context[9] = s870_expected_context[9] = 0x72;
    s870_stage = 5;
}
static int op_test_skills_save(void)
{
    int saved_route = skio_active;
    void *saved_input = op_save_input_stream, *saved_output = op_save_output_stream;
    unsigned char saved_context[116];
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, h, f, p, result, expected_result, expected_stage;
    memcpy(saved_context, op_save_error_context, sizeof(saved_context));
    skio_active = 1;
    for (a = 0; a < 7; ++a)
        for (h = 0; h < 5; ++h)
            for (f = 0; f < 7; ++f)
                for (p = 0; p < 4; ++p)
                    for (s870_mutate = 0; s870_mutate < 2; ++s870_mutate)
                    {
                        memset(&op_skills_state, 0x31 + p, sizeof(op_skills_state));
                        s870_expected_state = op_skills_state;
                        memset(&s870_payload, 0x61 + p, sizeof(s870_payload));
                        op_skills_active_index = s870_expected_active = values[(a + 1) % 7];
                        op_skills_previous_index = s870_expected_previous = values[(a + 3) % 7];
                        op_save_input_stream = s870_expected_input = &s870_tokens[p % 3];
                        op_save_output_stream = s870_expected_output = &s870_tokens[(p + 1) % 3];
                        memset(op_save_error_context, 0x57, sizeof(op_save_error_context));
                        memcpy(s870_expected_context, op_save_error_context, sizeof(s870_expected_context));
                        s870_context = values[a];
                        s870_header_result = values[h];
                        s870_file_result = values[f];
                        s870_stage = 0;
                        expected_result = s870_header_result == 1 || s870_file_result == 0;
                        expected_stage = s870_header_result == 1 ? 1 : s870_file_result == 0 ? 5 : 2;
                        result = op_skills_save(s870_context);
                        S870_CHECK(result == expected_result);
                        S870_CHECK(s870_stage == expected_stage);
                        s870_verify();
                    }
    op_save_input_stream = saved_input;
    op_save_output_stream = saved_output;
    memcpy(op_save_error_context, saved_context, sizeof(saved_context));
    skio_active = saved_route;
    printf("skills_save: %d checks, %d failures\n", s870_checks, s870_failures);
    return s870_failures != 0;
}

#undef S870_CHECK

#include "../src/skills_state.h"
#include "../src/save_stream.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
static int s871_checks, s871_failures;
static void s871_check(int ok, int line)
{
    ++s871_checks;
    if (!ok)
    {
        ++s871_failures;
        printf("line%d failed\n", line);
    }
}
#define S871_CHECK(x) s871_check(!!(x), __LINE__)

static OP_SKILLS_STATE s871_expected_state, s871_payload;
static int s871_expected_active, s871_expected_previous, s871_stage, s871_mutate, s871_context, s871_header_result,
    s871_file_result;
static unsigned int s871_tokens[3];
static void *s871_expected_input, *s871_expected_output;
static unsigned char s871_expected_context[116];
static void s871_verify(void)
{
    S871_CHECK(memcmp(&op_skills_state, &s871_expected_state, sizeof(s871_expected_state)) == 0);
    S871_CHECK(op_skills_active_index == s871_expected_active);
    S871_CHECK(op_skills_previous_index == s871_expected_previous);
    S871_CHECK(op_save_input_stream == s871_expected_input);
    S871_CHECK(op_save_output_stream == s871_expected_output);
    S871_CHECK(memcmp(op_save_error_context, s871_expected_context, sizeof(s871_expected_context)) == 0);
}
static int s871_header(int actual, int bytes, unsigned short kind)
{
    S871_CHECK(s871_stage == 0);
    if (s871_stage)
        exit(2);
    S871_CHECK(actual == s871_context);
    S871_CHECK(bytes == 0xd8 && kind == 2);
    s871_verify();
    if (s871_mutate)
    {
        op_skills_state.words[0] = s871_expected_state.words[0] = -731;
        op_save_output_stream = s871_expected_output = &s871_tokens[2];
        op_skills_active_index = s871_expected_active = -17;
    }
    s871_stage = 1;
    return s871_header_result;
}
static int s871_file_write(const void *data, unsigned int bytes, unsigned int count, void *stream)
{
    S871_CHECK(s871_stage == 1);
    if (s871_stage != 1)
        exit(2);
    S871_CHECK(data == &op_skills_state && bytes == 0xd8 && count == 1);
    S871_CHECK(stream == s871_expected_output);
    s871_verify();
    if (s871_mutate)
    {
        op_skills_state.words[1] = s871_expected_state.words[1] = 571;
        op_save_input_stream = s871_expected_input = &s871_tokens[2];
        op_skills_previous_index = s871_expected_previous = 313;
    }
    s871_stage = 2;
    return s871_file_result;
}
static int s871_file_read(void *data, unsigned int bytes, unsigned int count, void *stream)
{
    S871_CHECK(s871_stage == 0);
    if (s871_stage)
        exit(2);
    S871_CHECK(data == &op_skills_state && bytes == 0xd8 && count == 1);
    S871_CHECK(stream == s871_expected_input);
    s871_verify();
    if (data != &op_skills_state || bytes != 0xd8)
        exit(2);
    memcpy(data, &s871_payload, sizeof(s871_payload));
    s871_expected_state = s871_payload;
    if (s871_mutate)
    {
        op_skills_active_index = s871_expected_active = -91;
        op_skills_previous_index = s871_expected_previous = 719;
        op_save_output_stream = s871_expected_output = &s871_tokens[2];
    }
    s871_stage = 1;
    return s871_file_result;
}
static void s871_error_begin(void *p)
{
    S871_CHECK(s871_stage == 2 && p == op_save_error_context);
    if (s871_stage != 2 || p != op_save_error_context)
        exit(2);
    s871_verify();
    op_save_error_context[4] = s871_expected_context[4] = 0x41;
    s871_stage = 3;
}
static int s871_error_notify(int code, int value, int zero, const char *text, int flags)
{
    S871_CHECK(s871_stage == 3);
    if (s871_stage != 3)
        exit(2);
    S871_CHECK(code == 17 && value == -1 && zero == 0 && text == op_save_failure_text && flags == 3840);
    s871_verify();
    op_skills_state.words[3] = s871_expected_state.words[3] = -117;
    s871_stage = 4;
    return INT_MIN;
}
static void s871_error_end(void *p)
{
    S871_CHECK(s871_stage == 4 && p == op_save_error_context);
    if (s871_stage != 4 || p != op_save_error_context)
        exit(2);
    s871_verify();
    op_save_error_context[9] = s871_expected_context[9] = 0x72;
    s871_stage = 5;
}
static int op_test_skills_restore(void)
{
    int saved_route = skio_active;
    void *saved_input = op_save_input_stream, *saved_output = op_save_output_stream;
    unsigned char saved_context[116];
    static const int values[7] = {0, 1, -1, 2, -2, INT_MIN, INT_MAX};
    int a, h, f, p, result, expected_result, expected_stage;
    memcpy(saved_context, op_save_error_context, sizeof(saved_context));
    skio_active = 2;
    for (a = 0; a < 7; ++a)
        for (h = 0; h < 5; ++h)
            for (f = 0; f < 7; ++f)
                for (p = 0; p < 4; ++p)
                    for (s871_mutate = 0; s871_mutate < 2; ++s871_mutate)
                    {
                        memset(&op_skills_state, 0x31 + p, sizeof(op_skills_state));
                        s871_expected_state = op_skills_state;
                        memset(&s871_payload, 0x61 + p, sizeof(s871_payload));
                        op_skills_active_index = s871_expected_active = values[(a + 1) % 7];
                        op_skills_previous_index = s871_expected_previous = values[(a + 3) % 7];
                        op_save_input_stream = s871_expected_input = &s871_tokens[p % 3];
                        op_save_output_stream = s871_expected_output = &s871_tokens[(p + 1) % 3];
                        memset(op_save_error_context, 0x57, sizeof(op_save_error_context));
                        memcpy(s871_expected_context, op_save_error_context, sizeof(s871_expected_context));
                        s871_context = values[a];
                        s871_header_result = values[h];
                        s871_file_result = values[f];
                        s871_stage = 0;
                        expected_result = s871_context != 2 || s871_file_result == 0;
                        expected_stage = s871_context == 2 ? 1 : 0;
                        result = op_skills_restore(s871_context);
                        S871_CHECK(result == expected_result);
                        S871_CHECK(s871_stage == expected_stage);
                        s871_verify();
                    }
    op_save_input_stream = saved_input;
    op_save_output_stream = saved_output;
    memcpy(op_save_error_context, saved_context, sizeof(saved_context));
    skio_active = saved_route;
    printf("skills_restore: %d checks, %d failures\n", s871_checks, s871_failures);
    return s871_failures != 0;
}

#undef S871_CHECK

static int skio_header(int context, int bytes, unsigned short kind)
{
    if (skio_active == 1)
        return s870_header(context, bytes, kind);
    if (skio_active == 2)
        return s871_header(context, bytes, kind);
    s870_check(0,__LINE__);
    s871_check(0,__LINE__);
    return 0;
}

static int skio_file_write(const void *data, unsigned int bytes, unsigned int count, void *stream)
{
    if (skio_active == 1)
        return s870_file_write(data, bytes, count, stream);
    if (skio_active == 2)
        return s871_file_write(data, bytes, count, stream);
    s870_check(0,__LINE__);
    s871_check(0,__LINE__);
    return 0;
}

static int skio_file_read(void *data, unsigned int bytes, unsigned int count, void *stream)
{
    if (skio_active == 1)
        return s870_file_read(data, bytes, count, stream);
    if (skio_active == 2)
        return s871_file_read(data, bytes, count, stream);
    s870_check(0,__LINE__);
    s871_check(0,__LINE__);
    return 0;
}

static void skio_error_begin(void *context)
{
    if (skio_active == 1)
    {
        s870_error_begin(context);
        return;
    }
    if (skio_active == 2)
    {
        s871_error_begin(context);
        return;
    }
    exit(2);
}

static int skio_error_notify(int code, int value, int zero, const char *text, int flags)
{
    if (skio_active == 1)
        return s870_error_notify(code, value, zero, text, flags);
    if (skio_active == 2)
        return s871_error_notify(code, value, zero, text, flags);
    s870_check(0,__LINE__);
    s871_check(0,__LINE__);
    return 0;
}

static void skio_error_end(void *context)
{
    if (skio_active == 1)
    {
        s870_error_end(context);
        return;
    }
    if (skio_active == 2)
    {
        s871_error_end(context);
        return;
    }
    exit(2);
}
