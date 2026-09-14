#include "font_state.h"
#include "sprite_resources.h"
// FUNCTION: WMAIN 0x0046b576
void op_font_destroy(int index)
{
    OP_FONT3D *record;
    if (index < 0 || index >= 16)
        return;
    record = &op_font_pool.rows[index];
    if (op_current_font == record)
        op_current_font = 0;
    op_release_sprite(&record->sprite);
    ((OP_FONT_RESOURCE *)record->font)->material = 0;
    op_free_font_resource(record->font);
    record->font = 0;
    op_release_sprite(&record->backdrop);
}
