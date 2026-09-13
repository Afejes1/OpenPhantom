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

void *op_acquire_sprite(char *name)
{
    int i, n = calls++;
    void *result;
    CHECK(n >= 0 && n < 3);
    if (n < 0 || n >= 3)
        return 0;
    CHECK(name == name_at(n));
    verify_state();
    result = profile == 8 ? owned[0] : (profile & (1 << n) ? owned[n + 1] : 0);
    for (i = 0; i < 3; ++i)
    {
        *handle(i) = owned[4];
        expected[i] = owned[4];
    }
    if (n < 2)
    {
        name_at(n + 1)[1] = 'X';
        names[n + 1][1] = 'X';
    }
    owned[0][5] = 0x77;
    expected_owned[0][5] = 0x77;
    expected[n] = result;
    return result;
}
int main(void)
{
    int i;
    for (i = 0; i < 9; ++i)
    {
        initialize(i);
        op_halo_startup();
        CHECK(calls == 3);
        verify_state();
    }
    printf("halo startup: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
