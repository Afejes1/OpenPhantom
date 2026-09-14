#include "../src/directory.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
static int dop_active;
static int dop_checks, dop_failures;
static void dop_check(int ok, int line)
{
    ++dop_checks;
    if (!ok)
    {
        ++dop_failures;
        printf("line%d failed\n", line);
    }
}
#define DOP_CHECK(x) dop_check(!!(x), __LINE__)
typedef struct DOP_OWNED_DIRECTORY
{
    unsigned int before;
    OP_DIRECTORY value;
    unsigned int after;
} DOP_OWNED_DIRECTORY;
static DOP_OWNED_DIRECTORY dop_records[3], dop_expected_records[3];
static const int dop_answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int dop_row, dop_answer, dop_mutate, dop_stage;
static OP_DIRECTORY *dop_pick(void)
{
    return dop_row == 3 ? 0 : &dop_records[dop_row].value;
}
static void dop_seed(void)
{
    memset(dop_records, 0x57, sizeof(dop_records));
    memcpy(dop_expected_records, dop_records, sizeof(dop_records));
    dop_stage = 0;
}
static void dop_verify(void)
{
    DOP_CHECK(memcmp(dop_records, dop_expected_records, sizeof(dop_records)) == 0);
}

char op_directory_all_format[] = "authored-all", op_directory_extension_format[] = "authored-dop_extension";
static char dop_expected_all[sizeof(op_directory_all_format)],
    dop_expected_extension[sizeof(op_directory_extension_format)];
typedef struct DOP_OWNED_TEXT
{
    unsigned int before;
    char text[24];
    unsigned int after;
} DOP_OWNED_TEXT;
static DOP_OWNED_TEXT dop_path, dop_extension, dop_expected_path, dop_expected_ext;
static const char *dop_extensions[] = {".ext", "ext", "", ".", "..x", "\x80x"};
static const int dop_modes[] = {INT_MIN, -1, 0, 1, 2, 3, 4, INT_MAX};
static int dop_mode_case, dop_extension_case;
static char *dop_ext_argument;
static void dop_verify_all(void)
{
    dop_verify();
    DOP_CHECK(memcmp(&dop_path, &dop_expected_path, sizeof(dop_path)) == 0 &&
              memcmp(&dop_extension, &dop_expected_ext, sizeof(dop_extension)) == 0);
    DOP_CHECK(memcmp(op_directory_all_format, dop_expected_all, sizeof(dop_expected_all)) == 0 &&
              memcmp(op_directory_extension_format, dop_expected_extension, sizeof(dop_expected_extension)) == 0);
}
static void *dop_allocate(unsigned int size)
{
    DOP_CHECK(dop_stage == 0 && size == 524);
    dop_verify_all();
    if (dop_mutate)
    {
        dop_path.text[0] = dop_expected_path.text[0] = 'P';
        if (dop_ext_argument)
            dop_extension.text[3] = dop_expected_ext.text[3] = 'Q';
    }
    if (dop_row < 3)
        memset(&dop_expected_records[dop_row].value, 0, sizeof(OP_DIRECTORY));
    ++dop_stage;
    return dop_pick();
}
static int dop_format(char *dest, char *format, ...)
{
    va_list args;
    char *p, *ext = 0;
    va_start(args, format);
    p = va_arg(args, char *);
    if (dop_modes[dop_mode_case] == 3)
        ext = va_arg(args, char *);
    va_end(args);
    DOP_CHECK(dop_stage == 1 && dop_row < 3 && dest == dop_records[dop_row].value.pattern && p == dop_path.text);
    DOP_CHECK(format == (dop_modes[dop_mode_case] == 3 ? op_directory_extension_format : op_directory_all_format));
    if (dop_modes[dop_mode_case] == 3)
        DOP_CHECK(ext == dop_extension.text + (dop_extension.text[0] == '.'));
    dop_verify_all();
    if (dop_row >= 3 || dest != dop_records[dop_row].value.pattern)
        return 0;
    memcpy(dest, "authored/*", 10);
    memcpy(dop_expected_records[dop_row].value.pattern, "authored/*", 10);
    if (dop_mutate)
    {
        dop_records[dop_row].value.position = dop_expected_records[dop_row].value.position = 7;
        op_directory_all_format[0] = dop_expected_all[0] = 'A';
        op_directory_extension_format[0] = dop_expected_extension[0] = 'E';
    }
    ++dop_stage;
    return dop_answers[dop_answer];
}
static int op_test_directory_open(void)
{
    int valid;
    OP_DIRECTORY *result;
    dop_active = 1;
    for (dop_row = 0; dop_row < 4; ++dop_row)
        for (dop_answer = 0; dop_answer < 5; ++dop_answer)
            for (dop_mutate = 0; dop_mutate < 2; ++dop_mutate)
                for (dop_mode_case = 0; dop_mode_case < 8; ++dop_mode_case)
                    for (dop_extension_case = 0; dop_extension_case < 7; ++dop_extension_case)
                    {
                        if (dop_modes[dop_mode_case] == 3 && dop_extension_case == 6)
                            continue;
                        dop_seed();
                        memset(&dop_path, 0x63, sizeof(dop_path));
                        memset(&dop_extension, 0x72, sizeof(dop_extension));
                        memcpy(dop_path.text, "authored-dop_path", sizeof("authored-dop_path"));
                        if (dop_extension_case < 6)
                            memcpy(dop_extension.text, dop_extensions[dop_extension_case],
                                   strlen(dop_extensions[dop_extension_case]) + 1);
                        dop_ext_argument = dop_extension_case == 6 ? 0 : dop_extension.text;
                        memcpy(&dop_expected_path, &dop_path, sizeof(dop_path));
                        memcpy(&dop_expected_ext, &dop_extension, sizeof(dop_extension));
                        memcpy(dop_expected_all, op_directory_all_format, sizeof(dop_expected_all));
                        memcpy(dop_expected_extension, op_directory_extension_format, sizeof(dop_expected_extension));
                        result = op_directory_open(dop_path.text, dop_modes[dop_mode_case], dop_ext_argument);
                        valid = dop_modes[dop_mode_case] >= 0 && dop_modes[dop_mode_case] <= 3;
                        DOP_CHECK(result == dop_pick() && dop_stage == (dop_row < 3 && valid ? 2 : 1));
                        dop_verify_all();
                    }
    dop_active = 0;
    printf("directory_open: %d checks, %d failures\n", dop_checks, dop_failures);
    return dop_failures != 0;
}

