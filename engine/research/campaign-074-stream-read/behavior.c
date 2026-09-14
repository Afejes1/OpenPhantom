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
typedef struct ROOT
{
    unsigned int before;
    OP_STREAM value;
    unsigned int after;
} ROOT;
typedef struct OUTPUT
{
    unsigned int before;
    unsigned char bytes[128];
    unsigned int after;
} OUTPUT;
static ROOT root, expected, callback_before[64];
static OUTPUT output, expected_output, output_before[64];
static unsigned int handles[3];
static int codec, size_case, count_case, initial, plan, mutation, seed, alignment, return_case;
static int calls, expected_calls, direct_calls, consumed, total_requested;
static const int returns[] = {-7, 0, 1, 5, 0x7fffffff};
static const int initial_sizes[] = {0, 1, 3, 8};
static int chunk_size(int n)
{
    if (plan == 0 || n > 7)
        return 0;
    if (plan == 1)
        return 1;
    if (plan == 2)
        return n == 0 ? 3 : 0;
    return (n % 3) + 2;
}
static void apply_refill(ROOT *state, int n)
{
    int i, length = chunk_size(n);
    state->value.stage_count = 0;
    state->value.stage_remaining = length;
    for (i = 0; i < length; ++i)
        state->value.staging[i] = (unsigned char)(seed + 17 * n + i + 1);
    if (mutation)
    {
        state->value.codec_enabled = 0;
        state->value.file = &handles[2];
    }
    if (mutation == 2)
        state->value.stage_mode ^= 0x5a;
}
void op_stream_refill(OP_STREAM *stream)
{
    CHECK(stream == &root.value && calls < expected_calls);
    if (stream != &root.value || calls >= expected_calls)
        exit(2);
    CHECK(memcmp(&root, &callback_before[calls], sizeof(root)) == 0);
    CHECK(memcmp(&output, &output_before[calls], sizeof(output)) == 0);
    apply_refill(&root, calls);
    ++calls;
}
int op_save_file_read(void *buffer, unsigned int size, unsigned int count, void *file)
{
    int i, n;
    CHECK(!codec && !direct_calls && buffer == output.bytes + alignment && size == (unsigned int)size_case &&
          count == (unsigned int)count_case && file == &handles[0]);
    if (codec || direct_calls || buffer != output.bytes + alignment || size != (unsigned int)size_case ||
        count != (unsigned int)count_case || file != &handles[0])
        exit(2);
    CHECK(memcmp(&root, &callback_before[0], sizeof(root)) == 0);
    CHECK(memcmp(&output, &output_before[0], sizeof(output)) == 0);
    ++direct_calls;
    n = size_case * count_case;
    if (n > 8)
        n = 8;
    for (i = 0; i < n; ++i)
        output.bytes[alignment + i] = (unsigned char)(seed + i + 3);
    if (mutation)
    {
        root.value.file = &handles[2];
        root.value.stage_mode = 0x1357;
    }
    return returns[return_case];
}
static void run_case(void)
{
    int i, result, wanted;
    memset(&root, 0x54 + seed, sizeof(root));
    memset(&output, 0x78, sizeof(output));
    root.value.file = &handles[0];
    root.value.codec_enabled = codec;
    root.value.stage_remaining = initial_sizes[initial];
    root.value.stage_count = initial == 3 ? 2 : 0;
    for (i = 0; i < 16; ++i)
        root.value.staging[i] = (unsigned char)(seed + i + 1);
    memcpy(&expected, &root, sizeof(root));
    memcpy(&expected_output, &output, sizeof(output));
    calls = expected_calls = direct_calls = consumed = 0;
    total_requested = size_case * count_case;
    if (codec)
    {
        while (consumed < total_requested)
        {
            if (expected.value.stage_count >= expected.value.stage_remaining)
            {
                CHECK(expected_calls < 64);
                memcpy(&callback_before[expected_calls], &expected, sizeof(expected));
                memcpy(&output_before[expected_calls], &expected_output, sizeof(output));
                apply_refill(&expected, expected_calls);
                ++expected_calls;
                if (expected.value.stage_remaining == 0)
                    break;
            }
            expected_output.bytes[alignment + consumed] = expected.value.staging[expected.value.stage_count];
            ++expected.value.stage_count;
            ++consumed;
        }
        wanted = consumed / size_case;
    }
    else
    {
        memcpy(&callback_before[0], &root, sizeof(root));
        memcpy(&output_before[0], &output, sizeof(output));
        i = total_requested;
        if (i > 8)
            i = 8;
        while (i > 0)
        {
            --i;
            expected_output.bytes[alignment + i] = (unsigned char)(seed + i + 3);
        }
        if (mutation)
        {
            expected.value.file = &handles[2];
            expected.value.stage_mode = 0x1357;
        }
        wanted = returns[return_case];
    }
    result = op_stream_read(output.bytes + alignment, size_case, count_case, &root.value);
    CHECK(result == wanted);
    CHECK(calls == expected_calls && direct_calls == !codec);
    CHECK(memcmp(&root, &expected, sizeof(root)) == 0);
    CHECK(memcmp(&output, &expected_output, sizeof(output)) == 0);
}
int main(void)
{
    for (codec = 0; codec < 2; ++codec)
        for (size_case = 1; size_case <= 4; ++size_case)
            for (count_case = 0; count_case < 8; ++count_case)
                for (initial = 0; initial < 4; ++initial)
                    for (plan = 0; plan < 4; ++plan)
                        for (mutation = 0; mutation < 3; ++mutation)
                            for (seed = 0; seed < 2; ++seed)
                                for (alignment = 0; alignment < 4; ++alignment)
                                    for (return_case = 0; return_case < (codec ? 1 : 5); ++return_case)
                                        run_case();
    printf("stream_read: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
