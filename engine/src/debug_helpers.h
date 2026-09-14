#ifndef OP_DEBUG_HELPERS_H
#define OP_DEBUG_HELPERS_H
#include <stddef.h>
typedef struct OP_DEBUG_ROW
{
    unsigned int unknown00[3];
    char *text;
    unsigned int page, unknown20;
} OP_DEBUG_ROW;
typedef char debug_layout[(sizeof(void *) == 4 && sizeof(OP_DEBUG_ROW) == 24 && offsetof(OP_DEBUG_ROW, text) == 12 &&
                           offsetof(OP_DEBUG_ROW, page) == 16)
                              ? 1
                              : -1];
extern OP_DEBUG_ROW op_debug_rows[];
extern int op_debug_count;
extern unsigned int op_debug_visible, op_debug_pulse, op_debug_page, op_debug_size;
extern const char op_debug_label0[], op_debug_label1[], op_debug_label2[], op_debug_label3[];
void op_release(void *);
int op_debug_upsert(unsigned int, int, int, const char *);
int op_debug_resize(unsigned int, int, int);
void op_debug_clear_page4(void);
void op_debug_toggle_visible(void);
void op_debug_next_page(void);
void op_debug_reset(unsigned int);
void op_debug_toggle_size(void);
void op_debug_disable(void);
#endif
