#include "../src/save_stream.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int sv_checks, sv_failures;
static void sv_check(int ok, int line)
{
    ++sv_checks;
    if (!ok)
    {
        ++sv_failures;
        printf("line%d failed\n", line);
    }
}
#define SV_CHECK(x) sv_check(!!(x), __LINE__)
typedef struct SV_OWNED_BUFFER
{
    unsigned int before;
    unsigned char data[64];
    unsigned int after;
} SV_OWNED_BUFFER;
static SV_OWNED_BUFFER sv_buffers[3], sv_expected_buffers[3];
static unsigned int sv_stream_tokens[3];
void *op_save_output_stream = &sv_stream_tokens[1], *op_save_input_stream = &sv_stream_tokens[0];
unsigned char op_save_error_context[116];
const char op_save_failure_text[] = "authored-write-failure";
static unsigned char sv_expected_context[116];

static void *sv_expected_output, *sv_expected_input;
static const int sv_answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int sv_sizes[] = {0, 1, 8, 63, 0xffffffffu};
static int sv_row, sv_answer, sv_size_case, sv_stream_case, sv_mutate, sv_stage;
static void sv_seed(void)
{
    memset(sv_buffers, 0x57, sizeof(sv_buffers));
    memcpy(sv_expected_buffers, sv_buffers, sizeof(sv_buffers));
    memset(op_save_error_context, 0x63, sizeof(op_save_error_context));
    memcpy(sv_expected_context, op_save_error_context, sizeof(sv_expected_context));
    op_save_input_stream = sv_expected_input = sv_stream_case ? &sv_stream_tokens[sv_stream_case - 1] : 0;
    op_save_output_stream = sv_expected_output = sv_stream_case ? &sv_stream_tokens[2 - sv_stream_case] : 0;
    sv_stage = 0;
}
static void sv_verify(void)
{
    SV_CHECK(memcmp(sv_buffers, sv_expected_buffers, sizeof(sv_buffers)) == 0);
    SV_CHECK(memcmp(op_save_error_context, sv_expected_context, sizeof(sv_expected_context)) == 0);
    SV_CHECK(op_save_input_stream == sv_expected_input && op_save_output_stream == sv_expected_output);
}
static void sv_change(void)
{
    if (sv_mutate)
    {
        sv_buffers[sv_row].data[3] = sv_expected_buffers[sv_row].data[3] = 0x29;
        op_save_output_stream = sv_expected_output = &sv_stream_tokens[2];
        op_save_input_stream = sv_expected_input = &sv_stream_tokens[0];
        op_save_error_context[9] = sv_expected_context[9] = 0x35;
    }
}

