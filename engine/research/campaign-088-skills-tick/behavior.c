#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
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
OP_SKILLS_STATE op_skills_state;
int op_skills_active_index, op_skills_previous_index;
float op_skills_frame_delta;
int op_skills_level_status;
#include "skills_easier_support.c"
static OP_SKILLS_STATE expected;
static int expected_active, expected_previous, expected_status, calls, mutate, present, token;
static float expected_delta;
static void verify(void)
{
    CHECK(!memcmp(&op_skills_state, &expected, sizeof(expected)));
    CHECK(op_skills_active_index == expected_active);
    CHECK(op_skills_previous_index == expected_previous);
    CHECK(op_skills_level_status == expected_status);
    CHECK(op_skills_frame_delta == expected_delta);
}
void *op_skills_get_player(void)
{
    CHECK(calls == 0);
    if (calls)
        exit(2);
    verify();
    ++calls;
    if (mutate)
    {
        op_skills_state.words[2] = expected.words[2] = -417;
        op_skills_state.current = expected.current = -741;
        op_skills_state.elapsed = expected.elapsed = 17.5f;
        op_skills_frame_delta = expected_delta = -31.0f;
        op_skills_level_status = expected_status = expected_status < 0 ? 5 : -1;
        op_skills_previous_index = expected_previous = expected_previous < 3 ? 8 : 0;
        op_skills_active_index = expected_active = expected_active < 1 ? 2 : 0;
    }
    return present ? &token : NULL;
}
int main(void)
{
    static const int previous[8] = {INT_MIN, -1, 0, 2, 3, 6, 7, INT_MAX};
    static const int active[6] = {INT_MIN, -1, 0, 1, 2, INT_MAX};
    static const int difficulty[6] = {INT_MIN, -1, 0, 1, 4, INT_MAX};
    static const int status[5] = {INT_MIN, -1, 0, 7, INT_MAX};
    static const float elapsed[3] = {1.25f, -4.0f, 1024.0f}, delta[3] = {0.5f, 1.5f, -1023.75f},
                       sum[3] = {1.75f, -2.5f, 0.25f};
    int p, a, d, s, f, pattern, adjust;
    unsigned int saved = _controlfp(0, 0);
    _controlfp(_PC_53 | _RC_NEAR, _MCW_PC | _MCW_RC);
    for (p = 0; p < 8; ++p)
        for (a = 0; a < 6; ++a)
            for (d = 0; d < 6; ++d)
                for (s = 0; s < 5; ++s)
                    for (f = 0; f < 3; ++f)
                        for (pattern = 0; pattern < 2; ++pattern)
                            for (mutate = 0; mutate < 2; ++mutate)
                                for (present = 0; present < 2; ++present)
                                {
                                    memset(&op_skills_state, 0x31 + pattern, sizeof(op_skills_state));
                                    op_skills_state.difficulty = difficulty[d];
                                    op_skills_state.elapsed = elapsed[f];
                                    op_skills_state.current = -93;
                                    expected = op_skills_state;
                                    expected.elapsed = sum[f];
                                    op_skills_active_index = expected_active = active[a];
                                    op_skills_previous_index = expected_previous = previous[p];
                                    op_skills_level_status = expected_status = status[s];
                                    op_skills_frame_delta = expected_delta = delta[f];
                                    calls = 0;
                                    op_skills_tick();
                                    if (present && expected_status >= 0)
                                    {
                                        expected.current = expected_status;
                                        adjust = (expected_active <= 0 && expected_previous >= 3) ||
                                                 (expected_active == 1 && expected_previous >= 7);
                                        if (adjust)
                                        {
                                            if (expected.difficulty > 0)
                                                --expected.difficulty;
                                            ++expected_active;
                                        }
                                    }
                                    CHECK(calls == 1);
                                    verify();
                                }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("skills_tick: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
