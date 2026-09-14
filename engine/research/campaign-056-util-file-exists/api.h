#ifndef OP_CAMPAIGN056_API_H
#define OP_CAMPAIGN056_API_H
#include <stddef.h>
#include <stdarg.h>
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
typedef char conf_layout[(sizeof(OP_CONF_SERVICES) == 0x44 && offsetof(OP_CONF_SERVICES, open) == 0x30 &&
                          offsetof(OP_CONF_SERVICES, close) == 0x34 && offsetof(OP_CONF_SERVICES, read) == 0x38 &&
                          offsetof(OP_CONF_SERVICES, write) == 0x40)
                             ? 1
                             : -1];
extern OP_CONF_SERVICES *op_conf_services;
extern char op_conf_read_mode[];
void *op_allocate(unsigned int);
int op_vsnprintf(char *, unsigned int, const char *, va_list);
int op_tolower(int);
char *op_util_strdup(const char *);
void op_util_snprintf(char *, unsigned int, const char *, ...);
void op_util_strlwr(char *);
int op_util_file_exists(const char *);
#endif
