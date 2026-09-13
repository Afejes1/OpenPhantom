/* Authored material handles and deterministic callback order. */
#include "../src/ripple_effects.h"
#include <stdio.h>
OP_RIPPLE_MATERIAL op_ripple_materials[8];
#include "ripple_material_behavior.h"
static int rp_checks, rp_failures;
static void rp_check_at(int value, int line)
{
    ++rp_checks;
    if (!value)
    {
        ++rp_failures;
        printf("ripple integration failed at %d\n", line);
    }
}
#define RP_CHECK(x) rp_check_at(!!(x), __LINE__)
static void *ripple_effects_acquire_sprite(char *name)
{
    RP_CHECK(ripple_effects_active);
    return lc_ripple_material_op_acquire_sprite(name);
}
static void ripple_effects_release_sprite(void *resource)
{
    RP_CHECK(ripple_effects_active);
    lc_ripple_material_op_release_sprite(resource);
}
void *op_sprite_get_material(void *sprite)
{
    RP_CHECK(ripple_effects_active);
    return lc_ripple_material_op_sprite_get_material(sprite);
}
void *op_compose_materials(void *color, void *alpha)
{
    RP_CHECK(ripple_effects_active);
    return lc_ripple_material_op_compose_materials(color, alpha);
}
static int op_test_ripple_effects(void)
{
    int status;
    RP_CHECK(!ripple_effects_active && !zap_effects_active && !halo_overlay_active && !shield_lifecycle_active);
    ripple_effects_active = 1;
    status = lc_ripple_material_main();
    ripple_effects_active = 0;
    RP_CHECK(!zap_effects_active && !halo_overlay_active && !shield_lifecycle_active);
    printf("ripple material total: %d checks, %d integration failures\n", rp_checks + lc_ripple_material_checks,
           rp_failures);
    return status + (rp_failures != 0);
}
#undef RP_CHECK
