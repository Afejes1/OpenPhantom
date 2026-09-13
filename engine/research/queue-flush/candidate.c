#include "flush.h"
/* FUNCTION: WMAIN 0x00402155 */
/* VC5 /Od local identifiers and declaration order retain the observed layout. */
void op_flush_material_buckets(void)
{
    op_material_bucket *materialBucket;
    op_flush_packet *packet;
    op_backend_vertex *output;
    int bucket_index;
    op_flush_material *material;
    op_flush_face *packet_face;
    op_backend_vertex verts[64];
    float uScale;
    op_pixel_uv *uv;
    int corner;
    op_flush_texture *map_texture;
    op_projected_vertex *points_value;
    unsigned int num_vertices;
    void *native_texture;
    unsigned int diffuse;
    float uv_scale_v;
    int local_projected_index;
    int current_uv_index;
    int has_alpha_local;
    unsigned int flags;
    if (op_material_bucket_count <= 0) return;
    op_backend_set_flags(op_frame_render_flags);
    for (bucket_index = 0; bucket_index < op_material_bucket_count; ++bucket_index) {
        materialBucket = &op_material_buckets[bucket_index];
        material = (op_flush_material *)materialBucket->material;
        map_texture = material->first_texture;
        if (!op_prepare_material_texture(material, map_texture, 0)) {
            native_texture = 0;
            uScale = uv_scale_v = 1.0f;
        } else {
            native_texture = &map_texture->handle;
            uScale = 1.0f / map_texture->dimensions->width;
            uv_scale_v = 1.0f / map_texture->dimensions->height;
        }
        op_backend_bind_texture(native_texture);
        for (packet = (op_flush_packet *)materialBucket->head; packet != 0; packet = packet->next) {
            flags = 0;
            has_alpha_local = packet->opacity != 255;
            packet_face = packet->face;
            if (packet_face == 0) continue;
            num_vertices = packet->corner_count;
            output = verts;
            points_value = packet->projected;
            uv = packet->uv;
            for (corner = 0; corner < (int)num_vertices; ++corner) {
                local_projected_index = packet->projected_indices[corner];
                current_uv_index = packet->uv_indices[corner];
                output->x = points_value[local_projected_index].x;
                output->y = points_value[local_projected_index].y;
                output->rhw = points_value[local_projected_index].reciprocal_depth;
                output->z = points_value[local_projected_index].mapped_depth;
                local_projected_index = packet_face->vertex_indices[corner];
                output->u = uv[current_uv_index].u * uScale;
                output->v = uv[current_uv_index].v * uv_scale_v;
                if (packet->colors != 0) {
                    diffuse = packet->colors[local_projected_index];
                    if (has_alpha_local) diffuse = (diffuse & 0x00ffffff) | ((unsigned int)packet->opacity << 24);
                } else diffuse = 0xffffffff;
                output->diffuse = diffuse;
                if (op_vertex_fog_enabled) output->specular = op_projected_fog[(points_value - op_projected_vertices) + local_projected_index];
                else output->specular = 0;
                ++output;
            }
            if (has_alpha_local) {
                unsigned int state_mask = 0xffffffff;
                if (packet_face->flags & 0x10) flags = 0x40000;
                flags |= 0x600;
                if (packet->render_tag) state_mask &= ~0x40;
                op_backend_queue_vertices(native_texture, (op_frame_render_flags | op_frame_extra_render_flags | flags) & state_mask, verts, num_vertices, packet->draw_mode);
            } else {
                if (packet->render_tag) op_backend_set_flags(op_frame_render_flags & ~0x40);
                else op_backend_set_flags(op_frame_render_flags);
                op_backend_draw_vertices(verts, num_vertices, packet->draw_mode);
            }
        }
        materialBucket->material = 0;
        materialBucket->head = 0;
        materialBucket->tail = 0;
    }
    op_material_bucket_count = 0;
    op_last_material_bucket = 0;
    op_face_packet_count = 0;
    op_projected_vertex_count = 0;
}
