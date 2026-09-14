/* Retained C073 guarded matrices and allocation/close/release observers. */
static int s73o_active;
#include "../src/stream_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int s73o_checks, s73o_failures;
static void s73o_check(int ok, int line)
{
    ++s73o_checks;
    if (!ok)
    {
        ++s73o_failures;
        printf("line%d failed\n", line);
    }
}
#define S73O_CHECK(x) s73o_check(!!(x), __LINE__)
typedef struct S73O_OWNED_ROOT
{
    unsigned int before;
    OP_PACK_STREAM value;
    unsigned int after;
} S73O_OWNED_ROOT;
static S73O_OWNED_ROOT s73o_roots[4], s73o_expected_roots[4];
static unsigned int s73o_files[4];
static void *s73o_source_file, *s73o_captured_file, *s73o_expected_source;
static int s73o_scenario = 0, s73o_row, s73o_file_case, s73o_fail_allocate, s73o_mutate, s73o_seed;
static int s73o_allocate_calls, s73o_close_calls, s73o_release_calls;
static unsigned int s73o_wanted_size;
static OP_STREAM *s73o_selected;
static void s73o_verify(void)
{
    S73O_CHECK(memcmp(s73o_roots, s73o_expected_roots, sizeof(s73o_roots)) == 0);
}
static void *s73o_allocate(unsigned int bytes)
{
    S73O_CHECK(s73o_scenario != 2 && s73o_captured_file && !s73o_allocate_calls && bytes == s73o_wanted_size);
    if (s73o_scenario == 2 || !s73o_captured_file || s73o_allocate_calls || bytes != s73o_wanted_size)
        exit(2);
    s73o_verify();
    ++s73o_allocate_calls;
    if (s73o_mutate)
    {
        s73o_source_file = &s73o_files[3];
        s73o_expected_source = s73o_source_file;
    }
    return s73o_fail_allocate ? 0 : &s73o_roots[s73o_row].value;
}
static int s73o_close(void *file)
{
    S73O_CHECK(s73o_scenario == 1 && s73o_fail_allocate && s73o_captured_file && file == s73o_captured_file &&
               s73o_allocate_calls == 1 && !s73o_close_calls);
    if (s73o_scenario != 1 || !s73o_fail_allocate || !s73o_captured_file || file != s73o_captured_file ||
        s73o_allocate_calls != 1 || s73o_close_calls)
        exit(2);
    s73o_verify();
    ++s73o_close_calls;
    if (s73o_mutate == 2)
    {
        s73o_source_file = 0;
        s73o_expected_source = 0;
    }
    return s73o_seed & 1 ? -1 : 0;
}
static void s73o_release(void *memory)
{
    S73O_CHECK(s73o_scenario == 2 && memory == s73o_selected && !s73o_release_calls);
    if (s73o_scenario != 2 || memory != s73o_selected || s73o_release_calls)
        exit(2);
    s73o_verify();
    ++s73o_release_calls;
    if (s73o_mutate)
    {
        s73o_roots[s73o_row].value.base.file = &s73o_files[3];
        s73o_expected_roots[s73o_row].value.base.file = &s73o_files[3];
    }
    if (s73o_mutate == 2)
    {
        s73o_roots[s73o_row].value.base.codec_enabled ^= 0x1234;
        s73o_expected_roots[s73o_row].value.base.codec_enabled ^= 0x1234;
    }
}
static void s73o_run_case(void)
{
    void *result, *wanted;
    memset(s73o_roots, 0x40 + s73o_seed, sizeof(s73o_roots));
    s73o_captured_file = s73o_file_case ? &s73o_files[s73o_file_case - 1] : 0;
    s73o_source_file = s73o_captured_file;
    s73o_expected_source = s73o_source_file;
    s73o_roots[s73o_row].value.base.file = s73o_captured_file;
    memcpy(s73o_expected_roots, s73o_roots, sizeof(s73o_roots));
    s73o_allocate_calls = s73o_close_calls = s73o_release_calls = 0;
    s73o_selected = s73o_fail_allocate ? 0 : &s73o_roots[s73o_row].value.base;
    s73o_wanted_size = s73o_scenario == 1 ? sizeof(OP_PACK_STREAM) : sizeof(OP_STREAM);
    if (s73o_scenario != 2 && s73o_captured_file && !s73o_fail_allocate)
    {
        memset(&s73o_expected_roots[s73o_row].value, 0, s73o_wanted_size);
        s73o_expected_roots[s73o_row].value.base.file = s73o_captured_file;
        s73o_expected_roots[s73o_row].value.base.bit_mask = 0x80;
        s73o_expected_roots[s73o_row].value.base.staging_offset = 1;
        if (s73o_scenario == 1)
        {
            s73o_expected_roots[s73o_row].value.base.packing = 1;
            s73o_expected_roots[s73o_row].value.enabled = 1;
            s73o_expected_roots[s73o_row].value.window_count = 0x1000;
        }
    }
    /* Allocation/close observers see the untouched pre-initialization snapshot. */
    if (s73o_scenario != 2 && s73o_captured_file && !s73o_fail_allocate)
    {
        S73O_OWNED_ROOT completed;
        memcpy(&completed, &s73o_expected_roots[s73o_row], sizeof(completed));
        memcpy(&s73o_expected_roots[s73o_row], &s73o_roots[s73o_row], sizeof(completed));
        result = op_stream_open(s73o_source_file);
        memcpy(&s73o_expected_roots[s73o_row], &completed, sizeof(completed));
    }
    else
    {
        result = op_stream_open(s73o_source_file);
    }
    wanted = s73o_scenario == 2 ? (s73o_selected ? s73o_captured_file : 0)
                                : (s73o_captured_file && !s73o_fail_allocate ? (void *)&s73o_roots[s73o_row].value : 0);
    S73O_CHECK(result == wanted);
    s73o_verify();
    S73O_CHECK(s73o_source_file == s73o_expected_source);
    S73O_CHECK(s73o_allocate_calls == (s73o_scenario != 2 && s73o_captured_file != 0));
    S73O_CHECK(s73o_close_calls == (s73o_scenario == 1 && s73o_captured_file != 0 && s73o_fail_allocate));
    S73O_CHECK(s73o_release_calls == (s73o_scenario == 2 && s73o_selected != 0));
}
static int op_test_stream_open(void)
{
    s73o_active = 1;
    for (s73o_seed = 0; s73o_seed < 8; ++s73o_seed)
        for (s73o_row = 0; s73o_row < 4; ++s73o_row)
            for (s73o_file_case = 0; s73o_file_case < 4; ++s73o_file_case)
                for (s73o_fail_allocate = 0; s73o_fail_allocate < 2; ++s73o_fail_allocate)
                    for (s73o_mutate = 0; s73o_mutate < 3; ++s73o_mutate)
                        s73o_run_case();
    printf("stream_open: %d checks, %d failures\n", s73o_checks, s73o_failures);
    s73o_active = 0;
    return s73o_failures != 0;
}

