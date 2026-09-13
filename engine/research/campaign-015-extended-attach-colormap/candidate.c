#include "api.h"

// FUNCTION: WMAIN 0x00428dee
void op_extended_attach_colormap(OP_EXTENDED_MODEL *model, const char *name)
{
    void *loaded;
    int assigned;
    unsigned int index;

    loaded = op_extended_load_colormap(name);
    if (loaded != 0) {
        assigned = 0;
        for (index = 0; index < model->material_count; ++index) {
            if (model->materials[index]->colormap == 0) {
                model->materials[index]->colormap = (unsigned char *)loaded + 0x30;
                ++assigned;
            }
        }
    }
}
