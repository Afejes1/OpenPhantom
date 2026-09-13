#include "api.h"

// FUNCTION: WMAIN 0x00415b38
int op_sound_register_level(void)
{
    int i;
    op_sound_level_record *record;

    if (op_sound_initialized == 0)
        return 1;
    op_sound_load_failures = 0;
    op_sound_init_channels();
    op_sound_bind_listener(0, 0, 0);
    op_sound_category = 6;
    if (op_sound_world != 0) {
        record = op_sound_world->records;
        for (i = 0; i < op_sound_world->record_count; ++i) {
            op_sound_insert_name(op_sound_registry, record->name, record);
            ++op_sound_ref_count;
            if ((record->flags & 0x2000) == 0) {
                if (op_sound_load_ref(record) == 0) {
                    op_sound_remove_name(op_sound_registry, record->name);
                    --op_sound_ref_count;
                }
            }
            ++record;
        }
    }
    op_sound_level_registered = 1;
    return 0;
}