#undef DOP_CHECK

#include "../src/directory.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
static int dcl_active;
static int dcl_checks, dcl_failures;
static void dcl_check(int ok, int line)
{
    ++dcl_checks;
    if (!ok)
    {
        ++dcl_failures;
        printf("line%d failed\n", line);
    }
}
#define DCL_CHECK(x) dcl_check(!!(x), __LINE__)
typedef struct DCL_OWNED_DIRECTORY
{
    unsigned int before;
    OP_DIRECTORY value;
    unsigned int after;
} DCL_OWNED_DIRECTORY;
static DCL_OWNED_DIRECTORY dcl_records[3], dcl_expected_records[3];
static const int dcl_answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int dcl_row, dcl_answer, dcl_mutate, dcl_stage;
static OP_DIRECTORY *dcl_pick(void)
{
    return dcl_row == 3 ? 0 : &dcl_records[dcl_row].value;
}
static void dcl_seed(void)
{
    memset(dcl_records, 0x57, sizeof(dcl_records));
    memcpy(dcl_expected_records, dcl_records, sizeof(dcl_records));
    dcl_stage = 0;
}
static void dcl_verify(void)
{
    DCL_CHECK(memcmp(dcl_records, dcl_expected_records, sizeof(dcl_records)) == 0);
}

