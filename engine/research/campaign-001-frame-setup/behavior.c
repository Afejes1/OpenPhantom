#include "frame.h"
#include <stdio.h>
#include <string.h>
#include <stddef.h>

op_frame_camera_transform *op_frame_transform;
op_frame_camera *op_frame_camera_state;
float op_scan_transform[12], op_scan_bounds[4];
int op_scan_octant, op_scan_octant_table[8][8], op_scan_axis_flags[16];
float op_scan_direction_x, op_scan_direction_y;
int op_scan_direction_step_x, op_scan_direction_step_y;
float op_scan_position[3];
int op_scan_x, op_scan_y, op_scan_anchor_x, op_scan_anchor_y;
int op_scan_limit, op_scan_bias, op_scan_mode, op_scan_mode_enabled;
int op_scan_quadrant, op_scan_offsets[511][2], op_scan_offset_index;
int op_scan_turn_x, op_scan_turn_y, op_scan_turn_z;
int op_scan_pending, op_scan_cursor, op_scan_low_mark, op_scan_high_mark;
op_scan_line op_frame_scan_line;

static int checks, failures, sin_cos_calls, line_calls, step_calls;
static int line_args[2][4];
static float next_cosine, next_sine;
static float trig_angles[2];
static void check(int condition, const char *expression, int line)
{
    ++checks;
    if (!condition) { ++failures; printf("line %d failed: %s\n", line, expression); }
}
#define CHECK(c) check((c), #c, __LINE__)

void op_sin_cos_degrees(float angle, float *cosine_out, float *sine_out)
{
    if (sin_cos_calls < 2) trig_angles[sin_cos_calls] = angle;
    ++sin_cos_calls;
    if (cosine_out) *cosine_out = next_cosine;
    if (sine_out) *sine_out = next_sine;
}
void op_init_scan_line(op_scan_line *line, int x0, int y0, int x1, int y1)
{
    int slot = line_calls < 2 ? line_calls : 1;
    line_args[slot][0] = x0; line_args[slot][1] = y0;
    line_args[slot][2] = x1; line_args[slot][3] = y1; ++line_calls;
    memset(line, 0, sizeof(*line));
    line->x = x0; line->y = y0; line->error = -1;
    line->diagonal_x = 1; line->diagonal_y = 1; line->diagonal_error = -2;
    line->straight_x = 1; line->straight_y = 0; line->straight_error = 1;
}
void op_step_scan(float *position, int x_step, int y_step)
{
    CHECK(position == op_scan_position);
    CHECK(x_step == (step_calls == 0 ? 0 : 1));
    CHECK(y_step == (step_calls != 0 && step_calls % 3 == 2 ? 1 : 0));
    position[0] += (float)x_step;
    position[1] += (float)y_step;
    ++step_calls;
}

static void reset(op_frame_camera_transform *transform, op_frame_viewport *viewport,
                  op_frame_camera *camera)
{
    int i, j;
    unsigned int payload;
    memset(transform, 0, sizeof(*transform)); memset(viewport, 0, sizeof(*viewport));
    memset(camera, 0, sizeof(*camera)); memset(op_scan_transform, 0, sizeof(op_scan_transform));
    memset(op_scan_axis_flags, 0, sizeof(op_scan_axis_flags));
    memset(op_scan_offsets, 0x5a, sizeof(op_scan_offsets));
    for (i = 0; i < 8; ++i) for (j = 0; j < 8; ++j) op_scan_octant_table[i][j] = i * 100 + j;
    for (i = 0; i < 12; ++i) transform->basis[i] = (float)(i * 3 - 7);
    payload = 0x7f812345U; memcpy(&transform->basis[2], &payload, 4);
    payload = 0x80000000U; memcpy(&transform->basis[3], &payload, 4);
    viewport->left = -17; viewport->top = 23; viewport->right = 641; viewport->bottom = 479;
    transform->viewport = viewport; transform->half_span = 40.0f;
    op_frame_transform = transform; op_frame_camera_state = camera;
    op_scan_x = 7; op_scan_y = -11;
    op_scan_position[0] = -3.0f; op_scan_position[1] = 4.0f; op_scan_position[2] = 5.0f;
    memset(line_args, 0x5a, sizeof(line_args));
    memset(trig_angles, 0x5a, sizeof(trig_angles));
    op_scan_direction_x = 0.25f; op_scan_direction_y = 0.50f;
    op_scan_turn_x = op_scan_turn_y = op_scan_turn_z = 99;
    op_scan_pending = op_scan_cursor = op_scan_low_mark = op_scan_high_mark = 99;
    sin_cos_calls = line_calls = step_calls = 0; next_cosine = 0.5f; next_sine = 1.0f;
}

