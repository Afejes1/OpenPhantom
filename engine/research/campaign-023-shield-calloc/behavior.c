#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check_at(int value, int line)
{
    ++checks;
    if (!value)
    {
        ++failures;
        printf("failed at %d\n", line);
    }
}
#define CHECK(x) check_at(!!(x), __LINE__)

typedef struct STORAGE
{
    unsigned int before;
    unsigned char data[64];
    unsigned int after;
} STORAGE;
static STORAGE storage, expected;
static unsigned char other[8], expected_other[8];
static void *result_pointer;
static unsigned int wanted;
static int calls;
void *op_allocate(unsigned int bytes)
{
    CHECK(calls++ == 0);
    CHECK(bytes == wanted);
    CHECK(memcmp(&storage, &expected, sizeof(storage)) == 0);
    CHECK(memcmp(other, expected_other, sizeof(other)) == 0);
    other[3] = 0x42;
    expected_other[3] = 0x42;
    return result_pointer;
}
static void run_case(unsigned int bytes, int null_result)
{
    void *result;
    unsigned int i;
    memset(&storage, 0x79, sizeof(storage));
    memset(other, 0x38, sizeof(other));
    expected = storage;
    memcpy(expected_other, other, sizeof(other));
    wanted = bytes;
    calls = 0;
    result_pointer = null_result ? 0 : storage.data;
    result = op_shield_calloc(bytes);
    if (!null_result)
        for (i = 0; i < bytes; ++i)
            expected.data[i] = 0;
    CHECK(result == result_pointer);
    CHECK(calls == 1);
    CHECK(memcmp(&storage, &expected, sizeof(storage)) == 0);
    CHECK(memcmp(other, expected_other, sizeof(other)) == 0);
}
int main(void)
{
    static const unsigned int sizes[8] = {0, 1, 2, 3, 4, 5, 31, 64};
    int i;
    for (i = 0; i < 8; ++i)
    {
        run_case(sizes[i], 0);
        run_case(sizes[i], 1);
    }
    printf("shield calloc: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
