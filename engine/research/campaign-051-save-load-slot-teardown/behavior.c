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

typedef struct OWNED_PATH
{
    unsigned int before;
    char text[32];
    unsigned int after;
} OWNED_PATH;
static OWNED_PATH paths[3], expected_paths[3];
static const int slots[] = {0, 1, 98, 99, -1, INT_MIN, INT_MAX};
static const int answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int slot_case, answer, row, mutate, stage;
static void seed(void)
{
    memset(paths, 0x57, sizeof(paths));
    memcpy(expected_paths, paths, sizeof(paths));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(paths, expected_paths, sizeof(paths)) == 0);
}
static void change(int offset)
{
    if (mutate)
    {
        paths[row].text[offset] = expected_paths[row].text[offset] = (char)('A' + offset);
    }
}
void op_save_lifecycle(int a, int b)
{
    CHECK(stage == 0 && a == 0 && b == 6);
    verify();
    change(1);
    ++stage;
}
char *op_save_slot_path(int slot)
{
    CHECK(stage == 1 && slot == slots[slot_case]);
    verify();
    change(2);
    ++stage;
    return paths[mutate ? (row + 1) % 3 : row].text;
}
int op_load_saved_game(char *path, int mode)
{
    CHECK(stage == 2 && path == paths[mutate ? (row + 1) % 3 : row].text && mode == 0);
    verify();
    change(3);
    ++stage;
    return answers[answer];
}
int main(void)
{
    for (slot_case = 0; slot_case < 7; ++slot_case)
        for (answer = 0; answer < 5; ++answer)
            for (row = 0; row < 3; ++row)
                for (mutate = 0; mutate < 2; ++mutate)
                {
                    seed();
                    CHECK(op_save_load_slot_teardown(slots[slot_case]) == answers[answer]);
                    CHECK(stage == 3);
                    verify();
                }
    printf("save_load_slot_teardown: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
