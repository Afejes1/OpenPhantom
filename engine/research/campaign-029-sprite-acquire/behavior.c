#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
#include <stdarg.h>
char op_sprite_format[] = "unit:%s";
static char names[4][74], before[4][74], expected[80];
static int stage, which, mode, objects[3];
static char *buffer;
int op_format_sprite_name(char *dest, char *format, ...)
{
    va_list args;
    char *name;
    int n;
    CHECK(stage++ == 0);
    CHECK(format == op_sprite_format);
    va_start(args, format);
    name = va_arg(args, char *);
    va_end(args);
    CHECK(name == names[which]);
    buffer = dest;
    n = (int)strlen(name);
    CHECK(n + 6 <= 80);
    memcpy(dest, "unit:", 5);
    memcpy(dest + 5, name, n + 1);
    memcpy(expected, dest, n + 6);
    if (mode & 2)
        names[which][0] = 'z';
    return mode & 2 ? -1 : n + 5;
}
void *op_acquire_resource(unsigned int type, char *name)
{
    CHECK(stage++ == 1);
    CHECK(type == 0x53505254);
    CHECK(name == buffer);
    CHECK(strcmp(name, expected) == 0);
    CHECK(name != names[which]);
    return mode & 1 ? &objects[which % 3] : 0;
}
int main(void)
{
    int i, j;
    void *result;
    for (which = 0; which < 4; which++)
        for (mode = 0; mode < 4; mode++)
        {
            memset(names, 0, sizeof(names));
            for (i = 0; i < 4; i++)
            {
                for (j = 0; j < (i == 3 ? 73 : i * 12); j++)
                    names[i][j] = (char)('a' + i);
            }
            memcpy(before, names, sizeof(names));
            stage = 0;
            buffer = 0;
            result = op_sprite_acquire(names[which]);
            CHECK(result == (mode & 1 ? &objects[which % 3] : 0));
            CHECK(stage == 2);
            if (mode & 2)
                before[which][0] = 'z';
            CHECK(memcmp(names, before, sizeof(names)) == 0);
        }
    printf("sprite acquire: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
