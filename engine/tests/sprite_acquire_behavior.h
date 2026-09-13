#include "../src/sprite_resources.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sa_checks, sa_failures;
static void sa_check(int condition, int line)
{
    sa_checks++;
    if (!condition)
    {
        sa_failures++;
        printf("line %d failed\n", line);
    }
}
#define sa_CHECK(x) sa_check(!!(x), __LINE__)
#include <stdarg.h>

static char sa_names[5][75], sa_before[5][75], sa_expected[80];
static int sa_stage, sa_which, sa_mode, sa_objects[3];
static char *sa_buffer;
int sa_op_format_sprite_name(char *dest, char *format, ...)
{
    va_list args;
    char *name;
    int n;
    sa_CHECK(sa_stage++ == 0);
    sa_CHECK(format == op_sprite_format);
    va_start(args, format);
    name = va_arg(args, char *);
    va_end(args);
    sa_CHECK(name == sa_names[sa_which]);
    sa_buffer = dest;
    n = (int)strlen(name);
    sa_CHECK(n + 6 <= 80);
    memcpy(dest, "unit:", 5);
    memcpy(dest + 5, name, n + 1);
    if (sa_mode & 2)
        sa_names[sa_which][0] = 'z';
    return sa_mode & 2 ? -1 : n + 5;
}
void *sa_op_acquire_resource(unsigned int type, char *name)
{
    sa_CHECK(sa_stage++ == 1);
    sa_CHECK(type == 0x53505254);
    sa_CHECK(name == sa_buffer);
    sa_CHECK(strcmp(name, sa_expected) == 0);
    sa_CHECK(name != sa_names[sa_which]);
    return sa_mode & 1 ? &sa_objects[sa_which % 3] : 0;
}
int sa_main(void)
{
    int i, j;
    void *result;
    for (sa_which = 0; sa_which < 5; sa_which++)
        for (sa_mode = 0; sa_mode < 4; sa_mode++)
        {
            memset(sa_names, 0, sizeof(sa_names));
            for (i = 0; i < 5; i++)
            {
                for (j = 0; j < (i == 4 ? 74 : i == 3 ? 73 : i * 12); j++)
                    sa_names[i][j] = (char)('a' + i);
            }
            memcpy(sa_before, sa_names, sizeof(sa_names));
            memcpy(sa_expected, "unit:", 5);
            strcpy(sa_expected + 5, sa_names[sa_which]);
            sa_stage = 0;
            sa_buffer = 0;
            result = op_acquire_sprite(sa_names[sa_which]);
            sa_CHECK(result == (sa_mode & 1 ? &sa_objects[sa_which % 3] : 0));
            sa_CHECK(sa_stage == 2);
            if (sa_mode & 2)
                sa_before[sa_which][0] = 'z';
            sa_CHECK(memcmp(sa_names, sa_before, sizeof(sa_names)) == 0);
        }
    printf("sprite acquire: %d sa_checks, %d sa_failures\n", sa_checks, sa_failures);
    return sa_failures != 0;
}

#undef sa_CHECK
