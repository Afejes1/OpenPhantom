#include "api.h"
// FUNCTION: WMAIN 0x0043b09c
int op_shield_destroy(int slot)
{
    OP_SHIELD *shield;
    if (slot < 0 || slot >= 32)
        return 0;
    shield = &op_shields[slot];
    if (shield->active == 0)
        return 0;
    op_shield_free(shield->buffer_1c);
    shield->buffer_1c = 0;
    shield->count_18 = 0;
    op_shield_free(shield->buffer_40);
    shield->buffer_40 = 0;
    shield->count_3c = 0;
    op_shield_free(shield->buffer_20);
    shield->buffer_20 = 0;
    op_shield_free(shield->buffer_24);
    shield->buffer_24 = 0;
    op_shield_free(shield->buffer_28);
    shield->buffer_28 = 0;
    op_shield_free(shield->buffer_2c);
    shield->buffer_2c = 0;
    op_shield_free(shield->buffer_30);
    shield->buffer_30 = 0;
    op_shield_free(shield->buffer_34);
    shield->buffer_34 = 0;
    op_release_sprite(&shield->sprite);
    shield->attached = 0;
    shield->active = 0;
    --op_shield_count;
    return 1;
}
