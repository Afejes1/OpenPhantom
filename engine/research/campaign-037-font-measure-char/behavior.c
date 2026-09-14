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
static int row, retarget, stage, answer;
static unsigned char wanted_char;
static float w_answer, h_answer;
void op_set_font_scale(float x, float y)
{
    CHECK(stage == 0 && x == 0.5f && y == -2.0f);
    CHECK(op_current_font == &op_font_pool.rows[row]);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    if (retarget)
        op_current_font = &op_font_pool.rows[(row + 1) % 16];
    stage = 1;
}
int op_font_char_size(void *font, char ch, float *w, float *h)
{
    CHECK(stage == 1);
    CHECK(font == &tokens[(row + retarget) % 16]);
    CHECK((unsigned char)ch == wanted_char);
    CHECK(w && h && w != h);
    *w = w_answer;
    *h = h_answer;
    op_current_font = &op_font_pool.rows[15];
    stage = 2;
    return answer;
}
int main(void)
{
    int r, t, mode, a, c;
    float out[4], want[4], *w, *h;
    static unsigned char chars[] = {0, 1, 32, 127, 128, 255};
    static int answers[] = {0, 1, -1, INT_MIN};
    for (r = -1; r < 16; ++r)
        for (t = 0; t < 2; ++t)
            for (mode = 0; mode < 5; ++mode)
                for (a = 0; a < 4; ++a)
                    for (c = 0; c < 6; ++c)
                    {
                        seed(r);
                        row = r;
                        retarget = t;
                        stage = 0;
                        answer = answers[a];
                        wanted_char = chars[c];
                        w_answer = 0.5f;
                        h_answer = -2.0f;
                        if (r >= 0)
                        {
                            op_font_pool.rows[r].glyph_x = expected.rows[r].glyph_x = 0.5f;
                            op_font_pool.rows[r].glyph_y = expected.rows[r].glyph_y = -2.0f;
                        }
                        out[0] = out[1] = out[2] = out[3] = 17.0f;
                        memcpy(want, out, sizeof(want));
                        w = (mode == 1 || mode == 3) ? 0 : &out[1];
                        h = (mode == 2 || mode == 3) ? 0 : &out[mode == 4 ? 1 : 2];
                        if (r >= 0 && answer)
                        {
                            if (w)
                                want[1] = w_answer;
                            if (h)
                                want[mode == 4 ? 1 : 2] = h_answer;
                        }
                        CHECK(op_font_measure_char((char)wanted_char, w, h) == (r >= 0 && answer != 0));
                        CHECK(stage == (r < 0 ? 0 : 2));
                        CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[15]));
                        CHECK(memcmp(out, want, sizeof(out)) == 0);
                        CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
                    }
    printf("font_measure_char: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
