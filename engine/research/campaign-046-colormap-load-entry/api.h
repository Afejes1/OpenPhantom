#ifndef OP_CAMPAIGN046_API_H
#define OP_CAMPAIGN046_API_H
#include <stddef.h>
typedef struct OP_COLORMAP
{
    char name[32];
    unsigned int flags, field24, field28, field2c;
    unsigned char palette[768];
    unsigned char *light_table;
    void *handle334;
    unsigned char *alpha_table;
    void *handle33c;
    unsigned char tail[12];
} OP_COLORMAP;
typedef struct OP_CMP_HEADER
{
    char marker[4];
    unsigned int version, flags, field24, field28, field2c;
    unsigned char reserved[40];
} OP_CMP_HEADER;
typedef unsigned int OP_FILE_HANDLE;
typedef struct OP_CMP_SERVICES
{
    unsigned char prefix[48];
    OP_FILE_HANDLE (*open)(const char *, const char *);
    int (*close)(OP_FILE_HANDLE);
    unsigned int (*read)(OP_FILE_HANDLE, void *, unsigned int);
    void *unused3c;
    unsigned int (*write)(OP_FILE_HANDLE, const void *, unsigned int);
} OP_CMP_SERVICES;
typedef char cmp_layout[(sizeof(OP_COLORMAP) == 844 && offsetof(OP_COLORMAP, palette) == 48 &&
                         offsetof(OP_COLORMAP, light_table) == 816 && offsetof(OP_COLORMAP, handle334) == 820 &&
                         offsetof(OP_COLORMAP, alpha_table) == 824 && offsetof(OP_COLORMAP, handle33c) == 828 &&
                         sizeof(OP_CMP_HEADER) == 64 && offsetof(OP_CMP_HEADER, reserved) == 24 &&
                         offsetof(OP_CMP_SERVICES, open) == 48 && offsetof(OP_CMP_SERVICES, close) == 52 &&
                         offsetof(OP_CMP_SERVICES, read) == 56 && offsetof(OP_CMP_SERVICES, write) == 64)
                            ? 1
                            : -1];
extern OP_CMP_SERVICES *op_colormap_services;
extern const char op_colormap_read_mode[], op_colormap_write_mode[], op_colormap_marker[];
const char *op_resource_basename(const char *);
/* Existing accepted symbol for the original bounded string-copy runtime entry. */
char *op_copy_keyframe_name(char *, const char *, unsigned int);
int op_compare_marker(const char *, const char *, unsigned int);
void *op_allocate(unsigned int);
int op_colormap_build_gray(OP_COLORMAP *);
int op_colormap_load_entry(const char *, OP_COLORMAP *);
int op_colormap_write(const char *, OP_COLORMAP *);
#endif
