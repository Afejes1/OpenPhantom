#ifndef OP_WINDOW_STATE_H
#define OP_WINDOW_STATE_H
#include <stddef.h>
typedef unsigned int OP_WINDOW_HANDLE_WORD;
typedef struct OP_WINDOW_RECT
{
    unsigned int words[4];
} OP_WINDOW_RECT;
typedef char window_layout[(sizeof(OP_WINDOW_HANDLE_WORD) == 4 && sizeof(OP_WINDOW_RECT) == 16 &&
                            offsetof(OP_WINDOW_RECT, words) == 0)
                               ? 1
                               : -1];
extern OP_WINDOW_HANDLE_WORD op_window_handle, op_instance_handle;
extern OP_WINDOW_RECT op_window_rect;
void op_window_set_handle(OP_WINDOW_HANDLE_WORD);
OP_WINDOW_HANDLE_WORD op_window_get_handle(void);
void op_window_set_instance(OP_WINDOW_HANDLE_WORD);
OP_WINDOW_HANDLE_WORD op_window_get_instance(void);
void op_window_set_rect(const OP_WINDOW_RECT *);
OP_WINDOW_RECT *op_window_get_rect(void);
#endif
