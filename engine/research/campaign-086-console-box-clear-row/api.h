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
