#ifndef OP_PUPPET_TRACKS_H
#define OP_PUPPET_TRACKS_H
#include <stddef.h>
typedef void (*OP_TRACK_CALLBACK)(void *, unsigned int, int);
typedef struct OP_TRACK
{
    unsigned int flags;
    unsigned char unknown04[20];
    float weight, rate;
    unsigned char unknown20[256];
    float frame, previous;
    void *keyframe;
    OP_TRACK_CALLBACK callback;
    unsigned char tail[28];
} OP_TRACK;
typedef struct OP_THING
{
    unsigned char prefix[328];
    void *userdata;
} OP_THING;
typedef struct OP_PUPPET
{
    int paused;
    OP_THING *thing;
    OP_TRACK tracks[4];
} OP_PUPPET;
typedef char op_layout[(sizeof(void *) == 4 && sizeof(OP_TRACK) == 332 && offsetof(OP_TRACK, weight) == 24 &&
                        offsetof(OP_TRACK, rate) == 28 && offsetof(OP_TRACK, frame) == 288 &&
                        offsetof(OP_TRACK, previous) == 292 && offsetof(OP_TRACK, keyframe) == 296 &&
                        offsetof(OP_TRACK, callback) == 300 && offsetof(OP_THING, userdata) == 328 &&
                        offsetof(OP_PUPPET, tracks) == 8 && sizeof(OP_PUPPET) == 1336)
                           ? 1
                           : -1];
extern const float op_track_zero, op_track_one;
int op_puppet_init(OP_PUPPET *, OP_THING *);
int op_track_remove(OP_PUPPET *, unsigned int);
int op_track_play(OP_PUPPET *, unsigned int);
int op_track_fade_in(OP_PUPPET *, unsigned int, float);
int op_track_fade_out(OP_PUPPET *, unsigned int, float);
#endif