static const unsigned int dcl_positions[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fffffffu};
static const int dcl_handles[] = {0, 1, -1, INT_MIN, INT_MAX};
static int dcl_position_case, dcl_handle_case;
static int dcl_close(int handle)
{
    DCL_CHECK(dcl_stage == 0 && dcl_row < 3 && dcl_positions[dcl_position_case] != 0 &&
              handle == dcl_handles[dcl_handle_case]);
    dcl_verify();
    if (dcl_row >= 3)
        return 0;
    if (dcl_mutate)
    {
        dcl_records[dcl_row].value.position = dcl_expected_records[dcl_row].value.position = 73;
        dcl_records[dcl_row].value.find_handle = dcl_expected_records[dcl_row].value.find_handle = 0x12345678;
        dcl_records[dcl_row].value.pattern[11] = dcl_expected_records[dcl_row].value.pattern[11] = 'Q';
    }
    dcl_verify();
    ++dcl_stage;
    return dcl_answers[dcl_answer];
}
static void dcl_release(void *pointer)
{
    DCL_CHECK(dcl_row < 3 && pointer == dcl_pick() && dcl_stage == (dcl_positions[dcl_position_case] ? 1 : 0));
    dcl_verify();
    if (dcl_row < 3 && pointer == dcl_pick())
    {
        dcl_records[dcl_row].value.unused0 = dcl_expected_records[dcl_row].value.unused0 = 0x81234567u;
    }
    ++dcl_stage;
}
static int op_test_directory_close(void)
{
    dcl_active = 1;
    for (dcl_row = 0; dcl_row < 4; ++dcl_row)
        for (dcl_answer = 0; dcl_answer < 5; ++dcl_answer)
            for (dcl_mutate = 0; dcl_mutate < 2; ++dcl_mutate)
                for (dcl_position_case = 0; dcl_position_case < 5; ++dcl_position_case)
                    for (dcl_handle_case = 0; dcl_handle_case < 5; ++dcl_handle_case)
                    {
                        dcl_seed();
                        if (dcl_row < 3)
                        {
                            dcl_records[dcl_row].value.position = dcl_expected_records[dcl_row].value.position =
                                dcl_positions[dcl_position_case];
                            dcl_records[dcl_row].value.find_handle = dcl_expected_records[dcl_row].value.find_handle =
                                dcl_handles[dcl_handle_case];
                        }
                        op_directory_close(dcl_pick());
                        DCL_CHECK(dcl_stage == (dcl_row == 3 ? 0 : dcl_positions[dcl_position_case] ? 2 : 1));
                        dcl_verify();
                    }
    dcl_active = 0;
    printf("directory_close: %d checks, %d failures\n", dcl_checks, dcl_failures);
    return dcl_failures != 0;
}

#undef DCL_CHECK

#include "../src/directory.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
static int drw_active;
static int drw_checks, drw_failures;
static void drw_check(int ok, int line)
{
    ++drw_checks;
    if (!ok)
    {
        ++drw_failures;
        printf("line%d failed\n", line);
    }
}
#define DRW_CHECK(x) drw_check(!!(x), __LINE__)
typedef struct DRW_OWNED_DIRECTORY
{
    unsigned int before;
    OP_DIRECTORY value;
    unsigned int after;
} DRW_OWNED_DIRECTORY;
static DRW_OWNED_DIRECTORY drw_records[3], drw_expected_records[3];
static const int drw_answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int drw_row, drw_answer, drw_mutate, drw_stage;
static OP_DIRECTORY *drw_pick(void)
{
    return drw_row == 3 ? 0 : &drw_records[drw_row].value;
}
static void drw_seed(void)
{
    memset(drw_records, 0x57, sizeof(drw_records));
    memcpy(drw_expected_records, drw_records, sizeof(drw_records));
    drw_stage = 0;
}
static void drw_verify(void)
{
    DRW_CHECK(memcmp(drw_records, drw_expected_records, sizeof(drw_records)) == 0);
}

