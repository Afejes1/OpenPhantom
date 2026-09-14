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

typedef struct OWNED
{
    unsigned int before;
    char data[72];
    unsigned int after;
} OWNED;
static OWNED input, destinations[2], expected_input, expected_destinations[2];
static unsigned int length, source_offset, destination_offset;
static int allocation_case, mutation, calls;
static void verify(void)
{
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
    CHECK(memcmp(destinations, expected_destinations, sizeof(destinations)) == 0);
}
void *op_allocate(unsigned int size)
{
    unsigned int i, new_length;
    int slot;
    CHECK(calls == 0);
    ++calls;
    CHECK(size == length + 1);
    verify();
    new_length = mutation == 1 ? length / 2 : length;
    for (i = 0; i < new_length; ++i)
    {
        if (mutation)
        {
            input.data[source_offset + i] = (char)('k' + i % 11);
            expected_input.data[source_offset + i] = (char)('k' + i % 11);
        }
    }
    if (mutation)
    {
        input.data[source_offset + new_length] = 0;
        expected_input.data[source_offset + new_length] = 0;
    }
    if (allocation_case == 0)
        return 0;
    slot = allocation_case - 1;
    if (slot < 0 || slot >= 2)
    {
        CHECK(0);
        return 0;
    }
    for (i = 0; i <= new_length; ++i)
        expected_destinations[slot].data[destination_offset + i] = expected_input.data[source_offset + i];
    return destinations[slot].data + destination_offset;
}
int main(void)
{
    static const unsigned int lengths[] = {0, 1, 2, 3, 4, 15, 31, 63};
    unsigned int li, i;
    char *result;
    for (li = 0; li < 8; ++li)
        for (source_offset = 0; source_offset < 4; ++source_offset)
            for (destination_offset = 0; destination_offset < 4; ++destination_offset)
                for (allocation_case = 0; allocation_case < 3; ++allocation_case)
                    for (mutation = 0; mutation < 3; ++mutation)
                    {
                        length = lengths[li];
                        memset(&input, 0x57, sizeof(input));
                        memset(destinations, 0x68, sizeof(destinations));
                        for (i = 0; i < length; ++i)
                            input.data[source_offset + i] = (char)('A' + i % 23);
                        input.data[source_offset + length] = 0;
                        memcpy(&expected_input, &input, sizeof(input));
                        memcpy(expected_destinations, destinations, sizeof(destinations));
                        calls = 0;
                        result = op_util_strdup(input.data + source_offset);
                        CHECK(result ==
                              (allocation_case ? destinations[allocation_case - 1].data + destination_offset : 0));
                        CHECK(calls == 1);
                        verify();
                    }
    printf("util_strdup: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
