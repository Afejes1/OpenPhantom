#ifndef OP_CONF_STREAM_H
#define OP_CONF_STREAM_H
#include <stddef.h>
#include <string.h>
typedef struct OP_CONF_SERVICES
{
    unsigned int unknown_00[12];
    int(__cdecl *open)(const char *, const char *);
    int(__cdecl *close)(int);
    unsigned int(__cdecl *read)(int, void *, unsigned int);
    unsigned int unknown_3c;
    unsigned int(__cdecl *write)(int, const void *, unsigned int);
} OP_CONF_SERVICES;
typedef char
    conf_service_offsets[(sizeof(OP_CONF_SERVICES) == 0x44 && offsetof(OP_CONF_SERVICES, open) == 0x30 &&
                          offsetof(OP_CONF_SERVICES, close) == 0x34 && offsetof(OP_CONF_SERVICES, read) == 0x38 &&
                          offsetof(OP_CONF_SERVICES, write) == 0x40)
                             ? 1
                             : -1];
extern OP_CONF_SERVICES *op_conf_services;
extern int op_conf_read_active, op_conf_read_handle, op_conf_write_handle;
extern char op_conf_write_name[], op_conf_read_mode[], op_conf_not_open[];
int op_conf_open(const char *, const char *);
int op_conf_open_read(const char *);
void op_conf_close_write(void);
int op_conf_write_string(const char *);
int op_conf_write_bytes(const void *, unsigned int);
int op_conf_read_bytes(void *, unsigned int);
int op_conf_get_file(void);
char *op_copy_keyframe_name(char *, const char *, unsigned int);
#endif
