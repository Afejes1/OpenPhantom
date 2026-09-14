#ifndef OP_SAVE_SLOTS_H
#define OP_SAVE_SLOTS_H
#include <stddef.h>
typedef struct OP_SAVE_LABEL_ROW
{
    unsigned int value;
    void *label;
} OP_SAVE_LABEL_ROW;
typedef char op_save_label_layout[sizeof(OP_SAVE_LABEL_ROW) == 8 && offsetof(OP_SAVE_LABEL_ROW, label) == 4 ? 1 : -1];
extern char op_save_path[], op_save_path_format[], op_save_path_prefix[];
void op_save_lifecycle(int, int);
char *op_save_slot_path(int);
int op_load_saved_game(char *, int);
int op_delete_file(const char *);
void op_release(void *);
int op_format_sprite_name(char *, char *, ...);
int op_save_load_slot_teardown(int);
void op_save_load_slot(int);
int op_save_delete_slot(int);
void op_save_free_slot_labels(OP_SAVE_LABEL_ROW *);
#endif
