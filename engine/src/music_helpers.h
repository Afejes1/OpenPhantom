#ifndef OP_MUSIC_HELPERS_H
#define OP_MUSIC_HELPERS_H
#include <stddef.h>
typedef struct OP_MUSIC_STATUS
{
    unsigned int callback, value, item, track;
} OP_MUSIC_STATUS;
typedef char music_status_layout[(sizeof(OP_MUSIC_STATUS) == 16 && offsetof(OP_MUSIC_STATUS, value) == 4 &&
                                  offsetof(OP_MUSIC_STATUS, item) == 8)
                                     ? 1
                                     : -1];
extern unsigned int op_music_gate, op_music_device;
extern unsigned int(__stdcall *op_music_command)(unsigned int, unsigned int, unsigned int, void *);
void op_music_stop(void);
int op_music_is_playing(void);
extern int op_music_aux_device;
extern unsigned int(__stdcall *op_music_aux_volume)(unsigned int, unsigned int);
void op_music_set_volume(float);
#endif
