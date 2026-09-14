#include "api.h"
#include "oracle.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
const float op_colour_one = 1.0f;
static unsigned int control(unsigned int v, unsigned int mask)
{
#if _MSC_VER == 1100
    return _controlfp(v, mask);
#else
    unsigned int current;
    CHECK(_controlfp_s(&current, v, mask) == 0);
    return current;
#endif
}
typedef struct OWNED_PALETTE
{
    unsigned int before;
    unsigned char bytes[768];
    unsigned int after;
} OWNED_PALETTE;
static OWNED_PALETTE palette, palette_expected;
static int mode, calls;
static unsigned int bits(float value)
{
    unsigned int word;
    memcpy(&word, &value, 4);
    return word;
}

typedef struct OWNED_TABLE
{
    unsigned int before;
    unsigned char bytes[65536];
    unsigned int after;
} OWNED_TABLE;
static OWNED_TABLE table, expected;
unsigned int op_palette_nearest(unsigned char *input, unsigned int count, float red, float green, float blue)
{
    int outer, inner;
    unsigned int result;
    if (calls >= 65536)
    {
        CHECK(0);
        return 0;
    }
    outer = calls / 256;
    inner = calls % 256;
    CHECK(input == palette.bytes + 3 && count == 254);
    CHECK(bits(red) == blend_expected[mode][outer % 8][inner % 8][0] &&
          bits(green) == blend_expected[mode][outer % 8][inner % 8][1] &&
          bits(blue) == blend_expected[mode][outer % 8][inner % 8][2]);
    CHECK(table.before == 0x12345678u && table.after == 0x87654321u);
    CHECK(memcmp(&palette, &palette_expected, sizeof(palette)) == 0);
    if (calls)
        CHECK(table.bytes[calls - 1] == expected.bytes[calls - 1]);
    CHECK(table.bytes[calls] == 0xa7);
    result = (unsigned int)(calls * 17 + mode) & 255u;
    ++calls;
    return result;
}
int main(void)
{
    int i, j;
    unsigned int old, current;
    static float blends[] = {0.0f, 0.5f, 1.0f, -0.5f, 1.5f};
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (mode = 0; mode < 5; ++mode)
    {
        memset(&table, 0xa7, sizeof(table));
        table.before = 0x12345678u;
        table.after = 0x87654321u;
        expected = table;
        memset(&palette, 0x59, sizeof(palette));
        for (i = 0; i < 256; ++i)
            for (j = 0; j < 3; ++j)
                palette.bytes[i * 3 + j] = (unsigned char)authored_colours[i % 8][j];
        palette_expected = palette;
        for (i = 0; i < 65536; ++i)
            expected.bytes[i] = (unsigned char)((i * 17 + mode + 1) & 255);
        calls = 0;
        CHECK(op_colormap_build_blend(table.bytes, palette.bytes, blends[mode]) == 1);
        CHECK(calls == 65536);
        CHECK(memcmp(&table, &expected, sizeof(table)) == 0);
        CHECK(memcmp(&palette, &palette_expected, sizeof(palette)) == 0);
    }
    control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    current = control(0, 0);
    CHECK((current & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("colormap_build_blend: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
