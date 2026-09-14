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
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_WINDOW_HANDLE_WORD op_window_handle, op_instance_handle;
OP_WINDOW_RECT op_window_rect;
typedef struct OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} OWNED_RECT;
static OWNED_RECT input, expected_input;
static OP_WINDOW_RECT expected_rect;
static unsigned int expected_window, expected_instance;
static const unsigned int bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void seed(unsigned int profile)
{
    unsigned int i;
    memset(&input, 0x57, sizeof(input));
    op_window_handle = bits[profile % 6];
    op_instance_handle = bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = bits[(profile / 36 + i) % 6];
        input.value.words[i] = bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&expected_input, &input, sizeof(input));
    memcpy(&expected_rect, &op_window_rect, sizeof(op_window_rect));
    expected_window = op_window_handle;
    expected_instance = op_instance_handle;
}
static void verify(void)
{
    CHECK(op_window_handle == expected_window);
    CHECK(op_instance_handle == expected_instance);
    CHECK(memcmp(&op_window_rect, &expected_rect, sizeof(op_window_rect)) == 0);
    CHECK(memcmp(&input, &expected_input, sizeof(input)) == 0);
}
int main(void)
{
    unsigned int profile;
    int alias;
    for (profile = 0; profile < 216; ++profile)
        for (alias = 0; alias < 2; ++alias)
        {
            seed(profile);
            if (!alias)
                memcpy(&expected_rect, &expected_input.value, sizeof(expected_rect));
            op_window_set_rect(alias ? &op_window_rect : &input.value);
            verify();
        }
    printf("window_set_rect: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
