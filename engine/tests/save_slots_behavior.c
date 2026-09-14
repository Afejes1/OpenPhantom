#include "../src/save_slots.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
static int sfl_active;
static int sfl_checks, sfl_failures;
static void sfl_check(int ok, int line)
{
    ++sfl_checks;
    if (!ok)
    {
        ++sfl_failures;
        printf("line%d failed\n", line);
    }
}
#define SFL_CHECK(x) sfl_check(!!(x), __LINE__)

typedef struct SFL_OWNED_ROWS
{
    unsigned int before;
    OP_SAVE_LABEL_ROW rows[99];
    unsigned int after;
} SFL_OWNED_ROWS;
static SFL_OWNED_ROWS sfl_owned, sfl_expected;
static unsigned int sfl_tokens[4];
static int sfl_profile, sfl_mutate, sfl_next_index, sfl_calls;
static void sfl_verify(void)
{
    SFL_CHECK(memcmp(&sfl_owned, &sfl_expected, sizeof(sfl_owned)) == 0);
}
static void sfl_release(void *pointer)
{
    int i = sfl_next_index;
    while (i < 99 && sfl_expected.rows[i].label == 0)
        ++i;
    SFL_CHECK(i < 99);
    if (i >= 99)
        return;
    SFL_CHECK(pointer == sfl_expected.rows[i].label);
    sfl_verify();
    if (pointer != sfl_expected.rows[i].label)
        return;
    ++sfl_calls;
    if (sfl_mutate)
    {
        sfl_owned.rows[i].label = sfl_expected.rows[i].label = &sfl_tokens[3];
        if (i + 2 < 99)
            sfl_owned.rows[i + 2].label = sfl_expected.rows[i + 2].label = sfl_mutate == 1 ? &sfl_tokens[2] : 0;
        if (i > 0 && sfl_mutate == 2)
            sfl_owned.rows[i - 1].label = sfl_expected.rows[i - 1].label = &sfl_tokens[1];
    }
    sfl_verify();
    sfl_expected.rows[i].label = 0;
    sfl_next_index = i + 1;
}
static int op_test_save_slot_labels(void)
{
    int i;
    sfl_active = 1;
    for (sfl_profile = 0; sfl_profile < 4; ++sfl_profile)
        for (sfl_mutate = 0; sfl_mutate < 3; ++sfl_mutate)
        {
            memset(&sfl_owned, 0x57, sizeof(sfl_owned));
            for (i = 0; i < 99; ++i)
                sfl_owned.rows[i].label = sfl_profile == 0   ? 0
                                          : sfl_profile == 1 ? &sfl_tokens[i % 3]
                                          : sfl_profile == 2 ? (i % 2 ? &sfl_tokens[0] : 0)
                                                             : (i == 0 || i == 98 ? &sfl_tokens[1] : 0);
            memcpy(&sfl_expected, &sfl_owned, sizeof(sfl_expected));
            sfl_next_index = sfl_calls = 0;
            op_save_free_slot_labels(sfl_owned.rows);
            sfl_verify();
            for (i = sfl_next_index; i < 99; ++i)
                SFL_CHECK(sfl_expected.rows[i].label == 0);
            SFL_CHECK(sfl_calls >= 0 && sfl_calls <= 99);
            if (sfl_profile == 0)
                SFL_CHECK(sfl_calls == 0);
        }
    sfl_active = 0;
    printf("save_free_slot_labels: %d checks, %d failures\n", sfl_checks, sfl_failures);
    return sfl_failures != 0;
}

#undef SFL_CHECK

#include "../src/save_slots.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int ssp_active, ssp_checks, ssp_failures;
static void ssp_check(int ok, int line)
{
    ++ssp_checks;
    if (!ok)
    {
        ++ssp_failures;
        printf("save-slot chain line%d failed\n", line);
    }
}
#define SSP_CHECK(x) ssp_check(!!(x), __LINE__)
char op_save_path[40], op_save_path_format[] = "authored-format", op_save_path_prefix[] = "authored-prefix";
static char ssp_expected_path[40], ssp_expected_format[sizeof(op_save_path_format)],
    ssp_expected_prefix[sizeof(op_save_path_prefix)];
