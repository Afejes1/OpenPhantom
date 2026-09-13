#ifndef OP_CAMPAIGN027_API_H
#define OP_CAMPAIGN027_API_H
#include <stddef.h>
typedef struct OP_ACTOR
{
    unsigned int flags;
    unsigned char data[12];
} OP_ACTOR;
typedef struct OP_ZAP
{
    int active;
    unsigned char before_mode[12];
    int mode;
    OP_ACTOR *endpoints[4];
    unsigned char tail[84];
} OP_ZAP;
typedef struct OP_HALO
{
    OP_ACTOR *owner;
    unsigned char before_sprite[16];
    void *sprite;
    unsigned char tail[4];
} OP_HALO;
typedef char op_word_size[sizeof(int) == 4 && sizeof(void *) == 4 ? 1 : -1];
typedef char op_zap_size[sizeof(OP_ZAP) == 120 ? 1 : -1];
typedef char op_zap_mode[offsetof(OP_ZAP, mode) == 16 ? 1 : -1];
typedef char op_zap_endpoints[offsetof(OP_ZAP, endpoints) == 20 ? 1 : -1];
typedef char op_halo_size[sizeof(OP_HALO) == 28 ? 1 : -1];
typedef char op_halo_sprite[offsetof(OP_HALO, sprite) == 20 ? 1 : -1];
extern OP_ZAP op_zaps[64];
extern int op_zap_count;
extern char op_zap_name[];
extern void *op_zap_sprite;
extern OP_HALO op_halos[32];
extern int op_halo_count;
void *op_acquire_sprite(char *name);
void op_release_sprite(void **sprite);
void op_detach_zap(OP_ACTOR *actor);
int op_zap_startup(void);
int op_zap_shutdown(void);
void op_zap_reset(void);
int op_zap_has_actor(OP_ACTOR *actor);
void op_halo_free_all(void);
#endif
