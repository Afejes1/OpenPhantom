#include "api.h"
#include <stdio.h>
#include <string.h>

static int release_calls;
static void *released_pointer;
static int checks;
static int failures;

void op_scan_release(void *memory)
{
    ++release_calls;
    released_pointer = memory;
}

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

int main(void)
{
    op_scan_line first;
    op_scan_line second;
    op_scan_line first_before;
    op_scan_line second_before;

    memset(&first, 0x35, sizeof(first));
    memset(&second, 0xa7, sizeof(second));
    first_before = first;
    second_before = second;

    release_calls = 0;
    released_pointer = &first;
    op_free_scan_line(0);
    check(release_calls == 0, "null record does not release");
    check(released_pointer == &first, "null record does not alter stub state");

    op_free_scan_line(&first);
    check(release_calls == 1, "first record releases once");
    check(released_pointer == &first, "first pointer identity");
    check(memcmp(&first, &first_before, sizeof(first)) == 0,
          "first record remains unchanged");

    op_free_scan_line(&second);
    check(release_calls == 2, "second record adds one release");
    check(released_pointer == &second, "second pointer identity");
    check(memcmp(&second, &second_before, sizeof(second)) == 0,
          "second record remains unchanged");
    check(memcmp(&first, &first_before, sizeof(first)) == 0,
          "first record remains unchanged after second call");

    printf("Line free: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
