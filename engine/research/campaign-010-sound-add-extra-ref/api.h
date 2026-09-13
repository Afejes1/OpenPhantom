#ifndef OP_CAMPAIGN_010_SOUND_ADD_EXTRA_REF_API_H
#define OP_CAMPAIGN_010_SOUND_ADD_EXTRA_REF_API_H
typedef struct op_sound_ref_node { unsigned int flags; char name[52]; void *resource; struct op_sound_ref_node *next; } op_sound_ref_node;
extern int op_sound_initialized,op_sound_ref_count,op_sound_extra_count;extern void *op_sound_registry;extern op_sound_ref_node *op_sound_extra_head;
void *op_sound_allocate_node(int size);int op_sound_insert_name(void *registry,const char *name,op_sound_ref_node *value);
op_sound_ref_node *op_sound_add_extra_ref(const char *name);
#endif
