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

char op_halo_name_a[8] = "unit-a", op_halo_name_b[8] = "unit-b", op_halo_name_c[8] = "unit-c";
void *op_halo_sprite_a, *op_halo_sprite_b, *op_halo_sprite_c;
static void *expected[3];
static unsigned char owned[5][16], expected_owned[5][16];
static char names[3][8];
static int calls, profile;
static void **handle(int n)
{
    return n == 0 ? &op_halo_sprite_a : n == 1 ? &op_halo_sprite_b : &op_halo_sprite_c;
}
static char *name_at(int n)
{
    return n == 0 ? op_halo_name_a : n == 1 ? op_halo_name_b : op_halo_name_c;
}
static void verify_state(void)
{
    int i;
    for (i = 0; i < 3; ++i)
    {
        CHECK(*handle(i) == expected[i]);
        CHECK(memcmp(name_at(i), names[i], 8) == 0);
    }
    CHECK(memcmp(owned, expected_owned, sizeof(owned)) == 0);
}
static void initialize(int mode)
{
    int i;
    profile = mode;
    calls = 0;
    memset(owned, 0x53, sizeof(owned));
    memcpy(expected_owned, owned, sizeof(owned));
    for (i = 0; i < 3; ++i)
    {
        *handle(i) = mode & (1 << i) ? owned[i] : 0;
        expected[i] = *handle(i);
        memcpy(name_at(i), i == 0 ? "unit-a" : i == 1 ? "unit-b" : "unit-c", 7);
        name_at(i)[7] = (char)(0x61 + i);
        memcpy(names[i], name_at(i), 8);
    }
}

void op_release_sprite(void **sprite)
{
    int n = calls++;
    CHECK(n >= 0 && n < 3);
    if (n < 0 || n >= 3)
        return;
    CHECK(sprite == handle(n));
    verify_state();
    *sprite = profile % 2 ? 0 : owned[4];
    expected[n] = *sprite;
    if (n < 2)
    {
        *handle(n + 1) = owned[3];
        expected[n + 1] = owned[3];
    }
    owned[0][6] = 0x66;
    expected_owned[0][6] = 0x66;
}
int main(void)
{
    int i;
    for (i = 0; i < 8; ++i)
    {
        initialize(i);
        op_halo_shutdown();
        CHECK(calls == 3);
        verify_state();
    }
    printf("halo shutdown: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
