#include "../src/application_helpers.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int ah_diagnostics_close_checks, ah_diagnostics_close_failures;
static void ah_diagnostics_close_check(int ok, int line)
{
    ah_diagnostics_close_checks++;
    if (!ok)
    {
        ah_diagnostics_close_failures++;
        printf("line %d failed\n", line);
    }
}
#define ah_diagnostics_close_CHECK(x) ah_diagnostics_close_check(!!(x), __LINE__)
int op_diagnostics_enabled;
void *op_diagnostic_stream;
static unsigned char ah_diagnostics_close_streams[3][64], ah_diagnostics_close_expected[3][64];
static int ah_diagnostics_close_calls, ah_diagnostics_close_enabled, ah_diagnostics_close_result,
    ah_diagnostics_close_mutation;
static void *ah_diagnostics_close_selected;
int op_close_diagnostic_stream(void *stream)
{
    if(s73o_active)return s73o_close(stream);
    if(s73p_active)return s73p_close(stream);
    if(s73c_active)return s73c_close(stream);
    ah_diagnostics_close_CHECK(ah_diagnostics_close_calls++ == 0);
    ah_diagnostics_close_CHECK(stream == ah_diagnostics_close_selected);
    ah_diagnostics_close_CHECK(op_diagnostics_enabled == ah_diagnostics_close_enabled);
    ah_diagnostics_close_CHECK(op_diagnostic_stream == ah_diagnostics_close_selected);
    ah_diagnostics_close_CHECK(
        memcmp(ah_diagnostics_close_streams, ah_diagnostics_close_expected, sizeof(ah_diagnostics_close_streams)) == 0);
    if (ah_diagnostics_close_mutation)
    {
        op_diagnostics_enabled = 0;
        op_diagnostic_stream = ah_diagnostics_close_streams[2];
        ah_diagnostics_close_streams[1][63] = ah_diagnostics_close_expected[1][63] = 23;
    }
    return ah_diagnostics_close_result;
}
int ah_diagnostics_close_main(void)
{
    int e, r, p;
    int states[5] = {0, 1, -1, INT_MIN, INT_MAX};
    for (e = 0; e < 5; e++)
        for (r = 0; r < 5; r++)
            for (p = 0; p < 3; p++)
                for (ah_diagnostics_close_mutation = 0; ah_diagnostics_close_mutation < 2;
                     ah_diagnostics_close_mutation++)
                {
                    memset(ah_diagnostics_close_streams, 0x37, sizeof(ah_diagnostics_close_streams));
                    memcpy(ah_diagnostics_close_expected, ah_diagnostics_close_streams,
                           sizeof(ah_diagnostics_close_streams));
                    ah_diagnostics_close_enabled = states[e];
                    ah_diagnostics_close_result = states[r];
                    ah_diagnostics_close_selected = p == 0 ? 0 : (void *)ah_diagnostics_close_streams[p - 1];
                    op_diagnostic_stream = ah_diagnostics_close_selected;
                    op_diagnostics_enabled = ah_diagnostics_close_enabled;
                    ah_diagnostics_close_calls = 0;
                    op_diagnostics_close();
                    ah_diagnostics_close_CHECK(ah_diagnostics_close_calls == (ah_diagnostics_close_enabled != 0));
                    ah_diagnostics_close_CHECK(op_diagnostics_enabled ==
                                               ((ah_diagnostics_close_enabled && ah_diagnostics_close_mutation)
                                                    ? 0
                                                    : ah_diagnostics_close_enabled));
                    ah_diagnostics_close_CHECK(op_diagnostic_stream ==
                                               ((ah_diagnostics_close_enabled && ah_diagnostics_close_mutation)
                                                    ? (void *)ah_diagnostics_close_streams[2]
                                                    : ah_diagnostics_close_selected));
                    ah_diagnostics_close_CHECK(memcmp(ah_diagnostics_close_streams, ah_diagnostics_close_expected,
                                                      sizeof(ah_diagnostics_close_streams)) == 0);
                }
    printf("diagnostics close: %d checks, %d failures\n", ah_diagnostics_close_checks, ah_diagnostics_close_failures);
    return ah_diagnostics_close_failures != 0;
}
