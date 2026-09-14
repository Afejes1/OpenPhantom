#include "../src/font_state.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static OP_FONT_POOL mc_expected;
static unsigned int mc_tokens[32];
static int mc_checks, mc_failures;
static void mc_check(int ok, int line)
{
    ++mc_checks;
    if (!ok)
    {
        ++mc_failures;
        printf("line %d failed\n", line);
    }
}
#define mc_CHECK(x) mc_check(!!(x), __LINE__)
static void mc_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &mc_tokens[i];
        op_font_pool.rows[i].sprite = &mc_tokens[i + 1];
        op_font_pool.rows[i].backdrop = &mc_tokens[i + 2];
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&mc_expected, &op_font_pool, sizeof(mc_expected));
}
static int mc_row, mc_retarget, mc_stage, mc_answer;
static unsigned char mc_wanted_char;
static float mc_w_answer, mc_h_answer;
static void mc_op_set_font_scale(float x, float y)
{
    mc_CHECK(mc_stage == 0 && x == 0.5f && y == -2.0f);
    mc_CHECK(op_current_font == &op_font_pool.rows[mc_row]);
    mc_CHECK(memcmp(&op_font_pool, &mc_expected, sizeof(mc_expected)) == 0);
    if (mc_retarget)
        op_current_font = &op_font_pool.rows[(mc_row + 1) % 16];
    mc_stage = 1;
}
static int mc_op_font_char_size(void *font, char ch, float *w, float *h)
{
    mc_CHECK(mc_stage == 1);
    mc_CHECK(font == &mc_tokens[(mc_row + mc_retarget) % 16]);
    mc_CHECK((unsigned char)ch == mc_wanted_char);
    mc_CHECK(w && h && w != h);
    *w = mc_w_answer;
    *h = mc_h_answer;
    op_current_font = &op_font_pool.rows[15];
    mc_stage = 2;
    return mc_answer;
}
static int mc_main(void)
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
                        mc_seed(r);
                        mc_row = r;
                        mc_retarget = t;
                        mc_stage = 0;
                        mc_answer = answers[a];
                        mc_wanted_char = chars[c];
                        mc_w_answer = 0.5f;
                        mc_h_answer = -2.0f;
                        if (r >= 0)
                        {
                            op_font_pool.rows[r].glyph_x = mc_expected.rows[r].glyph_x = 0.5f;
                            op_font_pool.rows[r].glyph_y = mc_expected.rows[r].glyph_y = -2.0f;
                        }
                        out[0] = out[1] = out[2] = out[3] = 17.0f;
                        memcpy(want, out, sizeof(want));
                        w = (mode == 1 || mode == 3) ? 0 : &out[1];
                        h = (mode == 2 || mode == 3) ? 0 : &out[mode == 4 ? 1 : 2];
                        if (r >= 0 && mc_answer)
                        {
                            if (w)
                                want[1] = mc_w_answer;
                            if (h)
                                want[mode == 4 ? 1 : 2] = mc_h_answer;
                        }
                        mc_CHECK(op_font_measure_char((char)mc_wanted_char, w, h) == (r >= 0 && mc_answer != 0));
                        mc_CHECK(mc_stage == (r < 0 ? 0 : 2));
                        mc_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[15]));
                        mc_CHECK(memcmp(out, want, sizeof(out)) == 0);
                        mc_CHECK(memcmp(&op_font_pool, &mc_expected, sizeof(mc_expected)) == 0);
                    }
    printf("font_measure_char: %d checks, %d failures\n", mc_checks, mc_failures);
    return mc_failures != 0;
}
