#include "api.h"
// FUNCTION: WMAIN 0x0041d5fd
int op_world_read_dialog(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header)
{
    int index;
    OP_DIALOG_RECORD *record;
    world->dialog_count = 0;
    if (op_stream_read(world->dialog_records, (int)header->dialog_payload_size,
                       1, stream) == 0) return 0;
    record = world->dialog_records;
    for (index = 0; index < (int)header->dialog_count; ++index, ++record) {
        record->link = 0;
    }
    world->dialog_count = header->dialog_count;
    return 1;
}
