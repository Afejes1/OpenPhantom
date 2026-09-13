#include "api.h"
#include <stdio.h>
#include <string.h>
struct OP_FONT
{
    unsigned int marker;
    unsigned char payload[28];
};
OP_FONT *op_system_font;
static OP_FONT objects[3], expected[3];
static int checks, failures;
static void check(int ok)
{
    ++checks;
    if (!ok)
        ++failures;
}
int main(void)
{
    int i;
    OP_FONT *p;
    memset(objects, 0x6d, sizeof(objects));
    memcpy(expected, objects, sizeof(objects));
    for (i = 0; i < 4; ++i)
    {
        p = i == 3 ? 0 : &objects[i];
        op_system_font = p;
        check(op_get_system_font() == p);
        check(op_system_font == p);
        check(memcmp(objects, expected, sizeof(objects)) == 0);
        check(op_get_system_font() == p);
        check(op_system_font == p);
        check(memcmp(objects, expected, sizeof(objects)) == 0);
    }
    printf("system font getter: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
