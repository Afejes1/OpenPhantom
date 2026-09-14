#include "../src/skills_state.h"
float op_skills_frame_delta;
int op_skills_level_status;
#include "../src/skills_state.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
static int s880_checks, s880_failures;
static void s880_check(int ok, int line)
{
    ++s880_checks;
    if (!ok)
    {
        ++s880_failures;
        printf("line%d failed\n", line);
    }
}
#define S880_CHECK(x) s880_check(!!(x), __LINE__)
static OP_SKILLS_STATE s880_expected;
static int s880_expected_active, s880_expected_previous, s880_expected_status, s880_calls, s880_mutate, s880_present,
    s880_token;
static float s880_expected_delta;
static void s880_verify(void)
{
    S880_CHECK(!memcmp(&op_skills_state, &s880_expected, sizeof(s880_expected)));
    S880_CHECK(op_skills_active_index == s880_expected_active);
    S880_CHECK(op_skills_previous_index == s880_expected_previous);
    S880_CHECK(op_skills_level_status == s880_expected_status);
    S880_CHECK(op_skills_frame_delta == s880_expected_delta);
}
void *op_skills_get_player(void)
{
    S880_CHECK(s880_calls == 0);
    if (s880_calls)
        exit(2);
    s880_verify();
    ++s880_calls;
    if (s880_mutate)
    {
        op_skills_state.words[2] = s880_expected.words[2] = -417;
        op_skills_state.current = s880_expected.current = -741;
        op_skills_state.elapsed = s880_expected.elapsed = 17.5f;
        op_skills_frame_delta = s880_expected_delta = -31.0f;
        op_skills_level_status = s880_expected_status = s880_expected_status < 0 ? 5 : -1;
        op_skills_previous_index = s880_expected_previous = s880_expected_previous < 3 ? 8 : 0;
        op_skills_active_index = s880_expected_active = s880_expected_active < 1 ? 2 : 0;
    }
    return s880_present ? &s880_token : NULL;
}
static int op_test_skills_tick(void)
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
                            for (s880_mutate = 0; s880_mutate < 2; ++s880_mutate)
                                for (s880_present = 0; s880_present < 2; ++s880_present)
                                {
                                    memset(&op_skills_state, 0x31 + pattern, sizeof(op_skills_state));
                                    op_skills_state.difficulty = difficulty[d];
                                    op_skills_state.elapsed = elapsed[f];
                                    op_skills_state.current = -93;
                                    s880_expected = op_skills_state;
                                    s880_expected.elapsed = sum[f];
                                    op_skills_active_index = s880_expected_active = active[a];
                                    op_skills_previous_index = s880_expected_previous = previous[p];
                                    op_skills_level_status = s880_expected_status = status[s];
                                    op_skills_frame_delta = s880_expected_delta = delta[f];
                                    s880_calls = 0;
                                    op_skills_tick();
                                    if (s880_present && s880_expected_status >= 0)
                                    {
                                        s880_expected.current = s880_expected_status;
                                        adjust = (s880_expected_active <= 0 && s880_expected_previous >= 3) ||
                                                 (s880_expected_active == 1 && s880_expected_previous >= 7);
                                        if (adjust)
                                        {
                                            if (s880_expected.difficulty > 0)
                                                --s880_expected.difficulty;
                                            ++s880_expected_active;
                                        }
                                    }
                                    S880_CHECK(s880_calls == 1);
                                    s880_verify();
                                }
    _controlfp(saved, _MCW_PC | _MCW_RC);
    printf("skills_tick: %d checks, %d failures\n", s880_checks, s880_failures);
    return s880_failures != 0;
}

#undef S880_CHECK

#include "../src/skills_state.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
static int s881_checks, s881_failures;
static void s881_check(int ok, int line)
{
    ++s881_checks;
    if (!ok)
    {
        ++s881_failures;
        printf("line%d failed\n", line);
    }
}
#define S881_CHECK(x) s881_check(!!(x), __LINE__)
static int op_test_skills_show_level(void)
{
    OP_SKILLS_STATE before;
    int p, a, b;
    static const int values[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    for (p = 0; p < 8; ++p)
        for (a = 0; a < 5; ++a)
            for (b = 0; b < 5; ++b)
            {
                memset(&op_skills_state, 0x41 + p, sizeof(op_skills_state));
                before = op_skills_state;
                op_skills_active_index = values[a];
                op_skills_previous_index = values[b];
                op_skills_frame_delta = 1.25f;
                op_skills_level_status = -17;
                op_skills_show_level();
                S881_CHECK(!memcmp(&op_skills_state, &before, sizeof(before)));
                S881_CHECK(op_skills_active_index == values[a]);
                S881_CHECK(op_skills_previous_index == values[b]);
                S881_CHECK(op_skills_frame_delta == 1.25f);
                S881_CHECK(op_skills_level_status == -17);
            }
    printf("skills_show_level: %d checks, %d failures\n", s881_checks, s881_failures);
    return s881_failures != 0;
}

#undef S881_CHECK
