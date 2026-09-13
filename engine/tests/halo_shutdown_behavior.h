#include "../src/halo_overlay.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int hl_halo_shutdown_checks, hl_halo_shutdown_failures;
static void hl_halo_shutdown_check_at(int value, int hl_halo_shutdown_line)
{
    ++hl_halo_shutdown_checks;
    if (!value)
    {
        ++hl_halo_shutdown_failures;
        printf("failed at %d\n", hl_halo_shutdown_line);
    }
}
#define hl_halo_shutdown_CHECK(hl_halo_shutdown_x)                                                    \
    hl_halo_shutdown_check_at(!!(hl_halo_shutdown_x), __LINE__)

static void *hl_halo_shutdown_expected[3];
static unsigned char hl_halo_shutdown_owned[5][16], hl_halo_shutdown_expected_owned[5][16];
static char hl_halo_shutdown_names[3][8];
static int hl_halo_shutdown_calls, hl_halo_shutdown_profile;
static void **hl_halo_shutdown_handle(int hl_halo_shutdown_n)
{
    return hl_halo_shutdown_n == 0   ? &op_halo_sprite_a
           : hl_halo_shutdown_n == 1 ? &op_halo_sprite_b
                                     : &op_halo_sprite_c;
}
static char *hl_halo_shutdown_name_at(int hl_halo_shutdown_n)
{
    return hl_halo_shutdown_n == 0 ? op_halo_name_a : hl_halo_shutdown_n == 1 ? op_halo_name_b : op_halo_name_c;
}
static void hl_halo_shutdown_verify_state(void)
{
    int hl_halo_shutdown_i;
    for (hl_halo_shutdown_i = 0; hl_halo_shutdown_i < 3; ++hl_halo_shutdown_i)
    {
        hl_halo_shutdown_CHECK(*hl_halo_shutdown_handle(hl_halo_shutdown_i) ==
                               hl_halo_shutdown_expected[hl_halo_shutdown_i]);
        hl_halo_shutdown_CHECK(
            memcmp(hl_halo_shutdown_name_at(hl_halo_shutdown_i), hl_halo_shutdown_names[hl_halo_shutdown_i], 8) == 0);
    }
    hl_halo_shutdown_CHECK(
        memcmp(hl_halo_shutdown_owned, hl_halo_shutdown_expected_owned, sizeof(hl_halo_shutdown_owned)) == 0);
}
static void hl_halo_shutdown_initialize(int hl_halo_shutdown_mode)
{
    int hl_halo_shutdown_i;
    hl_halo_shutdown_profile = hl_halo_shutdown_mode;
    hl_halo_shutdown_calls = 0;
    memset(hl_halo_shutdown_owned, 0x53, sizeof(hl_halo_shutdown_owned));
    memcpy(hl_halo_shutdown_expected_owned, hl_halo_shutdown_owned, sizeof(hl_halo_shutdown_owned));
    for (hl_halo_shutdown_i = 0; hl_halo_shutdown_i < 3; ++hl_halo_shutdown_i)
    {
        *hl_halo_shutdown_handle(hl_halo_shutdown_i) =
            hl_halo_shutdown_mode & (1 << hl_halo_shutdown_i) ? hl_halo_shutdown_owned[hl_halo_shutdown_i] : 0;
        hl_halo_shutdown_expected[hl_halo_shutdown_i] = *hl_halo_shutdown_handle(hl_halo_shutdown_i);
        memcpy(hl_halo_shutdown_name_at(hl_halo_shutdown_i),
               hl_halo_shutdown_i == 0   ? "unit-a"
               : hl_halo_shutdown_i == 1 ? "unit-b"
                                         : "unit-c",
               7);
        hl_halo_shutdown_name_at(hl_halo_shutdown_i)[7] = (char)(0x61 + hl_halo_shutdown_i);
        memcpy(hl_halo_shutdown_names[hl_halo_shutdown_i], hl_halo_shutdown_name_at(hl_halo_shutdown_i), 8);
    }
}

void hl_halo_shutdown_op_release_sprite(void **sprite)
{
    int hl_halo_shutdown_n = hl_halo_shutdown_calls++;
    hl_halo_shutdown_CHECK(hl_halo_shutdown_n >= 0 && hl_halo_shutdown_n < 3);
    if (hl_halo_shutdown_n < 0 || hl_halo_shutdown_n >= 3)
        return;
    hl_halo_shutdown_CHECK(sprite == hl_halo_shutdown_handle(hl_halo_shutdown_n));
    hl_halo_shutdown_verify_state();
    *sprite = hl_halo_shutdown_profile % 2 ? 0 : hl_halo_shutdown_owned[4];
    hl_halo_shutdown_expected[hl_halo_shutdown_n] = *sprite;
    if (hl_halo_shutdown_n < 2)
    {
        *hl_halo_shutdown_handle(hl_halo_shutdown_n + 1) = hl_halo_shutdown_owned[3];
        hl_halo_shutdown_expected[hl_halo_shutdown_n + 1] = hl_halo_shutdown_owned[3];
    }
    hl_halo_shutdown_owned[0][6] = 0x66;
    hl_halo_shutdown_expected_owned[0][6] = 0x66;
}
int hl_halo_shutdown_main(void)
{
    int hl_halo_shutdown_i;
    for (hl_halo_shutdown_i = 0; hl_halo_shutdown_i < 8; ++hl_halo_shutdown_i)
    {
        hl_halo_shutdown_initialize(hl_halo_shutdown_i);
        op_halo_shutdown();
        hl_halo_shutdown_CHECK(hl_halo_shutdown_calls == 3);
        hl_halo_shutdown_verify_state();
    }
    printf("halo shutdown: %d checks, %d failures\n", hl_halo_shutdown_checks, hl_halo_shutdown_failures);
    return hl_halo_shutdown_failures != 0;
}

#undef hl_halo_shutdown_CHECK
