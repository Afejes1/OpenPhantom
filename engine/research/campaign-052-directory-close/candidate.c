#include "api.h"
// FUNCTION: WMAIN 0x0046adef
void op_directory_close(OP_DIRECTORY *directory)
{
    OP_DIRECTORY *state;
    state = directory;
    if (state && state->position)
        op_file_find_close(state->find_handle);
    if (state)
        op_release(state);
}