static const unsigned int drw_positions[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fffffffu};
static const int drw_handles[] = {0, 1, -1, INT_MIN, INT_MAX};
static int drw_position_case, drw_handle_case;
static int drw_close(int handle)
{
    DRW_CHECK(drw_stage == 0 && drw_row < 3 && drw_positions[drw_position_case] != 0 &&
              handle == drw_handles[drw_handle_case]);
    drw_verify();
    if (drw_row >= 3)
        return 0;
    if (drw_mutate)
    {
        drw_records[drw_row].value.position = drw_expected_records[drw_row].value.position = 73;
        drw_records[drw_row].value.find_handle = drw_expected_records[drw_row].value.find_handle = 0x12345678;
        drw_records[drw_row].value.pattern[11] = drw_expected_records[drw_row].value.pattern[11] = 'Q';
    }
    drw_verify();
    drw_expected_records[drw_row].value.position = 0;
    ++drw_stage;
    return drw_answers[drw_answer];
}
static int op_test_directory_rewind(void)
{
    drw_active = 1;
    for (drw_row = 0; drw_row < 4; ++drw_row)
        for (drw_answer = 0; drw_answer < 5; ++drw_answer)
            for (drw_mutate = 0; drw_mutate < 2; ++drw_mutate)
                for (drw_position_case = 0; drw_position_case < 5; ++drw_position_case)
                    for (drw_handle_case = 0; drw_handle_case < 5; ++drw_handle_case)
                    {
                        drw_seed();
                        if (drw_row < 3)
                        {
                            drw_records[drw_row].value.position = drw_expected_records[drw_row].value.position =
                                drw_positions[drw_position_case];
                            drw_records[drw_row].value.find_handle = drw_expected_records[drw_row].value.find_handle =
                                drw_handles[drw_handle_case];
                        }
                        op_directory_rewind(drw_pick());
                        DRW_CHECK(drw_stage == (drw_row == 3 ? 0 : drw_positions[drw_position_case] ? 1 : 0));
                        drw_verify();
                    }
    drw_active = 0;
    printf("directory_rewind: %d checks, %d failures\n", drw_checks, drw_failures);
    return drw_failures != 0;
}

#undef DRW_CHECK

#include "../src/directory.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdarg.h>
static int drd_active;
static int drd_checks, drd_failures;
static void drd_check(int ok, int line)
{
    ++drd_checks;
    if (!ok)
    {
        ++drd_failures;
        printf("line%d failed\n", line);
    }
}
#define DRD_CHECK(x) drd_check(!!(x), __LINE__)
typedef struct DRD_OWNED_DIRECTORY
{
    unsigned int before;
    OP_DIRECTORY value;
    unsigned int after;
} DRD_OWNED_DIRECTORY;
static DRD_OWNED_DIRECTORY drd_records[3], drd_expected_records[3];
static const int drd_answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int drd_row, drd_answer, drd_mutate, drd_stage;
static OP_DIRECTORY *drd_pick(void)
{
    return drd_row == 3 ? 0 : &drd_records[drd_row].value;
}
static void drd_seed(void)
{
    memset(drd_records, 0x57, sizeof(drd_records));
    memcpy(drd_expected_records, drd_records, sizeof(drd_records));
    drd_stage = 0;
}
static void drd_verify(void)
{
    DRD_CHECK(memcmp(drd_records, drd_expected_records, sizeof(drd_records)) == 0);
}

typedef struct DRD_OWNED_ENTRY
{
    unsigned int before;
    OP_DIRECTORY_ENTRY value;
    unsigned int after;
} DRD_OWNED_ENTRY;
static DRD_OWNED_ENTRY drd_output, drd_expected_output;
static const unsigned int drd_positions[] = {0, 1, 2, 0x7fffffffu, 0xffffffffu},
                          drd_attributes[] = {0, 0x10, 0x20, 0xffffffffu, 0x80000010u},
                          drd_times[] = {0, 1, 0xffffffffu, 0x80000000u, 0x12345678u};
