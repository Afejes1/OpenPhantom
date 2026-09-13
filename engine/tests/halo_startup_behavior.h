#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int hl_halo_startup_checks, hl_halo_startup_failures;
static void hl_halo_startup_check_at(int value, int hl_halo_startup_line)
{
    ++hl_halo_startup_checks;
    if (!value)
    {
        ++hl_halo_startup_failures;
        printf("failed at %d\n", hl_halo_startup_line);
    }
}
#hl_halo_startup_define hl_halo_startup_CHECK(hl_halo_startup_x)                                                       \
    hl_halo_startup_check_at(!!(hl_halo_startup_x), __LINE__)

static void *hl_halo_startup_expected[3];
static unsigned char hl_halo_startup_owned[5][16], hl_halo_startup_expected_owned[5][16];
static char hl_halo_startup_names[3][8];
static int hl_halo_startup_calls, hl_halo_startup_profile;
static void **hl_halo_startup_handle(int hl_halo_startup_n)
{
    return hl_halo_startup_n == 0 ? &op_halo_sprite_a : hl_halo_startup_n == 1 ? &op_halo_sprite_b : &op_halo_sprite_c;
}
static char *hl_halo_startup_name_at(int hl_halo_startup_n)
{
    return hl_halo_startup_n == 0 ? op_halo_name_a : hl_halo_startup_n == 1 ? op_halo_name_b : op_halo_name_c;
}
static void hl_halo_startup_verify_state(void)
{
    int hl_halo_startup_i;
    for (hl_halo_startup_i = 0; hl_halo_startup_i < 3; ++hl_halo_startup_i)
    {
        hl_halo_startup_CHECK(*hl_halo_startup_handle(hl_halo_startup_i) ==
                              hl_halo_startup_expected[hl_halo_startup_i]);
        hl_halo_startup_CHECK(
            memcmp(hl_halo_startup_name_at(hl_halo_startup_i), hl_halo_startup_names[hl_halo_startup_i], 8) == 0);
    }
    hl_halo_startup_CHECK(
        memcmp(hl_halo_startup_owned, hl_halo_startup_expected_owned, sizeof(hl_halo_startup_owned)) == 0);
}
static void hl_halo_startup_initialize(int hl_halo_startup_mode)
{
    int hl_halo_startup_i;
    hl_halo_startup_profile = hl_halo_startup_mode;
    hl_halo_startup_calls = 0;
    memset(hl_halo_startup_owned, 0x53, sizeof(hl_halo_startup_owned));
    memcpy(hl_halo_startup_expected_owned, hl_halo_startup_owned, sizeof(hl_halo_startup_owned));
    for (hl_halo_startup_i = 0; hl_halo_startup_i < 3; ++hl_halo_startup_i)
    {
        *hl_halo_startup_handle(hl_halo_startup_i) =
            hl_halo_startup_mode & (1 << hl_halo_startup_i) ? hl_halo_startup_owned[hl_halo_startup_i] : 0;
        hl_halo_startup_expected[hl_halo_startup_i] = *hl_halo_startup_handle(hl_halo_startup_i);
        memcpy(hl_halo_startup_name_at(hl_halo_startup_i),
               hl_halo_startup_i == 0   ? "unit-a"
               : hl_halo_startup_i == 1 ? "unit-b"
                                        : "unit-c",
               7);
        hl_halo_startup_name_at(hl_halo_startup_i)[7] = (char)(0x61 + hl_halo_startup_i);
        memcpy(hl_halo_startup_names[hl_halo_startup_i], hl_halo_startup_name_at(hl_halo_startup_i), 8);
    }
}

void *hl_halo_startup_op_acquire_sprite(char *hl_halo_startup_name)
{
    int hl_halo_startup_i, hl_halo_startup_n = hl_halo_startup_calls++;
    void *hl_halo_startup_result;
    hl_halo_startup_CHECK(hl_halo_startup_n >= 0 && hl_halo_startup_n < 3);
    if (hl_halo_startup_n < 0 || hl_halo_startup_n >= 3)
        return 0;
    hl_halo_startup_CHECK(hl_halo_startup_name == hl_halo_startup_name_at(hl_halo_startup_n));
    hl_halo_startup_verify_state();
    hl_halo_startup_result =
        hl_halo_startup_profile == 8
            ? hl_halo_startup_owned[0]
            : (hl_halo_startup_profile & (1 << hl_halo_startup_n) ? hl_halo_startup_owned[hl_halo_startup_n + 1] : 0);
    for (hl_halo_startup_i = 0; hl_halo_startup_i < 3; ++hl_halo_startup_i)
    {
        *hl_halo_startup_handle(hl_halo_startup_i) = hl_halo_startup_owned[4];
        hl_halo_startup_expected[hl_halo_startup_i] = hl_halo_startup_owned[4];
    }
    if (hl_halo_startup_n < 2)
    {
        hl_halo_startup_name_at(hl_halo_startup_n + 1)[1] = 'X';
        hl_halo_startup_names[hl_halo_startup_n + 1][1] = 'X';
    }
    hl_halo_startup_owned[0][5] = 0x77;
    hl_halo_startup_expected_owned[0][5] = 0x77;
    hl_halo_startup_expected[hl_halo_startup_n] = hl_halo_startup_result;
    return hl_halo_startup_result;
}
int hl_halo_startup_main(void)
{
    int hl_halo_startup_i;
    for (hl_halo_startup_i = 0; hl_halo_startup_i < 9; ++hl_halo_startup_i)
    {
        hl_halo_startup_initialize(hl_halo_startup_i);
        op_halo_startup();
        hl_halo_startup_CHECK(hl_halo_startup_calls == 3);
        hl_halo_startup_verify_state();
    }
    printf("halo startup: %d checks, %d failures\n", hl_halo_startup_checks, hl_halo_startup_failures);
    return hl_halo_startup_failures != 0;
}

#undef hl_halo_startup_CHECK
