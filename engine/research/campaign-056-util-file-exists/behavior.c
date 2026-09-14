#include "api.h"
#include <stdio.h>
#include <limits.h>
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

typedef struct OWNED_TABLE
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} OWNED_TABLE;
typedef struct OWNED_NAME
{
    unsigned int before;
    char data[32];
    unsigned int after;
} OWNED_NAME;
static OWNED_TABLE tables[2], expected_tables[2];
static OWNED_NAME path_text, expected_path;
OP_CONF_SERVICES *op_conf_services;
char op_conf_read_mode[] = "authored-mode";
static char expected_mode[sizeof(op_conf_read_mode)];
static int start_table, retarget, handle, close_result, stage;
static const char *path;
static OP_CONF_SERVICES *expected_services;
static void verify(void)
{
    CHECK(memcmp(tables, expected_tables, sizeof(tables)) == 0);
    CHECK(memcmp(&path_text, &expected_path, sizeof(path_text)) == 0);
    CHECK(memcmp(op_conf_read_mode, expected_mode, sizeof(expected_mode)) == 0);
    CHECK(op_conf_services == expected_services);
}
static int opening(int which, const char *name, const char *mode)
{
    CHECK(stage == 0);
    CHECK(which == start_table);
    CHECK(name == path);
    CHECK(mode == op_conf_read_mode);
    verify();
    stage = 1;
    if (retarget)
    {
        op_conf_services = &tables[1 - start_table].value;
        expected_services = &tables[1 - start_table].value;
    }
    return handle;
}
static int closing(int which, int captured)
{
    CHECK(stage == 1);
    CHECK(which == (retarget ? 1 - start_table : start_table));
    CHECK(captured == handle);
    CHECK(handle != 0);
    verify();
    stage = 2;
    return close_result;
}
static int open0(const char *name, const char *mode)
{
    return opening(0, name, mode);
}
static int open1(const char *name, const char *mode)
{
    return opening(1, name, mode);
}
static int close0(int value)
{
    return closing(0, value);
}
static int close1(int value)
{
    return closing(1, value);
}
int main(void)
{
    static const int handles[] = {0, 1, -1, INT_MIN, INT_MAX};
    int hi, ci, path_case;
    for (start_table = 0; start_table < 2; ++start_table)
        for (retarget = 0; retarget < 2; ++retarget)
            for (hi = 0; hi < 5; ++hi)
                for (ci = 0; ci < 3; ++ci)
                    for (path_case = 0; path_case < 2; ++path_case)
                    {
                        handle = handles[hi];
                        close_result = ci == 0 ? 0 : ci == 1 ? -1 : INT_MAX;
                        memset(tables, 0x57, sizeof(tables));
                        memset(&path_text, 0x68, sizeof(path_text));
                        memcpy(path_text.data, "fixture-file", sizeof("fixture-file"));
                        tables[0].value.open = open0;
                        tables[1].value.open = open1;
                        tables[0].value.close = close0;
                        tables[1].value.close = close1;
                        op_conf_services = &tables[start_table].value;
                        expected_services = op_conf_services;
                        memcpy(expected_tables, tables, sizeof(tables));
                        memcpy(&expected_path, &path_text, sizeof(path_text));
                        memcpy(expected_mode, op_conf_read_mode, sizeof(expected_mode));
                        path = path_case ? path_text.data : 0;
                        stage = 0;
                        CHECK(op_util_file_exists(path) == (handle != 0));
                        CHECK(stage == (handle ? 2 : 1));
                        verify();
                    }
    printf("util_file_exists: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
