#include "api.h"

// FUNCTION: WMAIN 0x004157d0
int op_sound_resource_handler(OP_SOUND_RESOURCE_EVENT *event)
{
    int result = 0;
    unsigned int file_size;
    void *file_buffer;
    int handle;

    switch (event->operation) {
    case 1:
        file_size = 0;
        file_buffer = 0;
        handle = 0;
        handle = op_sound_file_api->open(event->name, op_sound_read_mode);
        file_size = op_sound_file_api->size(event->name);
        if (handle != 0) {
            file_buffer = op_sound_allocate(op_sound_allocation_tag, file_size);
            op_sound_file_api->read(handle, file_buffer, file_size);
            op_sound_file_api->close(handle);
        }
        if (file_buffer != 0) {
            op_sound_register_loaded(event->resource_type, event->name, file_buffer);
            result = 1;
        }
        break;
    case 2:
        op_sound_free_resource(event->data);
        result = 1;
        break;
    case 3:
        event->size = op_sound_resource_size(event->data);
        result = 1;
        break;
    }
    return result;
}
