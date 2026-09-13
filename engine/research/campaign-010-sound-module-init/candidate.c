#include "api.h"

// FUNCTION: WMAIN 0x004159f0
int op_sound_module_init(void)
{
    int volume;
    int disabled;

    if (op_sound_initialized == 1)
        return 1;
    op_sound_quick_startup(1, 0, 22050, 16, 2);
    op_sound_preference(1, 12);
    op_sound_quick_handles(&op_sound_driver, 0, 0);
    op_sound_registry = op_sound_create_registry(512);
    if (op_sound_registry == 0)
        return 1;
    op_sound_init3d();
    op_sound_init_channels();
    op_sound_read_option(op_sound_volume_key, 127, &volume);
    op_sound_set_master_volume(volume);
    op_sound_read_option(op_sound_disabled_key, 0, &disabled);
    if (disabled != 0)
        op_sound_disable();
    else
        op_sound_enable();
    op_sound_initialized = 1;
    return 0;
}
