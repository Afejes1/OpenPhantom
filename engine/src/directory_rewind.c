#include "directory.h"
// FUNCTION: WMAIN 0x0046ae30
void op_directory_rewind(OP_DIRECTORY *directory)
{
    OP_DIRECTORY *state;
    state = directory;
    if (state && state->position)
        op_file_find_close(state->find_handle);
    if (state)
        state->position = 0;
}
