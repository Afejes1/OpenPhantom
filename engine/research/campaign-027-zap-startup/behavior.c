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
char op_zap_name[12] = "unit-zap";
void *op_zap_sprite;
static unsigned char owned[4][16], expected_owned[4][16];
static char expected_name[12];
static void *result, *expected_sprite;
static int calls;
static void verify_state(void)
{
    CHECK(op_zap_sprite == expected_sprite);
    CHECK(memcmp(op_zap_name, expected_name, 12) == 0);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
void *op_acquire_sprite(char *name)
{
    CHECK(calls++ == 0);
    CHECK(name == op_zap_name);
    verify_state();
    op_zap_sprite = owned[3];
    expected_sprite = result;
    op_zap_name[1] = 'Q';
    expected_name[1] = 'Q';
    owned[2][7] = 0x29;
    expected_owned[2][7] = 0x29;
    return result;
}
int main(void)
{
    int cached, r, v;
    for (cached = 0; cached < 3; ++cached)
        for (r = 0; r < 3; ++r)
        {
            memset(owned, 0x56, sizeof(owned));
            memcpy(expected_owned, owned, sizeof(owned));
            memset(op_zap_name, 0x67, 12);
            memcpy(op_zap_name, "unit-zap", 9);
            memcpy(expected_name, op_zap_name, 12);
            op_zap_sprite = cached ? owned[cached - 1] : 0;
            expected_sprite = op_zap_sprite;
            result = r ? owned[r - 1] : 0;
            calls = 0;
            v = op_zap_startup();
            CHECK(v == ((cached != 0) || (r != 0)));
            CHECK(calls == (cached == 0));
            verify_state();
        }
    printf("zap startup: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
