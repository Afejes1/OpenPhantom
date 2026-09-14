#ifndef OP_MATERIAL_HOOKS_H
#define OP_MATERIAL_HOOKS_H
#include "sprite_resources.h"
typedef OP_MATERIAL *(*OP_MATERIAL_LOAD_HOOK)(char *, void *, void *);
typedef void (*OP_MATERIAL_UNLOAD_HOOK)(OP_MATERIAL *);
extern OP_MATERIAL_LOAD_HOOK op_material_load_hook;
extern OP_MATERIAL_UNLOAD_HOOK op_material_unload_hook;
OP_MATERIAL_LOAD_HOOK op_material_set_load_hook(OP_MATERIAL_LOAD_HOOK);
OP_MATERIAL_UNLOAD_HOOK op_material_set_unload_hook(OP_MATERIAL_UNLOAD_HOOK);
#endif
