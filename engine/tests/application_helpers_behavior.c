/* Shared original implementations with retained owned callback fixtures. */
#include "../src/application_helpers.h"
#include "application_run_behavior.h"
#include "graphics_close_behavior.h"
#include "diagnostics_close_behavior.h"
#include "simulation_set_halted_behavior.h"
#include "halo_flicker_behavior.h"
#include "projection_token_behavior.h"
#include "overlay_size_behavior.h"
static int op_test_application_helpers(void)
{
    int result = 0;
    result += ah_application_run_main();
    result += ah_graphics_close_main();
    result += ah_diagnostics_close_main();
    result += ah_simulation_set_halted_main();
    result += ah_halo_flicker_main();
    result += ah_projection_token_main();
    result += ah_overlay_size_main();
    return result;
}
