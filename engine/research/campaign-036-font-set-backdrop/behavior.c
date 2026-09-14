#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
OP_FONT_POOL op_font_pool;
OP_FONT3D *op_current_font;
static OP_FONT_POOL expected;
static unsigned int tokens[16];
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
        op_font_pool.rows[i].backdrop = &tokens[(i + 1) % 16];
        op_font_pool.rows[i].colours[0] = 0x11223344u;
        op_font_pool.rows[i].colours[1] = 0x55667788u;
        op_font_pool.rows[i].colours[2] = 0x99aabbccu;
        op_font_pool.rows[i].colours[3] = 0xddeeff00u;
    }
    op_current_font = selected < 0 ? 0 : &op_font_pool.rows[selected];
    memcpy(&expected, &op_font_pool, sizeof(expected));
}
static int stage, selected, release_shift, acquire_shift, acquire_ok;
static const char *wanted_name;
int op_release_sprite(void **slot)
{
    CHECK(stage == 0);
    CHECK(slot == &op_font_pool.rows[selected].backdrop);
    CHECK(*slot == &tokens[(selected + 1) % 16]);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    *slot = 0;
    expected.rows[selected].backdrop = 0;
    if (release_shift)
        op_current_font = &op_font_pool.rows[(selected + 1) % 16];
    stage = 1;
    return 1;
}
void *op_acquire_sprite(const char *name)
{
    CHECK(stage == 1);
    CHECK(name == wanted_name);
    CHECK(op_current_font == &op_font_pool.rows[(selected + release_shift) % 16]);
    CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
    if (acquire_shift)
        op_current_font = &op_font_pool.rows[(selected + 2) % 16];
    stage = 2;
    return acquire_ok ? &tokens[15] : 0;
}
int main(void)
{
    int r, rs, as, ok, n, ret, destination;
    static const char name[] = "authored-font";
    for (r = -1; r < 16; ++r)
        for (rs = 0; rs < 2; ++rs)
            for (as = 0; as < 2; ++as)
                for (ok = 0; ok < 2; ++ok)
                    for (n = 0; n < 2; ++n)
                    {
                        seed(r);
                        selected = r;
                        release_shift = rs;
                        acquire_shift = as;
                        acquire_ok = ok;
                        wanted_name = n ? name : 0;
                        stage = 0;
                        if (r >= 0)
                        {
                            expected.rows[r].backdrop_width = 0.5f;
                            expected.rows[r].backdrop_height = -2.0f;
                        }
                        ret = op_font_set_backdrop(wanted_name, 0.5f, -2.0f);
                        destination = r < 0 ? -1 : (as && n ? (r + 2) % 16 : (r + rs) % 16);
                        if (r >= 0 && n)
                            expected.rows[destination].backdrop = ok ? &tokens[15] : 0;
                        CHECK(ret == (r < 0 ? 0 : (!n || ok)));
                        CHECK(stage == (r < 0 ? 0 : (n ? 2 : 1)));
                        CHECK(op_current_font == (r < 0 ? 0 : &op_font_pool.rows[destination]));
                        CHECK(memcmp(&op_font_pool, &expected, sizeof(expected)) == 0);
                    }
    printf("font_set_backdrop: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
