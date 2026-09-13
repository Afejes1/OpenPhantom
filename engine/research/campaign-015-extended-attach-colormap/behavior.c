#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "api.h"

typedef char check_model_materials[(offsetof(OP_EXTENDED_MODEL, materials) == 0x48) ? 1 : -1];
typedef char check_model_count[(offsetof(OP_EXTENDED_MODEL, material_count) == 0x4c) ? 1 : -1];
typedef char check_material_colormap[(offsetof(OP_EXTENDED_MATERIAL, colormap) == 0x60) ? 1 : -1];

static int checks;
static int failures;

static void check(int expression)
{
    ++checks;
    if (!expression) {
        ++failures;
    }
}

static unsigned char loaded_storage[64];
static OP_EXTENDED_MODEL *callback_model;
static OP_EXTENDED_MATERIAL **callback_materials;
static unsigned int callback_count;
static int loader_mode;
static int loader_calls;
static const char *seen_name;

void *op_extended_load_colormap(const char *name)
{
    ++loader_calls;
    seen_name = name;
    if (loader_mode == 0) {
        return 0;
    }
    if (loader_mode == 2) {
        callback_model->materials = callback_materials;
        callback_model->material_count = callback_count;
    }
    return loaded_storage;
}

static void reset_loader(int mode)
{
    loader_mode = mode;
    loader_calls = 0;
    seen_name = 0;
}

int main(void)
{
    OP_EXTENDED_MODEL model;
    OP_EXTENDED_MODEL expected_model;
    OP_EXTENDED_MATERIAL material[3];
    OP_EXTENDED_MATERIAL expected_material[3];
    OP_EXTENDED_MATERIAL *initial_list[1];
    OP_EXTENDED_MATERIAL *expanded_list[4];
    const char failed_name[] = "failed";
    const char mixed_name[] = "mixed";
    const char changed_name[] = "changed";
    void *existing;
    OP_EXTENDED_MATERIAL *expected_initial[1];
    OP_EXTENDED_MATERIAL *expected_expanded[4];
    unsigned char expected_storage[64];

    memset(loaded_storage, 0x7c, sizeof(loaded_storage));
    memset(expected_storage, 0x7c, sizeof(expected_storage));
    reset_loader(0);
    op_extended_attach_colormap(0, failed_name);
    check(loader_calls == 1);
    check(seen_name == failed_name);

    memset(&model, 0x41, sizeof(model));
    model.materials = 0;
    model.material_count = 0;
    expected_model = model;
    reset_loader(1);
    op_extended_attach_colormap(&model, mixed_name);
    check(loader_calls == 1);
    check(seen_name == mixed_name);
    check(memcmp(&model, &expected_model, sizeof(model)) == 0);

    memset(material, 0x52, sizeof(material));
    existing = loaded_storage + 7;
    material[0].colormap = 0;
    material[1].colormap = existing;
    material[2].colormap = 0;
    expanded_list[0] = &material[0];
    expanded_list[1] = &material[1];
    expanded_list[2] = &material[0];
    expanded_list[3] = &material[2];
    initial_list[0] = &material[1];
    memset(&model, 0x63, sizeof(model));
    model.materials = initial_list;
    model.material_count = 1;
    callback_model = &model;
    callback_materials = expanded_list;
    callback_count = 4;
    expected_model = model;
    expected_model.materials = expanded_list;
    expected_model.material_count = 4;
    memcpy(expected_material, material, sizeof(material));
    expected_material[0].colormap = loaded_storage + 0x30;
    expected_material[1].colormap = existing;
    expected_material[2].colormap = loaded_storage + 0x30;
    memcpy(expected_initial, initial_list, sizeof(initial_list));
    memcpy(expected_expanded, expanded_list, sizeof(expanded_list));
    reset_loader(2);
    op_extended_attach_colormap(&model, changed_name);

    check(loader_calls == 1);
    check(seen_name == changed_name);
    check(memcmp(&model, &expected_model, sizeof(model)) == 0);
    check(memcmp(material, expected_material, sizeof(material)) == 0);
    check(memcmp(loaded_storage, expected_storage, sizeof(loaded_storage)) == 0);
    check(memcmp(initial_list, expected_initial, sizeof(initial_list)) == 0);
    check(memcmp(expanded_list, expected_expanded, sizeof(expanded_list)) == 0);

    printf("%d checks, %d failures\n", checks, failures);
    return failures != 0;
}