#include "../src/kernel_helpers.h"
unsigned int op_kernel_window, op_kernel_field, op_kernel_callback, op_kernel_callback_enabled;
int op_kernel_show_command;
unsigned int op_kernel_dialog_count, op_kernel_hook_count, op_kernel_dialogs[16];
OP_KERNEL_HOOK op_kernel_hooks[16];
int(__stdcall *op_kernel_show_window)(unsigned int, int);

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks0_checks, ks0_failures;
static void ks0_check(int ok, int line)
{
    ++ks0_checks;
    if (!ok)
    {
        ++ks0_failures;
        printf("line%d failed\n", line);
    }
}
#define KS0_CHECK(x) ks0_check(!!(x), __LINE__)
static unsigned int ks0_expected_window, ks0_expected_field, ks0_expected_callback, ks0_expected_enabled,
    ks0_expected_dialog_count, ks0_expected_hook_count, ks0_expected_dialogs[16];
static int ks0_expected_command;
static OP_KERNEL_HOOK ks0_expected_hooks[16];
static int ks0_calls, ks0_mutate, ks0_observed_command;
static unsigned int ks0_observed_window, ks0_return_word;
static const unsigned int ks0_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks0_verify(void)
{
    KS0_CHECK(op_kernel_window == ks0_expected_window);
    KS0_CHECK(op_kernel_field == ks0_expected_field);
    KS0_CHECK(op_kernel_callback == ks0_expected_callback);
    KS0_CHECK(op_kernel_callback_enabled == ks0_expected_enabled);
    KS0_CHECK(op_kernel_show_command == ks0_expected_command);
    KS0_CHECK(op_kernel_dialog_count == ks0_expected_dialog_count);
    KS0_CHECK(op_kernel_hook_count == ks0_expected_hook_count);
    KS0_CHECK(memcmp(op_kernel_dialogs, ks0_expected_dialogs, sizeof(ks0_expected_dialogs)) == 0);
    KS0_CHECK(memcmp(op_kernel_hooks, ks0_expected_hooks, sizeof(ks0_expected_hooks)) == 0);
}
static int __stdcall ks0_show(unsigned int window, int command)
{
    ++ks0_calls;
    KS0_CHECK(ks0_calls == 1);
    KS0_CHECK(window == ks0_observed_window);
    KS0_CHECK(command == ks0_observed_command);
    ks0_verify();
    if (ks0_mutate)
    {
        op_kernel_window = ks0_expected_window = 0x76543210u;
        op_kernel_show_command = ks0_expected_command = -7;
        op_kernel_field = ks0_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks0_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks0_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks0_return_word;
}
static void ks0_snapshot(void)
{
    ks0_expected_window = op_kernel_window;
    ks0_expected_field = op_kernel_field;
    ks0_expected_callback = op_kernel_callback;
    ks0_expected_enabled = op_kernel_callback_enabled;
    ks0_expected_command = op_kernel_show_command;
    ks0_expected_dialog_count = op_kernel_dialog_count;
    ks0_expected_hook_count = op_kernel_hook_count;
    memcpy(ks0_expected_dialogs, op_kernel_dialogs, sizeof(ks0_expected_dialogs));
    memcpy(ks0_expected_hooks, op_kernel_hooks, sizeof(ks0_expected_hooks));
}
static void ks0_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks0_show;
    op_kernel_window = ks0_words[profile % 8];
    op_kernel_field = ks0_words[(profile + 1) % 8];
    op_kernel_callback = ks0_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks0_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks0_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks0_words[(profile + 5) % 8];
    op_kernel_hook_count = ks0_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks0_words[(profile + i) % 8];
    }
    ks0_calls = 0;
    ks0_mutate = 0;
    ks0_return_word = ks0_words[(profile + 7) % 8];
    ks0_snapshot();
}
static int op_test_kernel_show(void)
{
    unsigned int p, r;
    int m;
    for (p = 0; p < 64; ++p)
        for (r = 0; r < 8; ++r)
            for (m = 0; m < 2; ++m)
            {
                ks0_seed(p);
                ks0_mutate = m;
                ks0_return_word = ks0_words[r];
                ks0_observed_window = op_kernel_window;
                ks0_observed_command = op_kernel_show_command;
                op_kernel_show();
                KS0_CHECK(ks0_calls == 1);
                ks0_verify();
                KS0_CHECK(op_kernel_show_window == ks0_show);
            }
    printf("kernel_show: %d checks, %d failures\n", ks0_checks, ks0_failures);
    return ks0_failures != 0;
}

#undef KS0_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks1_checks, ks1_failures;
static void ks1_check(int ok, int line)
{
    ++ks1_checks;
    if (!ok)
    {
        ++ks1_failures;
        printf("line%d failed\n", line);
    }
}
#define KS1_CHECK(x) ks1_check(!!(x), __LINE__)
static unsigned int ks1_expected_window, ks1_expected_field, ks1_expected_callback, ks1_expected_enabled,
    ks1_expected_dialog_count, ks1_expected_hook_count, ks1_expected_dialogs[16];
