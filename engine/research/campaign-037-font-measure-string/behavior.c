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
static int stage, length, fail_late;
static unsigned char *wanted_text;
static float *first_width;
int op_font_measure_char(char ch, float *w, float *h)
{
    CHECK(stage < length && (unsigned char)ch == wanted_text[stage]);
    CHECK(w && !h);
    if (!stage)
        first_width = w;
    CHECK(w == first_width);
    if (!fail_late || !stage)
        *w = stage == 0 ? 0.5f : (stage == 1 ? 1.0f : 2.0f);
    ++stage;
    return fail_late && stage > 1 ? 0 : 1;
}
int main(void)
{
    int len, fail;
    float result;
    unsigned char text[5] = {65, 128, 255, 66, 0}, before[5];
    static float normal[] = {0.0f, 0.5f, 1.5f, 3.5f, 5.5f};
    static float repeated[] = {0.0f, 0.5f, 1.0f, 1.5f, 2.0f};
    for (len = 0; len < 5; ++len)
        for (fail = 0; fail < 2; ++fail)
        {
            seed(0);
            text[0] = 65;
            text[1] = 128;
            text[2] = 255;
            text[3] = 66;
            text[4] = 0;
            text[len] = 0;
            memcpy(before, text, sizeof(text));
            stage = 0;
            length = len;
            fail_late = fail;
            wanted_text = text;
            first_width = 0;
            result = op_font_measure_string((char *)text);
            CHECK(result == (fail ? repeated[len] : normal[len]));
            CHECK(stage == len);
            CHECK(memcmp(before, text, sizeof(text)) == 0);
            CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
        }
    printf("font_measure_string: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
