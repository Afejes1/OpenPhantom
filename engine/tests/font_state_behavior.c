#include "../src/font_state.h"
#include "../src/sprite_resources.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_FONT_POOL op_font_pool;
OP_FONT3D *op_current_font;
#include "font_get_colours_behavior.h"
#include "font_last_textbox_behavior.h"
#include "font_select_behavior.h"
#include "font_set_align_behavior.h"
#include "font_set_colour_behavior.h"
#include "font_set_colours_behavior.h"
#include "font_set_glyph_scale_behavior.h"
#include "font_set_position_scale_behavior.h"
#include "font_set_word20_behavior.h"
#include "font_set_word24_behavior.h"
#include "font_set_zrange_behavior.h"
static int fc_active, fc_checks, fc_failures;
static OP_FONT_POOL fc_expected;
static unsigned int fc_tokens[16];
static int fc_selected, fc_has, fc_rs, fc_fs, fc_as, fc_ok, fc_replace, fc_stage;
static char fc_name[] = "font-fixture";
static char *fc_buffer;
static void fc_check(int ok, int line)
{
    ++fc_checks;
    if (!ok)
    {
        ++fc_failures;
        printf("font chain failed at %d\n", line);
    }
}
#define FC_CHECK(x) fc_check(!!(x), __LINE__)
static void fc_seed(int selected)
{
    int i;
    memset(&op_font_pool, 0x69, sizeof(op_font_pool));
    for (i = 0; i < 16; ++i)
    {
        op_font_pool.rows[i].font = &fc_tokens[i];
        op_font_pool.rows[i].backdrop = &fc_tokens[(i + 1) % 16];
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&fc_expected, &op_font_pool, sizeof(fc_expected));
}
static void fc_release_resource(void *resource)
{
    FC_CHECK(fc_stage == 0 && fc_has);
    FC_CHECK(resource == &fc_tokens[(fc_selected + 1) % 16]);
    FC_CHECK(op_current_font == &op_font_pool.rows[fc_selected]);
    FC_CHECK(memcmp(&op_font_pool, &fc_expected, sizeof(fc_expected)) == 0);
    if (fc_replace)
        op_font_pool.rows[fc_selected].backdrop = &fc_tokens[14];
    if (fc_rs)
        op_current_font = &op_font_pool.rows[(fc_selected + 1) % 16];
    /* Predict the canonical release wrapper's subsequent captured-slot clear. */
    fc_expected.rows[fc_selected].backdrop = 0;
    fc_stage = 1;
}
static int fc_format_sprite(char *dest, char *format, char *base)
{
    FC_CHECK(fc_stage == fc_has);
    FC_CHECK(format == op_sprite_format && base == fc_name && dest != base);
    FC_CHECK(op_current_font == &op_font_pool.rows[(fc_selected + (fc_has ? fc_rs : 0)) % 16]);
    FC_CHECK(memcmp(&op_font_pool, &fc_expected, sizeof(fc_expected)) == 0);
    memcpy(dest, "unit:font-fixture", 18);
    fc_buffer = dest;
    if (fc_fs)
        op_current_font = &op_font_pool.rows[(fc_selected + 2) % 16];
    fc_stage = 2;
    return 17;
}
static void *fc_acquire_resource(unsigned int type, char *name)
{
    int before = fc_fs ? (fc_selected + 2) % 16 : (fc_selected + (fc_has ? fc_rs : 0)) % 16;
    FC_CHECK(fc_stage == 2);
    FC_CHECK(type == 0x53505254U);
    FC_CHECK(name == fc_buffer && strcmp(name, "unit:font-fixture") == 0);
    FC_CHECK(op_current_font == &op_font_pool.rows[before]);
    FC_CHECK(memcmp(&op_font_pool, &fc_expected, sizeof(fc_expected)) == 0);
    if (fc_as)
        op_current_font = &op_font_pool.rows[(fc_selected + 3) % 16];
    fc_stage = 3;
    fc_buffer = 0;
    return fc_ok ? &fc_tokens[15] : 0;
}
static int fc_backdrop(void)
{
    int r, has, rs, fs, as, ok, n, replacement, result, destination;
    fc_active = 1;
    for (r = -1; r < 16; ++r)
        for (has = 0; has < 2; ++has)
            for (rs = 0; rs < 2; ++rs)
                for (fs = 0; fs < 2; ++fs)
                    for (as = 0; as < 2; ++as)
                        for (ok = 0; ok < 2; ++ok)
                            for (n = 0; n < 2; ++n)
                                for (replacement = 0; replacement < 2; ++replacement)
                                {
                                    fc_seed(r);
                                    fc_selected = r;
                                    fc_has = has;
                                    fc_rs = rs;
                                    fc_fs = fs;
                                    fc_as = as;
                                    fc_ok = ok;
                                    fc_replace = replacement;
                                    fc_stage = 0;
                                    fc_buffer = 0;
                                    if (r >= 0)
                                    {
                                        if (!has)
                                            op_font_pool.rows[r].backdrop = fc_expected.rows[r].backdrop = 0;
                                        fc_expected.rows[r].backdrop_width = 0.5f;
                                        fc_expected.rows[r].backdrop_height = -2.0f;
                                    }
                                    result = op_font_set_backdrop(n ? fc_name : 0, 0.5f, -2.0f);
                                    destination = r < 0 ? -1 : (r + (has ? rs : 0)) % 16;
                                    if (r >= 0 && n)
                                    {
                                        if (fs)
                                            destination = (r + 2) % 16;
                                        if (as)
                                            destination = (r + 3) % 16;
                                        fc_expected.rows[destination].backdrop = ok ? &fc_tokens[15] : 0;
                                    }
                                    FC_CHECK(result == (r < 0 ? 0 : (!n || ok)));
                                    FC_CHECK(fc_stage == (r < 0 ? 0 : (n ? 3 : has)));
                                    FC_CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[destination]));
                                    FC_CHECK(memcmp(&op_font_pool, &fc_expected, sizeof(fc_expected)) == 0);
                                    FC_CHECK(fc_buffer == 0);
                                }
    fc_active = 0;
    return fc_failures != 0;
}
static int fc_state_chain(void)
{
    int r;
    unsigned int outputs[6];
    for (r = 0; r < 16; ++r)
    {
        fc_seed(-1);
        op_font_select(r);
        FC_CHECK(op_current_font == &op_font_pool.rows[r]);
        op_font_set_colour(0xabcdef01u);
        fc_expected.rows[r].colours[0] = fc_expected.rows[r].colours[1] = fc_expected.rows[r].colours[2] =
            fc_expected.rows[r].colours[3] = 0xabcdef01u;
        FC_CHECK(memcmp(&op_font_pool, &fc_expected, sizeof(fc_expected)) == 0);
        op_font_set_colours(11, 22, 33, 44);
        fc_expected.rows[r].colours[0] = 11;
        fc_expected.rows[r].colours[1] = 22;
        fc_expected.rows[r].colours[2] = 33;
        fc_expected.rows[r].colours[3] = 44;
        memset(outputs, 0x17, sizeof(outputs));
        op_font_get_colours(outputs + 1, outputs + 2, outputs + 3, outputs + 4);
        FC_CHECK(outputs[0] == 0x17171717u && outputs[1] == 11 && outputs[2] == 22 && outputs[3] == 33 &&
                 outputs[4] == 44 && outputs[5] == 0x17171717u);
        op_font_set_align(2);
        fc_expected.rows[r].align = 4;
        op_font_set_glyph_scale(0.5f, 2.0f);
        fc_expected.rows[r].glyph_x = 0.5f;
        fc_expected.rows[r].glyph_y = 2.0f;
        op_font_set_position_scale(-1.0f, 0.25f);
        fc_expected.rows[r].position_x = -1.0f;
        fc_expected.rows[r].position_y = 0.25f;
        op_font_set_word20(0x87654321u);
        fc_expected.rows[r].word20 = 0x87654321u;
        FC_CHECK(op_font_set_word24(0xfedcba98u) == 1);
        fc_expected.rows[r].word24 = 0xfedcba98u;
        FC_CHECK(op_font_set_zrange(0.5f, 0.25f) == 1);
        fc_expected.rows[r].z_start = 0.5f;
        fc_expected.rows[r].z_end = 0.75f;
        FC_CHECK(memcmp(&op_font_pool, &fc_expected, sizeof(fc_expected)) == 0);
        op_font_select(16);
        FC_CHECK(op_current_font == 0);
        op_font_set_colour(0);
        op_font_set_align(0);
        op_font_get_colours(0, 0, 0, 0);
        FC_CHECK(memcmp(&op_font_pool, &fc_expected, sizeof(fc_expected)) == 0);
    }
    return fc_failures != 0;
}
static int op_test_font_state(void)
{
    int status = 0, total;
    status += fs0_main();
    status += fs1_main();
    status += fs2_main();
    status += fs3_main();
    status += fs5_main();
    status += fs6_main();
    status += fs7_main();
    status += fs8_main();
    status += fs9_main();
    status += fs10_main();
    status += fs11_main();
    status += fc_backdrop();
    status += fc_state_chain();
    total = fc_checks + fs0_checks + fs1_checks + fs2_checks + fs3_checks + fs5_checks + fs6_checks + fs7_checks +
            fs8_checks + fs9_checks + fs10_checks + fs11_checks;
    printf("font state connected: %d checks, %d integration failures\n", total, fc_failures);
    return status;
}
