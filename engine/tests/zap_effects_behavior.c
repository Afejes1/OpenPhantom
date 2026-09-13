/* Authored zapline state and callback boundaries. */
#include "../src/zap_effects.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_ZAP op_zaps[64];
int op_zap_count;
char op_zap_name[12] = "unit-zap";
void *op_zap_sprite;
/*17 proven points followed by two authored trailing guards.*/
OP_VEC3 op_zap_points[19];
const float op_zap_amplitude_scale = 0.4f;
const float op_zap_half = 0.5f, op_random_scale = 3.0518509447574615e-5f, op_zap_decay = 0.6666666865348816f;
#include "detach_zap_behavior.h"
#include "zap_has_actor_behavior.h"
#include "zap_reset_behavior.h"
#include "zap_shutdown_behavior.h"
#include "zap_startup_behavior.h"
static int zp_mode, zp_checks, zp_failures;
static void zp_check_at(int value, int line)
{
    ++zp_checks;
    if (!value)
    {
        ++zp_failures;
        printf("zap integration failed at %d\n", line);
    }
}
#define ZP_CHECK(x) zp_check_at(!!(x), __LINE__)
enum
{
    ZP_START = 1,
    ZP_STOP,
    ZP_SEGMENT,
    ZP_DEFAULT,
    ZP_CHAIN
};
static unsigned char zp_owned[2][16];
static int zp_acquires, zp_releases, zp_result;
static void *zap_effects_acquire_sprite(char *name)
{
    ZP_CHECK(zap_effects_active);
    if (zp_mode == ZP_START)
        return lc_zap_startup_op_acquire_sprite(name);
    ZP_CHECK(zp_mode == ZP_CHAIN && zp_acquires++ == 0);
    ZP_CHECK(name == op_zap_name && op_zap_sprite == 0);
    return zp_result ? zp_owned[0] : 0;
}
static void zap_effects_release_sprite(void **sprite)
{
    ZP_CHECK(zap_effects_active);
    if (zp_mode == ZP_STOP)
    {
        lc_zap_shutdown_op_release_sprite(sprite);
        return;
    }
    ZP_CHECK(zp_mode == ZP_CHAIN && zp_releases++ == 0);
    ZP_CHECK(sprite == &op_zap_sprite && *sprite == (zp_result ? zp_owned[0] : 0));
    *sprite = zp_owned[1];
}
#include "zap_geometry_behavior.h"
typedef struct ZP_ACTORS
{
    unsigned int before;
    OP_ATTACHED_ACTOR value[2];
    unsigned int after;
} ZP_ACTORS;
static ZP_ACTORS zp_actors, zp_expected_actors;
static OP_ZAP zp_expected[64];
static void zp_chain_tests(void)
{
    int i, j;
    for (zp_result = 0; zp_result < 2; ++zp_result)
    {
        op_zap_sprite = 0;
        zp_acquires = zp_releases = 0;
        ZP_CHECK(op_zap_startup() == zp_result);
        ZP_CHECK(zp_acquires == 1);
        ZP_CHECK(op_zap_shutdown() == 1);
        ZP_CHECK(zp_releases == 1 && op_zap_sprite == zp_owned[1]);
    }
    memset(&zp_actors, 0x56, sizeof(zp_actors));
    zp_expected_actors = zp_actors;
    memset(op_zaps, 0x67, sizeof(op_zaps));
    for (i = 0; i < 64; ++i)
    {
        op_zaps[i].active = 1;
        op_zaps[i].mode = 1;
        for (j = 0; j < 4; ++j)
            op_zaps[i].endpoints[j] = &zp_actors.value[j % 2];
    }
    memcpy(zp_expected, op_zaps, sizeof(op_zaps));
    for (i = 0; i < 64; ++i)
    {
        zp_expected[i].endpoints[0] = 0;
        zp_expected[i].endpoints[2] = 0;
    }
    ZP_CHECK(op_zap_has_actor(&zp_actors.value[0]) == 1);
    op_detach_zap(&zp_actors.value[0]);
    ZP_CHECK(op_zap_has_actor(&zp_actors.value[0]) == 0);
    ZP_CHECK(op_zap_has_actor(&zp_actors.value[1]) == 1);
    ZP_CHECK(memcmp(op_zaps, zp_expected, sizeof(op_zaps)) == 0);
    ZP_CHECK(memcmp(&zp_actors, &zp_expected_actors, sizeof(zp_actors)) == 0);
    op_zap_count = INT_MIN;
    op_zap_reset();
    memset(zp_expected, 0, sizeof(zp_expected));
    ZP_CHECK(op_zap_count == 0 && memcmp(op_zaps, zp_expected, sizeof(op_zaps)) == 0);
    ZP_CHECK(op_zap_has_actor(&zp_actors.value[0]) == 0 && op_zap_has_actor(&zp_actors.value[1]) == 0);
}
static int op_test_zap_effects(void)
{
    int status = 0, total;
    ZP_CHECK(!zap_effects_active && !halo_overlay_active && !shield_lifecycle_active && !ripple_effects_active);
    zap_effects_active = 1;
    status += lc_detach_zap_main();
    status += lc_zap_has_actor_main();
    status += lc_zap_reset_main();
    zp_mode = ZP_START;
    status += lc_zap_startup_main();
    zp_mode = ZP_STOP;
    status += lc_zap_shutdown_main();
    status += zg_main();
    zp_mode = ZP_CHAIN;
    zp_chain_tests();
    zap_effects_active = 0;
    zp_mode = 0;
    ZP_CHECK(!halo_overlay_active && !shield_lifecycle_active && !ripple_effects_active);
    total = zp_checks + lc_detach_zap_checks + lc_zap_has_actor_checks + lc_zap_reset_checks + lc_zap_startup_checks +
            lc_zap_shutdown_checks + zg_checks + zsub_checks + sf_checks;
    printf("zap lifecycle total: %d checks, %d integration failures\n", total, zp_failures);
    return status + (zp_failures != 0);
}
#undef ZP_CHECK