static int sv_active, sv_mode, sv_header_answer, sv_kind_case, sv_context_case, sv_origin_case;
static int sv_legacy_reads, sv_legacy_writes;
static unsigned char sv_legacy_context[116];
static const unsigned short sv_kinds[] = {0, 1, 0x7fff, 0x8000, 0xffff};
static const int sv_contexts[] = {0, -1, INT_MIN, INT_MAX};
static const long sv_offsets[] = {LONG_MIN, -1, 0, 1, LONG_MAX};
static const int sv_origins[] = {-7, 0, 1, 2, INT_MAX};
static void sv_legacy_write(const void *, unsigned int);
static int sv_legacy_read(void *, unsigned int);
static void sv_check_legacy(void)
{
    SV_CHECK(op_save_output_stream == &sv_stream_tokens[1] && op_save_input_stream == &sv_stream_tokens[0]);
    SV_CHECK(memcmp(op_save_error_context, sv_legacy_context, sizeof(sv_legacy_context)) == 0);
}
int op_save_file_write(const void *data, unsigned int bytes, unsigned int count, void *stream)
{
    if (!sv_active)
    {
        sv_check_legacy();
        SV_CHECK(count == 1 && stream == &sv_stream_tokens[1]);
        sv_legacy_write(data, bytes);
        ++sv_legacy_writes;
        sv_check_legacy();
        return 1;
    }
    SV_CHECK((sv_mode == 0 || sv_mode == 1 || sv_mode == 4) && sv_stage == (sv_mode == 4 ? 1 : 0) &&
             data == sv_buffers[sv_row].data && bytes == sv_sizes[sv_size_case] && count == 1 &&
             stream == sv_expected_output);
    sv_verify();
    sv_change();
    ++sv_stage;
    return sv_answers[sv_answer];
}
int op_save_file_read(void *data, unsigned int bytes, unsigned int count, void *stream)
{
    int result;
    if (!sv_active)
    {
        sv_check_legacy();
        SV_CHECK(count == 1 && stream == &sv_stream_tokens[0]);
        result = sv_legacy_read(data, bytes);
        ++sv_legacy_reads;
        sv_check_legacy();
        return result;
    }
    SV_CHECK((sv_mode == 2 || sv_mode == 3) && sv_stage == 0 && data == sv_buffers[sv_row].data &&
             bytes == sv_sizes[sv_size_case] && count == 1 && stream == sv_expected_input);
    sv_verify();
    sv_change();
    ++sv_stage;
    return sv_answers[sv_answer];
}
static int sv_seek(void *stream, long offset, int origin)
{
    SV_CHECK(sv_active && sv_mode == 5 && sv_stage == 0 && stream == sv_expected_input &&
             offset == sv_offsets[sv_size_case] && origin == sv_origins[sv_origin_case]);
    sv_verify();
    sv_change();
    ++sv_stage;
    return sv_answers[sv_answer];
}
static int sv_header(int context, int bytes, unsigned short kind)
{
    SV_CHECK(sv_active && sv_mode == 4 && sv_stage == 0 && context == sv_contexts[sv_context_case] &&
             (unsigned int)bytes == sv_sizes[sv_size_case] && kind == sv_kinds[sv_kind_case]);
    sv_verify();
    sv_change();
    ++sv_stage;
    return sv_answers[sv_header_answer];
}
void op_save_error_begin(void *context)
{
    SV_CHECK(sv_active && sv_answers[sv_answer] == 0 && sv_stage == (sv_mode == 4 ? 2 : 1) &&
             context == op_save_error_context);
    sv_verify();
    op_save_error_context[48] = sv_expected_context[48] = 0x41;
    ++sv_stage;
}
int op_save_error_notify(int code, int value, int zero, const char *text, int flags)
{
    SV_CHECK(sv_active && sv_stage == (sv_mode == 4 ? 3 : 2) && code == 17 && value == -1 && zero == 0 &&
             text == op_save_failure_text && flags == 3840);
    sv_verify();
    op_save_error_context[84] = sv_expected_context[84] = 0x72;
    sv_buffers[sv_row].data[8] = sv_expected_buffers[sv_row].data[8] = 0x28;
    ++sv_stage;
    return sv_mutate ? INT_MIN : INT_MAX;
}
void op_save_error_end(void *context)
{
    SV_CHECK(sv_active && sv_stage == (sv_mode == 4 ? 4 : 3) && context == op_save_error_context);
    sv_verify();
    op_save_error_context[107] = sv_expected_context[107] = 0x23;
    ++sv_stage;
}
static int op_test_save_stream(void)
{
    int result, expected_stage;
    sv_active = 1;
    for (sv_mode = 0; sv_mode < 4; ++sv_mode)
        for (sv_row = 0; sv_row < 3; ++sv_row)
            for (sv_answer = 0; sv_answer < 5; ++sv_answer)
                for (sv_size_case = 0; sv_size_case < 5; ++sv_size_case)
                    for (sv_stream_case = 0; sv_stream_case < 3; ++sv_stream_case)
                        for (sv_mutate = 0; sv_mutate < 2; ++sv_mutate)
                        {
                            sv_seed();
                            if (sv_mode == 0)
                                result = op_save_write_raw(sv_buffers[sv_row].data, sv_sizes[sv_size_case]);
                            else if (sv_mode == 1)
                                result = op_save_write(sv_buffers[sv_row].data, sv_sizes[sv_size_case]);
                            else if (sv_mode == 2)
                                result = op_save_read_raw(sv_buffers[sv_row].data, sv_sizes[sv_size_case]);
                            else
                                result = op_save_read(sv_buffers[sv_row].data, sv_sizes[sv_size_case]);
                            expected_stage = sv_mode < 2 && sv_answers[sv_answer] == 0 ? 4 : 1;
                            SV_CHECK(result == sv_answers[sv_answer] && sv_stage == expected_stage);
                            sv_verify();
                        }
    sv_mode = 4;
    for (sv_row = 0; sv_row < 3; ++sv_row)
        for (sv_answer = 0; sv_answer < 5; ++sv_answer)
            for (sv_size_case = 0; sv_size_case < 5; ++sv_size_case)
                for (sv_stream_case = 0; sv_stream_case < 3; ++sv_stream_case)
                    for (sv_mutate = 0; sv_mutate < 2; ++sv_mutate)
                        for (sv_header_answer = 0; sv_header_answer < 5; ++sv_header_answer)
                            for (sv_kind_case = 0; sv_kind_case < 5; ++sv_kind_case)
                                for (sv_context_case = 0; sv_context_case < 4; ++sv_context_case)
                                {
                                    sv_seed();
                                    result = op_save_write_chunk(sv_contexts[sv_context_case], sv_buffers[sv_row].data,
                                                                 sv_sizes[sv_size_case], sv_kinds[sv_kind_case]);
                                    expected_stage = sv_answers[sv_header_answer] == 1 ? 1
                                                     : sv_answers[sv_answer] == 0      ? 5
                                                                                       : 2;
                                    SV_CHECK(result ==
                                                 ((sv_answers[sv_header_answer] == 1 || sv_answers[sv_answer] == 0)
                                                      ? 1
                                                      : 0) &&
                                             sv_stage == expected_stage);
                                    sv_verify();
                                }
    sv_mode = 5;
    for (sv_row = 0; sv_row < 3; ++sv_row)
        for (sv_answer = 0; sv_answer < 5; ++sv_answer)
            for (sv_size_case = 0; sv_size_case < 5; ++sv_size_case)
                for (sv_stream_case = 0; sv_stream_case < 3; ++sv_stream_case)
                    for (sv_mutate = 0; sv_mutate < 2; ++sv_mutate)
                        for (sv_origin_case = 0; sv_origin_case < 5; ++sv_origin_case)
                        {
                            sv_seed();
                            SV_CHECK(op_save_seek_restore(sv_offsets[sv_size_case], sv_origins[sv_origin_case]) ==
                                         sv_answers[sv_answer] &&
                                     sv_stage == 1);
                            sv_verify();
                        }
    SV_CHECK(sv_legacy_reads > 0 && sv_legacy_writes > 0);
    sv_active = 0;
    op_save_input_stream = &sv_stream_tokens[0];
    op_save_output_stream = &sv_stream_tokens[1];
    memcpy(op_save_error_context, sv_legacy_context, sizeof(sv_legacy_context));
    sv_check_legacy();
    printf("save stream connected: %d checks, %d failures; legacy read/write calls %d/%d\n", sv_checks, sv_failures,
           sv_legacy_reads, sv_legacy_writes);
    return sv_failures != 0;
}
#undef SV_CHECK
