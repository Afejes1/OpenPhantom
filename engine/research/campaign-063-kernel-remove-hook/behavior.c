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
unsigned int op_kernel_window, op_kernel_field, op_kernel_callback, op_kernel_callback_enabled;
int op_kernel_show_command;
unsigned int op_kernel_dialog_count, op_kernel_hook_count, op_kernel_dialogs[16];
OP_KERNEL_HOOK op_kernel_hooks[16];
static unsigned int expected_window, expected_field, expected_callback, expected_enabled, expected_dialog_count,
    expected_hook_count, expected_dialogs[16];
static int expected_command;
static OP_KERNEL_HOOK expected_hooks[16];
static int calls, mutate, observed_command;
static unsigned int observed_window, return_word;
static const unsigned int words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void verify(void)
{
    CHECK(op_kernel_window == expected_window);
    CHECK(op_kernel_field == expected_field);
    CHECK(op_kernel_callback == expected_callback);
    CHECK(op_kernel_callback_enabled == expected_enabled);
    CHECK(op_kernel_show_command == expected_command);
    CHECK(op_kernel_dialog_count == expected_dialog_count);
    CHECK(op_kernel_hook_count == expected_hook_count);
    CHECK(memcmp(op_kernel_dialogs, expected_dialogs, sizeof(expected_dialogs)) == 0);
    CHECK(memcmp(op_kernel_hooks, expected_hooks, sizeof(expected_hooks)) == 0);
}
static int __stdcall show(unsigned int window, int command)
{
    ++calls;
    CHECK(calls == 1);
    CHECK(window == observed_window);
    CHECK(command == observed_command);
    verify();
    if (mutate)
    {
        op_kernel_window = expected_window = 0x76543210u;
        op_kernel_show_command = expected_command = -7;
        op_kernel_field = expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = expected_hooks[5].word = 0x55555555u;
    }
    return (int)return_word;
}
int(__stdcall *op_kernel_show_window)(unsigned int, int) = show;
static void snapshot(void)
{
    expected_window = op_kernel_window;
    expected_field = op_kernel_field;
    expected_callback = op_kernel_callback;
    expected_enabled = op_kernel_callback_enabled;
    expected_command = op_kernel_show_command;
    expected_dialog_count = op_kernel_dialog_count;
    expected_hook_count = op_kernel_hook_count;
    memcpy(expected_dialogs, op_kernel_dialogs, sizeof(expected_dialogs));
    memcpy(expected_hooks, op_kernel_hooks, sizeof(expected_hooks));
}
static void seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_window = words[profile % 8];
    op_kernel_field = words[(profile + 1) % 8];
    op_kernel_callback = words[(profile + 2) % 8];
    op_kernel_callback_enabled = words[(profile + 3) % 8];
    op_kernel_show_command = (int)words[(profile + 4) % 8];
    op_kernel_dialog_count = words[(profile + 5) % 8];
    op_kernel_hook_count = words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = words[(profile + i) % 8];
    }
    calls = 0;
    mutate = 0;
    return_word = words[(profile + 7) % 8];
    snapshot();
}
int main(void)
{
    unsigned int p, slot, v, i, needle;
    int found, want;
    for (p = 0; p < 64; ++p)
        for (slot = 0; slot <= 16; ++slot)
            for (v = 0; v < 8; ++v)
            {
                seed(p);
                needle = words[v];
                if (slot < 16)
                {
                    op_kernel_hooks[slot].word = needle;
                    if (slot + 1 < 16)
                        op_kernel_hooks[slot + 1].word = needle;
                }
                snapshot();
                found = -1;
                if ((int)expected_hook_count > 0)
                    for (i = 0; i < 16; ++i)
                        if (expected_hooks[i].word == needle)
                        {
                            found = (int)i;
                            break;
                        }
                want = found >= 0;
                if (want)
                {
                    expected_hooks[found].word = expected_hooks[found].active = 0;
                    expected_hook_count--;
                }
                CHECK(op_kernel_remove_hook(needle) == want);
                verify();
                CHECK(calls == 0);
            }
    printf("kernel_remove_hook: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
