#ifndef OP_CONSOLE_HELPERS_H
#define OP_CONSOLE_HELPERS_H
#include <stddef.h>
typedef struct OP_CONSOLE_COORD
{
    short x, y;
} OP_CONSOLE_COORD;
typedef struct OP_CONSOLE_RECT
{
    short left, top, right, bottom;
} OP_CONSOLE_RECT;
typedef struct OP_CONSOLE_INFO
{
    OP_CONSOLE_COORD size, cursor;
    unsigned short attributes;
    OP_CONSOLE_RECT window;
    OP_CONSOLE_COORD maximum;
} OP_CONSOLE_INFO;
/* Word aliases preserve the untouched upper16 bits beside attribute storage. */
typedef union OP_CONSOLE_WORD {
    unsigned int bits;
    unsigned short low;
    OP_CONSOLE_COORD coord;
} OP_CONSOLE_WORD;
typedef char console_layout[(sizeof(void *) == 4 && sizeof(OP_CONSOLE_COORD) == 4 && sizeof(OP_CONSOLE_INFO) == 22 &&
                             offsetof(OP_CONSOLE_INFO, cursor) == 4 && offsetof(OP_CONSOLE_INFO, attributes) == 8 &&
                             offsetof(OP_CONSOLE_INFO, maximum) == 18 && sizeof(OP_CONSOLE_WORD) == 4)
                                ? 1
                                : -1];
extern OP_CONSOLE_WORD op_console_output, op_console_input, op_console_default_attr, op_console_current_attr;
extern int(__stdcall *op_console_free)(void);
extern int(__stdcall *op_console_set_position)(unsigned int, OP_CONSOLE_COORD);
extern int(__stdcall *op_console_get_info)(unsigned int, OP_CONSOLE_INFO *);
extern int(__stdcall *op_console_set_text_attribute)(unsigned int, unsigned short);
extern int(__stdcall *op_console_flush_input)(unsigned int);
void op_console_close(void);
void op_console_goto(short, short);
void op_console_get_xy(OP_CONSOLE_COORD *);
int op_console_set_default_attr(unsigned short);
void op_console_set_attr(unsigned short);
void op_console_flush(void);
#endif
