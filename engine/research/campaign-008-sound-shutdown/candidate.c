#include "api.h"

// FUNCTION: WMAIN 0x00415ab5
int op_sound_shutdown(void)
{
    int i;

    if (op_sound_initialized == 0)
        return 0;
    op_sound_resume_all();
    op_sound_stop_all();
    op_sound_free_extra_refs();
    for (i = 0; i < 12; ++i)
        op_sound_release_handle(i);
    op_sound_clear_backend();
    if (op_sound_registry != 0) {
        op_sound_destroy_registry(op_sound_registry);
        op_sound_registry = 0;
    }
    op_sound_quick_shutdown();
    op_sound_initialized = 0;
    return 0;
}
