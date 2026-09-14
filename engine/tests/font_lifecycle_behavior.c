#include "../src/font_state.h"
#include "../src/sprite_resources.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
char op_system_font_name[] = "font";
static char fl_input_name[] = "font";
typedef struct FL_SPRITE
{
    unsigned int before;
    OP_SPRITE value;
    unsigned int after;
} FL_SPRITE;
typedef struct FL_FONT
{
    unsigned int before;
    OP_FONT_RESOURCE value;
    unsigned int after;
} FL_FONT;
static FL_SPRITE fl_sprites[4], fl_expected_sprites[4];
static FL_FONT fl_fonts[3], fl_expected_fonts[3];
static OP_MATERIAL fl_materials[2], fl_expected_materials[2];
static OP_FONT_POOL fl_expected;
static OP_FONT3D *fl_current;
static int fl_active, fl_checks, fl_failures, fl_mode, fl_index, fl_stage, fl_acquire_ok, fl_load_ok, fl_retarget,
    fl_has_sprite, fl_has_backdrop, fl_defaults, fl_freed, fl_expected_slot;
static char *fl_name, *fl_buffer;
enum
{
    FL_CREATE = 1,
    FL_DESTROY = 2
};
static void fl_check(int ok, int line)
{
    ++fl_checks;
    if (!ok)
    {
        ++fl_failures;
        printf("font lifecycle failed at %d\n", line);
    }
}
#define FL_CHECK(x) fl_check(!!(x), __LINE__)
static void fl_verify(void)
{
    FL_CHECK(memcmp(&op_font_pool, &fl_expected, sizeof(fl_expected)) == 0);
    FL_CHECK(memcmp(fl_sprites, fl_expected_sprites, sizeof(fl_sprites)) == 0);
    FL_CHECK(memcmp(fl_fonts, fl_expected_fonts, sizeof(fl_fonts)) == 0);
    FL_CHECK(memcmp(fl_materials, fl_expected_materials, sizeof(fl_materials)) == 0);
    FL_CHECK(op_current_font == fl_current);
    FL_CHECK(op_get_system_font() == fl_expected_slot);
}
static void fl_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    memset(fl_sprites, 0x47, sizeof(fl_sprites));
    memset(fl_fonts, 0x58, sizeof(fl_fonts));
    memset(fl_materials, 0x39, sizeof(fl_materials));
    for (i = 0; i < 4; ++i)
        fl_sprites[i].value.material = &fl_materials[i % 2];
    for (i = 0; i < 3; ++i)
        fl_fonts[i].value.material = &fl_materials[i % 2];
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].sprite = &fl_sprites[0].value;
        op_font_pool.rows[i].font = &fl_fonts[0].value;
        op_font_pool.rows[i].backdrop = &fl_sprites[2].value;
    }
    op_current_font = fl_current = selected < 0 ? 0 : &op_font_pool.rows[selected];
    op_system_font = fl_expected_slot = 7;
    memcpy(&fl_expected, &op_font_pool, sizeof(fl_expected));
    memcpy(fl_expected_sprites, fl_sprites, sizeof(fl_sprites));
    memcpy(fl_expected_fonts, fl_fonts, sizeof(fl_fonts));
    memcpy(fl_expected_materials, fl_materials, sizeof(fl_materials));
    fl_stage = fl_freed = fl_defaults = 0;
    fl_buffer = 0;
    fl_name = fl_input_name;
}
static void fl_predict_defaults(void)
{
    int j;
    if (fl_defaults)
        return;
    for (j = 0; j < 4; ++j)
        fl_expected.rows[fl_index].colours[j] = 0xffffffffu;
    fl_expected.rows[fl_index].align = 2;
    fl_expected.rows[fl_index].glyph_x = fl_expected.rows[fl_index].glyph_y = 1.0f;
    fl_expected.rows[fl_index].position_x = fl_expected.rows[fl_index].position_y = 1.0f;
    fl_expected.rows[fl_index].word20 = 0;
    fl_expected.rows[fl_index].backdrop_width = fl_expected.rows[fl_index].backdrop_height = 0.0f;
    fl_current = &op_font_pool.rows[fl_index];
    fl_defaults = 1;
}
static int fl_format_sprite(char *dest, char *format, char *base)
{
    FL_CHECK(fl_mode == FL_CREATE && fl_stage == 0);
    FL_CHECK(base == fl_name && format == op_sprite_format && dest != base);
    fl_verify();
    memcpy(dest, "unit:font", 10);
    fl_buffer = dest;
    fl_stage = 1;
    return 9;
}
static void *fl_acquire_resource(unsigned int type, char *name)
{
    FL_CHECK(fl_mode == FL_CREATE && fl_stage == 1);
    FL_CHECK(type == 0x53505254U && name == fl_buffer && strcmp(name, "unit:font") == 0);
    fl_verify();
    op_current_font = fl_current = &op_font_pool.rows[15];
    op_system_font = fl_expected_slot = 77;
    op_font_pool.rows[fl_index].word20 = fl_expected.rows[fl_index].word20 = 0x12345678u;
    fl_expected.rows[fl_index].sprite = fl_acquire_ok ? &fl_sprites[0].value : 0;
    fl_stage = 2;
    fl_buffer = 0;
    return fl_acquire_ok ? &fl_sprites[0].value : 0;
}
void *op_load_font_resource(char *name, void *material)
{
    FL_CHECK(fl_mode == FL_CREATE && fl_stage == 2);
    FL_CHECK(name == fl_name && material == &fl_materials[0]);
    fl_verify();
    op_font_pool.rows[fl_index].font = &fl_fonts[1].value;
    fl_expected.rows[fl_index].font = fl_load_ok ? &fl_fonts[0].value : 0;
    fl_stage = 3;
    return fl_load_ok ? &fl_fonts[0].value : 0;
}
static void fl_release_resource(void *resource)
{
    if (fl_mode == FL_CREATE)
    {
        FL_CHECK(fl_stage == 3);
        if (fl_load_ok)
        {
            fl_predict_defaults();
            FL_CHECK(resource == &fl_sprites[2].value);
        }
        else
            FL_CHECK(resource == &fl_sprites[0].value);
        fl_verify();
        if (fl_load_ok)
        {
            op_font_pool.rows[fl_index].backdrop = &fl_sprites[1].value;
            fl_expected.rows[fl_index].backdrop = 0;
            if (fl_retarget)
                op_current_font = fl_current = &op_font_pool.rows[(fl_index + 1) % 16];
        }
        else
        {
            op_font_pool.rows[fl_index].sprite = &fl_sprites[1].value;
            fl_expected.rows[fl_index].sprite = 0;
            op_current_font = fl_current = &op_font_pool.rows[14];
        }
        fl_stage = 4;
        return;
    }
    FL_CHECK(fl_mode == FL_DESTROY);
    if (!fl_freed)
    {
        FL_CHECK(fl_has_sprite && resource == &fl_sprites[0].value);
        fl_verify();
        op_font_pool.rows[fl_index].sprite = &fl_sprites[1].value;
        fl_expected.rows[fl_index].sprite = 0;
        if (fl_retarget)
            op_font_pool.rows[fl_index].font = fl_expected.rows[fl_index].font = &fl_fonts[1].value;
        op_current_font = fl_current = &op_font_pool.rows[14];
        ++fl_stage;
    }
    else
    {
        FL_CHECK(resource == (fl_retarget ? &fl_sprites[3].value : &fl_sprites[2].value));
        fl_verify();
        op_font_pool.rows[fl_index].backdrop = &fl_sprites[1].value;
        fl_expected.rows[fl_index].backdrop = 0;
        op_current_font = fl_current = &op_font_pool.rows[13];
        ++fl_stage;
    }
}
void op_free_font_resource(void *font)
{
    int index = fl_retarget && fl_has_sprite ? 1 : 0;
    FL_CHECK(fl_mode == FL_DESTROY && !fl_freed && font == &fl_fonts[index].value);
    fl_expected_fonts[index].value.material = 0;
    fl_verify();
    op_font_pool.rows[fl_index].font = &fl_fonts[2].value;
    fl_expected.rows[fl_index].font = 0;
    if (fl_retarget)
        op_font_pool.rows[fl_index].backdrop = fl_expected.rows[fl_index].backdrop = &fl_sprites[3].value;
    op_system_font = fl_expected_slot = 99;
    fl_freed = 1;
}
static void fl_create_case(int free_index, int acquire_ok, int load_ok, int backdrop, int retarget, int module)
{
    int result, destination;
    fl_seed(3);
    fl_mode = FL_CREATE;
    fl_index = free_index;
    fl_acquire_ok = acquire_ok;
    fl_load_ok = load_ok;
    fl_has_backdrop = backdrop;
    fl_retarget = retarget;
    if (module)
    {
        fl_index = 0;
        fl_name = op_system_font_name;
        op_system_font = fl_expected_slot = -1;
        memset(&fl_expected, 0, sizeof(fl_expected));
    }
    else if (free_index < 16)
    {
        op_font_pool.rows[free_index].font = fl_expected.rows[free_index].font = 0;
        if (!backdrop)
            op_font_pool.rows[free_index].backdrop = fl_expected.rows[free_index].backdrop = 0;
    }
    result = module ? op_font_module(3) : op_font_create(fl_name);
    if (free_index < 16 && acquire_ok && load_ok)
    {
        fl_predict_defaults();
        fl_expected.rows[fl_index].backdrop = 0;
        destination = (backdrop && retarget && !module) ? (fl_index + 1) % 16 : fl_index;
        fl_expected.rows[destination].z_start = 0.0f;
        fl_expected.rows[destination].z_end = 1.0f;
        fl_expected.rows[destination].word24 = 1;
    }
    if (module)
        fl_expected_slot = acquire_ok && load_ok ? 0 : -1;
    FL_CHECK(result == (module ? 0 : (free_index < 16 && acquire_ok && load_ok ? free_index : -1)));
    FL_CHECK(fl_stage == (free_index == 16 ? 0 : (!acquire_ok ? 2 : (!load_ok ? 4 : ((backdrop && !module) ? 4 : 3)))));
    FL_CHECK(fl_buffer == 0);
    fl_verify();
}
static void fl_destroy_case(int index, int selected, int sprite, int backdrop, int retarget, int module)
{
    fl_seed(selected ? index : (index + 1) % 16);
    fl_mode = FL_DESTROY;
    fl_index = index;
    fl_has_sprite = sprite;
    fl_has_backdrop = backdrop;
    fl_retarget = retarget;
    if (!sprite)
        op_font_pool.rows[index].sprite = fl_expected.rows[index].sprite = 0;
    if (!backdrop)
        op_font_pool.rows[index].backdrop = fl_expected.rows[index].backdrop = 0;
    if (selected)
        fl_current = 0;
    if (module)
    {
        op_system_font = fl_expected_slot = index;
        FL_CHECK(op_font_module(4) == 0);
        fl_expected_slot = -1;
    }
    else
        op_font_destroy(index);
    FL_CHECK(fl_freed == 1);
    FL_CHECK(fl_stage == sprite + (backdrop || retarget));
    fl_verify();
}
static int op_test_font_lifecycle(void)
{
    int i, a, l, b, r, sel;
    static int invalid[] = {INT_MIN, -1, 16, INT_MAX};
    static int events[] = {INT_MIN, -1, 0, 2, 5, INT_MAX};
    fl_active = 1;
    for (i = 0; i <= 16; ++i)
        for (a = 0; a < 2; ++a)
            for (l = 0; l < 2; ++l)
                for (b = 0; b < 2; ++b)
                    for (r = 0; r < 2; ++r)
                        fl_create_case(i, a, l, b, r, 0);
    for (i = 0; i < 16; ++i)
        for (sel = 0; sel < 2; ++sel)
            for (a = 0; a < 2; ++a)
                for (b = 0; b < 2; ++b)
                    for (r = 0; r < 2; ++r)
                        fl_destroy_case(i, sel, a, b, r, 0);
    for (a = 0; a < 2; ++a)
        for (l = 0; l < 2; ++l)
            fl_create_case(0, a, l, 0, 0, 1);
    for (i = 0; i < 16; i += 15)
        for (a = 0; a < 2; ++a)
            for (b = 0; b < 2; ++b)
                for (r = 0; r < 2; ++r)
                    fl_destroy_case(i, 1, a, b, r, 1);
    for (i = 0; i < 4; ++i)
    {
        fl_seed(3);
        fl_mode = FL_DESTROY;
        op_font_destroy(invalid[i]);
        FL_CHECK(fl_stage == 0 && fl_freed == 0);
        fl_verify();
        op_system_font = fl_expected_slot = invalid[i];
        FL_CHECK(op_font_module(4) == 0);
        fl_expected_slot = -1;
        FL_CHECK(fl_stage == 0 && fl_freed == 0);
        fl_verify();
    }
    for (i = 0; i < 4; ++i)
    {
        fl_seed(3);
        op_system_font = fl_expected_slot = i == 3 ? INT_MAX : i;
        memset(&fl_expected, 0, sizeof(fl_expected));
        FL_CHECK(op_font_module(3) == 0);
        FL_CHECK(fl_stage == 0 && fl_freed == 0);
        fl_verify();
    }
    for (i = 0; i < 6; ++i)
    {
        fl_seed(3);
        FL_CHECK(op_font_module(events[i]) == 2);
        FL_CHECK(fl_stage == 0 && fl_freed == 0);
        fl_verify();
    }
    fl_seed(3);
    FL_CHECK(op_sprite_get_material(0) == 0);
    for (i = 0; i < 4; ++i)
        FL_CHECK(op_sprite_get_material(&fl_sprites[i].value) == &fl_materials[i % 2]);
    fl_verify();
    fl_sprites[0].value.material = fl_expected_sprites[0].value.material = 0;
    FL_CHECK(op_sprite_get_material(&fl_sprites[0].value) == 0);
    fl_verify();
    fl_active = 0;
    printf("font lifecycle connected: %d checks, %d failures\n", fl_checks, fl_failures);
    return fl_failures != 0;
}
