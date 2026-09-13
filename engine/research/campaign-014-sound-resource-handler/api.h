#ifndef OP_CAMPAIGN_014_SOUND_RESOURCE_HANDLER_API_H
#define OP_CAMPAIGN_014_SOUND_RESOURCE_HANDLER_API_H

typedef struct OP_SOUND_RESOURCE_EVENT {
    int operation;
    const char *name;
    unsigned int resource_type;
    void *data;
    unsigned int size;
} OP_SOUND_RESOURCE_EVENT;

typedef struct OP_SOUND_FILE_API {
    unsigned char opaque_00[0x30];
    int (__cdecl *open)(const char *name, const char *mode);
    int (__cdecl *close)(int handle);
    int (__cdecl *read)(int handle, void *buffer, unsigned int size);
    unsigned char opaque_3c[0x14];
    unsigned int (__cdecl *size)(const char *name);
} OP_SOUND_FILE_API;

extern OP_SOUND_FILE_API *op_sound_file_api;
extern const char op_sound_read_mode[];
extern unsigned int op_sound_allocation_tag;

void *op_sound_allocate(unsigned int tag, unsigned int size);
void op_sound_free_resource(void *data);
unsigned int op_sound_resource_size(void *data);
int op_sound_register_loaded(unsigned int type, const char *name, void *buffer);
int op_sound_resource_handler(OP_SOUND_RESOURCE_EVENT *event);

#endif
