#ifndef OP_CAMPAIGN_011_LIGHT_DISABLE_WORLD_API_H
#define OP_CAMPAIGN_011_LIGHT_DISABLE_WORLD_API_H
typedef struct OP_LIGHT_RECORD {
    unsigned char opaque00[0x74];
    int active;
    unsigned char opaque78[8];
} OP_LIGHT_RECORD;
typedef struct OP_LIGHT_WORLD {
    unsigned char opaque000[0x1c4];
    int count;
    unsigned int opaque1c8;
    OP_LIGHT_RECORD *records;
} OP_LIGHT_WORLD;
void op_light_deactivate_record(OP_LIGHT_WORLD *world, int bank,
                                OP_LIGHT_RECORD *record);
void op_light_disable_world(OP_LIGHT_WORLD *world, int index);
#endif
