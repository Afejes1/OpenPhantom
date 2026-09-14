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
static char path[] = "authored-path", extension[] = "authored-extension";
static const int modes[] = {0, 1, 3, -1, INT_MIN, INT_MAX};
static int mode_case;
static void check_all(void)
{
    verify();
    CHECK(memcmp(&output, &expected_output, sizeof(output)) == 0);
}
OP_DIRECTORY *op_directory_open(char *p, int mode, char *ext)
{
    CHECK(stage == 0 && p == path && mode == modes[mode_case] && ext == extension);
    check_all();
    if (mutate && row < 3)
        records[row].value.position = expected_records[row].value.position = 7;
    ++stage;
    return pick();
}
int op_directory_read(OP_DIRECTORY *state, OP_DIRECTORY_ENTRY *out)
{
    CHECK(stage == 1 && row < 3 && state == pick() && out == &output.value);
    if (row < 0 || row >= 3 || state != pick() || out != &output.value)
        return 0;
    check_all();
    if (mutate)
    {
        output.value.name[0] = expected_output.value.name[0] = 'R';
        records[row].value.position = expected_records[row].value.position = 9;
    }
    ++stage;
    return answers[answer];
}
void op_directory_close(OP_DIRECTORY *state)
{
    CHECK(stage == 2 && row < 3 && state == pick());
    if (row < 0 || row >= 3 || state != pick())
        return;
    check_all();
    if (mutate)
        records[row].value.unused0 = expected_records[row].value.unused0 = 0x81234567u;
    ++stage;
}
int main(void)
{
    for (row = 0; row < 4; ++row)
        for (answer = 0; answer < 5; ++answer)
            for (mutate = 0; mutate < 2; ++mutate)
                for (mode_case = 0; mode_case < 6; ++mode_case)
                {
                    seed();
                    memset(&output, 0x75, sizeof(output));
                    memcpy(&expected_output, &output, sizeof(output));
                    CHECK(op_directory_find_one(path, modes[mode_case], extension, &output.value) ==
                          (row == 3 ? 0 : answers[answer]));
                    CHECK(stage == (row == 3 ? 1 : 3));
                    check_all();
                }
    printf("directory_find_one: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