#undef S73O_CHECK

static int s73p_active;
#include "../src/stream_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int s73p_checks, s73p_failures;
static void s73p_check(int ok, int line)
{
    ++s73p_checks;
    if (!ok)
    {
        ++s73p_failures;
        printf("line%d failed\n", line);
    }
}
#define S73P_CHECK(x) s73p_check(!!(x), __LINE__)
typedef struct S73P_OWNED_ROOT
{
    unsigned int before;
    OP_PACK_STREAM value;
    unsigned int after;
} S73P_OWNED_ROOT;
static S73P_OWNED_ROOT s73p_roots[4], s73p_expected_roots[4];
static unsigned int s73p_files[4];
static void *s73p_source_file, *s73p_captured_file, *s73p_expected_source;
static int s73p_scenario = 1, s73p_row, s73p_file_case, s73p_fail_allocate, s73p_mutate, s73p_seed;
static int s73p_allocate_calls, s73p_close_calls, s73p_release_calls;
static unsigned int s73p_wanted_size;
static OP_STREAM *s73p_selected;
static void s73p_verify(void)
{
    S73P_CHECK(memcmp(s73p_roots, s73p_expected_roots, sizeof(s73p_roots)) == 0);
}
static void *s73p_allocate(unsigned int bytes)
{
    S73P_CHECK(s73p_scenario != 2 && s73p_captured_file && !s73p_allocate_calls && bytes == s73p_wanted_size);
    if (s73p_scenario == 2 || !s73p_captured_file || s73p_allocate_calls || bytes != s73p_wanted_size)
        exit(2);
    s73p_verify();
    ++s73p_allocate_calls;
    if (s73p_mutate)
    {
        s73p_source_file = &s73p_files[3];
        s73p_expected_source = s73p_source_file;
    }
    return s73p_fail_allocate ? 0 : &s73p_roots[s73p_row].value;
}
static int s73p_close(void *file)
{
    S73P_CHECK(s73p_scenario == 1 && s73p_fail_allocate && s73p_captured_file && file == s73p_captured_file &&
               s73p_allocate_calls == 1 && !s73p_close_calls);
    if (s73p_scenario != 1 || !s73p_fail_allocate || !s73p_captured_file || file != s73p_captured_file ||
        s73p_allocate_calls != 1 || s73p_close_calls)
        exit(2);
    s73p_verify();
    ++s73p_close_calls;
    if (s73p_mutate == 2)
    {
        s73p_source_file = 0;
        s73p_expected_source = 0;
    }
    return s73p_seed & 1 ? -1 : 0;
}
static void s73p_release(void *memory)
{
    S73P_CHECK(s73p_scenario == 2 && memory == s73p_selected && !s73p_release_calls);
    if (s73p_scenario != 2 || memory != s73p_selected || s73p_release_calls)
        exit(2);
    s73p_verify();
    ++s73p_release_calls;
    if (s73p_mutate)
    {
        s73p_roots[s73p_row].value.base.file = &s73p_files[3];
        s73p_expected_roots[s73p_row].value.base.file = &s73p_files[3];
    }
    if (s73p_mutate == 2)
    {
        s73p_roots[s73p_row].value.base.codec_enabled ^= 0x1234;
        s73p_expected_roots[s73p_row].value.base.codec_enabled ^= 0x1234;
    }
}
static void s73p_run_case(void)
{
    void *result, *wanted;
    memset(s73p_roots, 0x40 + s73p_seed, sizeof(s73p_roots));
    s73p_captured_file = s73p_file_case ? &s73p_files[s73p_file_case - 1] : 0;
    s73p_source_file = s73p_captured_file;
    s73p_expected_source = s73p_source_file;
    s73p_roots[s73p_row].value.base.file = s73p_captured_file;
    memcpy(s73p_expected_roots, s73p_roots, sizeof(s73p_roots));
    s73p_allocate_calls = s73p_close_calls = s73p_release_calls = 0;
    s73p_selected = s73p_fail_allocate ? 0 : &s73p_roots[s73p_row].value.base;
    s73p_wanted_size = s73p_scenario == 1 ? sizeof(OP_PACK_STREAM) : sizeof(OP_STREAM);
    if (s73p_scenario != 2 && s73p_captured_file && !s73p_fail_allocate)
    {
        memset(&s73p_expected_roots[s73p_row].value, 0, s73p_wanted_size);
        s73p_expected_roots[s73p_row].value.base.file = s73p_captured_file;
        s73p_expected_roots[s73p_row].value.base.bit_mask = 0x80;
        s73p_expected_roots[s73p_row].value.base.staging_offset = 1;
        if (s73p_scenario == 1)
        {
            s73p_expected_roots[s73p_row].value.base.packing = 1;
            s73p_expected_roots[s73p_row].value.enabled = 1;
            s73p_expected_roots[s73p_row].value.window_count = 0x1000;
        }
    }
    /* Allocation/close observers see the untouched pre-initialization snapshot. */
    if (s73p_scenario != 2 && s73p_captured_file && !s73p_fail_allocate)
    {
        S73P_OWNED_ROOT completed;
        memcpy(&completed, &s73p_expected_roots[s73p_row], sizeof(completed));
        memcpy(&s73p_expected_roots[s73p_row], &s73p_roots[s73p_row], sizeof(completed));
        result = op_pack_stream_open(s73p_source_file);
        memcpy(&s73p_expected_roots[s73p_row], &completed, sizeof(completed));
    }
    else
    {
        result = op_pack_stream_open(s73p_source_file);
    }
    wanted = s73p_scenario == 2 ? (s73p_selected ? s73p_captured_file : 0)
                                : (s73p_captured_file && !s73p_fail_allocate ? (void *)&s73p_roots[s73p_row].value : 0);
    S73P_CHECK(result == wanted);
    s73p_verify();
    S73P_CHECK(s73p_source_file == s73p_expected_source);
    S73P_CHECK(s73p_allocate_calls == (s73p_scenario != 2 && s73p_captured_file != 0));
    S73P_CHECK(s73p_close_calls == (s73p_scenario == 1 && s73p_captured_file != 0 && s73p_fail_allocate));
    S73P_CHECK(s73p_release_calls == (s73p_scenario == 2 && s73p_selected != 0));
}
static int op_test_pack_stream_open(void)
{
    s73p_active = 1;
    for (s73p_seed = 0; s73p_seed < 8; ++s73p_seed)
        for (s73p_row = 0; s73p_row < 4; ++s73p_row)
            for (s73p_file_case = 0; s73p_file_case < 4; ++s73p_file_case)
                for (s73p_fail_allocate = 0; s73p_fail_allocate < 2; ++s73p_fail_allocate)
                    for (s73p_mutate = 0; s73p_mutate < 3; ++s73p_mutate)
                        s73p_run_case();
    printf("pack_stream_open: %d checks, %d failures\n", s73p_checks, s73p_failures);
    s73p_active = 0;
    return s73p_failures != 0;
}

