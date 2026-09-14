#include "api.h"
#include <stdio.h>
#include <stdlib.h>
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
typedef struct OWNED_ROOT
{
    unsigned int before;
    OP_PACK_STREAM value;
    unsigned int after;
} OWNED_ROOT;
static OWNED_ROOT roots[4], expected_roots[4];
static unsigned int files[4];
static void *source_file, *captured_file, *expected_source;
static int scenario = 1, row, file_case, fail_allocate, mutate, seed;
static int allocate_calls, close_calls, release_calls;
static unsigned int wanted_size;
static OP_STREAM *selected;
static void verify(void)
{
    CHECK(memcmp(roots, expected_roots, sizeof(roots)) == 0);
}
void *op_allocate(unsigned int bytes)
{
    CHECK(scenario != 2 && captured_file && !allocate_calls && bytes == wanted_size);
    if (scenario == 2 || !captured_file || allocate_calls || bytes != wanted_size)
        exit(2);
    verify();
    ++allocate_calls;
    if (mutate)
    {
        source_file = &files[3];
        expected_source = source_file;
    }
    return fail_allocate ? 0 : &roots[row].value;
}
int op_close_diagnostic_stream(void *file)
{
    CHECK(scenario == 1 && fail_allocate && captured_file && file == captured_file && allocate_calls == 1 &&
          !close_calls);
    if (scenario != 1 || !fail_allocate || !captured_file || file != captured_file || allocate_calls != 1 ||
        close_calls)
        exit(2);
    verify();
    ++close_calls;
    if (mutate == 2)
    {
        source_file = 0;
        expected_source = 0;
    }
    return seed & 1 ? -1 : 0;
}
void op_release(void *memory)
{
    CHECK(scenario == 2 && memory == selected && !release_calls);
    if (scenario != 2 || memory != selected || release_calls)
        exit(2);
    verify();
    ++release_calls;
    if (mutate)
    {
        roots[row].value.base.file = &files[3];
        expected_roots[row].value.base.file = &files[3];
    }
    if (mutate == 2)
    {
        roots[row].value.base.codec_enabled ^= 0x1234;
        expected_roots[row].value.base.codec_enabled ^= 0x1234;
    }
}
static void run_case(void)
{
    void *result, *wanted;
    memset(roots, 0x40 + seed, sizeof(roots));
    captured_file = file_case ? &files[file_case - 1] : 0;
    source_file = captured_file;
    expected_source = source_file;
    roots[row].value.base.file = captured_file;
    memcpy(expected_roots, roots, sizeof(roots));
    allocate_calls = close_calls = release_calls = 0;
    selected = fail_allocate ? 0 : &roots[row].value.base;
    wanted_size = scenario == 1 ? sizeof(OP_PACK_STREAM) : sizeof(OP_STREAM);
    if (scenario != 2 && captured_file && !fail_allocate)
    {
        memset(&expected_roots[row].value, 0, wanted_size);
        expected_roots[row].value.base.file = captured_file;
        expected_roots[row].value.base.bit_mask = 0x80;
        expected_roots[row].value.base.staging_offset = 1;
        if (scenario == 1)
        {
            expected_roots[row].value.base.packing = 1;
            expected_roots[row].value.enabled = 1;
            expected_roots[row].value.window_count = 0x1000;
        }
    }
    /* Allocation/close observers see the untouched pre-initialization snapshot. */
    if (scenario != 2 && captured_file && !fail_allocate)
    {
        OWNED_ROOT completed;
        memcpy(&completed, &expected_roots[row], sizeof(completed));
        memcpy(&expected_roots[row], &roots[row], sizeof(completed));
        result = op_pack_stream_open(source_file);
        memcpy(&expected_roots[row], &completed, sizeof(completed));
    }
    else
    {
        result = op_pack_stream_open(source_file);
    }
    wanted = scenario == 2 ? (selected ? captured_file : 0)
                           : (captured_file && !fail_allocate ? (void *)&roots[row].value : 0);
    CHECK(result == wanted);
    verify();
    CHECK(source_file == expected_source);
    CHECK(allocate_calls == (scenario != 2 && captured_file != 0));
    CHECK(close_calls == (scenario == 1 && captured_file != 0 && fail_allocate));
    CHECK(release_calls == (scenario == 2 && selected != 0));
}
int main(void)
{
    for (seed = 0; seed < 8; ++seed)
        for (row = 0; row < 4; ++row)
            for (file_case = 0; file_case < 4; ++file_case)
                for (fail_allocate = 0; fail_allocate < 2; ++fail_allocate)
                    for (mutate = 0; mutate < 3; ++mutate)
                        run_case();
    printf("pack_stream_open: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
