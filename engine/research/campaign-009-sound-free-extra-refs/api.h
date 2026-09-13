#ifndef OP_CAMPAIGN_009_SOUND_FREE_EXTRA_REFS_API_H
#define OP_CAMPAIGN_009_SOUND_FREE_EXTRA_REFS_API_H
typedef struct op_sound_ref_node {
    unsigned char opaque_00[4];
    char name[52];
    void *resource;
    struct op_sound_ref_node *next;
} op_sound_ref_node;
extern op_sound_ref_node *op_sound_extra_head;
extern void *op_sound_registry;
extern int op_sound_ref_count;
extern int op_sound_extra_count;
int op_sound_mark_resource(void *resource, int flag);
int op_sound_release_resource(void *resource);
int op_sound_remove_name(void *registry, char *name);
void op_sound_free_node(void *node);
void op_sound_free_extra_refs(void);
#endif
