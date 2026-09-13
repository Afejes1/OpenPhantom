#ifndef OP_CAMPAIGN_015_EXTENDED_ATTACH_COLORMAP_API_H
#define OP_CAMPAIGN_015_EXTENDED_ATTACH_COLORMAP_API_H

typedef struct OP_EXTENDED_MATERIAL {
    unsigned char opaque_00[0x60];
    void *colormap;
} OP_EXTENDED_MATERIAL;

typedef struct OP_EXTENDED_MODEL {
    unsigned char opaque_00[0x48];
    OP_EXTENDED_MATERIAL **materials;
    unsigned int material_count;
} OP_EXTENDED_MODEL;

void *op_extended_load_colormap(const char *name);
void op_extended_attach_colormap(OP_EXTENDED_MODEL *model, const char *name);

#endif
