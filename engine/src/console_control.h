#include "console_helpers.h"
#ifndef OP_CONSOLE_CONTROL_H
#define OP_CONSOLE_CONTROL_H
typedef struct OP_CONSOLE_CURSOR_INFO
{
    unsigned int size;
    int visible;
} OP_CONSOLE_CURSOR_INFO;
typedef struct OP_CONSOLE_BOX
{
    unsigned char prefix[0x14];
    int width;
    unsigned char unknown18[0x60];
    void *buffer;
    unsigned char unknown7c[4];
    unsigned int inner_size, total_size, column, row;
} OP_CONSOLE_BOX;
typedef char console_control_layout[(sizeof(OP_CONSOLE_CURSOR_INFO) == 8 && offsetof(OP_CONSOLE_BOX, buffer) == 0x78 &&
                                     sizeof(OP_CONSOLE_BOX) == 0x90)
                                        ? 1
                                        : -1];
extern unsigned int op_console_cursor_depth;
extern OP_CONSOLE_CURSOR_INFO op_console_cursor_info;
extern int(__stdcall *op_console_set_cursor_info)(unsigned int, OP_CONSOLE_CURSOR_INFO *);
extern int(__stdcall *op_console_fill_character)(unsigned int, char, unsigned int, OP_CONSOLE_COORD, unsigned int *);
void op_platform_heap_free(void *);
void op_console_box_destroy(OP_CONSOLE_BOX *);
void op_console_show_cursor(int);
void op_console_clear(void);
void op_console_clear_row(short);
#endif

#ifndef OP_CONSOLE_TEXT_H
#define OP_CONSOLE_TEXT_H
extern int(__stdcall *op_console_write)(unsigned int, const void *, unsigned int, unsigned int *, void *);
void op_console_put_char(char, unsigned int);
void op_console_put_string(const char *, unsigned int);
void op_console_box_clear(OP_CONSOLE_BOX *);
void op_console_box_clear_row(OP_CONSOLE_BOX *, int);
typedef char
    console_box_fields[(offsetof(OP_CONSOLE_BOX, width) == 0x14 && offsetof(OP_CONSOLE_BOX, inner_size) == 0x80 &&
                        offsetof(OP_CONSOLE_BOX, column) == 0x88 && offsetof(OP_CONSOLE_BOX, row) == 0x8c)
                           ? 1
                           : -1];
#endif
