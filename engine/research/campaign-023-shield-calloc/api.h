#ifndef OP_SHIELD_LIFECYCLE_API_H
#define OP_SHIELD_LIFECYCLE_API_H
#include <stddef.h>
typedef struct OP_ATTACHED_ACTOR
{
    unsigned char before_slot[0x100];
    int shield_slot;
} OP_ATTACHED_ACTOR;
typedef struct OP_SHIELD
{
    int active;
    int visible;
    int stopping;
    int no_save;
    OP_ATTACHED_ACTOR *attached;
    float elapsed;
    unsigned char before_sprite[0x44];
    void *sprite;
    char name[0x54];
} OP_SHIELD;
typedef char op_shield_size[sizeof(OP_SHIELD) == 0xb4 ? 1 : -1];
typedef char op_shield_stopping[offsetof(OP_SHIELD, stopping) == 8 ? 1 : -1];
typedef char op_shield_no_save[offsetof(OP_SHIELD, no_save) == 0xc ? 1 : -1];
typedef char op_shield_attached[offsetof(OP_SHIELD, attached) == 0x10 ? 1 : -1];
typedef char op_shield_elapsed[offsetof(OP_SHIELD, elapsed) == 0x14 ? 1 : -1];
typedef char op_shield_sprite[offsetof(OP_SHIELD, sprite) == 0x5c ? 1 : -1];
typedef char op_shield_name[offsetof(OP_SHIELD, name) == 0x60 ? 1 : -1];
typedef char op_actor_slot[offsetof(OP_ATTACHED_ACTOR, shield_slot) == 0x100 ? 1 : -1];
typedef char op_word_sizes[sizeof(void *) == 4 && sizeof(int) == 4 && sizeof(float) == 4 ? 1 : -1];
extern OP_SHIELD op_shields[32];
void op_release(void *memory);
void *op_allocate(unsigned int bytes);
int op_shield_destroy(int slot);
void op_release_sprite(void **sprite);
void *op_acquire_sprite(char *name);
void op_shield_free(void *memory);
void op_shield_destroy_all(void);
int op_shield_stop(int slot);
int op_shield_save_size(void);
void *op_shield_calloc(unsigned int bytes);
int op_shield_set_texture(int slot, char *name);
#endif
