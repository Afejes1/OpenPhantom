#ifndef OP_CAMPAIGN_017_WORLD_FREE_SCRIPTS_API_H
#define OP_CAMPAIGN_017_WORLD_FREE_SCRIPTS_API_H
typedef struct OP_SCRIPT { unsigned char opaque[8]; } OP_SCRIPT;
typedef struct OP_WORLD { unsigned char a[0x1f8]; int script_count; unsigned char b[4]; OP_SCRIPT **scripts; } OP_WORLD;
void op_free_script_data(OP_SCRIPT *script);
void op_world_free_scripts(OP_WORLD *world);
#endif
