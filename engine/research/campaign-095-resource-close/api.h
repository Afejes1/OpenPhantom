#ifndef OP_RESOURCE_FILE_API_H
#define OP_RESOURCE_FILE_API_H
#include "conf_stream.h"
typedef struct OP_RESOURCE_SLOT
{
    int backend_handle, position;
    unsigned char reserved08[16];
    int eof, real_file;
    unsigned char reserved20[0x100c];
} OP_RESOURCE_SLOT;
typedef char
    resource_slot_layout[(sizeof(OP_RESOURCE_SLOT) == 0x102c && offsetof(OP_RESOURCE_SLOT, position) == 4 &&
                          offsetof(OP_RESOURCE_SLOT, eof) == 0x18 && offsetof(OP_RESOURCE_SLOT, real_file) == 0x1c)
                             ? 1
                             : -1];
extern OP_RESOURCE_SLOT op_resource_slots[];
extern OP_CONF_SERVICES *op_resource_backend;
extern int op_resource_log_handle;
extern const char op_resource_source[], op_resource_invalid_close[], op_resource_invalid_access[],
    op_resource_close_format[];
int op_resource_close(int);
int op_resource_eof(int);
int op_resource_tell(int);
#endif
