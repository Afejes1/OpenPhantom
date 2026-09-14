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
static int row, stage, retarget;
static float wanted[4];
void op_get_last_textbox(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d)
{
    static float bounds[] = {1.0f, 3.0f, 2.0f, 5.0f};
    CHECK(stage == 0 && a && b && c && d);
    memcpy(a, &bounds[0], 4);
    memcpy(b, &bounds[1], 4);
    memcpy(c, &bounds[2], 4);
    memcpy(d, &bounds[3], 4);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    if (retarget)
        op_current_font = &op_font_pool.rows[(row + 1) % 16];
    op_font_screen_width = 4.0f;
    op_font_screen_height = 2.0f;
    stage = 1;
}
void op_draw_font_backdrop(void *resource, float left, float right, float top, float bottom)
{
    CHECK(stage == 1);
    CHECK(resource == &tokens[(row + retarget) % 16 + 2]);
    CHECK(left == wanted[0] && right == wanted[1] && top == wanted[2] && bottom == wanted[3]);
    CHECK(op_current_font == &op_font_pool.rows[(row + retarget) % 16]);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    op_current_font = &op_font_pool.rows[15];
    stage = 2;
}
int main(void)
{
    int r, has, t, i;
    for (r = -1; r < 16; ++r)
        for (has = 0; has < 2; ++has)
            for (t = 0; t < 2; ++t)
            {
                seed(r);
                row = r;
                retarget = t;
                stage = 0;
                for (i = 0; i < 16; ++i)
                {
                    op_font_pool.rows[i].backdrop_width = expected.rows[i].backdrop_width = 0.5f;
                    op_font_pool.rows[i].backdrop_height = expected.rows[i].backdrop_height = -0.25f;
                }
                if (r >= 0)
                {
                    op_font_pool.rows[(r + 1) % 16].backdrop_width = expected.rows[(r + 1) % 16].backdrop_width = 1.0f;
                    op_font_pool.rows[(r + 1) % 16].backdrop_height = expected.rows[(r + 1) % 16].backdrop_height =
                        0.5f;
                }
                if (r >= 0 && !has)
                    op_font_pool.rows[r].backdrop = expected.rows[r].backdrop = 0;
                op_font_screen_width = 9.0f;
                op_font_screen_height = 7.0f;
                wanted[0] = t ? -3.0f : -1.0f;
                wanted[1] = t ? 7.0f : 5.0f;
                wanted[2] = t ? 1.0f : 2.5f;
                wanted[3] = t ? 6.0f : 4.5f;
                op_font_draw_backdrop();
                CHECK(stage == (r >= 0 && has ? 2 : 0));
                CHECK(op_current_font ==
                      (r >= 0 && has ? &op_font_pool.rows[15] : (r < 0 ? 0 : &op_font_pool.rows[r])));
                CHECK(op_font_screen_width == (r >= 0 && has ? 4.0f : 9.0f));
                CHECK(op_font_screen_height == (r >= 0 && has ? 2.0f : 7.0f));
                CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
            }
    printf("font_draw_backdrop: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