static int ks1_expected_command;
static OP_KERNEL_HOOK ks1_expected_hooks[16];
static int ks1_calls, ks1_mutate, ks1_observed_command;
static unsigned int ks1_observed_window, ks1_return_word;
static const unsigned int ks1_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks1_verify(void)
{
    KS1_CHECK(op_kernel_window == ks1_expected_window);
    KS1_CHECK(op_kernel_field == ks1_expected_field);
    KS1_CHECK(op_kernel_callback == ks1_expected_callback);
    KS1_CHECK(op_kernel_callback_enabled == ks1_expected_enabled);
    KS1_CHECK(op_kernel_show_command == ks1_expected_command);
    KS1_CHECK(op_kernel_dialog_count == ks1_expected_dialog_count);
    KS1_CHECK(op_kernel_hook_count == ks1_expected_hook_count);
    KS1_CHECK(memcmp(op_kernel_dialogs, ks1_expected_dialogs, sizeof(ks1_expected_dialogs)) == 0);
    KS1_CHECK(memcmp(op_kernel_hooks, ks1_expected_hooks, sizeof(ks1_expected_hooks)) == 0);
}
static int __stdcall ks1_show(unsigned int window, int command)
{
    ++ks1_calls;
    KS1_CHECK(ks1_calls == 1);
    KS1_CHECK(window == ks1_observed_window);
    KS1_CHECK(command == ks1_observed_command);
    ks1_verify();
    if (ks1_mutate)
    {
        op_kernel_window = ks1_expected_window = 0x76543210u;
        op_kernel_show_command = ks1_expected_command = -7;
        op_kernel_field = ks1_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks1_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks1_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks1_return_word;
}
static void ks1_snapshot(void)
{
    ks1_expected_window = op_kernel_window;
    ks1_expected_field = op_kernel_field;
    ks1_expected_callback = op_kernel_callback;
    ks1_expected_enabled = op_kernel_callback_enabled;
    ks1_expected_command = op_kernel_show_command;
    ks1_expected_dialog_count = op_kernel_dialog_count;
    ks1_expected_hook_count = op_kernel_hook_count;
    memcpy(ks1_expected_dialogs, op_kernel_dialogs, sizeof(ks1_expected_dialogs));
    memcpy(ks1_expected_hooks, op_kernel_hooks, sizeof(ks1_expected_hooks));
}
static void ks1_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks1_show;
    op_kernel_window = ks1_words[profile % 8];
    op_kernel_field = ks1_words[(profile + 1) % 8];
    op_kernel_callback = ks1_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks1_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks1_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks1_words[(profile + 5) % 8];
    op_kernel_hook_count = ks1_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks1_words[(profile + i) % 8];
    }
    ks1_calls = 0;
    ks1_mutate = 0;
    ks1_return_word = ks1_words[(profile + 7) % 8];
    ks1_snapshot();
}
static int op_test_kernel_hide(void)
{
    unsigned int p, r;
    int m;
    for (p = 0; p < 64; ++p)
        for (r = 0; r < 8; ++r)
            for (m = 0; m < 2; ++m)
            {
                ks1_seed(p);
                ks1_mutate = m;
                ks1_return_word = ks1_words[r];
                ks1_observed_window = op_kernel_window;
                ks1_observed_command = 0;
                op_kernel_hide();
                KS1_CHECK(ks1_calls == 1);
                ks1_verify();
                KS1_CHECK(op_kernel_show_window == ks1_show);
            }
    printf("kernel_hide: %d checks, %d failures\n", ks1_checks, ks1_failures);
    return ks1_failures != 0;
}

#undef KS1_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks2_checks, ks2_failures;
static void ks2_check(int ok, int line)
{
    ++ks2_checks;
    if (!ok)
    {
        ++ks2_failures;
        printf("line%d failed\n", line);
    }
}
#define KS2_CHECK(x) ks2_check(!!(x), __LINE__)
static unsigned int ks2_expected_window, ks2_expected_field, ks2_expected_callback, ks2_expected_enabled,
    ks2_expected_dialog_count, ks2_expected_hook_count, ks2_expected_dialogs[16];
static int ks2_expected_command;
static OP_KERNEL_HOOK ks2_expected_hooks[16];
static int ks2_calls, ks2_mutate, ks2_observed_command;
static unsigned int ks2_observed_window, ks2_return_word;
static const unsigned int ks2_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks2_verify(void)
{
    KS2_CHECK(op_kernel_window == ks2_expected_window);
    KS2_CHECK(op_kernel_field == ks2_expected_field);
    KS2_CHECK(op_kernel_callback == ks2_expected_callback);
    KS2_CHECK(op_kernel_callback_enabled == ks2_expected_enabled);
    KS2_CHECK(op_kernel_show_command == ks2_expected_command);
    KS2_CHECK(op_kernel_dialog_count == ks2_expected_dialog_count);
    KS2_CHECK(op_kernel_hook_count == ks2_expected_hook_count);
    KS2_CHECK(memcmp(op_kernel_dialogs, ks2_expected_dialogs, sizeof(ks2_expected_dialogs)) == 0);
    KS2_CHECK(memcmp(op_kernel_hooks, ks2_expected_hooks, sizeof(ks2_expected_hooks)) == 0);
}
static int __stdcall ks2_show(unsigned int window, int command)
{
    ++ks2_calls;
    KS2_CHECK(ks2_calls == 1);
    KS2_CHECK(window == ks2_observed_window);
    KS2_CHECK(command == ks2_observed_command);
    ks2_verify();
    if (ks2_mutate)
    {
        op_kernel_window = ks2_expected_window = 0x76543210u;
        op_kernel_show_command = ks2_expected_command = -7;
        op_kernel_field = ks2_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks2_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks2_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks2_return_word;
}
static void ks2_snapshot(void)
{
    ks2_expected_window = op_kernel_window;
    ks2_expected_field = op_kernel_field;
    ks2_expected_callback = op_kernel_callback;
    ks2_expected_enabled = op_kernel_callback_enabled;
    ks2_expected_command = op_kernel_show_command;
    ks2_expected_dialog_count = op_kernel_dialog_count;
    ks2_expected_hook_count = op_kernel_hook_count;
    memcpy(ks2_expected_dialogs, op_kernel_dialogs, sizeof(ks2_expected_dialogs));
    memcpy(ks2_expected_hooks, op_kernel_hooks, sizeof(ks2_expected_hooks));
}
static void ks2_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks2_show;
    op_kernel_window = ks2_words[profile % 8];
    op_kernel_field = ks2_words[(profile + 1) % 8];
    op_kernel_callback = ks2_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks2_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks2_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks2_words[(profile + 5) % 8];
    op_kernel_hook_count = ks2_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks2_words[(profile + i) % 8];
    }
    ks2_calls = 0;
    ks2_mutate = 0;
    ks2_return_word = ks2_words[(profile + 7) % 8];
    ks2_snapshot();
}
static int op_test_kernel_set_field(void)
{
    unsigned int p, v;
    for (p = 0; p < 64; ++p)
        for (v = 0; v < 8; ++v)
        {
            ks2_seed(p);
            ks2_expected_field = ks2_words[v];
            op_kernel_set_field(ks2_words[v]);
            ks2_verify();
            KS2_CHECK(ks2_calls == 0);
        }
    printf("kernel_set_field: %d checks, %d failures\n", ks2_checks, ks2_failures);
    return ks2_failures != 0;
}

