#include "api.h"

// FUNCTION: WMAIN 0x00428e63
int op_extended_load_material(const char *name, void **out)
{
    *out = op_load_mat_resource(name, 0, 0);
    if (*out == 0) {
        *out = op_load_mat_resource(op_default_material_name, 0, 0);
        if (*out == 0) {
            return 2;
        }
        return 1;
    }
    return 0;
}
