#include "api.h"

/* FUNCTION: WMAIN 0x00408735, submit_screen_space_polyline_strip. */
void op_submit_polyline(const float *points, int count, unsigned int color,
                        void *material, float width)
{
    op_polyline_vertex projected_vertices[4];
    op_projected_vertex *projected;
    op_face_packet *packet;
    int segment;

    projected = op_peek_projected_vertices(count);
    packet = op_peek_face_packet();
    if (projected == 0 || packet == 0)
        return;
    op_transform_project(points, (float *)projected, 0, 0, count,
                         (const float *)((const unsigned char *)op_camera + 8));

    for (segment = 0; segment < count - 1; ++segment) {
        projected_vertices[0].x = projected[segment].x;
        projected_vertices[0].y = projected[segment].y;
        projected_vertices[0].rhw = projected[segment].reciprocal_depth;
        projected_vertices[0].z = projected[segment].mapped_depth;
        if (projected_vertices[0].z <= 0.0f)
            return;
        projected_vertices[0].color = color;
        projected_vertices[0].specular = 0x00ffffff;
        projected_vertices[0].u = 0.0f;
        projected_vertices[0].v = 0.0f;

        projected_vertices[1].x = projected[segment + 1].x;
        projected_vertices[1].y = projected[segment + 1].y;
        projected_vertices[1].rhw = projected[segment + 1].reciprocal_depth;
        projected_vertices[1].z = projected[segment + 1].mapped_depth;
        if (projected_vertices[1].z <= 0.0f)
            return;
        projected_vertices[1].color = color;
        projected_vertices[1].specular = 0x00ffffff;
        projected_vertices[1].u = 1.0f;
        projected_vertices[1].v = 0.0f;

        projected_vertices[2] = projected_vertices[1];
        projected_vertices[2].x += width;
        projected_vertices[2].u = 1.0f;
        projected_vertices[2].v = 1.0f;
        projected_vertices[3] = projected_vertices[0];
        projected_vertices[3].x += width;
        projected_vertices[3].u = 0.0f;
        projected_vertices[3].v = 1.0f;
        op_submit_polyline_quad(material, 0x48e00, projected_vertices, 4, 1);
    }
}