#undef KS2_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks3_checks, ks3_failures;
static void ks3_check(int ok, int line)
{
    ++ks3_checks;
    if (!ok)
    {
        ++ks3_failures;
        printf("line%d failed\n", line);
    }
}
#define KS3_CHECK(x) ks3_check(!!(x), __LINE__)
static unsigned int ks3_expected_window, ks3_expected_field, ks3_expected_callback, ks3_expected_enabled,
    ks3_expected_dialog_count, ks3_expected_hook_count, ks3_expected_dialogs[16];
static int ks3_expected_command;
static OP_KERNEL_HOOK ks3_expected_hooks[16];
static int ks3_calls, ks3_mutate, ks3_observed_command;
static unsigned int ks3_observed_window, ks3_return_word;
static const unsigned int ks3_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks3_verify(void)
{
    KS3_CHECK(op_kernel_window == ks3_expected_window);
    KS3_CHECK(op_kernel_field == ks3_expected_field);
    KS3_CHECK(op_kernel_callback == ks3_expected_callback);
    KS3_CHECK(op_kernel_callback_enabled == ks3_expected_enabled);
    KS3_CHECK(op_kernel_show_command == ks3_expected_command);
    KS3_CHECK(op_kernel_dialog_count == ks3_expected_dialog_count);
    KS3_CHECK(op_kernel_hook_count == ks3_expected_hook_count);
    KS3_CHECK(memcmp(op_kernel_dialogs, ks3_expected_dialogs, sizeof(ks3_expected_dialogs)) == 0);
    KS3_CHECK(memcmp(op_kernel_hooks, ks3_expected_hooks, sizeof(ks3_expected_hooks)) == 0);
}
static int __stdcall ks3_show(unsigned int window, int command)
{
    ++ks3_calls;
    KS3_CHECK(ks3_calls == 1);
    KS3_CHECK(window == ks3_observed_window);
    KS3_CHECK(command == ks3_observed_command);
    ks3_verify();
    if (ks3_mutate)
    {
        op_kernel_window = ks3_expected_window = 0x76543210u;
        op_kernel_show_command = ks3_expected_command = -7;
        op_kernel_field = ks3_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks3_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks3_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks3_return_word;
}
static void ks3_snapshot(void)
{
    ks3_expected_window = op_kernel_window;
    ks3_expected_field = op_kernel_field;
    ks3_expected_callback = op_kernel_callback;
    ks3_expected_enabled = op_kernel_callback_enabled;
    ks3_expected_command = op_kernel_show_command;
    ks3_expected_dialog_count = op_kernel_dialog_count;
    ks3_expected_hook_count = op_kernel_hook_count;
    memcpy(ks3_expected_dialogs, op_kernel_dialogs, sizeof(ks3_expected_dialogs));
    memcpy(ks3_expected_hooks, op_kernel_hooks, sizeof(ks3_expected_hooks));
}
static void ks3_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks3_show;
    op_kernel_window = ks3_words[profile % 8];
    op_kernel_field = ks3_words[(profile + 1) % 8];
    op_kernel_callback = ks3_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks3_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks3_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks3_words[(profile + 5) % 8];
    op_kernel_hook_count = ks3_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks3_words[(profile + i) % 8];
    }
    ks3_calls = 0;
    ks3_mutate = 0;
    ks3_return_word = ks3_words[(profile + 7) % 8];
    ks3_snapshot();
}
static int op_test_kernel_set_callback(void)
{
    unsigned int p, v;
    for (p = 0; p < 64; ++p)
        for (v = 0; v < 8; ++v)
        {
            ks3_seed(p);
            ks3_expected_callback = ks3_words[v];
            ks3_expected_enabled = ks3_words[v] ? 1 : 0;
            KS3_CHECK(op_kernel_set_callback(ks3_words[v]) == 1);
            ks3_verify();
            KS3_CHECK(ks3_calls == 0);
        }
    printf("kernel_set_callback: %d checks, %d failures\n", ks3_checks, ks3_failures);
    return ks3_failures != 0;
}

