#ifndef OP_KERNEL_HELPERS_H
#define OP_KERNEL_HELPERS_H
#include <stddef.h>
typedef struct OP_KERNEL_HOOK
{
    unsigned int word, active;
} OP_KERNEL_HOOK;
typedef char kernel_layout
    [(sizeof(unsigned int) == 4 && sizeof(OP_KERNEL_HOOK) == 8 && offsetof(OP_KERNEL_HOOK, active) == 4) ? 1 : -1];
extern unsigned int op_kernel_window, op_kernel_field, op_kernel_callback, op_kernel_callback_enabled;
extern int op_kernel_show_command;
extern unsigned int op_kernel_dialog_count, op_kernel_hook_count;
extern unsigned int op_kernel_dialogs[16];
extern OP_KERNEL_HOOK op_kernel_hooks[16];
extern int(__stdcall *op_kernel_show_window)(unsigned int, int);
void op_kernel_show(void);
void op_kernel_hide(void);
void op_kernel_set_field(unsigned int);
int op_kernel_set_callback(unsigned int);
int op_kernel_add_hook(unsigned int);
int op_kernel_remove_hook(unsigned int);
int op_kernel_add_dialog(unsigned int);
int op_kernel_remove_dialog(unsigned int);
unsigned int op_kernel_get_window(void);
#endif