static const int drd_lengths[] = {0, 1, 3, 4, 31, 259};
static int drd_position_case, drd_attribute_case, drd_length_case;
static void drd_check_all(void)
{
    drd_verify();
    DRD_CHECK(memcmp(&drd_output, &drd_expected_output, sizeof(drd_output)) == 0);
}
static int drd_fill(OP_FIND_DATA *data, int first)
{
    int i, n;
    DRD_CHECK(drd_stage == 0 && drd_row < 3 && data != 0);
    drd_check_all();
    if (drd_row >= 3 || !data)
        return -1;
    if (drd_mutate)
    {
        drd_records[drd_row].value.position = drd_expected_records[drd_row].value.position = 91;
        drd_records[drd_row].value.find_handle = drd_expected_records[drd_row].value.find_handle = 0x13579bdf;
        drd_records[drd_row].value.pattern[11] = drd_expected_records[drd_row].value.pattern[11] = 'M';
    }
    if (drd_answers[drd_answer] != -1)
    {
        memset(data, 0x6a, sizeof(*data));
        data->attributes = drd_attributes[drd_attribute_case];
        data->time_written = drd_times[drd_attribute_case];
        data->size = drd_times[(drd_attribute_case + 1) % 5];
        n = drd_lengths[drd_length_case];
        for (i = 0; i < n; ++i)
            data->name[i] = (char)('a' + i % 23);
        data->name[n] = 0;
        memcpy(drd_expected_output.value.name, data->name, (unsigned int)n + 1);
        drd_expected_output.value.is_directory = drd_attributes[drd_attribute_case] & 0x10;
        drd_expected_output.value.time_written = drd_times[drd_attribute_case];
    }
    drd_verify();
    if (first)
        drd_expected_records[drd_row].value.find_handle = drd_answers[drd_answer];
    ++drd_stage;
    return drd_answers[drd_answer];
}
static int drd_first(const char *pattern, OP_FIND_DATA *data)
{
    DRD_CHECK(drd_position_case == 0 && drd_row < 3 && pattern == drd_records[drd_row].value.pattern);
    return drd_fill(data, 1);
}
static int drd_next(int handle, OP_FIND_DATA *data)
{
    DRD_CHECK(drd_position_case != 0 && drd_row < 3 && handle == 0x24681357);
    return drd_fill(data, 0);
}
static int op_test_directory_read(void)
{
    drd_active = 1;
    for (drd_row = 0; drd_row < 4; ++drd_row)
        for (drd_answer = 0; drd_answer < 5; ++drd_answer)
            for (drd_mutate = 0; drd_mutate < 2; ++drd_mutate)
                for (drd_position_case = 0; drd_position_case < 5; ++drd_position_case)
                    for (drd_attribute_case = 0; drd_attribute_case < 5; ++drd_attribute_case)
                        for (drd_length_case = 0; drd_length_case < 6; ++drd_length_case)
                        {
                            drd_seed();
                            memset(&drd_output, 0x75, sizeof(drd_output));
                            memcpy(&drd_expected_output, &drd_output, sizeof(drd_output));
                            if (drd_row < 3)
                            {
                                drd_records[drd_row].value.position = drd_positions[drd_position_case];
                                drd_expected_records[drd_row].value.position = drd_positions[drd_position_case] + 1u;
                                drd_records[drd_row].value.find_handle =
                                    drd_expected_records[drd_row].value.find_handle = 0x24681357;
                            }
                            DRD_CHECK(op_directory_read(drd_pick(), &drd_output.value) ==
                                      (drd_row < 3 && drd_answers[drd_answer] != -1));
                            DRD_CHECK(drd_stage == (drd_row == 3 ? 0 : 1));
                            drd_check_all();
                        }
    drd_active = 0;
    printf("directory_read: %d checks, %d failures\n", drd_checks, drd_failures);
    return drd_failures != 0;
}

#undef DRD_CHECK
#include "../src/directory.h"
static int dcy_active, dcy_checks, dcy_failures, dcy_phase, dcy_stage, dcy_profile, dcy_mutate;
static void dcy_check(int ok, int line)
{
    ++dcy_checks;
    if (!ok)
    {
        ++dcy_failures;
        printf("directory lifecycle line%d failed\n", line);
    }
}
#define DCY_CHECK(x) dcy_check(!!(x), __LINE__)
typedef struct DCY_RECORD
{
    unsigned int before;
    OP_DIRECTORY value;
    unsigned int after;
} DCY_RECORD;
typedef struct DCY_ENTRY
{
    unsigned int before;
    OP_DIRECTORY_ENTRY value;
    unsigned int after;
} DCY_ENTRY;
static DCY_RECORD dcy_record, dcy_expected;
static DCY_ENTRY dcy_output, dcy_expected_output;
static char dcy_path[] = "authored-directory", dcy_extension[] = ".unit";
static char dcy_expected_path[sizeof(dcy_path)], dcy_expected_extension[sizeof(dcy_extension)];
static char dcy_expected_all[sizeof(op_directory_all_format)],
    dcy_expected_format[sizeof(op_directory_extension_format)];
