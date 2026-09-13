#include "api.h"
#include <stdio.h>

void *op_sound_driver;
static int checks;
static int failures;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

int main(void)
{
    char first;
    char second;

    op_sound_driver = 0;
    check(op_sound_get_driver() == 0, "null driver returned");
    op_sound_driver = &first;
    check(op_sound_get_driver() == &first, "first identity returned");
    check(op_sound_driver == &first, "getter preserves first global");
    op_sound_driver = &second;
    check(op_sound_get_driver() == &second, "changed identity returned");
    check(op_sound_driver == &second, "getter preserves changed global");
    printf("Sound get driver: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}