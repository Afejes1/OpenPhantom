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
void *op_zap_sprite;
static unsigned char owned[4][16], expected_owned[4][16];
static void *expected_sprite, *replacement;
static int calls;
static void verify_state(void)
{
    CHECK(op_zap_sprite == expected_sprite);
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
void op_release_sprite(void **sprite)
{
    CHECK(calls++ == 0);
    CHECK(sprite == &op_zap_sprite);
    verify_state();
    *sprite = replacement;
    expected_sprite = replacement;
    owned[1][5] = 0x39;
    expected_owned[1][5] = 0x39;
}
int main(void)
{
    int cached, r;
    for (cached = 0; cached < 3; ++cached)
        for (r = 0; r < 3; ++r)
        {
            memset(owned, 0x56, sizeof(owned));
            memcpy(expected_owned, owned, sizeof(owned));
            op_zap_sprite = cached ? owned[cached - 1] : 0;
            expected_sprite = op_zap_sprite;
            replacement = r ? owned[r - 1] : 0;
            calls = 0;
            CHECK(op_zap_shutdown() == 1);
            CHECK(calls == 1);
            verify_state();
        }
    printf("zap shutdown: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
