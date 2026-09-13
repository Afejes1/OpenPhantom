#include "api.h"

// FUNCTION: WMAIN 0x004158f0
int op_sound_module_proc(int event)
{
    int result = 2;

    switch (event) {
    case 1:
        result = op_sound_module_init();
        break;
    case 2:
        result = op_sound_shutdown();
        break;
    case 25:
        result = op_sound_register_level();
        break;
    case 6:
        result = op_sound_remove_level(0);
        break;
    case 18:
        op_sound_reset_places();
        result = 0;
        break;
    case 19:
        op_sound_stop_all();
        result = 0;
        break;
    case 13:
        op_sound_quality_level = op_sound_performance_level - 1;
    case 14:
        result = op_sound_periodic();
        break;
    case 8:
        op_sound_pause_all();
        result = 0;
        break;
    case 9:
        op_sound_resume_all();
        result = 0;
        break;
    }
    return result;
}