#undef KS3_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks4_checks, ks4_failures;
static void ks4_check(int ok, int line)
{
    ++ks4_checks;
    if (!ok)
    {
        ++ks4_failures;
        printf("line%d failed\n", line);
    }
}
#define KS4_CHECK(x) ks4_check(!!(x), __LINE__)
static unsigned int ks4_expected_window, ks4_expected_field, ks4_expected_callback, ks4_expected_enabled,
    ks4_expected_dialog_count, ks4_expected_hook_count, ks4_expected_dialogs[16];
static int ks4_expected_command;
static OP_KERNEL_HOOK ks4_expected_hooks[16];
static int ks4_calls, ks4_mutate, ks4_observed_command;
static unsigned int ks4_observed_window, ks4_return_word;
static const unsigned int ks4_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks4_verify(void)
{
    KS4_CHECK(op_kernel_window == ks4_expected_window);
    KS4_CHECK(op_kernel_field == ks4_expected_field);
    KS4_CHECK(op_kernel_callback == ks4_expected_callback);
    KS4_CHECK(op_kernel_callback_enabled == ks4_expected_enabled);
    KS4_CHECK(op_kernel_show_command == ks4_expected_command);
    KS4_CHECK(op_kernel_dialog_count == ks4_expected_dialog_count);
    KS4_CHECK(op_kernel_hook_count == ks4_expected_hook_count);
    KS4_CHECK(memcmp(op_kernel_dialogs, ks4_expected_dialogs, sizeof(ks4_expected_dialogs)) == 0);
    KS4_CHECK(memcmp(op_kernel_hooks, ks4_expected_hooks, sizeof(ks4_expected_hooks)) == 0);
}
static int __stdcall ks4_show(unsigned int window, int command)
{
    ++ks4_calls;
    KS4_CHECK(ks4_calls == 1);
    KS4_CHECK(window == ks4_observed_window);
    KS4_CHECK(command == ks4_observed_command);
    ks4_verify();
    if (ks4_mutate)
    {
        op_kernel_window = ks4_expected_window = 0x76543210u;
        op_kernel_show_command = ks4_expected_command = -7;
        op_kernel_field = ks4_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks4_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks4_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks4_return_word;
}
static void ks4_snapshot(void)
{
    ks4_expected_window = op_kernel_window;
    ks4_expected_field = op_kernel_field;
    ks4_expected_callback = op_kernel_callback;
    ks4_expected_enabled = op_kernel_callback_enabled;
    ks4_expected_command = op_kernel_show_command;
    ks4_expected_dialog_count = op_kernel_dialog_count;
    ks4_expected_hook_count = op_kernel_hook_count;
    memcpy(ks4_expected_dialogs, op_kernel_dialogs, sizeof(ks4_expected_dialogs));
    memcpy(ks4_expected_hooks, op_kernel_hooks, sizeof(ks4_expected_hooks));
}
static void ks4_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks4_show;
    op_kernel_window = ks4_words[profile % 8];
    op_kernel_field = ks4_words[(profile + 1) % 8];
    op_kernel_callback = ks4_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks4_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks4_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks4_words[(profile + 5) % 8];
    op_kernel_hook_count = ks4_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks4_words[(profile + i) % 8];
    }
    ks4_calls = 0;
    ks4_mutate = 0;
    ks4_return_word = ks4_words[(profile + 7) % 8];
    ks4_snapshot();
}
static int op_test_kernel_get_window(void)
{
    unsigned int p, v;
    for (p = 0; p < 64; ++p)
        for (v = 0; v < 8; ++v)
        {
            ks4_seed(p);
            KS4_CHECK(op_kernel_get_window() == ks4_expected_window);
            ks4_verify();
            op_kernel_window = ks4_expected_window = ks4_words[v];
            KS4_CHECK(op_kernel_get_window() == ks4_expected_window);
            ks4_verify();
            KS4_CHECK(ks4_calls == 0);
        }
    printf("kernel_get_window: %d checks, %d failures\n", ks4_checks, ks4_failures);
    return ks4_failures != 0;
}

#undef KS4_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks5_checks, ks5_failures;
static void ks5_check(int ok, int line)
{
    ++ks5_checks;
    if (!ok)
    {
        ++ks5_failures;
        printf("line%d failed\n", line);
    }
}
#define KS5_CHECK(x) ks5_check(!!(x), __LINE__)
static unsigned int ks5_expected_window, ks5_expected_field, ks5_expected_callback, ks5_expected_enabled,
    ks5_expected_dialog_count, ks5_expected_hook_count, ks5_expected_dialogs[16];
