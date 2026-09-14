#include "api.h"
// FUNCTION: WMAIN 0x0046af4b
int op_directory_find_one(char *path, int mode, char *extension, OP_DIRECTORY_ENTRY *output)
{
    OP_DIRECTORY *state;
    int result;
    state = op_directory_open(path, mode, extension);
    if (!state)
        return 0;
    result = op_directory_read(state, output);
    op_directory_close(state);
    return result;
}
