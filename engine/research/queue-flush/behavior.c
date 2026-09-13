#include "flush.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int op_material_bucket_count, op_face_packet_count, op_vertex_fog_enabled;
unsigned int op_projected_vertex_count, op_frame_render_flags, op_frame_extra_render_flags;
op_material_bucket op_material_buckets[4], *op_last_material_bucket;
op_projected_vertex op_projected_vertices[96];
unsigned int op_projected_fog[96];

static op_flush_material materials[2];
static op_flush_texture textures[2];
static op_texture_dimensions dimensions[2];
static op_flush_packet packets[3];
static op_flush_face faces[2];
static int face_indices[20];
static op_pixel_uv coordinates[32];
static unsigned int colors[32];
static op_backend_vertex captured[64];
static int checks, failures, prepare_result, prepares, binds, draws, queued;
static int flag_count, captured_mode, append_after_draw, expand_on_bind;
static unsigned int flags_seen[8], captured_count, captured_flags;
static void *captured_handle;

static void check(int condition, const char *expression, int line)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("line %d failed: %s\n", line, expression);
    }
}
#define CHECK(c) check((c), #c, __LINE__)

static unsigned int word(const void *p)
{
    unsigned int value;
    memcpy(&value, p, 4);
    return value;
}

int op_prepare_material_texture(op_flush_material *material, op_flush_texture *texture, int mip)
{
    CHECK(material == &materials[0] || material == &materials[1]);
    CHECK(texture == material->first_texture);
    CHECK(mip == 0);
    ++prepares;
    return prepare_result;
}

unsigned int op_backend_set_flags(unsigned int flags)
{
    CHECK(flag_count < 8);
    flags_seen[flag_count++] = flags;
    return 0;
}

void op_backend_bind_texture(void *handle)
{
    CHECK(handle == (prepare_result ? (void *)&textures[binds].handle : 0));
    ++binds;
    if (expand_on_bind && binds == 1) op_material_bucket_count = 2;
}

static void capture(op_backend_vertex *vertices, unsigned int count, int mode)
{
    CHECK(count <= 64);
    captured_count = count;
    captured_mode = mode;
    if (count <= 64) memcpy(captured, vertices, count * sizeof(*vertices));
    ++draws;
    if (append_after_draw && draws == 1) packets[0].next = &packets[1];
}

void op_backend_draw_vertices(op_backend_vertex *vertices, int count, int mode)
{
    capture(vertices, count, mode);
}

void *op_backend_queue_vertices(void *handle, unsigned int flags,
    op_backend_vertex *vertices, unsigned int count, int mode)
{
    captured_handle = handle;
    captured_flags = flags;
    ++queued;
    capture(vertices, count, mode);
    return 0;
}

