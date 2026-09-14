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

typedef struct DISK_VALUES
{
    unsigned int sectors, bytes, free_clusters, total, available;
} DISK_VALUES;
static const DISK_VALUES values[] = {{1, 1, 0, 17, 0},
                                     {1, 1, 1, 0, 1},
                                     {2, 512, 3, 99, 3072},
                                     {0xffffffffu, 1, 1, 2, 0xffffffffu},
                                     {0x80000000u, 2, 1, 0, 0},
                                     {0x80000001u, 2, 1, 0, 2},
                                     {0xffffu, 0x10001u, 2, 8, 0xfffffffeu},
                                     {0, 0xffffffffu, 0xffffffffu, 0, 0}};
static const unsigned int requests[] = {0, 1, 2, 3, 3071, 3072, 3073, 0xfffffffeu, 0xffffffffu};
static const int answers[] = {0, 1, -1, INT_MIN, INT_MAX};
static int row, request_case, answer, calls;
static int __stdcall disk_query(const char *root, unsigned int *sectors, unsigned int *bytes,
                                unsigned int *free_clusters, unsigned int *total)
{
    int distinct;
    CHECK(root == 0 && calls++ == 0);
    distinct = sectors && bytes && free_clusters && total && sectors != bytes && sectors != free_clusters &&
               sectors != total && bytes != free_clusters && bytes != total && free_clusters != total;
    CHECK(distinct);
    if (!distinct)
        return 0;
    if (answers[answer])
    {
        *sectors = values[row].sectors;
        *bytes = values[row].bytes;
        *free_clusters = values[row].free_clusters;
        *total = values[row].total;
    }
    return answers[answer];
}
OP_DISK_QUERY op_disk_free_space = disk_query;
int main(void)
{
    int expected;
    for (row = 0; row < 8; ++row)
        for (request_case = 0; request_case < 9; ++request_case)
            for (answer = 0; answer < 5; ++answer)
            {
                calls = 0;
                expected = answers[answer] == 0 || requests[request_case] < values[row].available;
                CHECK(op_save_disk_free_at_least(requests[request_case]) == expected);
                CHECK(calls == 1 && op_disk_free_space == disk_query);
            }
    printf("save_disk_free_at_least: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
