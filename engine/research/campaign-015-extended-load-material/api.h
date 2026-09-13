#ifndef OP_CAMPAIGN_015_EXTENDED_LOAD_MATERIAL_API_H
#define OP_CAMPAIGN_015_EXTENDED_LOAD_MATERIAL_API_H

extern const char op_default_material_name[];
void *op_load_mat_resource(const char *name, void *arg1, void *arg2);
int op_extended_load_material(const char *name, void **out);

#endif
