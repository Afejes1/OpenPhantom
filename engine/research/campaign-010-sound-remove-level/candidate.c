#include "api.h"

// FUNCTION: WMAIN 0x00415c40
int op_sound_remove_level(int ignored)
{
    int index;
    op_sound_level_record *cursor;

    (void)ignored;
    if (op_sound_initialized == 0 || op_sound_world == 0)
        return 1;
    op_sound_resume();
    op_sound_stop_all();
    op_sound_free_extra();
    cursor = op_sound_world->records;
    for (index = 0; index < op_sound_world->record_count; ++index) {
        op_sound_mark_resource(cursor->resource, 1);
        op_sound_release_resource(cursor->resource);
        cursor->resource = 0;
        op_sound_remove_name(op_sound_registry, cursor->name);
        --op_sound_ref_count;
        cursor->resource = 0;
        ++cursor;
    }
    op_sound_level_registered = 0;
    return 0;
}
