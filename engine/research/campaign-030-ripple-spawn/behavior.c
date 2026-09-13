#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_RIPPLE op_ripples[256];
int op_ripples_enabled, op_projection_enabled;
float op_effect_seconds;
const float op_float_zero = 0;
static OP_RIPPLE expected[256];
static OP_VEC3 inputs[2], before_inputs[2];
static int selected, mutation, calls, token;
int op_allocate_projection_token(void)
{
    CHECK(calls++ == 0);
    CHECK(selected < 256);
    CHECK(memcmp(op_ripples, expected, sizeof(expected)) == 0);
    CHECK(memcmp(inputs, before_inputs, sizeof(inputs)) == 0);
    if (mutation)
    {
        op_ripples[selected].active = expected[selected].active = 0;
        op_ripples[selected].token = expected[selected].token = -77;
        op_ripples[selected].required_type = expected[selected].required_type = -99;
        op_ripples[(selected + 1) % 256].state = expected[(selected + 1) % 256].state = 123;
        op_effect_seconds = 999;
        op_ripples_enabled = 0;
        op_projection_enabled = 0;
    }
    return token;
}
int main(void)
{
    int i, enabled, velocity_present, r, profile, result;
    float radial[3] = {0, -0.0f, 2.5f};
    int tokens[3] = {0, -1, 12345};
    for (selected = 0; selected <= 256; selected++)
        for (enabled = 0; enabled < 3; enabled++)
            for (velocity_present = 0; velocity_present < 2; velocity_present++)
                for (r = 0; r < 3; r++)
                    for (mutation = 0; mutation < 2; mutation++)
                    {
                        memset(op_ripples, 0, sizeof(op_ripples));
                        for (i = 0; i < 256; i++)
                        {
                            op_ripples[i].active = -1;
                            op_ripples[i].token = i + 100;
                            op_ripples[i].required_type = -88;
                            op_ripples[i].yaw = (float)i;
                        }
                        if (selected < 256)
                            op_ripples[selected].active = 0;
                        inputs[0].x = 1.25f;
                        inputs[0].y = -2.5f;
                        inputs[0].z = 3;
                        inputs[1].x = -7;
                        inputs[1].y = 0;
                        inputs[1].z = 8.5f;
                        memcpy(before_inputs, inputs, sizeof(inputs));
                        memcpy(expected, op_ripples, sizeof(expected));
                        op_ripples_enabled = enabled == 0 ? 0 : -1;
                        op_projection_enabled = enabled == 1 ? 0 : 1;
                        op_effect_seconds = 123.25f;
                        profile = (selected + r) % 3;
                        token = tokens[profile];
                        calls = 0;
                        if (enabled == 2 && selected < 256)
                        {
                            expected[selected].active = 1;
                            expected[selected].appearance = INT_MIN;
                            expected[selected].position = inputs[0];
                            expected[selected].moving = velocity_present || radial[r] != 0;
                        }

                        if (enabled == 2 && selected < 256)
                        {
                            if (velocity_present)
                                expected[selected].velocity = inputs[1];
                            else
                                memset(&expected[selected].velocity, 0, 12);
                            expected[selected].yaw = -10;
                            expected[selected].radius = 20.5f;
                            expected[selected].radial_velocity = radial[r];
                            expected[selected].lifetime = -3;
                            expected[selected].render_parameter = 17;
                            expected[selected].time = 123.25f;
                            expected[selected].state = 0;
                        }
                        result =
                            op_ripple_spawn(INT_MIN, (enabled < 2 || selected == 256) ? 0 : &inputs[0],
                                            velocity_present ? &inputs[1] : 0, -10, 20.5f, radial[r], 17, -3, INT_MAX);
                        if (enabled == 2 && selected < 256)
                        {
                            expected[selected].token = token;
                            expected[selected].required_type = INT_MAX;
                            CHECK(result == token);
                            CHECK(calls == 1);
                        }
                        else
                        {
                            CHECK(result == 0);
                            CHECK(calls == 0);
                        }
                        CHECK(memcmp(op_ripples, expected, sizeof(expected)) == 0);
                        CHECK(memcmp(inputs, before_inputs, sizeof(inputs)) == 0);
                        CHECK(op_ripples_enabled ==
                              ((enabled == 2 && selected < 256 && mutation) ? 0 : (enabled == 0 ? 0 : -1)));
                        CHECK(op_projection_enabled ==
                              ((enabled == 2 && selected < 256 && mutation) ? 0 : (enabled == 1 ? 0 : 1)));
                        CHECK(op_effect_seconds == (enabled == 2 && selected < 256 && mutation ? 999.0f : 123.25f));
                    }
    printf("ripple spawn: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