static void check_common(op_frame_camera_transform *transform)
{
    int i;
    CHECK(memcmp(op_scan_transform, transform->basis, sizeof(op_scan_transform)) == 0);
    CHECK(op_scan_bounds[0] == -17.0f && op_scan_bounds[1] == 641.0f);
    CHECK(op_scan_bounds[2] == 23.0f && op_scan_bounds[3] == 479.0f);
    for (i = 0; i < 4; ++i) {
        CHECK(op_scan_axis_flags[i] == op_scan_octant_table[op_scan_octant / 45][i]);
        CHECK(op_scan_axis_flags[i + 4] == (op_scan_axis_flags[i] ^ 1));
        CHECK(op_scan_axis_flags[i + 8] == op_scan_octant_table[op_scan_octant / 45][i + 4]);
        CHECK(op_scan_axis_flags[i + 12] == (op_scan_axis_flags[i + 8] ^ 1));
    }
    CHECK(line_calls == 2 && op_scan_offset_index == 256);
    CHECK(line_args[1][2] == 256 && line_args[1][3] == 128);
    CHECK(op_scan_x == 7 + op_scan_mode - 1 && op_scan_y == -11 + op_scan_mode / 3);
    CHECK(op_scan_anchor_x == op_scan_x && op_scan_anchor_y == op_scan_y);
    CHECK(op_scan_position[0] == (float)(op_scan_mode - 4));
    CHECK(op_scan_position[1] == (float)(4 + op_scan_mode / 3) && op_scan_position[2] == 5.0f);
    CHECK(op_frame_scan_line.x == 255 && op_frame_scan_line.y == 85 && op_frame_scan_line.index == 255);
    CHECK(op_frame_scan_line.reserved_10 == 1 && op_frame_scan_line.reserved_14 == 0);
    CHECK(op_scan_offsets[255][0] == 0 && op_scan_offsets[255][1] == 0);
    for (i = 1; i < 256; ++i) {
        CHECK(op_scan_offsets[255 + i][0] == i && op_scan_offsets[255 + i][1] == (i + 1) / 3);
        CHECK(op_scan_offsets[255 - i][0] == -op_scan_offsets[255 + i][0]);
        CHECK(op_scan_offsets[255 - i][1] == -op_scan_offsets[255 + i][1]);
    }
    CHECK(op_scan_turn_x == 0 && op_scan_turn_y == 0 && op_scan_turn_z == 0);
    CHECK(op_scan_pending == 0 && op_scan_cursor == 256);
    CHECK(op_scan_low_mark == -1 && op_scan_high_mark == -1);
}