static int ks5_expected_command;
static OP_KERNEL_HOOK ks5_expected_hooks[16];
static int ks5_calls, ks5_mutate, ks5_observed_command;
static unsigned int ks5_observed_window, ks5_return_word;
static const unsigned int ks5_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks5_verify(void)
{
    KS5_CHECK(op_kernel_window == ks5_expected_window);
    KS5_CHECK(op_kernel_field == ks5_expected_field);
    KS5_CHECK(op_kernel_callback == ks5_expected_callback);
    KS5_CHECK(op_kernel_callback_enabled == ks5_expected_enabled);
    KS5_CHECK(op_kernel_show_command == ks5_expected_command);
    KS5_CHECK(op_kernel_dialog_count == ks5_expected_dialog_count);
    KS5_CHECK(op_kernel_hook_count == ks5_expected_hook_count);
    KS5_CHECK(memcmp(op_kernel_dialogs, ks5_expected_dialogs, sizeof(ks5_expected_dialogs)) == 0);
    KS5_CHECK(memcmp(op_kernel_hooks, ks5_expected_hooks, sizeof(ks5_expected_hooks)) == 0);
}
static int __stdcall ks5_show(unsigned int window, int command)
{
    ++ks5_calls;
    KS5_CHECK(ks5_calls == 1);
    KS5_CHECK(window == ks5_observed_window);
    KS5_CHECK(command == ks5_observed_command);
    ks5_verify();
    if (ks5_mutate)
    {
        op_kernel_window = ks5_expected_window = 0x76543210u;
        op_kernel_show_command = ks5_expected_command = -7;
        op_kernel_field = ks5_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks5_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks5_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks5_return_word;
}
static void ks5_snapshot(void)
{
    ks5_expected_window = op_kernel_window;
    ks5_expected_field = op_kernel_field;
    ks5_expected_callback = op_kernel_callback;
    ks5_expected_enabled = op_kernel_callback_enabled;
    ks5_expected_command = op_kernel_show_command;
    ks5_expected_dialog_count = op_kernel_dialog_count;
    ks5_expected_hook_count = op_kernel_hook_count;
    memcpy(ks5_expected_dialogs, op_kernel_dialogs, sizeof(ks5_expected_dialogs));
    memcpy(ks5_expected_hooks, op_kernel_hooks, sizeof(ks5_expected_hooks));
}
static void ks5_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks5_show;
    op_kernel_window = ks5_words[profile % 8];
    op_kernel_field = ks5_words[(profile + 1) % 8];
    op_kernel_callback = ks5_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks5_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks5_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks5_words[(profile + 5) % 8];
    op_kernel_hook_count = ks5_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks5_words[(profile + i) % 8];
    }
    ks5_calls = 0;
    ks5_mutate = 0;
    ks5_return_word = ks5_words[(profile + 7) % 8];
    ks5_snapshot();
}
static int op_test_kernel_add_hook(void)
{
    unsigned int p, slot, v, i;
    for (p = 0; p < 64; ++p)
        for (slot = 0; slot < 16; ++slot)
            for (v = 0; v < 8; ++v)
            {
                ks5_seed(p);
                for (i = 0; i < slot; ++i)
                    op_kernel_hooks[i].active = ks5_words[1 + (i % 7)];
                op_kernel_hooks[slot].active = 0;
                ks5_snapshot();
                ks5_expected_hooks[slot].word = ks5_words[v];
                ks5_expected_hooks[slot].active = 1;
                ks5_expected_hook_count++;
                KS5_CHECK(op_kernel_add_hook(ks5_words[v]) == 1);
                ks5_verify();
                KS5_CHECK(ks5_calls == 0);
            }
    printf("kernel_add_hook: %d checks, %d failures\n", ks5_checks, ks5_failures);
    return ks5_failures != 0;
}

#undef KS5_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks6_checks, ks6_failures;
static void ks6_check(int ok, int line)
{
    ++ks6_checks;
    if (!ok)
    {
        ++ks6_failures;
        printf("line%d failed\n", line);
    }
}
#define KS6_CHECK(x) ks6_check(!!(x), __LINE__)
static unsigned int ks6_expected_window, ks6_expected_field, ks6_expected_callback, ks6_expected_enabled,
    ks6_expected_dialog_count, ks6_expected_hook_count, ks6_expected_dialogs[16];
