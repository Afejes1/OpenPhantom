#ifndef OP_UI_INPUT_STATE_H
#define OP_UI_INPUT_STATE_H
#include <stddef.h>
typedef struct OP_UI_CURSOR
{
    int x, y;
} OP_UI_CURSOR;
typedef struct OP_UI_JOY_LATCHES
{
    int first, second;
} OP_UI_JOY_LATCHES;
typedef char ui_input_layout[(sizeof(OP_UI_CURSOR) == 8 && offsetof(OP_UI_CURSOR, y) == 4 &&
                              sizeof(OP_UI_JOY_LATCHES) == 8 && offsetof(OP_UI_JOY_LATCHES, second) == 4)
                                 ? 1
                                 : -1];
extern OP_UI_CURSOR op_ui_cursor;
extern OP_UI_JOY_LATCHES op_ui_joy_latches;
extern int op_ui_navigation_code, op_ui_joy_menu_navigation;
void op_ui_get_cursor(int *, int *);
int *op_ui_navigation_code_pointer(void);
void op_ui_set_joy_navigation(int);
void op_ui_clear_joy_latches(void);
#endif
