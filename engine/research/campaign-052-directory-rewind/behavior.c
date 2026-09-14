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

static const unsigned int positions[] = {0, 1, 0xffffffffu, 0x80000000u, 0x7fffffffu};
static const int handles[] = {0, 1, -1, INT_MIN, INT_MAX};
static int position_case, handle_case;
int op_file_find_close(int handle)
{
    CHECK(stage == 0 && row < 3 && positions[position_case] != 0 && handle == handles[handle_case]);
    verify();
    if (row >= 3)
        return 0;
    if (mutate)
    {
        records[row].value.position = expected_records[row].value.position = 73;
        records[row].value.find_handle = expected_records[row].value.find_handle = 0x12345678;
        records[row].value.pattern[11] = expected_records[row].value.pattern[11] = 'Q';
    }
    verify();
    expected_records[row].value.position = 0;
    ++stage;
    return answers[answer];
}
int main(void)
{
    for (row = 0; row < 4; ++row)
        for (answer = 0; answer < 5; ++answer)
            for (mutate = 0; mutate < 2; ++mutate)
                for (position_case = 0; position_case < 5; ++position_case)
                    for (handle_case = 0; handle_case < 5; ++handle_case)
                    {
                        seed();
                        if (row < 3)
                        {
                            records[row].value.position = expected_records[row].value.position =
                                positions[position_case];
                            records[row].value.find_handle = expected_records[row].value.find_handle =
                                handles[handle_case];
                        }
                        op_directory_rewind(pick());
                        CHECK(stage == (row == 3 ? 0 : positions[position_case] ? 1 : 0));
                        verify();
                    }
    printf("directory_rewind: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
