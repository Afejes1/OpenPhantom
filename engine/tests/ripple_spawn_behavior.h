#include "../src/ripple_spawn.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int rs_checks, rs_failures;
static void rs_check(int condition, int line)
{
    rs_checks++;
    if (!condition)
    {
        rs_failures++;
        printf("line %d failed\n", line);
    }
}
#define rs_CHECK(x) rs_check(!!(x), __LINE__)
OP_RIPPLE op_ripples[256];
int op_ripples_enabled, op_projection_enabled;
float op_effect_seconds;
const float op_float_zero = 0;
static OP_RIPPLE rs_expected[256];
static OP_VEC3 rs_inputs[2], rs_before_inputs[2];
static int rs_selected, rs_mutation, rs_calls, rs_token;
int op_allocate_projection_token(void)
{
    rs_CHECK(rs_calls++ == 0);
    rs_CHECK(rs_selected < 256);
    rs_CHECK(memcmp(op_ripples, rs_expected, sizeof(rs_expected)) == 0);
    rs_CHECK(memcmp(rs_inputs, rs_before_inputs, sizeof(rs_inputs)) == 0);
    if (rs_mutation)
    {
        op_ripples[rs_selected].active = rs_expected[rs_selected].active = 0;
        op_ripples[rs_selected].token = rs_expected[rs_selected].token = -77;
        op_ripples[rs_selected].required_type = rs_expected[rs_selected].required_type = -99;
        op_ripples[(rs_selected + 1) % 256].state = rs_expected[(rs_selected + 1) % 256].state = 123;
        op_effect_seconds = 999;
        op_ripples_enabled = 0;
        op_projection_enabled = 0;
    }
    return rs_token;
}
int rs_main(void)
{
    int i, enabled, velocity_present, r, profile, result;
    float radial[3] = {0, -0.0f, 2.5f};
    int tokens[3] = {0, -1, 12345};
    for (rs_selected = 0; rs_selected <= 256; rs_selected++)
        for (enabled = 0; enabled < 3; enabled++)
            for (velocity_present = 0; velocity_present < 2; velocity_present++)
                for (r = 0; r < 3; r++)
                    for (rs_mutation = 0; rs_mutation < 2; rs_mutation++)
                    {
                        memset(op_ripples, 0, sizeof(op_ripples));
                        for (i = 0; i < 256; i++)
                        {
                            op_ripples[i].active = -1;
                            op_ripples[i].token = i + 100;
                            op_ripples[i].required_type = -88;
                            op_ripples[i].yaw = (float)i;
                        }
                        if (rs_selected < 256)
                            op_ripples[rs_selected].active = 0;
                        rs_inputs[0].x = 1.25f;
                        rs_inputs[0].y = -2.5f;
                        rs_inputs[0].z = 3;
                        rs_inputs[1].x = -7;
                        rs_inputs[1].y = 0;
                        rs_inputs[1].z = 8.5f;
                        memcpy(rs_before_inputs, rs_inputs, sizeof(rs_inputs));
                        memcpy(rs_expected, op_ripples, sizeof(rs_expected));
                        op_ripples_enabled = enabled == 0 ? 0 : -1;
                        op_projection_enabled = enabled == 1 ? 0 : 1;
                        op_effect_seconds = 123.25f;
                        profile = (rs_selected + r) % 3;
                        rs_token = tokens[profile];
                        rs_calls = 0;
                        if (enabled == 2 && rs_selected < 256)
                        {
                            rs_expected[rs_selected].active = 1;
                            rs_expected[rs_selected].appearance = INT_MIN;
                            rs_expected[rs_selected].position = rs_inputs[0];
                            rs_expected[rs_selected].moving = velocity_present || radial[r] != 0;
                        }

                        if (enabled == 2 && rs_selected < 256)
                        {
                            if (velocity_present)
                                rs_expected[rs_selected].velocity = rs_inputs[1];
                            else
                                memset(&rs_expected[rs_selected].velocity, 0, 12);
                            rs_expected[rs_selected].yaw = -10;
                            rs_expected[rs_selected].radius = 20.5f;
                            rs_expected[rs_selected].radial_velocity = radial[r];
                            rs_expected[rs_selected].lifetime = -3;
                            rs_expected[rs_selected].render_parameter = 17;
                            rs_expected[rs_selected].time = 123.25f;
                            rs_expected[rs_selected].state = 0;
                        }
                        result = op_ripple_spawn(INT_MIN, (enabled < 2 || rs_selected == 256) ? 0 : &rs_inputs[0],
                                                 velocity_present ? &rs_inputs[1] : 0, -10, 20.5f, radial[r], 17, -3,
                                                 INT_MAX);
                        if (enabled == 2 && rs_selected < 256)
                        {
                            rs_expected[rs_selected].token = rs_token;
                            rs_expected[rs_selected].required_type = INT_MAX;
                            rs_CHECK(result == rs_token);
                            rs_CHECK(rs_calls == 1);
                        }
                        else
                        {
                            rs_CHECK(result == 0);
                            rs_CHECK(rs_calls == 0);
                        }
                        rs_CHECK(memcmp(op_ripples, rs_expected, sizeof(rs_expected)) == 0);
                        rs_CHECK(memcmp(rs_inputs, rs_before_inputs, sizeof(rs_inputs)) == 0);
                        rs_CHECK(op_ripples_enabled ==
                                 ((enabled == 2 && rs_selected < 256 && rs_mutation) ? 0 : (enabled == 0 ? 0 : -1)));
                        rs_CHECK(op_projection_enabled ==
                                 ((enabled == 2 && rs_selected < 256 && rs_mutation) ? 0 : (enabled == 1 ? 0 : 1)));
                        rs_CHECK(op_effect_seconds ==
                                 (enabled == 2 && rs_selected < 256 && rs_mutation ? 999.0f : 123.25f));
                    }
    printf("ripple spawn: %d checks, %d failures\n", rs_checks, rs_failures);
    return rs_failures != 0;
}

#undef rs_CHECK
