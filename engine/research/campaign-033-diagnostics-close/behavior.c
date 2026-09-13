#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    checks++;
    if (!ok)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
int op_diagnostics_enabled;
void *op_diagnostic_stream;
static unsigned char streams[3][64], expected[3][64];
static int calls, enabled, result, mutation;
static void *selected;
int op_close_diagnostic_stream(void *stream)
{
    CHECK(calls++ == 0);
    CHECK(stream == selected);
    CHECK(op_diagnostics_enabled == enabled);
    CHECK(op_diagnostic_stream == selected);
    CHECK(memcmp(streams, expected, sizeof(streams)) == 0);
    if (mutation)
    {
        op_diagnostics_enabled = 0;
        op_diagnostic_stream = streams[2];
        streams[1][63] = expected[1][63] = 23;
    }
    return result;
}
int main(void)
{
    int e, r, p;
    int states[5] = {0, 1, -1, INT_MIN, INT_MAX};
    for (e = 0; e < 5; e++)
        for (r = 0; r < 5; r++)
            for (p = 0; p < 3; p++)
                for (mutation = 0; mutation < 2; mutation++)
                {
                    memset(streams, 0x37, sizeof(streams));
                    memcpy(expected, streams, sizeof(streams));
                    enabled = states[e];
                    result = states[r];
                    selected = p == 0 ? 0 : (void *)streams[p - 1];
                    op_diagnostic_stream = selected;
                    op_diagnostics_enabled = enabled;
                    calls = 0;
                    op_diagnostics_close();
                    CHECK(calls == (enabled != 0));
                    CHECK(op_diagnostics_enabled == ((enabled && mutation) ? 0 : enabled));
                    CHECK(op_diagnostic_stream == ((enabled && mutation) ? (void *)streams[2] : selected));
                    CHECK(memcmp(streams, expected, sizeof(streams)) == 0);
                }
    printf("diagnostics close: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
