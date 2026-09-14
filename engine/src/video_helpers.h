#ifndef OP_VIDEO_HELPERS_H
#define OP_VIDEO_HELPERS_H
#include <stddef.h>
typedef struct OP_VIDEO_DESC
{
    unsigned int mode, unknown04[7], format[17], surface;
} OP_VIDEO_DESC;
typedef struct OP_VIDEO
{
    void *handle;
    unsigned int surface_type;
    OP_VIDEO_DESC *description;
    unsigned int argument3, argument4, done;
} OP_VIDEO;
typedef char video_layout[(sizeof(void *) == 4 && sizeof(OP_VIDEO) == 24 && offsetof(OP_VIDEO, done) == 20 &&
                           sizeof(OP_VIDEO_DESC) == 104 && offsetof(OP_VIDEO_DESC, format) == 32 &&
                           offsetof(OP_VIDEO_DESC, surface) == 100)
                              ? 1
                              : -1];
extern void *op_video_sound_driver, *op_video_open_miles;
extern unsigned int(__stdcall *op_video_dd_type)(unsigned int);
extern int(__stdcall *op_video_sound_system)(void *, void *);
extern void *(__stdcall *op_video_bink_open)(const char *, unsigned int);
extern int(__stdcall *op_video_bink_close)(void *);
extern int(__stdcall *op_video_bink_volume)(void *, unsigned int);
void *op_sound_get_driver(void);
void *op_allocate(unsigned int);
void op_release(void *);
unsigned int op_video_surface_type(OP_VIDEO_DESC *);
OP_VIDEO *op_video_open(const char *, OP_VIDEO_DESC *, unsigned int, unsigned int);
int op_video_close(OP_VIDEO *);
void op_video_set_volume(OP_VIDEO *, unsigned int);
void op_video_set_done(OP_VIDEO *, unsigned int);
#endif
