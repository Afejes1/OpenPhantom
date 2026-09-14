#ifndef OP_UI_WIDGET_CONTROL_H
#define OP_UI_WIDGET_CONTROL_H
#include <stddef.h>
typedef struct OP_UI_WIDGET_PREFIX
{
    unsigned int unknown00;
    int id;
} OP_UI_WIDGET_PREFIX;
typedef char ui_widget_id_layout[(sizeof(OP_UI_WIDGET_PREFIX) == 8 && offsetof(OP_UI_WIDGET_PREFIX, id) == 4) ? 1 : -1];
extern int op_ui_debug_boxes, op_ui_mouse_focus_clears, op_ui3d_active;
void op_ui_toggle_debug_boxes(void);
void op_ui_set_mouse_focus_clears(int);
int op_ui_widget_activation_id(const OP_UI_WIDGET_PREFIX *);
void op_ui3d_begin(void);
void op_ui3d_end(void);
void op_ui3d_flush_material_buckets(void);
int op_ui3d_flush_draw_queue(void);
void op_ui3d_finish_backend(void);
#endif
