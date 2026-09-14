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
    unsigned char prefix[0x78];
    void *buffer;
    unsigned char tail[0x14];
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
