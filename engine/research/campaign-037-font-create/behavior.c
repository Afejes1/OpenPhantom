#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_FONT_POOL op_font_pool;
OP_FONT3D *op_current_font;
int op_system_font_slot;
char op_system_font_name[] = "authored-default";
float op_font_screen_width, op_font_screen_height;
static OP_FONT_POOL expected;
static unsigned int tokens[32];
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
static void seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &tokens[i];
        op_font_pool.rows[i].sprite = &tokens[i + 1];
        op_font_pool.rows[i].backdrop = &tokens[i + 2];
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&expected, &op_font_pool, sizeof(expected));
}
static int free_index, stage, acquire_ok, load_ok, mutate;
static char input_name[] = "authored-font";
static void verify(void)
{
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
}
void *op_acquire_sprite(char *name)
{
    CHECK(stage == 0 && name == input_name);
    verify();
    op_current_font = &op_font_pool.rows[15];
    stage = 1;
    return acquire_ok ? &tokens[20] : 0;
}
void *op_sprite_get_material(void *sprite)
{
    expected.rows[free_index].sprite = &tokens[20];
    CHECK(stage == 1 && sprite == &tokens[20]);
    verify();
    if (mutate)
        op_font_pool.rows[free_index].sprite = expected.rows[free_index].sprite = &tokens[21];
    stage = 2;
    return &tokens[22];
}
void *op_load_font_resource(char *name, void *material)
{
    CHECK(stage == 2 && name == input_name && material == &tokens[22]);
    verify();
    stage = 3;
    return load_ok ? &tokens[23] : 0;
}
int op_release_sprite(void **slot)
{
    CHECK(stage == 3 && slot == &op_font_pool.rows[free_index].sprite);
    verify();
    *slot = 0;
    expected.rows[free_index].sprite = 0;
    op_current_font = &op_font_pool.rows[14];
    stage = 4;
    return 1;
}
void op_font_select(int index)
{
    expected.rows[free_index].font = &tokens[23];
    CHECK(stage == 3 && index == free_index);
    verify();
    op_current_font = &op_font_pool.rows[index];
    stage = 4;
}
void op_font_set_colour(unsigned int word)
{
    CHECK(stage == 4 && word == 0xffffffffu);
    verify();
    op_current_font = &op_font_pool.rows[13];
    stage = 5;
}
void op_font_set_align(int align)
{
    CHECK(stage == 5 && align == 1);
    verify();
    stage = 6;
}
void op_font_set_glyph_scale(float x, float y)
{
    CHECK(stage == 6 && x == 1.0f && y == 1.0f);
    verify();
    stage = 7;
}
void op_font_set_position_scale(float x, float y)
{
    CHECK(stage == 7 && x == 1.0f && y == 1.0f);
    verify();
    stage = 8;
}
void op_font_set_word20(unsigned int word)
{
    CHECK(stage == 8 && word == 0);
    verify();
    stage = 9;
}
int op_font_set_backdrop(char *name, float x, float y)
{
    CHECK(stage == 9 && name == 0 && x == 0.0f && y == 0.0f);
    verify();
    stage = 10;
    return 0;
}
int op_font_set_zrange(float x, float y)
{
    CHECK(stage == 10 && x == 0.0f && y == 1.0f);
    verify();
    stage = 11;
    return 0;
}
int op_font_set_word24(unsigned int word)
{
    CHECK(stage == 11 && word == 1);
    verify();
    stage = 12;
    return 0;
}
int main(void)
{
    int f, a, l, m, result;
    for (f = 0; f <= 16; ++f)
        for (a = 0; a < 2; ++a)
            for (l = 0; l < 2; ++l)
                for (m = 0; m < 2; ++m)
                {
                    seed(3);
                    free_index = f;
                    acquire_ok = a;
                    load_ok = l;
                    mutate = m;
                    stage = 0;
                    if (f < 16)
                    {
                        op_font_pool.rows[f].font = expected.rows[f].font = 0;
                        if (f < 15)
                            op_font_pool.rows[15].font = expected.rows[15].font = 0;
                    }
                    result = op_font_create(input_name);
                    if (f < 16 && !a)
                        expected.rows[f].sprite = 0;
                    CHECK(result == (f < 16 && a && l ? f : -1));
                    CHECK(stage == (f == 16 ? 0 : (!a ? 1 : (!l ? 4 : 12))));
                    CHECK(op_current_font == &op_font_pool.rows[f == 16 ? 3 : (!a ? 15 : (!l ? 14 : 13))]);
                    verify();
                }
    printf("font_create: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
