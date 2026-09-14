#include "api.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x0046ad50
OP_DIRECTORY *op_directory_open(char *path, int mode, char *extension)
{
    OP_DIRECTORY *state;
    char *ext;
    state = (OP_DIRECTORY *)op_allocate(sizeof(OP_DIRECTORY));
    if (!state)
        return 0;
    memset(state, 0, sizeof(OP_DIRECTORY));
    ext = extension;
    switch (mode)
    {
    case 0:
    case 1:
    case 2:
        op_format_sprite_name(state->pattern, op_directory_all_format, path);
        break;
    case 3:
        if (*ext == '.')
            ++ext;
        op_format_sprite_name(state->pattern, op_directory_extension_format, path, ext);
        break;
    }
    return state;
}
