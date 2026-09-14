#include "api.h"
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
char op_default_resource_name[] = "authored-default-resource-name-for-fixture-only";
typedef struct OWNED_KEYFRAME
{
    unsigned int before;
    OP_KEYFRAME value;
    unsigned int after;
} OWNED_KEYFRAME;
static OWNED_KEYFRAME entries[4], want_entries[4];
static int row, stage, mode;
static char expected_name[sizeof(op_default_resource_name)];
char *op_copy_keyframe_name(char *out, const char *in, unsigned int count)
{
    CHECK(stage == 0 && out == entries[row].value.name && in == op_default_resource_name && count == 31);
    CHECK(memcmp(entries, want_entries, sizeof(entries)) == 0);
    strncpy(out, in, count);
    strncpy(want_entries[row].value.name, in, count);
    if (mode)
    {
        out[31] = 'X';
        want_entries[row].value.name[31] = 'X';
    }
    stage = 1;
    return out;
}
int main(void)
{
    int fill, len;
    static int lengths[] = {0, 1, 30, 31, 40};
    for (row = 0; row < 4; ++row)
        for (fill = 0; fill < 3; ++fill)
            for (len = 0; len < 5; ++len)
                for (mode = 0; mode < 2; ++mode)
                {
                    memset(entries, fill == 0 ? 0 : (fill == 1 ? 0xff : 0x69), sizeof(entries));
                    memcpy(want_entries, entries, sizeof(entries));
                    memset(&want_entries[row].value, 0, sizeof(OP_KEYFRAME));
                    memset(op_default_resource_name, 'Q', sizeof(op_default_resource_name));
                    op_default_resource_name[lengths[len]] = 0;
                    memcpy(expected_name, op_default_resource_name, sizeof(expected_name));
                    stage = 0;
                    op_keyframe_init_header(&entries[row].value);
                    want_entries[row].value.name[31] = 0;
                    CHECK(stage == 1);
                    CHECK(memcmp(entries, want_entries, sizeof(entries)) == 0);
                    CHECK(memcmp(op_default_resource_name, expected_name, sizeof(expected_name)) == 0);
                }
    printf("keyframe_init_header: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