static void reset(unsigned int count, unsigned int alpha, unsigned int tag,
    unsigned int face_flags, int fog, int use_colors, int ready)
{
    unsigned int i;
    memset(materials, 0, sizeof(materials));
    memset(textures, 0, sizeof(textures));
    memset(dimensions, 0, sizeof(dimensions));
    memset(packets, 0, sizeof(packets));
    memset(faces, 0, sizeof(faces));
    memset(op_material_buckets, 0, sizeof(op_material_buckets));
    memset(captured, 0xa5, sizeof(captured));
    for (i = 0; i < 2; ++i) {
        materials[i].first_texture = &textures[i];
        textures[i].dimensions = &dimensions[i];
        textures[i].handle = 0xa1b2c3d4;
        dimensions[i].width = 32;
        dimensions[i].height = 64;
        op_material_buckets[i].material = &materials[i];
    }
    for (i = 0; i < 96; ++i) {
        op_projected_vertices[i].x = (float)i + 0.125f;
        op_projected_vertices[i].y = (float)i + 0.25f;
        op_projected_vertices[i].mapped_depth = (float)i + 0.5f;
        op_projected_vertices[i].reciprocal_depth = (float)i + 0.75f;
        op_projected_fog[i] = 0x81000000 + i * 0x10000;
    }
    for (i = 0; i < 32; ++i) {
        coordinates[i].u = (float)(i + 1) * 8.0f;
        coordinates[i].v = (float)(i + 1) * 16.0f;
        colors[i] = 0x12340000 | i;
    }
    for (i = 0; i < 20; ++i) {
        face_indices[i] = (int)((i * 7 + 2) % 20);
        packets[0].projected_indices[i] = (int)((i * 3 + 1) % 20);
        packets[0].uv_indices[i] = (int)(19 - i);
    }
    faces[0].flags = face_flags;
    faces[0].vertex_indices = face_indices;
    packets[0].face = &faces[0];
    packets[0].projected = &op_projected_vertices[2];
    packets[0].uv = coordinates;
    packets[0].colors = use_colors ? colors : 0;
    packets[0].corner_count = (unsigned char)count;
    packets[0].opacity = (unsigned char)alpha;
    packets[0].render_tag = (unsigned char)tag;
    packets[0].draw_mode = 255;
    op_material_buckets[0].head = op_material_buckets[0].tail = (op_face_packet *)&packets[0];
    op_material_buckets[3].material = &materials[1];
    op_last_material_bucket = &op_material_buckets[0];
    op_material_bucket_count = 1;
    op_face_packet_count = 57;
    op_projected_vertex_count = 83;
    op_vertex_fog_enabled = fog;
    op_frame_render_flags = 0x100040;
    op_frame_extra_render_flags = 0x100;
    prepare_result = ready;
    prepares = binds = draws = queued = flag_count = 0;
    captured_count = captured_flags = 0;
    captured_mode = -1;
    captured_handle = 0;
    append_after_draw = expand_on_bind = 0;
}

static void check_cleanup(void)
{
    CHECK(op_material_bucket_count == 0);
    CHECK(op_face_packet_count == 0);
    CHECK(op_projected_vertex_count == 0);
    CHECK(op_last_material_bucket == 0);
    CHECK(op_material_buckets[0].material == 0);
    CHECK(op_material_buckets[0].head == 0);
    CHECK(op_material_buckets[0].tail == 0);
    CHECK(op_material_buckets[3].material == &materials[1]);
}

static void check_single(unsigned int count, unsigned int alpha, unsigned int tag,
    unsigned int face_flags, int fog, int use_colors, int ready)
{
    unsigned int i, expected;
    reset(count, alpha, tag, face_flags, fog, use_colors, ready);
    op_flush_material_buckets();
    CHECK(prepares == 1 && binds == 1 && draws == 1);
    CHECK(captured_count == count && captured_mode == 255);
    CHECK(flags_seen[0] == 0x100040);
    if (alpha == 255) {
        CHECK(queued == 0 && flag_count == 2);
        CHECK(flags_seen[1] == (tag ? 0x100000U : 0x100040U));
    } else {
        CHECK(queued == 1 && flag_count == 1);
        expected = 0x100740 | ((face_flags & 0x10) ? 0x40000 : 0);
        if (tag) expected &= ~0x40U;
        CHECK(captured_flags == expected);
        CHECK(captured_handle == (ready ? (void *)&textures[0].handle : 0));
    }
    for (i = 0; i < count; ++i) {
        int pi = packets[0].projected_indices[i] + 2;
        int ui = packets[0].uv_indices[i];
        int ci = face_indices[i];
        CHECK(word(&captured[i].x) == word(&op_projected_vertices[pi].x));
        CHECK(word(&captured[i].y) == word(&op_projected_vertices[pi].y));
        CHECK(word(&captured[i].z) == word(&op_projected_vertices[pi].mapped_depth));
        CHECK(word(&captured[i].rhw) == word(&op_projected_vertices[pi].reciprocal_depth));
        CHECK(captured[i].u == (float)(ui + 1) * (ready ? 0.25f : 8.0f));
        CHECK(captured[i].v == (float)(ui + 1) * (ready ? 0.25f : 16.0f));
        expected = use_colors ? colors[ci] : 0xffffffff;
        if (use_colors && alpha != 255) expected = (expected & 0xffffff) | (alpha << 24);
        CHECK(captured[i].diffuse == expected);
        CHECK(captured[i].specular == (fog ? op_projected_fog[ci + 2] : 0));
    }
    check_cleanup();
}

