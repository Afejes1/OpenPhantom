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
    unsigned char bytes[16384];
    unsigned int after;
} OWNED_TABLE;
static OWNED_TABLE table, expected;
static int prior_offset;
static void check_guards(void)
{
    CHECK(table.before == 0x12345678u && table.after == 0x87654321u);
    CHECK(memcmp(&palette, &palette_expected, sizeof(palette)) == 0);
}
unsigned int op_palette_nearest(unsigned char *input, unsigned int count, float red, float green, float blue)
{
    int colour, step, level, offset;
    unsigned int result;
    if (calls >= 224 * 63)
    {
        CHECK(0);
        return 0;
    }
    colour = 32 + calls / 63;
    step = calls % 63;
    level = 62 - step;
    offset = level * 256 + colour;
    CHECK(input == palette.bytes + 3 && count == 254);
    CHECK(bits(red) == mono_expected[mode][colour % 8][step][0] &&
          bits(green) == mono_expected[mode][colour % 8][step][1] &&
          bits(blue) == mono_expected[mode][colour % 8][step][2]);
    check_guards();
    if (prior_offset >= 0)
        CHECK(table.bytes[prior_offset] == expected.bytes[prior_offset]);
    CHECK(table.bytes[offset] == 0xa7);
    CHECK(table.bytes[63 * 256 + colour] == (unsigned char)colour);
    prior_offset = offset;
    result = (unsigned int)(calls * 17 + mode) & 255u;
    ++calls;
    return result;
}
int main(void)
{
    int i, j;
    unsigned int old, current;
    old = control(0, 0);
    control(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (mode = 0; mode < 4; ++mode)
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
        for (i = 0; i < 32; ++i)
            for (j = 0; j < 64; ++j)
                expected.bytes[j * 256 + i] = (unsigned char)i;
        for (i = 32; i < 256; ++i)
        {
            expected.bytes[63 * 256 + i] = (unsigned char)i;
            for (j = 0; j < 63; ++j)
                expected.bytes[(62 - j) * 256 + i] = (unsigned char)((((i - 32) * 63 + j) * 17 + mode + 1) & 255);
        }
        calls = 0;
        prior_offset = -1;
        CHECK(op_colormap_build_mono(table.bytes, palette.bytes, (unsigned char)targets[mode][0],
                                     (unsigned char)targets[mode][1], (unsigned char)targets[mode][2]) == 1);
        CHECK(calls == 224 * 63);
        CHECK(memcmp(&table, &expected, sizeof(table)) == 0);
        CHECK(memcmp(&palette, &palette_expected, sizeof(palette)) == 0);
    }
    control(old, _MCW_EM | _MCW_PC | _MCW_RC);
    current = control(0, 0);
    CHECK((current & (_MCW_EM | _MCW_PC | _MCW_RC)) == (old & (_MCW_EM | _MCW_PC | _MCW_RC)));
    printf("colormap_build_mono: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