int main(void)
{
    op_frame_camera_transform transform; op_frame_viewport viewport;
    op_frame_camera camera; op_frame_world world;
    static const int octants[] = {44, 45, 134, 135, 224, 225, 314, 315};
    static const int quadrants[] = {0, 1, 1, 0, 0, 1, 1, 0};
    int case_index;
    CHECK(sizeof(op_scan_line) == 0x4c);
    CHECK(offsetof(op_scan_line, error) == 0x30 && offsetof(op_scan_line, straight_x) == 0x40);
    CHECK(offsetof(op_frame_camera_transform, basis) == 8 && offsetof(op_frame_camera_transform, half_span) == 0x38);
    CHECK(offsetof(op_frame_camera, pitch_degrees) == 0x34 && offsetof(op_frame_camera, eye_z) == 0x2c);
    CHECK(offsetof(op_frame_world, lower_z) == 0x78 && offsetof(op_frame_world, upper_z) == 0x7c);
    memset(&world, 0, sizeof(world)); world.lower_z = -20; world.upper_z = 20;
    reset(&transform, &viewport, &camera);
    camera.eye_z = 10.0f; camera.pitch_degrees = 30.0f; op_scan_octant = 90;
    op_frame_setup(&world);
    CHECK(sin_cos_calls == 1 && op_scan_mode_enabled == 0);
    CHECK(trig_angles[0] == 10.0f);
    CHECK(op_scan_mode == 2 && op_scan_limit == 25 && step_calls == 2);
    CHECK(op_scan_quadrant == 1);
    CHECK(line_args[0][0] == 0 && line_args[0][1] == 0);
    CHECK(line_args[0][2] == 128 && line_args[0][3] == -256);
    check_common(&transform);
    reset(&transform, &viewport, &camera);
    camera.eye_z = 10.0f; camera.pitch_degrees = 355.0f; op_scan_octant = 0;
    op_frame_setup(&world);
    CHECK(sin_cos_calls == 0 && op_scan_mode_enabled == 1);
    CHECK(op_scan_mode == 2 && op_scan_limit == 64 && step_calls == 2);
    CHECK(op_scan_quadrant == 0);
    check_common(&transform);
    reset(&transform, &viewport, &camera);
    camera.eye_z = 10.0f; camera.pitch_degrees = 30.0f; op_scan_octant = 180;
    next_cosine = 1.0f; next_sine = 0.5f;
    op_frame_setup(&world);
    CHECK(sin_cos_calls == 1 && op_scan_limit == 25);
    CHECK(op_scan_quadrant == 0);
    check_common(&transform);

    reset(&transform, &viewport, &camera);
    camera.eye_z = 10.0f; camera.pitch_degrees = 80.0f; op_scan_octant = 270;
    op_frame_setup(&world);
    CHECK(sin_cos_calls == 2 && op_scan_mode_enabled == 0);
    CHECK(trig_angles[0] == 60.0f && trig_angles[1] == 10.0f);
    CHECK(op_scan_mode == 8 && op_scan_limit == 31 && step_calls == 8);
    CHECK(op_scan_quadrant == 1);
    check_common(&transform);

    reset(&transform, &viewport, &camera);
    camera.eye_z = 10.0f; camera.pitch_degrees = -30.0f; op_scan_octant = 90;
    op_frame_setup(&world);
    CHECK(sin_cos_calls == 1 && trig_angles[0] == 10.0f && op_scan_limit == 25);
    check_common(&transform);

    reset(&transform, &viewport, &camera);
    camera.eye_z = 10.0f; camera.pitch_degrees = 180.0f; op_scan_octant = 90;
    op_frame_setup(&world);
    CHECK(sin_cos_calls == 2 && trig_angles[0] == 160.0f && trig_angles[1] == 110.0f);
    CHECK(op_scan_mode == 12 && op_scan_limit == 64 && step_calls == 12);
    check_common(&transform);

    reset(&transform, &viewport, &camera);
    camera.eye_z = 20.0f; camera.pitch_degrees = 30.0f; op_scan_octant = 90;
    op_frame_setup(&world);
    CHECK(op_scan_limit == 16 && op_scan_mode == 2);
    check_common(&transform);

    reset(&transform, &viewport, &camera);
    camera.eye_z = 10.0f; camera.pitch_degrees = 80.0f; op_scan_octant = 90;
    next_cosine = next_sine = 0.0f;
    op_frame_setup(&world);
    CHECK(op_scan_limit == 64 && op_scan_mode == 3 && sin_cos_calls == 2);
    check_common(&transform);

    for (case_index = 0; case_index < 8; ++case_index) {
        reset(&transform, &viewport, &camera);
        camera.eye_z = 10.0f; camera.pitch_degrees = 355.0f;
        op_scan_octant = octants[case_index];
        op_frame_setup(&world);
        CHECK(op_scan_quadrant == quadrants[case_index]);
    }

    printf("Frame setup: %d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
