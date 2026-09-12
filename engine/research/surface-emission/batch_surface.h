#ifndef OP_SURFACE_EMISSION_RESEARCH_H
#define OP_SURFACE_EMISSION_RESEARCH_H
#include "baseline.h"
typedef struct op_emission_surface {
 unsigned short record_tag;
 unsigned char opaque_02[10];
 unsigned char reject_flags, opaque_0d, opacity, opaque_0f[13];
 unsigned char height, frustum_skip_extra, room_skip_extra, opaque_1f[5];
 unsigned char texture_index, opaque_25[2], clip_marker;
 unsigned char render_flags, opaque_29, visibility_group, packed_opacity;
 unsigned short flags;
 unsigned char opaque_2e[2];
 unsigned short unique_index;
 unsigned char opaque_32[6];
 unsigned char room_mask, opaque_39[2], height_extent, opaque_3c[4];
} op_emission_surface;
#ifdef __cplusplus
extern "C" {
#endif
extern unsigned char op_surface_reject_flags, op_room_reject_mask;
extern unsigned char op_surface_group_state[], op_surface_seen[];
extern unsigned char op_room_new_mask, op_room_old_mask;
extern int op_room_fade_active;
extern unsigned int op_rejected_surface_count;
extern float op_room_fade_remaining;
void op_emit_surface(op_emission_surface *surface);
#ifdef __cplusplus
}
#endif

typedef struct op_legacy_camera { unsigned char opaque_00[0x2c]; float eye_z; } op_legacy_camera;
#ifdef __cplusplus
extern "C" {
#endif
extern int op_allow_upper_surfaces, op_legacy_fade_frames, op_legacy_cull_enabled;
extern int op_legacy_scan_cull_side, op_legacy_inverted_height;
extern unsigned char op_legacy_selected_room_mask;
extern op_legacy_camera *op_legacy_camera_state;
extern float op_legacy_cached_height, op_legacy_scan_position[3], op_scan_step_z[3];
int op_cull_scan_plane(int plane, const float *position, float height);
void op_emit_legacy_surface(op_emission_surface *surface);
#ifdef __cplusplus
}
#endif

#endif