static int ks6_expected_command;
static OP_KERNEL_HOOK ks6_expected_hooks[16];
static int ks6_calls, ks6_mutate, ks6_observed_command;
static unsigned int ks6_observed_window, ks6_return_word;
static const unsigned int ks6_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks6_verify(void)
{
    KS6_CHECK(op_kernel_window == ks6_expected_window);
    KS6_CHECK(op_kernel_field == ks6_expected_field);
    KS6_CHECK(op_kernel_callback == ks6_expected_callback);
    KS6_CHECK(op_kernel_callback_enabled == ks6_expected_enabled);
    KS6_CHECK(op_kernel_show_command == ks6_expected_command);
    KS6_CHECK(op_kernel_dialog_count == ks6_expected_dialog_count);
    KS6_CHECK(op_kernel_hook_count == ks6_expected_hook_count);
    KS6_CHECK(memcmp(op_kernel_dialogs, ks6_expected_dialogs, sizeof(ks6_expected_dialogs)) == 0);
    KS6_CHECK(memcmp(op_kernel_hooks, ks6_expected_hooks, sizeof(ks6_expected_hooks)) == 0);
}
static int __stdcall ks6_show(unsigned int window, int command)
{
    ++ks6_calls;
    KS6_CHECK(ks6_calls == 1);
    KS6_CHECK(window == ks6_observed_window);
    KS6_CHECK(command == ks6_observed_command);
    ks6_verify();
    if (ks6_mutate)
    {
        op_kernel_window = ks6_expected_window = 0x76543210u;
        op_kernel_show_command = ks6_expected_command = -7;
        op_kernel_field = ks6_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks6_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks6_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks6_return_word;
}
static void ks6_snapshot(void)
{
    ks6_expected_window = op_kernel_window;
    ks6_expected_field = op_kernel_field;
    ks6_expected_callback = op_kernel_callback;
    ks6_expected_enabled = op_kernel_callback_enabled;
    ks6_expected_command = op_kernel_show_command;
    ks6_expected_dialog_count = op_kernel_dialog_count;
    ks6_expected_hook_count = op_kernel_hook_count;
    memcpy(ks6_expected_dialogs, op_kernel_dialogs, sizeof(ks6_expected_dialogs));
    memcpy(ks6_expected_hooks, op_kernel_hooks, sizeof(ks6_expected_hooks));
}
static void ks6_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks6_show;
    op_kernel_window = ks6_words[profile % 8];
    op_kernel_field = ks6_words[(profile + 1) % 8];
    op_kernel_callback = ks6_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks6_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks6_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks6_words[(profile + 5) % 8];
    op_kernel_hook_count = ks6_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks6_words[(profile + i) % 8];
    }
    ks6_calls = 0;
    ks6_mutate = 0;
    ks6_return_word = ks6_words[(profile + 7) % 8];
    ks6_snapshot();
}
static int op_test_kernel_remove_hook(void)
{
    unsigned int p, slot, v, i, needle;
    int found, want;
    for (p = 0; p < 64; ++p)
        for (slot = 0; slot <= 16; ++slot)
            for (v = 0; v < 8; ++v)
            {
                ks6_seed(p);
                needle = ks6_words[v];
                if (slot < 16)
                {
                    op_kernel_hooks[slot].word = needle;
                    if (slot + 1 < 16)
                        op_kernel_hooks[slot + 1].word = needle;
                }
                ks6_snapshot();
                found = -1;
                if ((int)ks6_expected_hook_count > 0)
                    for (i = 0; i < 16; ++i)
                        if (ks6_expected_hooks[i].word == needle)
                        {
                            found = (int)i;
                            break;
                        }
                want = found >= 0;
                if (want)
                {
                    ks6_expected_hooks[found].word = ks6_expected_hooks[found].active = 0;
                    ks6_expected_hook_count--;
                }
                KS6_CHECK(op_kernel_remove_hook(needle) == want);
                ks6_verify();
                KS6_CHECK(ks6_calls == 0);
            }
    printf("kernel_remove_hook: %d checks, %d failures\n", ks6_checks, ks6_failures);
    return ks6_failures != 0;
}

#undef KS6_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks7_checks, ks7_failures;
static void ks7_check(int ok, int line)
{
    ++ks7_checks;
    if (!ok)
    {
        ++ks7_failures;
        printf("line%d failed\n", line);
    }
}
#define KS7_CHECK(x) ks7_check(!!(x), __LINE__)
static unsigned int ks7_expected_window, ks7_expected_field, ks7_expected_callback, ks7_expected_enabled,
    ks7_expected_dialog_count, ks7_expected_hook_count, ks7_expected_dialogs[16];
static int ks7_expected_command;
static OP_KERNEL_HOOK ks7_expected_hooks[16];
static int ks7_calls, ks7_mutate, ks7_observed_command;
static unsigned int ks7_observed_window, ks7_return_word;
static const unsigned int ks7_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks7_verify(void)
{
    KS7_CHECK(op_kernel_window == ks7_expected_window);
    KS7_CHECK(op_kernel_field == ks7_expected_field);
    KS7_CHECK(op_kernel_callback == ks7_expected_callback);
    KS7_CHECK(op_kernel_callback_enabled == ks7_expected_enabled);
    KS7_CHECK(op_kernel_show_command == ks7_expected_command);
    KS7_CHECK(op_kernel_dialog_count == ks7_expected_dialog_count);
    KS7_CHECK(op_kernel_hook_count == ks7_expected_hook_count);
    KS7_CHECK(memcmp(op_kernel_dialogs, ks7_expected_dialogs, sizeof(ks7_expected_dialogs)) == 0);
    KS7_CHECK(memcmp(op_kernel_hooks, ks7_expected_hooks, sizeof(ks7_expected_hooks)) == 0);
}
static int __stdcall ks7_show(unsigned int window, int command)
{
    ++ks7_calls;
    KS7_CHECK(ks7_calls == 1);
    KS7_CHECK(window == ks7_observed_window);
    KS7_CHECK(command == ks7_observed_command);
    ks7_verify();
    if (ks7_mutate)
    {
        op_kernel_window = ks7_expected_window = 0x76543210u;
        op_kernel_show_command = ks7_expected_command = -7;
        op_kernel_field = ks7_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks7_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks7_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks7_return_word;
}
static void ks7_snapshot(void)
{
    ks7_expected_window = op_kernel_window;
    ks7_expected_field = op_kernel_field;
    ks7_expected_callback = op_kernel_callback;
    ks7_expected_enabled = op_kernel_callback_enabled;
    ks7_expected_command = op_kernel_show_command;
    ks7_expected_dialog_count = op_kernel_dialog_count;
    ks7_expected_hook_count = op_kernel_hook_count;
    memcpy(ks7_expected_dialogs, op_kernel_dialogs, sizeof(ks7_expected_dialogs));
    memcpy(ks7_expected_hooks, op_kernel_hooks, sizeof(ks7_expected_hooks));
}
static void ks7_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks7_show;
    op_kernel_window = ks7_words[profile % 8];
    op_kernel_field = ks7_words[(profile + 1) % 8];
    op_kernel_callback = ks7_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks7_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks7_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks7_words[(profile + 5) % 8];
    op_kernel_hook_count = ks7_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks7_words[(profile + i) % 8];
    }
    ks7_calls = 0;
    ks7_mutate = 0;
    ks7_return_word = ks7_words[(profile + 7) % 8];
    ks7_snapshot();
}
static int op_test_kernel_add_dialog(void)
{
    unsigned int p, n, v;
    int want;
    for (p = 0; p < 64; ++p)
        for (n = 0; n < 20; ++n)
            for (v = 0; v < 8; ++v)
            {
                ks7_seed(p);
                op_kernel_dialog_count = n < 17 ? n : ks7_words[n - 13];
                ks7_snapshot();
                want = ks7_expected_dialog_count < 16;
                if (want)
                {
                    ks7_expected_dialogs[ks7_expected_dialog_count] = ks7_words[v];
                    ks7_expected_dialog_count++;
                }
                KS7_CHECK(op_kernel_add_dialog(ks7_words[v]) == want);
                ks7_verify();
                KS7_CHECK(ks7_calls == 0);
            }
    printf("kernel_add_dialog: %d checks, %d failures\n", ks7_checks, ks7_failures);
    return ks7_failures != 0;
}

