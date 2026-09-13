#include "api.h"
#include <string.h>
#pragma intrinsic(strcpy)

// FUNCTION: WMAIN 0x0041618b
int op_sound_load_ref(op_sound_ref *record)
{
    char filename[256];

    if (record == 0)
        return 0;
    if (record->resource != 0)
        return 1;
    strcpy(filename, record->name);
    record->resource = op_sound_acquire_resource(0x42534e44U, filename);
    if (record->resource == 0) {
        ++op_sound_load_failures;
        if (op_sound_load_failures > 20)
            op_sound_load_failures = 0;
        return 0;
    }
    return 1;
}
