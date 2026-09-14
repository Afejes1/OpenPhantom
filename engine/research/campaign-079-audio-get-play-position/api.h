#ifndef OP_AUDIO_HELPERS_H
#define OP_AUDIO_HELPERS_H
#include <stddef.h>
typedef struct OP_AUDIO_DEVICE OP_AUDIO_DEVICE;
typedef struct OP_AUDIO_BUFFER OP_AUDIO_BUFFER;
typedef struct OP_AUDIO_SPATIAL OP_AUDIO_SPATIAL;
typedef struct OP_AUDIO_LISTENER OP_AUDIO_LISTENER;
typedef struct OP_AUDIO_VECTOR
{
    float x, y, z;
} OP_AUDIO_VECTOR;
typedef struct OP_AUDIO_DEVICE_VT
{
    unsigned int unknown00[6];
    int(__stdcall *cooperative)(OP_AUDIO_DEVICE *, unsigned int, unsigned int);
} OP_AUDIO_DEVICE_VT;
typedef struct OP_AUDIO_BUFFER_VT
{
    unsigned int unknown00[2];
    unsigned int(__stdcall *release)(OP_AUDIO_BUFFER *);
    unsigned int unknown0c;
    int(__stdcall *get_position)(OP_AUDIO_BUFFER *, unsigned int *, unsigned int *);
    unsigned int unknown14[4];
    int(__stdcall *get_status)(OP_AUDIO_BUFFER *, unsigned int *);
    unsigned int unknown28[2];
    int(__stdcall *play)(OP_AUDIO_BUFFER *, unsigned int, unsigned int, unsigned int);
    int(__stdcall *current_position)(OP_AUDIO_BUFFER *, unsigned int);
    unsigned int unknown38[3];
    int(__stdcall *frequency)(OP_AUDIO_BUFFER *, unsigned int);
    int(__stdcall *stop)(OP_AUDIO_BUFFER *);
    int(__stdcall *unlock)(OP_AUDIO_BUFFER *, void *, unsigned int, void *, unsigned int);
} OP_AUDIO_BUFFER_VT;
typedef struct OP_AUDIO_SPATIAL_VT
{
    unsigned int unknown00[2];
    unsigned int(__stdcall *release)(OP_AUDIO_SPATIAL *);
    unsigned int unknown0c[13];
    int(__stdcall *maximum)(OP_AUDIO_SPATIAL *, float, unsigned int);
    int(__stdcall *minimum)(OP_AUDIO_SPATIAL *, float, unsigned int);
    int(__stdcall *mode)(OP_AUDIO_SPATIAL *, unsigned int, unsigned int);
    int(__stdcall *position)(OP_AUDIO_SPATIAL *, float, float, float, unsigned int);
    int(__stdcall *velocity)(OP_AUDIO_SPATIAL *, float, float, float, unsigned int);
} OP_AUDIO_SPATIAL_VT;
typedef struct OP_AUDIO_LISTENER_VT
{
    unsigned int unknown00[17];
    int(__stdcall *commit)(OP_AUDIO_LISTENER *);
} OP_AUDIO_LISTENER_VT;
struct OP_AUDIO_DEVICE
{
    OP_AUDIO_DEVICE_VT *vt;
};
struct OP_AUDIO_BUFFER
{
    OP_AUDIO_BUFFER_VT *vt;
};
struct OP_AUDIO_SPATIAL
{
    OP_AUDIO_SPATIAL_VT *vt;
};
struct OP_AUDIO_LISTENER
{
    OP_AUDIO_LISTENER_VT *vt;
};
typedef char audio_layout
    [(sizeof(void *) == 4 && sizeof(OP_AUDIO_VECTOR) == 12 && offsetof(OP_AUDIO_DEVICE_VT, cooperative) == 0x18 &&
      offsetof(OP_AUDIO_BUFFER_VT, release) == 8 && offsetof(OP_AUDIO_BUFFER_VT, get_position) == 0x10 &&
      offsetof(OP_AUDIO_BUFFER_VT, get_status) == 0x24 && offsetof(OP_AUDIO_BUFFER_VT, play) == 0x30 &&
      offsetof(OP_AUDIO_BUFFER_VT, unlock) == 0x4c && offsetof(OP_AUDIO_BUFFER_VT, current_position) == 0x34 &&
      offsetof(OP_AUDIO_BUFFER_VT, frequency) == 0x44 && offsetof(OP_AUDIO_BUFFER_VT, stop) == 0x48 &&
      offsetof(OP_AUDIO_SPATIAL_VT, release) == 8 && offsetof(OP_AUDIO_SPATIAL_VT, maximum) == 0x40 &&
      offsetof(OP_AUDIO_SPATIAL_VT, minimum) == 0x44 && offsetof(OP_AUDIO_SPATIAL_VT, mode) == 0x48 &&
      offsetof(OP_AUDIO_SPATIAL_VT, position) == 0x4c && offsetof(OP_AUDIO_SPATIAL_VT, velocity) == 0x50 &&
      offsetof(OP_AUDIO_LISTENER_VT, commit) == 0x44)
         ? 1
         : -1];
extern unsigned int op_audio_gate_word;
extern OP_AUDIO_LISTENER *op_audio_listener;
extern int(__stdcall *op_audio_co_initialize)(void *);
extern void(__stdcall *op_audio_co_uninitialize)(void);
int op_audio_com_initialize(void);
void op_audio_com_uninitialize(void);
void op_audio_set_cooperative(OP_AUDIO_DEVICE *, unsigned int);
void op_audio_set_frequency(OP_AUDIO_BUFFER *, unsigned int);
void op_audio3d_set_distance_bounds(OP_AUDIO_SPATIAL *, float, float);
void op_audio3d_set_position(OP_AUDIO_SPATIAL *, const OP_AUDIO_VECTOR *);
void op_audio3d_set_velocity(OP_AUDIO_SPATIAL *, const OP_AUDIO_VECTOR *);
void op_audio_commit_listener(void);
void op_audio3d_set_mode(OP_AUDIO_SPATIAL *, unsigned int);
int op_audio_stop(OP_AUDIO_BUFFER *);
void op_audio_release_buffer(OP_AUDIO_BUFFER *);
void op_audio3d_release(OP_AUDIO_SPATIAL *);
int op_audio_pause(OP_AUDIO_BUFFER *);
int op_audio_play(OP_AUDIO_BUFFER *, int);
int op_audio_unlock(OP_AUDIO_BUFFER *, void *, unsigned int);
int op_audio_get_play_position(OP_AUDIO_BUFFER *, unsigned int *);
#endif
