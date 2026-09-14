#include "font_measure_char_behavior.h"
static int mc_active, ms_checks, ms_failures, ms_stage, ms_count, ms_length, ms_late_failure, ms_retarget, ms_row;
static unsigned char *ms_input;
static OP_FONT_POOL ms_expected;
static unsigned int ms_tokens[16];
static void ms_check(int ok, int line)
{
    ++ms_checks;
    if (!ok)
    {
        ++ms_failures;
        printf("font measure chain failed at %d\n", line);
    }
}
#define MS_CHECK(x) ms_check(!!(x), __LINE__)
void op_set_font_scale(float x, float y)
{
    if (mc_active)
    {
        mc_op_set_font_scale(x, y);
        return;
    }
    MS_CHECK(ms_stage == 0 && ms_count < ms_length);
    MS_CHECK(op_current_font == &op_font_pool.rows[ms_row]);
    MS_CHECK(x == (ms_row ? 1.0f : 0.5f) && y == (ms_row ? 3.0f : -2.0f));
    MS_CHECK(memcmp(&op_font_pool, &ms_expected, sizeof(ms_expected)) == 0);
    if (ms_retarget)
    {
        ms_row = 1;
        op_current_font = &op_font_pool.rows[1];
    }
    ms_stage = 1;
}
int op_font_char_size(void *font, char ch, float *w, float *h)
{
    int answer;
    if (mc_active)
        return mc_op_font_char_size(font, ch, w, h);
    MS_CHECK(ms_stage == 1 && ms_count < ms_length);
    MS_CHECK(font == &ms_tokens[ms_row]);
    MS_CHECK((unsigned char)ch == ms_input[ms_count]);
    MS_CHECK(w && h && w != h);
    answer = !(ms_late_failure && ms_count > 0);
    *w = answer ? (ms_count == 0 ? 0.5f : (ms_count == 1 ? 1.0f : 2.0f)) : 99.0f;
    *h = -7.0f;
    ++ms_count;
    ms_stage = 0;
    return answer;
}
static int op_test_font_measure(void)
{
    int status, i, len, late, change;
    float result;
    unsigned char text[5], before[5];
    static float normal[] = {0.0f, 0.5f, 1.5f, 3.5f, 5.5f};
    static float repeated[] = {0.0f, 0.5f, 1.0f, 1.5f, 2.0f};
    mc_active = 1;
    status = mc_main();
    mc_active = 0;
    for (len = 0; len < 5; ++len)
        for (late = 0; late < 2; ++late)
            for (change = 0; change < 2; ++change)
            {
                memset(&op_font_pool, 0x69, sizeof(op_font_pool));
                for (i = 0; i < 16; ++i)
                {
                    op_font_pool.rows[i].font = &ms_tokens[i];
                    op_font_pool.rows[i].glyph_x = i ? 1.0f : 0.5f;
                    op_font_pool.rows[i].glyph_y = i ? 3.0f : -2.0f;
                }
                memcpy(&ms_expected, &op_font_pool, sizeof(ms_expected));
                op_current_font = &op_font_pool.rows[0];
                text[0] = 65;
                text[1] = 128;
                text[2] = 255;
                text[3] = 66;
                text[4] = 0;
                text[len] = 0;
                memcpy(before, text, sizeof(text));
                ms_input = text;
                ms_stage = ms_count = ms_row = 0;
                ms_length = len;
                ms_late_failure = late;
                ms_retarget = change;
                result = op_font_measure_string((char *)text);
                MS_CHECK(result == (late ? repeated[len] : normal[len]));
                MS_CHECK(ms_count == len && ms_stage == 0);
                MS_CHECK(op_current_font == &op_font_pool.rows[len && change ? 1 : 0]);
                MS_CHECK(memcmp(before, text, sizeof(text)) == 0);
                MS_CHECK(memcmp(&op_font_pool, &ms_expected, sizeof(ms_expected)) == 0);
            }
    op_current_font = 0;
    text[0] = 0;
    ms_length = ms_stage = ms_count = 0;
    MS_CHECK(op_font_measure_string((char *)text) == 0.0f);
    MS_CHECK(ms_count == 0 && op_current_font == 0);
    printf("font measurement connected: %d checks, %d integration failures\n", ms_checks + mc_checks, ms_failures);
    return status + (ms_failures != 0);
}