#undef S73P_CHECK

static int s73c_active;
#include "../src/stream_helpers.h"
#include <stdio.h>
#include <stdlib.h>
static int s73c_checks, s73c_failures;
static void s73c_check(int ok, int line)
{
    ++s73c_checks;
    if (!ok)
    {
        ++s73c_failures;
        printf("line%d failed\n", line);
    }
}
#define S73C_CHECK(x) s73c_check(!!(x), __LINE__)
typedef struct S73C_OWNED_ROOT
{
    unsigned int before;
    OP_PACK_STREAM value;
    unsigned int after;
} S73C_OWNED_ROOT;
static S73C_OWNED_ROOT s73c_roots[4], s73c_expected_roots[4];
static unsigned int s73c_files[4];
static void *s73c_source_file, *s73c_captured_file, *s73c_expected_source;
static int s73c_scenario = 2, s73c_row, s73c_file_case, s73c_fail_allocate, s73c_mutate, s73c_seed;
static int s73c_allocate_calls, s73c_close_calls, s73c_release_calls;
static unsigned int s73c_wanted_size;
static OP_STREAM *s73c_selected;
static void s73c_verify(void)
{
    S73C_CHECK(memcmp(s73c_roots, s73c_expected_roots, sizeof(s73c_roots)) == 0);
}
static void *s73c_allocate(unsigned int bytes)
{
    S73C_CHECK(s73c_scenario != 2 && s73c_captured_file && !s73c_allocate_calls && bytes == s73c_wanted_size);
    if (s73c_scenario == 2 || !s73c_captured_file || s73c_allocate_calls || bytes != s73c_wanted_size)
        exit(2);
    s73c_verify();
    ++s73c_allocate_calls;
    if (s73c_mutate)
    {
        s73c_source_file = &s73c_files[3];
        s73c_expected_source = s73c_source_file;
    }
    return s73c_fail_allocate ? 0 : &s73c_roots[s73c_row].value;
}
static int s73c_close(void *file)
{
    S73C_CHECK(s73c_scenario == 1 && s73c_fail_allocate && s73c_captured_file && file == s73c_captured_file &&
               s73c_allocate_calls == 1 && !s73c_close_calls);
    if (s73c_scenario != 1 || !s73c_fail_allocate || !s73c_captured_file || file != s73c_captured_file ||
        s73c_allocate_calls != 1 || s73c_close_calls)
        exit(2);
    s73c_verify();
    ++s73c_close_calls;
    if (s73c_mutate == 2)
    {
        s73c_source_file = 0;
        s73c_expected_source = 0;
    }
    return s73c_seed & 1 ? -1 : 0;
}
static void s73c_release(void *memory)
{
    S73C_CHECK(s73c_scenario == 2 && memory == s73c_selected && !s73c_release_calls);
    if (s73c_scenario != 2 || memory != s73c_selected || s73c_release_calls)
        exit(2);
    s73c_verify();
    ++s73c_release_calls;
    if (s73c_mutate)
    {
        s73c_roots[s73c_row].value.base.file = &s73c_files[3];
        s73c_expected_roots[s73c_row].value.base.file = &s73c_files[3];
    }
    if (s73c_mutate == 2)
    {
        s73c_roots[s73c_row].value.base.codec_enabled ^= 0x1234;
        s73c_expected_roots[s73c_row].value.base.codec_enabled ^= 0x1234;
    }
}
static void s73c_run_case(void)
{
    void *result, *wanted;
    memset(s73c_roots, 0x40 + s73c_seed, sizeof(s73c_roots));
    s73c_captured_file = s73c_file_case ? &s73c_files[s73c_file_case - 1] : 0;
    s73c_source_file = s73c_captured_file;
    s73c_expected_source = s73c_source_file;
    s73c_roots[s73c_row].value.base.file = s73c_captured_file;
    memcpy(s73c_expected_roots, s73c_roots, sizeof(s73c_roots));
    s73c_allocate_calls = s73c_close_calls = s73c_release_calls = 0;
    s73c_selected = s73c_fail_allocate ? 0 : &s73c_roots[s73c_row].value.base;
    s73c_wanted_size = s73c_scenario == 1 ? sizeof(OP_PACK_STREAM) : sizeof(OP_STREAM);
    if (s73c_scenario != 2 && s73c_captured_file && !s73c_fail_allocate)
    {
        memset(&s73c_expected_roots[s73c_row].value, 0, s73c_wanted_size);
        s73c_expected_roots[s73c_row].value.base.file = s73c_captured_file;
        s73c_expected_roots[s73c_row].value.base.bit_mask = 0x80;
        s73c_expected_roots[s73c_row].value.base.staging_offset = 1;
        if (s73c_scenario == 1)
        {
            s73c_expected_roots[s73c_row].value.base.packing = 1;
            s73c_expected_roots[s73c_row].value.enabled = 1;
            s73c_expected_roots[s73c_row].value.window_count = 0x1000;
        }
    }
    /* Allocation/close observers see the untouched pre-initialization snapshot. */
    if (s73c_scenario != 2 && s73c_captured_file && !s73c_fail_allocate)
    {
        S73C_OWNED_ROOT completed;
        memcpy(&completed, &s73c_expected_roots[s73c_row], sizeof(completed));
        memcpy(&s73c_expected_roots[s73c_row], &s73c_roots[s73c_row], sizeof(completed));
        result = op_stream_close(s73c_selected);
        memcpy(&s73c_expected_roots[s73c_row], &completed, sizeof(completed));
    }
    else
    {
        result = op_stream_close(s73c_selected);
    }
    wanted = s73c_scenario == 2 ? (s73c_selected ? s73c_captured_file : 0)
                                : (s73c_captured_file && !s73c_fail_allocate ? (void *)&s73c_roots[s73c_row].value : 0);
    S73C_CHECK(result == wanted);
    s73c_verify();
    S73C_CHECK(s73c_source_file == s73c_expected_source);
    S73C_CHECK(s73c_allocate_calls == (s73c_scenario != 2 && s73c_captured_file != 0));
    S73C_CHECK(s73c_close_calls == (s73c_scenario == 1 && s73c_captured_file != 0 && s73c_fail_allocate));
    S73C_CHECK(s73c_release_calls == (s73c_scenario == 2 && s73c_selected != 0));
}
static int op_test_stream_close(void)
{
    s73c_active = 1;
    for (s73c_seed = 0; s73c_seed < 8; ++s73c_seed)
        for (s73c_row = 0; s73c_row < 4; ++s73c_row)
            for (s73c_file_case = 0; s73c_file_case < 4; ++s73c_file_case)
                for (s73c_fail_allocate = 0; s73c_fail_allocate < 2; ++s73c_fail_allocate)
                    for (s73c_mutate = 0; s73c_mutate < 3; ++s73c_mutate)
                        s73c_run_case();
    printf("stream_close: %d checks, %d failures\n", s73c_checks, s73c_failures);
    s73c_active = 0;
    return s73c_failures != 0;
}

#undef S73C_CHECK