int main(void)
{
    static const unsigned int alphas[] = {0, 1, 127, 128, 254, 255};
    static const unsigned int tags[] = {0, 1, 255};
    unsigned int a, t, f, c, n, payload;
    CHECK(sizeof(op_backend_vertex) == 32);
    CHECK(sizeof(op_flush_packet) == sizeof(op_face_packet));
    CHECK(offsetof(op_flush_packet, projected_indices) == 0x18);
    CHECK(offsetof(op_flush_packet, uv_indices) == 0x68);
    CHECK(offsetof(op_flush_packet, render_tag) == offsetof(op_face_packet, render_tag));
    CHECK(offsetof(op_flush_texture, dimensions) == 0x1c);
    CHECK(offsetof(op_flush_texture, handle) == 0x2c);
    CHECK(offsetof(op_flush_material, first_texture) == 0xb0);
    CHECK(offsetof(op_flush_face, vertex_indices) == 0x18);
    for (a = 0; a < 6; ++a) for (t = 0; t < 3; ++t)
        for (f = 0; f < 2; ++f) for (c = 0; c < 2; ++c)
            check_single(3, alphas[a], tags[t], f ? 0x10 : 0, f ? -1 : 0, (int)c, 1);
    for (n = 0; n <= 20; ++n) check_single(n, 255, 0, 0, 1, 1, 1);
    check_single(3, 128, 255, 0x10, 0, 1, 0);
    check_single(3, 255, 0, 0, 1, 0, -7);

    reset(3, 255, 0, 0, 0, 1, 0);
    materials[0].first_texture = 0;
    op_flush_material_buckets();
    CHECK(draws == 1 && captured[0].u == 160.0f);
    check_cleanup();

    reset(1, 255, 0, 0, 0, 1, 1);
    packets[0].uv_indices[0] = 0;
    dimensions[0].width = dimensions[0].height = 0x80000000U;
    payload = 0x80000000U;
    memcpy(&op_projected_vertices[3].x, &payload, 4);
    payload = 0x7fc12345U;
    memcpy(&op_projected_vertices[3].y, &payload, 4);
    op_flush_material_buckets();
    CHECK(word(&captured[0].u) == 0x31800000U);
    CHECK(word(&captured[0].v) == 0x32000000U);
    CHECK(word(&captured[0].x) == 0x80000000U);
    CHECK(word(&captured[0].y) == 0x7fc12345U);
    check_cleanup();

    reset(3, 255, 0, 0, 0, 1, 1);
    packets[2].next = &packets[0];
    op_material_buckets[0].head = (op_face_packet *)&packets[2];
    op_flush_material_buckets();
    CHECK(draws == 1);
    check_cleanup();

    reset(0, 0, 0, 0, 1, 0, 1);
    packets[0].projected = 0;
    packets[0].uv = 0;
    faces[0].vertex_indices = 0;
    op_flush_material_buckets();
    CHECK(draws == 1 && queued == 1 && captured_count == 0);
    check_cleanup();

    reset(3, 255, 0, 0, 0, 1, 1);
    packets[1] = packets[0];
    append_after_draw = 1;
    op_flush_material_buckets();
    CHECK(draws == 2 && flag_count == 3);
    check_cleanup();

    reset(3, 255, 0, 0, 0, 1, 1);
    expand_on_bind = 1;
    op_flush_material_buckets();
    CHECK(prepares == 2 && binds == 2 && draws == 1);
    CHECK(op_material_buckets[1].material == 0);
    check_cleanup();

    for (n = 0; n < 2; ++n) {
        reset(3, 255, 0, 0, 0, 1, 1);
        op_material_bucket_count = n ? -1 : 0;
        op_flush_material_buckets();
        CHECK(prepares == 0 && binds == 0 && flag_count == 0 && draws == 0);
        CHECK(op_material_bucket_count == (n ? -1 : 0));
        CHECK(op_face_packet_count == 57 && op_projected_vertex_count == 83);
        CHECK(op_last_material_bucket == &op_material_buckets[0]);
        CHECK(op_material_buckets[0].head == (op_face_packet *)&packets[0]);
    }
    printf("Queue flush: %d checks, %d failures\n", checks, failures);
    return failures ? 1 : 0;
}