static void dcy_verify(void)
{
    DCY_CHECK(memcmp(&dcy_record, &dcy_expected, sizeof(dcy_record)) == 0);
    DCY_CHECK(memcmp(&dcy_output, &dcy_expected_output, sizeof(dcy_output)) == 0);
    DCY_CHECK(memcmp(dcy_path, dcy_expected_path, sizeof(dcy_path)) == 0 &&
              memcmp(dcy_extension, dcy_expected_extension, sizeof(dcy_extension)) == 0);
    DCY_CHECK(memcmp(op_directory_all_format, dcy_expected_all, sizeof(dcy_expected_all)) == 0 &&
              memcmp(op_directory_extension_format, dcy_expected_format, sizeof(dcy_expected_format)) == 0);
}
static void *dcy_allocate(unsigned int size)
{
    DCY_CHECK(dcy_active && dcy_phase == 0 && dcy_stage == 0 && size == 524);
    dcy_verify();
    memset(&dcy_expected.value, 0, 524);
    ++dcy_stage;
    return &dcy_record.value;
}
static int dcy_format(char *dest, char *format, char *path, char *ext)
{
    DCY_CHECK(dcy_phase == 0 && dcy_stage == 1 && dest == dcy_record.value.pattern &&
              format == op_directory_extension_format && path == dcy_path && ext == dcy_extension + 1);
    dcy_verify();
    memcpy(dcy_record.value.pattern, "authored/*.unit", sizeof("authored/*.unit"));
    memcpy(dcy_expected.value.pattern, "authored/*.unit", sizeof("authored/*.unit"));
    ++dcy_stage;
    return -1;
}
static int dcy_fill(OP_FIND_DATA *data, int first)
{
    int result;
    const char *name;
    DCY_CHECK(dcy_active && dcy_stage == 0 && data != 0 &&
              ((first && (dcy_phase == 1 || dcy_phase == 4)) || (!first && dcy_phase == 2)));
    dcy_verify();
    if (!data)
        return -1;
    result = dcy_phase == 1   ? (dcy_profile == 1 ? -1 : 0x12345678)
             : dcy_phase == 2 ? (dcy_profile == 2 ? -1 : 0)
                              : 0x34567812;
    if (dcy_mutate)
    {
        dcy_record.value.position = dcy_expected.value.position = 77;
        dcy_record.value.find_handle = dcy_expected.value.find_handle = 0x65432178;
        dcy_record.value.pattern[9] = dcy_expected.value.pattern[9] = 'M';
    }
    dcy_verify();
    if (result != -1)
    {
        memset(data, 0x6a, sizeof(*data));
        data->attributes = dcy_phase == 2 ? 0x20 : 0x80000010u;
        data->time_written = 0x81234567u + (unsigned int)dcy_phase;
        data->size = 11;
        name = dcy_phase == 2 ? "next.unit" : "first.unit";
        memcpy(data->name, name, strlen(name) + 1);
        memcpy(dcy_expected_output.value.name, name, strlen(name) + 1);
        dcy_expected_output.value.is_directory = data->attributes & 0x10;
        dcy_expected_output.value.time_written = 0x81234567u + (unsigned int)dcy_phase;
    }
    if (first)
        dcy_expected.value.find_handle = result;
    ++dcy_stage;
    return result;
}
static int dcy_first(const char *pattern, OP_FIND_DATA *data)
{
    DCY_CHECK(pattern == dcy_record.value.pattern);
    return dcy_fill(data, 1);
}
static int dcy_next(int handle, OP_FIND_DATA *data)
{
    DCY_CHECK(handle == dcy_expected.value.find_handle);
    return dcy_fill(data, 0);
}
static int dcy_close(int handle)
{
    DCY_CHECK(dcy_active && (dcy_phase == 3 || dcy_phase == 5) && dcy_stage == 0 &&
              handle == dcy_expected.value.find_handle);
    dcy_verify();
    if (dcy_mutate)
    {
        dcy_record.value.position = dcy_expected.value.position = 55;
        dcy_record.value.find_handle = dcy_expected.value.find_handle = 0x76543218;
        dcy_record.value.pattern[7] = dcy_expected.value.pattern[7] = 'C';
    }
    dcy_verify();
    if (dcy_phase == 3)
        dcy_expected.value.position = 0;
    ++dcy_stage;
    return -1;
}
static void dcy_release(void *pointer)
{
    DCY_CHECK(dcy_active && dcy_phase == 5 && dcy_stage == 1 && pointer == &dcy_record.value);
    dcy_verify();
    ++dcy_stage;
}
int op_file_find_close(int handle)
{
    if (dcy_active)
        return dcy_close(handle);
    if (dcl_active)
        return dcl_close(handle);
    if (drw_active)
        return drw_close(handle);
    dcy_check(0, __LINE__);
    return -1;
}
int op_file_find_first(const char *pattern, OP_FIND_DATA *data)
{
    if (dcy_active)
        return dcy_first(pattern, data);
    if (drd_active)
        return drd_first(pattern, data);
    dcy_check(0, __LINE__);
    return -1;
}
int op_file_find_next(int handle, OP_FIND_DATA *data)
{
    if (dcy_active)
        return dcy_next(handle, data);
    if (drd_active)
        return drd_next(handle, data);
    dcy_check(0, __LINE__);
    return -1;
}
static int op_test_directory_lifecycle(void)
{
    OP_DIRECTORY *state;
    int result;
    dcy_active = 1;
    for (dcy_profile = 0; dcy_profile < 3; ++dcy_profile)
        for (dcy_mutate = 0; dcy_mutate < 2; ++dcy_mutate)
        {
            memset(&dcy_record, 0x53, sizeof(dcy_record));
            memcpy(&dcy_expected, &dcy_record, sizeof(dcy_record));
            memset(&dcy_output, 0x63, sizeof(dcy_output));
            memcpy(&dcy_expected_output, &dcy_output, sizeof(dcy_output));
            memcpy(dcy_expected_path, dcy_path, sizeof(dcy_path));
            memcpy(dcy_expected_extension, dcy_extension, sizeof(dcy_extension));
            memcpy(dcy_expected_all, op_directory_all_format, sizeof(dcy_expected_all));
            memcpy(dcy_expected_format, op_directory_extension_format, sizeof(dcy_expected_format));
            dcy_phase = dcy_stage = 0;
            state = op_directory_open(dcy_path, 3, dcy_extension);
            DCY_CHECK(state == &dcy_record.value && dcy_stage == 2);
            dcy_verify();
            if (state != &dcy_record.value)
                continue;
            dcy_phase = 1;
            dcy_stage = 0;
            ++dcy_expected.value.position;
            result = op_directory_read(state, &dcy_output.value);
            DCY_CHECK(result == (dcy_profile != 1) && dcy_stage == 1);
            dcy_verify();
            dcy_phase = 2;
            dcy_stage = 0;
            ++dcy_expected.value.position;
            result = op_directory_read(state, &dcy_output.value);
            DCY_CHECK(result == (dcy_profile != 2) && dcy_stage == 1);
            dcy_verify();
            dcy_phase = 3;
            dcy_stage = 0;
            op_directory_rewind(state);
            DCY_CHECK(dcy_stage == 1);
            dcy_verify();
            dcy_phase = 4;
            dcy_stage = 0;
            ++dcy_expected.value.position;
            result = op_directory_read(state, &dcy_output.value);
            DCY_CHECK(result == 1 && dcy_stage == 1);
            dcy_verify();
            dcy_phase = 5;
            dcy_stage = 0;
            op_directory_close(state);
            DCY_CHECK(dcy_stage == 2);
            dcy_verify();
        }
    dcy_active = 0;
    printf("directory lifecycle connected: %d checks, %d failures\n", dcy_checks, dcy_failures);
    return dcy_failures != 0;
}
#undef DCY_CHECK
