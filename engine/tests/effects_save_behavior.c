/* Actual effects-load -> overlay-read -> serializer chain. */
#include "../src/effects_save.h"
#include "../src/halo_overlay.h"
#include <stdio.h>
#include <string.h>
OP_EFFECTS_SAVE op_effects_save;
#include "effects_load_behavior.h"
static int es_checks, es_failures, es_overlay_reads;
static OP_OVERLAY_SAVE es_expected_overlay;
static void es_check_at(int value, int line)
{
    ++es_checks;
    if (!value)
    {
        ++es_failures;
        printf("effects save integration failed at %d\n", line);
    }
}
#define ES_CHECK(x) es_check_at(!!(x), __LINE__)
static int effects_save_read(void *memory, unsigned int bytes)
{
    int status;
    ES_CHECK(effects_save_active);
    if (memory == &op_effects_save)
    {
        ES_CHECK(bytes == 96);
        return lc_effects_load_op_save_read(memory, bytes);
    }
    ES_CHECK(memory == &op_overlay_save && bytes == 28);
    ES_CHECK(memcmp(&op_overlay_save, &es_expected_overlay, 28) == 0);
    status = lc_effects_load_op_overlay_read_state();
    if (memory == &op_overlay_save && bytes == 28)
    {
        memset(memory, 0x30 + (es_overlay_reads & 7), 28);
        es_expected_overlay = op_overlay_save;
    }
    ++es_overlay_reads;
    /* Actual overlay_read inverts the serializer result. */
    return status == 0 ? 1 : 0;
}
int op_shield_load(void)
{
    ES_CHECK(effects_save_active);
    return lc_effects_load_op_shield_load();
}
static int op_test_effects_save(void)
{
    int status;
    ES_CHECK(!effects_save_active && !halo_overlay_active && !effects_state_active);
    es_expected_overlay = op_overlay_save;
    es_overlay_reads = 0;
    effects_save_active = 1;
    status = lc_effects_load_main();
    effects_save_active = 0;
    ES_CHECK(es_overlay_reads == 36);
    ES_CHECK(memcmp(&op_overlay_save, &es_expected_overlay, 28) == 0);
    ES_CHECK(!halo_overlay_active && !effects_state_active);
    printf("effects save total: %d checks, %d integration failures\n", es_checks + lc_effects_load_checks, es_failures);
    return status + (es_failures != 0);
}
#undef ES_CHECK
