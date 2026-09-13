#include "../src/focused_accessors.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sl_shield_calloc_checks, sl_shield_calloc_failures;
static void sl_shield_calloc_check_at(int value, int line)
{
    ++sl_shield_calloc_checks;
    if (!value)
    {
        ++sl_shield_calloc_failures;
        printf("failed at %d\n", line);
    }
}
#define SL_SHIELD_CALLOC_CHECK(x) sl_shield_calloc_check_at(!!(x), __LINE__)

typedef struct sl_shield_calloc_STORAGE
{
    unsigned int before;
    unsigned char data[64];
    unsigned int after;
} sl_shield_calloc_STORAGE;
static sl_shield_calloc_STORAGE sl_shield_calloc_storage, sl_shield_calloc_expected;
static unsigned char sl_shield_calloc_other[8], sl_shield_calloc_expected_other[8];
static void *sl_shield_calloc_result_pointer;
static unsigned int sl_shield_calloc_wanted;
static int sl_shield_calloc_calls;
static void *sl_shield_calloc_op_allocate(unsigned int bytes)
{
    SL_SHIELD_CALLOC_CHECK(sl_shield_calloc_calls++ == 0);
    SL_SHIELD_CALLOC_CHECK(bytes == sl_shield_calloc_wanted);
    SL_SHIELD_CALLOC_CHECK(
        memcmp(&sl_shield_calloc_storage, &sl_shield_calloc_expected, sizeof(sl_shield_calloc_storage)) == 0);
    SL_SHIELD_CALLOC_CHECK(
        memcmp(sl_shield_calloc_other, sl_shield_calloc_expected_other, sizeof(sl_shield_calloc_other)) == 0);
    sl_shield_calloc_other[3] = 0x42;
    sl_shield_calloc_expected_other[3] = 0x42;
    return sl_shield_calloc_result_pointer;
}
static void sl_shield_calloc_run_case(unsigned int bytes, int null_result)
{
    void *result;
    unsigned int i;
    memset(&sl_shield_calloc_storage, 0x79, sizeof(sl_shield_calloc_storage));
    memset(sl_shield_calloc_other, 0x38, sizeof(sl_shield_calloc_other));
    sl_shield_calloc_expected = sl_shield_calloc_storage;
    memcpy(sl_shield_calloc_expected_other, sl_shield_calloc_other, sizeof(sl_shield_calloc_other));
    sl_shield_calloc_wanted = bytes;
    sl_shield_calloc_calls = 0;
    sl_shield_calloc_result_pointer = null_result ? 0 : sl_shield_calloc_storage.data;
    result = op_shield_calloc(bytes);
    if (!null_result)
        for (i = 0; i < bytes; ++i)
            sl_shield_calloc_expected.data[i] = 0;
    SL_SHIELD_CALLOC_CHECK(result == sl_shield_calloc_result_pointer);
    SL_SHIELD_CALLOC_CHECK(sl_shield_calloc_calls == 1);
    SL_SHIELD_CALLOC_CHECK(
        memcmp(&sl_shield_calloc_storage, &sl_shield_calloc_expected, sizeof(sl_shield_calloc_storage)) == 0);
    SL_SHIELD_CALLOC_CHECK(
        memcmp(sl_shield_calloc_other, sl_shield_calloc_expected_other, sizeof(sl_shield_calloc_other)) == 0);
}
static int sl_shield_calloc_main(void)
{
    static const unsigned int sizes[8] = {0, 1, 2, 3, 4, 5, 31, 64};
    int i;
    for (i = 0; i < 8; ++i)
    {
        sl_shield_calloc_run_case(sizes[i], 0);
        sl_shield_calloc_run_case(sizes[i], 1);
    }
    printf("shield calloc: %d checks, %d failures\n", sl_shield_calloc_checks, sl_shield_calloc_failures);
    return sl_shield_calloc_failures != 0;
}

#undef SL_SHIELD_CALLOC_CHECK
