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
typedef struct OWNED_DIRECTORY
{
    unsigned int before;
    OP_DIRECTORY value;
    unsigned int after;
} OWNED_DIRECTORY;
static OWNED_DIRECTORY records[3], expected_records[3];
static const int answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int row, answer, mutate, stage;
static OP_DIRECTORY *pick(void)
{
    return row == 3 ? 0 : &records[row].value;
}
static void seed(void)
{
    memset(records, 0x57, sizeof(records));
    memcpy(expected_records, records, sizeof(records));
    stage = 0;
}
static void verify(void)
{
    CHECK(memcmp(records, expected_records, sizeof(records)) == 0);
}

char op_directory_all_format[] = "authored-all", op_directory_extension_format[] = "authored-extension";
static char expected_all[sizeof(op_directory_all_format)], expected_extension[sizeof(op_directory_extension_format)];
typedef struct OWNED_TEXT
{
    unsigned int before;
    char text[24];
    unsigned int after;
} OWNED_TEXT;
static OWNED_TEXT path, extension, expected_path, expected_ext;
static const char *extensions[] = {".ext", "ext", "", ".", "..x", "\x80x"};
static const int modes[] = {INT_MIN, -1, 0, 1, 2, 3, 4, INT_MAX};
static int mode_case, extension_case;
static char *ext_argument;
static void verify_all(void)
{
    verify();
    CHECK(memcmp(&path, &expected_path, sizeof(path)) == 0 &&
          memcmp(&extension, &expected_ext, sizeof(extension)) == 0);
    CHECK(memcmp(op_directory_all_format, expected_all, sizeof(expected_all)) == 0 &&
          memcmp(op_directory_extension_format, expected_extension, sizeof(expected_extension)) == 0);
}
void *op_allocate(unsigned int size)
{
    CHECK(stage == 0 && size == 524);
    verify_all();
    if (mutate)
    {
        path.text[0] = expected_path.text[0] = 'P';
        if (ext_argument)
            extension.text[3] = expected_ext.text[3] = 'Q';
    }
    if (row < 3)
        memset(&expected_records[row].value, 0, sizeof(OP_DIRECTORY));
    ++stage;
    return pick();
}
int op_format_sprite_name(char *dest, char *format, ...)
{
    va_list args;
    char *p, *ext = 0;
    va_start(args, format);
    p = va_arg(args, char *);
    if (modes[mode_case] == 3)
        ext = va_arg(args, char *);
    va_end(args);
    CHECK(stage == 1 && row < 3 && dest == records[row].value.pattern && p == path.text);
    CHECK(format == (modes[mode_case] == 3 ? op_directory_extension_format : op_directory_all_format));
    if (modes[mode_case] == 3)
        CHECK(ext == extension.text + (extension.text[0] == '.'));
    verify_all();
    if (row >= 3 || dest != records[row].value.pattern)
        return 0;
    memcpy(dest, "authored/*", 10);
    memcpy(expected_records[row].value.pattern, "authored/*", 10);
    if (mutate)
    {
        records[row].value.position = expected_records[row].value.position = 7;
        op_directory_all_format[0] = expected_all[0] = 'A';
        op_directory_extension_format[0] = expected_extension[0] = 'E';
    }
    ++stage;
    return answers[answer];
}
int main(void)
{
    int valid;
    OP_DIRECTORY *result;
    for (row = 0; row < 4; ++row)
        for (answer = 0; answer < 5; ++answer)
            for (mutate = 0; mutate < 2; ++mutate)
                for (mode_case = 0; mode_case < 8; ++mode_case)
                    for (extension_case = 0; extension_case < 7; ++extension_case)
                    {
                        if (modes[mode_case] == 3 && extension_case == 6)
                            continue;
                        seed();
                        memset(&path, 0x63, sizeof(path));
                        memset(&extension, 0x72, sizeof(extension));
                        memcpy(path.text, "authored-path", sizeof("authored-path"));
                        if (extension_case < 6)
                            memcpy(extension.text, extensions[extension_case], strlen(extensions[extension_case]) + 1);
                        ext_argument = extension_case == 6 ? 0 : extension.text;
                        memcpy(&expected_path, &path, sizeof(path));
                        memcpy(&expected_ext, &extension, sizeof(extension));
                        memcpy(expected_all, op_directory_all_format, sizeof(expected_all));
                        memcpy(expected_extension, op_directory_extension_format, sizeof(expected_extension));
                        result = op_directory_open(path.text, modes[mode_case], ext_argument);
                        valid = modes[mode_case] >= 0 && modes[mode_case] <= 3;
                        CHECK(result == pick() && stage == (row < 3 && valid ? 2 : 1));
                        verify_all();
                    }
    printf("directory_open: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
