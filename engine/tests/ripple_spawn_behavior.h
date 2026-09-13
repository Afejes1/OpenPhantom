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
static int rs_selected, rs_token;
int rs_main(void)
{
    int i, enabled, velocity_present, r, profile, result, seed;
    unsigned int expected_counter, counter_before;
    float radial[3] = {0, -0.0f, 2.5f};
    unsigned int seeds[6] = {0, 0x7FFFFFFEU, 0x7FFFFFFFU, 0xFFFFFFFEU, 0xFFFFFFFFU, 12344};
    for (rs_selected = 0; rs_selected <= 256; rs_selected++)
        for (enabled = 0; enabled < 3; enabled++)
            for (velocity_present = 0; velocity_present < 2; velocity_present++)
                for (r = 0; r < 3; r++)
                    for (seed = 0; seed < 6; seed++)
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
                        profile = (rs_selected + r) % 6;
                        counter_before = seeds[(profile + seed) % 6];
                        op_projection_counter = counter_before;
                        expected_counter = counter_before;
                        if (enabled == 2 && rs_selected < 256)
                            expected_counter++;
                        rs_token = (int)expected_counter;
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
                            rs_CHECK(op_projection_counter == expected_counter);
                        }
                        else
                        {
                            rs_CHECK(result == 0);
                            rs_CHECK(op_projection_counter == counter_before);
                        }
                        rs_CHECK(memcmp(op_ripples, rs_expected, sizeof(rs_expected)) == 0);
                        rs_CHECK(memcmp(rs_inputs, rs_before_inputs, sizeof(rs_inputs)) == 0);
                        rs_CHECK(op_ripples_enabled == (enabled == 0 ? 0 : -1));
                        rs_CHECK(op_projection_enabled == (enabled == 1 ? 0 : 1));
                        rs_CHECK(op_effect_seconds == 123.25f);
                    }
    printf("ripple spawn: %d checks, %d failures\n", rs_checks, rs_failures);
    return rs_failures != 0;
}

#undef rs_CHECK
