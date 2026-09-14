#include "api.h"
#include <stdio.h>
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
static ROOT root, expected;
static const unsigned char masks[] = {0x80, 0x40, 0x20, 0x10, 8, 4, 2, 1, 0};
static int count, mask_case, available, position, seed, high_bits;
static void run_case(void)
{
    unsigned int result, wanted = 0;
    int i, exhausted = 0;
    memset(&root, 0x63, sizeof(root));
    root.value.bit_mask = masks[mask_case];
    root.value.bit_accumulator = (unsigned int)(seed * 37 + 0x51) | (high_bits ? 0xffffff00u : 0);
    root.value.buffer_position = position;
    root.value.buffer_limit = available;
    for (i = 0; i < 16; ++i)
        root.value.output[i] = (unsigned char)(seed * 41 + i * 17);
    memcpy(&expected, &root, sizeof(root));
    for (i = 0; i < count; ++i)
    {
        if (expected.value.bit_mask == 0x80)
        {
            if (expected.value.buffer_position >= expected.value.buffer_limit)
            {
                exhausted = 1;
                break;
            }
            expected.value.bit_accumulator = expected.value.output[expected.value.buffer_position++];
        }
        wanted = (wanted << 1) | ((expected.value.bit_accumulator & expected.value.bit_mask) != 0);
        expected.value.bit_mask = (unsigned char)(expected.value.bit_mask / 2);
        if (!expected.value.bit_mask)
            expected.value.bit_mask = 0x80;
    }
    if (exhausted)
        wanted = 0;
    result = op_stream_bits(&root.value, count);
    CHECK(result == wanted);
    CHECK(memcmp(&root, &expected, sizeof(root)) == 0);
}
int main(void)
{
    for (count = 1; count <= 32; ++count)
        for (mask_case = 0; mask_case < 9; ++mask_case)
            for (available = 0; available <= 6; ++available)
                for (position = 0; position <= 6; ++position)
                    for (seed = 0; seed < 8; ++seed)
                        for (high_bits = 0; high_bits < 2; ++high_bits)
                            run_case();
    printf("stream_bits: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
