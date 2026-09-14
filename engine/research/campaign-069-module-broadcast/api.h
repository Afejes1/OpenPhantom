#ifndef OP_C069_BROADCAST_H
#define OP_C069_BROADCAST_H
#include <stddef.h>
typedef union OP_MODULE_STATUS {
    unsigned int raw;
    struct
    {
        signed int bit0 : 1;
        signed int bit1 : 1;
        signed int bit2 : 1;
        signed int bit3 : 1;
        signed int remaining : 28;
    } bits;
} OP_MODULE_STATUS;
typedef struct OP_MODULE
{
    struct OP_MODULE *next, *previous;
    int id;
    OP_MODULE_STATUS status;
    unsigned int callback_word;
    char name[16];
} OP_MODULE;
typedef char module_layout[(sizeof(OP_MODULE_STATUS) == 4 && sizeof(OP_MODULE) == 36 && offsetof(OP_MODULE, id) == 8 &&
                            offsetof(OP_MODULE, status) == 12 && offsetof(OP_MODULE, callback_word) == 16 &&
                            offsetof(OP_MODULE, name) == 20)
                               ? 1
                               : -1];
extern OP_MODULE *op_module_head, *op_module_tail;
extern int op_module_initialized;
int op_module_initialize_once(void);
void op_module_remove_void(int);
unsigned int op_module_get_flags(int, unsigned int);
OP_MODULE *op_module_find_by_id(int);
int op_module_remove(int);
int op_module_find_by_name(const char *, unsigned int);
int op_compare_tag(const char *, const char *, unsigned int);
void op_release(void *);
typedef int(__cdecl *OP_MODULE_CALLBACK)(unsigned int, unsigned int, unsigned int);
int op_module_send_restore(int, unsigned int);
int op_module_broadcast_save(int);
int op_module_broadcast_event(int, unsigned int, unsigned int);
int op_module_suspend_all(int);
int op_module_resume_all(int);
int op_module_stop_all(int);
extern int op_module_open;
int op_module_start_all(int);
int op_module_open_all(void);
void op_module_close_all(void);
int op_module_install(unsigned int, const char *);
void *op_allocate(unsigned int);
char *op_copy_keyframe_name(char *, const char *, unsigned int);
int op_save_lifecycle(int, unsigned int);
#endif
