#include "directory.h"
#include <string.h>
#pragma intrinsic(strcpy)
// FUNCTION: WMAIN 0x0046ae6f
int op_directory_read(OP_DIRECTORY *directory, OP_DIRECTORY_ENTRY *output)
{
    OP_DIRECTORY *directory_state;
    int find_result;
    OP_FIND_DATA file_data;
    directory_state = directory;
    if (!directory_state)
        return 0;
    if (directory_state->position++)
        find_result = op_file_find_next(directory_state->find_handle, &file_data);
    else
    {
        directory_state->find_handle = op_file_find_first(directory_state->pattern, &file_data);
        find_result = directory_state->find_handle;
    }
    if (find_result == -1)
        return 0;
    strcpy(output->name, file_data.name);
    output->is_directory = file_data.attributes & 0x10;
    output->time_written = file_data.time_written;
    return 1;
}
