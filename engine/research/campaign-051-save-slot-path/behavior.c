#include "api.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
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

char op_save_path[40], op_save_path_format[] = "authored-format", op_save_path_prefix[] = "authored-prefix";
static char expected_path[40], expected_format[sizeof(op_save_path_format)],
    expected_prefix[sizeof(op_save_path_prefix)];
static const int slots[] = {0, 1, 98, 99, -1, INT_MIN, INT_MAX}, answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int slot_case, answer, mutate, stage;
static void verify(void)
{
    CHECK(memcmp(op_save_path, expected_path, sizeof(expected_path)) == 0);
    CHECK(memcmp(op_save_path_format, expected_format, sizeof(expected_format)) == 0);
    CHECK(memcmp(op_save_path_prefix, expected_prefix, sizeof(expected_prefix)) == 0);
}
int op_format_sprite_name(char *dest, char *format, ...)
{
    va_list args;
    char *prefix;
    int slot;
    va_start(args, format);
    prefix = va_arg(args, char *);
    slot = va_arg(args, int);
    va_end(args);
    CHECK(stage == 0 && dest == op_save_path && format == op_save_path_format && prefix == op_save_path_prefix &&
          slot == slots[slot_case]);
    verify();
    memcpy(op_save_path, "authored-slot", 13);
    memcpy(expected_path, "authored-slot", 13);
    if (mutate)
    {
        op_save_path[1] = expected_path[1] = 'Q';
        op_save_path_format[0] = expected_format[0] = 'F';
        op_save_path_prefix[0] = expected_prefix[0] = 'P';
    }
    ++stage;
    return answers[answer];
}
int main(void)
{
    for (slot_case = 0; slot_case < 7; ++slot_case)
        for (answer = 0; answer < 5; ++answer)
            for (mutate = 0; mutate < 2; ++mutate)
            {
                memset(op_save_path, 0x63, sizeof(op_save_path));
                memcpy(expected_path, op_save_path, sizeof(expected_path));
                memcpy(expected_format, op_save_path_format, sizeof(expected_format));
                memcpy(expected_prefix, op_save_path_prefix, sizeof(expected_prefix));
                stage = 0;
                CHECK(op_save_slot_path(slots[slot_case]) == op_save_path);
                CHECK(stage == 1);
                verify();
            }
    printf("save_slot_path: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
