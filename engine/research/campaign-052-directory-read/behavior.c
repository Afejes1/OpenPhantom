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

typedef struct OWNED_ENTRY
{
    unsigned int before;
    OP_DIRECTORY_ENTRY value;
    unsigned int after;
} OWNED_ENTRY;
static OWNED_ENTRY output, expected_output;
static const unsigned int positions[] = {0, 1, 2, 0x7fffffffu, 0xffffffffu},
                          attributes[] = {0, 0x10, 0x20, 0xffffffffu, 0x80000010u},
                          times[] = {0, 1, 0xffffffffu, 0x80000000u, 0x12345678u};
static const int lengths[] = {0, 1, 3, 4, 31, 259};
static int position_case, attribute_case, length_case;
static void check_all(void)
{
    verify();
    CHECK(memcmp(&output, &expected_output, sizeof(output)) == 0);
}
static int fill(OP_FIND_DATA *data, int first)
{
    int i, n;
    CHECK(stage == 0 && row < 3 && data != 0);
    check_all();
    if (row >= 3 || !data)
        return -1;
    if (mutate)
    {
        records[row].value.position = expected_records[row].value.position = 91;
        records[row].value.find_handle = expected_records[row].value.find_handle = 0x13579bdf;
        records[row].value.pattern[11] = expected_records[row].value.pattern[11] = 'M';
    }
    if (answers[answer] != -1)
    {
        memset(data, 0x6a, sizeof(*data));
        data->attributes = attributes[attribute_case];
        data->time_written = times[attribute_case];
        data->size = times[(attribute_case + 1) % 5];
        n = lengths[length_case];
        for (i = 0; i < n; ++i)
            data->name[i] = (char)('a' + i % 23);
        data->name[n] = 0;
        memcpy(expected_output.value.name, data->name, (unsigned int)n + 1);
        expected_output.value.is_directory = attributes[attribute_case] & 0x10;
        expected_output.value.time_written = times[attribute_case];
    }
    verify();
    if (first)
        expected_records[row].value.find_handle = answers[answer];
    ++stage;
    return answers[answer];
}
int op_file_find_first(const char *pattern, OP_FIND_DATA *data)
{
    CHECK(position_case == 0 && row < 3 && pattern == records[row].value.pattern);
    return fill(data, 1);
}
int op_file_find_next(int handle, OP_FIND_DATA *data)
{
    CHECK(position_case != 0 && row < 3 && handle == 0x24681357);
    return fill(data, 0);
}
int main(void)
{
    for (row = 0; row < 4; ++row)
        for (answer = 0; answer < 5; ++answer)
            for (mutate = 0; mutate < 2; ++mutate)
                for (position_case = 0; position_case < 5; ++position_case)
                    for (attribute_case = 0; attribute_case < 5; ++attribute_case)
                        for (length_case = 0; length_case < 6; ++length_case)
                        {
                            seed();
                            memset(&output, 0x75, sizeof(output));
                            memcpy(&expected_output, &output, sizeof(output));
                            if (row < 3)
                            {
                                records[row].value.position = positions[position_case];
                                expected_records[row].value.position = positions[position_case] + 1u;
                                records[row].value.find_handle = expected_records[row].value.find_handle = 0x24681357;
                            }
                            CHECK(op_directory_read(pick(), &output.value) == (row < 3 && answers[answer] != -1));
                            CHECK(stage == (row == 3 ? 0 : 1));
                            check_all();
                        }
    printf("directory_read: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
