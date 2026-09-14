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
typedef struct OWNED_FONT
{
    unsigned int before;
    OP_FONT_RESOURCE value;
    unsigned int after;
} OWNED_FONT;
static OWNED_FONT fonts[3], expected_fonts[3];
static int row, stage, mutate, selected_mode;
int op_release_sprite(void **slot)
{
    CHECK(stage == 0 || stage == 2);
    CHECK(slot == (stage == 0 ? &op_font_pool.rows[row].sprite : &op_font_pool.rows[row].backdrop));
    CHECK(op_current_font ==
          (stage == 0 ? (selected_mode ? 0 : &op_font_pool.rows[(row + 1) % 16]) : &op_font_pool.rows[14]));
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    CHECK(memcmp(fonts, expected_fonts, sizeof(fonts)) == 0);
    *slot = 0;
    if (stage == 0)
    {
        expected.rows[row].sprite = 0;
        if (mutate)
            op_font_pool.rows[row].font = expected.rows[row].font = &fonts[1].value;
        op_current_font = &op_font_pool.rows[14];
    }
    else
    {
        expected.rows[row].backdrop = 0;
        op_current_font = &op_font_pool.rows[13];
    }
    ++stage;
    return 1;
}
void op_free_font_resource(void *font)
{
    CHECK(stage == 1);
    CHECK(font == &fonts[mutate ? 1 : 0].value);
    expected_fonts[mutate ? 1 : 0].value.material = 0;
    CHECK(memcmp(fonts, expected_fonts, sizeof(fonts)) == 0);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    op_font_pool.rows[row].font = expected.rows[row].font = &fonts[2].value;
    op_font_pool.rows[row].backdrop = expected.rows[row].backdrop = &tokens[31];
    expected.rows[row].font = 0;
    stage = 2;
}
int main(void)
{
    int index, sel, m;
    static int invalid[] = {INT_MIN, -1, 16, INT_MAX};
    for (index = 0; index < 4; ++index)
    {
        seed(0);
        stage = 0;
        op_font_destroy(invalid[index]);
        CHECK(stage == 0 && op_current_font == &op_font_pool.rows[0]);
        CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    }
    for (row = 0; row < 16; ++row)
        for (sel = 0; sel < 2; ++sel)
            for (m = 0; m < 2; ++m)
            {
                seed(sel ? row : (row + 1) % 16);
                selected_mode = sel;
                mutate = m;
                stage = 0;
                memset(fonts, 0x79, sizeof(fonts));
                memcpy(expected_fonts, fonts, sizeof(fonts));
                op_font_pool.rows[row].font = expected.rows[row].font = &fonts[0].value;
                op_font_destroy(row);
                CHECK(stage == 3);
                CHECK(op_current_font == &op_font_pool.rows[13]);
                CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
                CHECK(memcmp(fonts, expected_fonts, sizeof(fonts)) == 0);
            }
    printf("font_destroy: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