static const int ssp_slots[] = {0, 1, 98, 99, -1, INT_MIN, INT_MAX}, ssp_answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int ssp_slot_case, ssp_answer, ssp_format_answer, ssp_mutate, ssp_stage, ssp_mode;
static void ssp_verify(void)
{
    SSP_CHECK(memcmp(op_save_path, ssp_expected_path, sizeof(ssp_expected_path)) == 0);
    SSP_CHECK(memcmp(op_save_path_format, ssp_expected_format, sizeof(ssp_expected_format)) == 0);
    SSP_CHECK(memcmp(op_save_path_prefix, ssp_expected_prefix, sizeof(ssp_expected_prefix)) == 0);
}
void op_save_lifecycle(int a, int b)
{
    SSP_CHECK(ssp_active && ssp_mode == 1 && ssp_stage == 0 && a == 0 && b == 6);
    ssp_verify();
    if (ssp_mutate)
        op_save_path[2] = ssp_expected_path[2] = 'L';
    ++ssp_stage;
}
static int ssp_format(char *dest, char *format, char *prefix, int slot)
{
    SSP_CHECK(ssp_active && ssp_stage == (ssp_mode == 1 ? 1 : 0) && dest == op_save_path &&
              format == op_save_path_format && prefix == op_save_path_prefix && slot == ssp_slots[ssp_slot_case]);
    ssp_verify();
    memcpy(op_save_path, "authored-slot", 13);
    memcpy(ssp_expected_path, "authored-slot", 13);
    if (ssp_mutate)
    {
        op_save_path[1] = ssp_expected_path[1] = 'Q';
        op_save_path_format[0] = ssp_expected_format[0] = 'F';
        op_save_path_prefix[0] = ssp_expected_prefix[0] = 'P';
    }
    ++ssp_stage;
    return ssp_answers[ssp_format_answer];
}
int op_load_saved_game(char *path, int mode)
{
    SSP_CHECK(ssp_active && (ssp_mode == 1 || ssp_mode == 2) && ssp_stage == (ssp_mode == 1 ? 2 : 1) &&
              path == op_save_path && mode == (ssp_mode == 1 ? 0 : 1));
    ssp_verify();
    if (ssp_mutate)
        op_save_path[3] = ssp_expected_path[3] = 'R';
    ++ssp_stage;
    return ssp_answers[ssp_answer];
}
int op_delete_file(const char *path)
{
    SSP_CHECK(ssp_active && ssp_mode == 3 && ssp_stage == 1 && path == op_save_path);
    ssp_verify();
    if (ssp_mutate)
        op_save_path[3] = ssp_expected_path[3] = 'D';
    ++ssp_stage;
    return ssp_answers[ssp_answer];
}
static int op_test_save_slot_chains(void)
{
    ssp_active = 1;
    for (ssp_mode = 0; ssp_mode < 4; ++ssp_mode)
        for (ssp_slot_case = 0; ssp_slot_case < 7; ++ssp_slot_case)
            for (ssp_answer = 0; ssp_answer < 5; ++ssp_answer)
                for (ssp_format_answer = 0; ssp_format_answer < 5; ++ssp_format_answer)
                    for (ssp_mutate = 0; ssp_mutate < 2; ++ssp_mutate)
                    {
                        memset(op_save_path, 0x63, sizeof(op_save_path));
                        memcpy(ssp_expected_path, op_save_path, sizeof(ssp_expected_path));
                        memcpy(ssp_expected_format, op_save_path_format, sizeof(ssp_expected_format));
                        memcpy(ssp_expected_prefix, op_save_path_prefix, sizeof(ssp_expected_prefix));
                        ssp_stage = 0;
                        if (ssp_mode == 0)
                            SSP_CHECK(op_save_slot_path(ssp_slots[ssp_slot_case]) == op_save_path);
                        else if (ssp_mode == 1)
                            SSP_CHECK(op_save_load_slot_teardown(ssp_slots[ssp_slot_case]) == ssp_answers[ssp_answer]);
                        else if (ssp_mode == 2)
                            op_save_load_slot(ssp_slots[ssp_slot_case]);
                        else
                            SSP_CHECK(op_save_delete_slot(ssp_slots[ssp_slot_case]) == (ssp_answers[ssp_answer] != 0));
                        SSP_CHECK(ssp_stage == (ssp_mode == 0 ? 1 : ssp_mode == 1 ? 3 : 2));
                        ssp_verify();
                    }
    ssp_active = 0;
    printf("save slot connected: %d checks, %d failures\n", ssp_checks, ssp_failures);
    return ssp_failures != 0;
}
#undef SSP_CHECK
