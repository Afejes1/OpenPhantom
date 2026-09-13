#include "api.h"

// FUNCTION: WMAIN 0x00417459
int op_sound_get_master_volume(void)
{
    if (op_sound_initialized == 0)
        return 0;
    return op_sound_query_volume(op_sound_driver);
}