#undef KS7_CHECK

#include "../src/kernel_helpers.h"
#include <stdio.h>
#include <string.h>
static int ks8_checks, ks8_failures;
static void ks8_check(int ok, int line)
{
    ++ks8_checks;
    if (!ok)
    {
        ++ks8_failures;
        printf("line%d failed\n", line);
    }
}
#define KS8_CHECK(x) ks8_check(!!(x), __LINE__)
static unsigned int ks8_expected_window, ks8_expected_field, ks8_expected_callback, ks8_expected_enabled,
    ks8_expected_dialog_count, ks8_expected_hook_count, ks8_expected_dialogs[16];
static int ks8_expected_command;
static OP_KERNEL_HOOK ks8_expected_hooks[16];
static int ks8_calls, ks8_mutate, ks8_observed_command;
static unsigned int ks8_observed_window, ks8_return_word;
static const unsigned int ks8_words[] = {0, 1, 15, 16, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void ks8_verify(void)
{
    KS8_CHECK(op_kernel_window == ks8_expected_window);
    KS8_CHECK(op_kernel_field == ks8_expected_field);
    KS8_CHECK(op_kernel_callback == ks8_expected_callback);
    KS8_CHECK(op_kernel_callback_enabled == ks8_expected_enabled);
    KS8_CHECK(op_kernel_show_command == ks8_expected_command);
    KS8_CHECK(op_kernel_dialog_count == ks8_expected_dialog_count);
    KS8_CHECK(op_kernel_hook_count == ks8_expected_hook_count);
    KS8_CHECK(memcmp(op_kernel_dialogs, ks8_expected_dialogs, sizeof(ks8_expected_dialogs)) == 0);
    KS8_CHECK(memcmp(op_kernel_hooks, ks8_expected_hooks, sizeof(ks8_expected_hooks)) == 0);
}
static int __stdcall ks8_show(unsigned int window, int command)
{
    ++ks8_calls;
    KS8_CHECK(ks8_calls == 1);
    KS8_CHECK(window == ks8_observed_window);
    KS8_CHECK(command == ks8_observed_command);
    ks8_verify();
    if (ks8_mutate)
    {
        op_kernel_window = ks8_expected_window = 0x76543210u;
        op_kernel_show_command = ks8_expected_command = -7;
        op_kernel_field = ks8_expected_field = 0xabcdef01u;
        op_kernel_dialogs[3] = ks8_expected_dialogs[3] = 0x44444444u;
        op_kernel_hooks[5].word = ks8_expected_hooks[5].word = 0x55555555u;
    }
    return (int)ks8_return_word;
}
static void ks8_snapshot(void)
{
    ks8_expected_window = op_kernel_window;
    ks8_expected_field = op_kernel_field;
    ks8_expected_callback = op_kernel_callback;
    ks8_expected_enabled = op_kernel_callback_enabled;
    ks8_expected_command = op_kernel_show_command;
    ks8_expected_dialog_count = op_kernel_dialog_count;
    ks8_expected_hook_count = op_kernel_hook_count;
    memcpy(ks8_expected_dialogs, op_kernel_dialogs, sizeof(ks8_expected_dialogs));
    memcpy(ks8_expected_hooks, op_kernel_hooks, sizeof(ks8_expected_hooks));
}
static void ks8_seed(unsigned int profile)
{
    unsigned int i;
    op_kernel_show_window = ks8_show;
    op_kernel_window = ks8_words[profile % 8];
    op_kernel_field = ks8_words[(profile + 1) % 8];
    op_kernel_callback = ks8_words[(profile + 2) % 8];
    op_kernel_callback_enabled = ks8_words[(profile + 3) % 8];
    op_kernel_show_command = (int)ks8_words[(profile + 4) % 8];
    op_kernel_dialog_count = ks8_words[(profile + 5) % 8];
    op_kernel_hook_count = ks8_words[(profile + 6) % 8];
    for (i = 0; i < 16; ++i)
    {
        op_kernel_dialogs[i] = 0x11000000u + i;
        op_kernel_hooks[i].word = 0x22000000u + i;
        op_kernel_hooks[i].active = ks8_words[(profile + i) % 8];
    }
    ks8_calls = 0;
    ks8_mutate = 0;
    ks8_return_word = ks8_words[(profile + 7) % 8];
    ks8_snapshot();
}
static int op_test_kernel_remove_dialog(void)
{
    unsigned int p, n, slot, v, i, out, needle;
    int removed;
    for (p = 0; p < 8; ++p)
        for (n = 0; n <= 16; ++n)
            for (slot = 0; slot <= 16; ++slot)
                for (v = 0; v < 8; ++v)
                {
                    ks8_seed(p);
                    op_kernel_dialog_count = n;
                    needle = ks8_words[v];
                    if (slot < n)
                    {
                        op_kernel_dialogs[slot] = needle;
                        if (slot + 1 < n)
                            op_kernel_dialogs[slot + 1] = needle;
                    }
                    ks8_snapshot();
                    removed = 0;
                    out = 0;
                    for (i = 0; i < n; ++i)
                    {
                        if (!removed && op_kernel_dialogs[i] == needle)
                        {
                            removed = 1;
                            continue;
                        }
                        ks8_expected_dialogs[out++] = op_kernel_dialogs[i];
                    }
                    if (removed)
                    {
                        ks8_expected_dialogs[n - 1] = 0;
                        ks8_expected_dialog_count = n - 1;
                    }
                    KS8_CHECK(op_kernel_remove_dialog(needle) == removed);
                    ks8_verify();
                    KS8_CHECK(ks8_calls == 0);
                }
    printf("kernel_remove_dialog: %d checks, %d failures\n", ks8_checks, ks8_failures);
    return ks8_failures != 0;
}

#undef KS8_CHECK

static int op_test_kernel_connected(void)
{
    unsigned int profile, i, j, needle;
    int mutation;
    int before_checks = ks0_checks, before_failures = ks0_failures;
    for (profile = 0; profile < 16; ++profile)
        for (mutation = 0; mutation < 2; ++mutation)
        {
            ks0_seed(profile);
            memset(op_kernel_hooks, 0, sizeof(op_kernel_hooks));
            memset(op_kernel_dialogs, 0, sizeof(op_kernel_dialogs));
            op_kernel_hook_count = op_kernel_dialog_count = 0;
            ks0_snapshot();
            ks0_mutate = mutation;
            ks0_observed_window = op_kernel_window;
            ks0_observed_command = op_kernel_show_command;
            op_kernel_show();
            ks0_check(ks0_calls == 1, __LINE__);
            ks0_verify();
            ks0_check(op_kernel_get_window() == ks0_expected_window, __LINE__);
            ks0_verify();
            ks0_calls = 0;
            ks0_observed_window = op_kernel_window;
            ks0_observed_command = 0;
            op_kernel_hide();
            ks0_check(ks0_calls == 1, __LINE__);
            ks0_verify();
            ks0_expected_field = ks0_words[profile % 8];
            op_kernel_set_field(ks0_expected_field);
            ks0_verify();
            ks0_expected_callback = ks0_words[(profile + 1) % 8];
            ks0_expected_enabled = ks0_expected_callback != 0;
            ks0_check(op_kernel_set_callback(ks0_expected_callback) == 1, __LINE__);
            ks0_verify();
            for (i = 0; i < 16; ++i)
            {
                needle = 0x30000000u + i;
                ks0_expected_hooks[i].word = needle;
                ks0_expected_hooks[i].active = 1;
                ++ks0_expected_hook_count;
                ks0_check(op_kernel_add_hook(needle) == 1, __LINE__);
                ks0_verify();
            }
            for (i = 0; i < 16; ++i)
            {
                needle = 0x40000000u + (i % 7);
                ks0_expected_dialogs[i] = needle;
                ++ks0_expected_dialog_count;
                ks0_check(op_kernel_add_dialog(needle) == 1, __LINE__);
                ks0_verify();
            }
            ks0_check(op_kernel_add_dialog(0x87654321u) == 0, __LINE__);
            ks0_verify();
            for (i = 0; i < 16; ++i)
            {
                needle = 0x30000000u + i;
                ks0_expected_hooks[i].word = ks0_expected_hooks[i].active = 0;
                --ks0_expected_hook_count;
                ks0_check(op_kernel_remove_hook(needle) == 1, __LINE__);
                ks0_verify();
            }
            ks0_check(op_kernel_remove_hook(0x12345678u) == 0, __LINE__);
            ks0_verify();
            for (i = 16; i > 0; --i)
            {
                needle = ks0_expected_dialogs[0];
                for (j = 1; j < i; ++j)
                    ks0_expected_dialogs[j - 1] = ks0_expected_dialogs[j];
                ks0_expected_dialogs[i - 1] = 0;
                --ks0_expected_dialog_count;
                ks0_check(op_kernel_remove_dialog(needle) == 1, __LINE__);
                ks0_verify();
            }
            ks0_check(op_kernel_remove_dialog(0x12345678u) == 0, __LINE__);
            ks0_verify();
            ks0_expected_callback = ks0_expected_enabled = 0;
            ks0_check(op_kernel_set_callback(0) == 1, __LINE__);
            ks0_verify();
            ks0_check(op_kernel_get_window() == ks0_expected_window, __LINE__);
            ks0_verify();
            ks0_check(op_kernel_show_window == ks0_show, __LINE__);
        }
    printf("kernel connected: %d checks, %d failures\n", ks0_checks - before_checks, ks0_failures - before_failures);
    return ks0_failures != before_failures;
}
