#include "api.h"
#include <stdio.h>
#include <string.h>
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
typedef struct OWNED_MATERIAL
{
    unsigned int before;
    OP_MATERIAL value;
    unsigned int after;
} OWNED_MATERIAL;
static OWNED_MATERIAL materials[4], expected[4];
static int row, stage;
static void seed(void)
{
    memset(materials, 0x69, sizeof(materials));
    memcpy(expected, materials, sizeof(materials));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(materials, expected, sizeof(materials)) == 0);
}
void *op_current_palette;
unsigned int op_current_palette_index;
void op_install_palette(void *p)
{
    CHECK(stage == 0 && p == materials[row].value.palette);
    verify();
    materials[row].value.palette[0] = expected[row].value.palette[0] = 0xa5;
    op_current_palette = materials[(row + 1) % 4].value.palette;
    op_current_palette_index = 0xffffffffu;
    stage = 1;
}
int main(void)
{
    for (row = 0; row < 4; ++row)
    {
        seed();
        op_current_palette = 0;
        op_current_palette_index = 99;
        op_material_install_palette(&materials[row].value);
        CHECK(stage == 1);
        CHECK(op_current_palette == materials[row].value.palette && op_current_palette_index == 0);
        verify();
    }
    printf("material_install_palette: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
