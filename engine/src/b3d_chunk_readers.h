#ifndef OP_B3D_CHUNK_READERS_H
#define OP_B3D_CHUNK_READERS_H

#include <stddef.h>

/* Partial views. Unnamed bytes preserve verified retail field offsets. */
typedef struct OP_B3D_STREAM
{
    void *handle;
    int mode;
} OP_B3D_STREAM;

typedef struct OP_B3D_HEADER
{
    unsigned char before_palettes[0x20];
    unsigned int palette_count;
    unsigned char before_global_vertices[0x24];
    unsigned int global_vertex_count;
    unsigned int global_vertex_record_size;
    unsigned int local_vertex_count;
    unsigned int local_vertex_record_size;
    unsigned char before_sprites[0x278];
    unsigned int sprite_ref_count;
    unsigned char before_emitters[0x14];
    unsigned int emitter_count;
    unsigned int emitter_record_size;
    unsigned char before_payload[0x510];
    unsigned int payload_size;
} OP_B3D_HEADER;

typedef struct OP_B3D_GLOBAL_VERTEX_RECORD
{
    unsigned int words[8];
} OP_B3D_GLOBAL_VERTEX_RECORD;

typedef struct OP_B3D_LOCAL_VERTEX_RECORD
{
    unsigned int words[7];
} OP_B3D_LOCAL_VERTEX_RECORD;

typedef struct OP_B3D_WORLD
{
    unsigned char before_palettes[0x64];
    unsigned int palette_count;
    unsigned int palette_capacity;
    void *palettes;
    unsigned char before_global_vertices[0x24];
    unsigned int global_vertex_count;
    unsigned char before_global_vertices_pointer[4];
    OP_B3D_GLOBAL_VERTEX_RECORD *global_vertices;
    unsigned char before_local_vertex_count[4];
    unsigned int local_vertex_count;
    unsigned char before_local_vertices_pointer[4];
    OP_B3D_LOCAL_VERTEX_RECORD *local_vertices;
    unsigned char before_sprites[0xc04];
    unsigned int sprite_count;
    void *sprites;
    unsigned char before_emitters[0x10];
    unsigned int emitter_count;
    void *emitters;
} OP_B3D_WORLD;

typedef char op_b3d_word_width[(sizeof(int) == 4 && sizeof(unsigned int) == 4) ? 1 : -1];
typedef char op_b3d_pointer_width[(sizeof(void *) == 4) ? 1 : -1];
typedef char op_b3d_stream_mode[(offsetof(OP_B3D_STREAM, mode) == 4) ? 1 : -1];
typedef char op_b3d_header_palette[(offsetof(OP_B3D_HEADER, palette_count) == 0x20) ? 1 : -1];
typedef char op_b3d_header_global_vertex_count[(offsetof(OP_B3D_HEADER, global_vertex_count) == 0x48) ? 1 : -1];
typedef char op_b3d_header_global_vertex_size[(offsetof(OP_B3D_HEADER, global_vertex_record_size) == 0x4c) ? 1 : -1];
typedef char op_b3d_header_local_vertex_count[(offsetof(OP_B3D_HEADER, local_vertex_count) == 0x50) ? 1 : -1];
typedef char op_b3d_header_local_vertex_size[(offsetof(OP_B3D_HEADER, local_vertex_record_size) == 0x54) ? 1 : -1];
typedef char op_b3d_header_sprite[(offsetof(OP_B3D_HEADER, sprite_ref_count) == 0x2d0) ? 1 : -1];
typedef char op_b3d_header_emitter[(offsetof(OP_B3D_HEADER, emitter_count) == 0x2e8) ? 1 : -1];
typedef char op_b3d_header_emitter_size[(offsetof(OP_B3D_HEADER, emitter_record_size) == 0x2ec) ? 1 : -1];
typedef char op_b3d_header_payload[(offsetof(OP_B3D_HEADER, payload_size) == 0x800) ? 1 : -1];
typedef char op_b3d_header_extent[(sizeof(OP_B3D_HEADER) == 0x804) ? 1 : -1];
typedef char op_b3d_world_palette[(offsetof(OP_B3D_WORLD, palette_count) == 0x64) ? 1 : -1];
typedef char op_b3d_world_capacity[(offsetof(OP_B3D_WORLD, palette_capacity) == 0x68) ? 1 : -1];
typedef char op_b3d_world_palettes[(offsetof(OP_B3D_WORLD, palettes) == 0x6c) ? 1 : -1];
typedef char op_b3d_global_vertex_record_size[(sizeof(OP_B3D_GLOBAL_VERTEX_RECORD) == 0x20) ? 1 : -1];
typedef char op_b3d_local_vertex_record_size[(sizeof(OP_B3D_LOCAL_VERTEX_RECORD) == 0x1c) ? 1 : -1];
typedef char op_b3d_world_global_vertex_count[(offsetof(OP_B3D_WORLD, global_vertex_count) == 0x94) ? 1 : -1];
typedef char op_b3d_world_global_vertices[(offsetof(OP_B3D_WORLD, global_vertices) == 0x9c) ? 1 : -1];
typedef char op_b3d_world_local_vertex_count[(offsetof(OP_B3D_WORLD, local_vertex_count) == 0xa4) ? 1 : -1];
typedef char op_b3d_world_local_vertices[(offsetof(OP_B3D_WORLD, local_vertices) == 0xac) ? 1 : -1];
typedef char op_b3d_world_sprite_count[(offsetof(OP_B3D_WORLD, sprite_count) == 0xcb4) ? 1 : -1];
typedef char op_b3d_world_sprites[(offsetof(OP_B3D_WORLD, sprites) == 0xcb8) ? 1 : -1];
typedef char op_b3d_world_emitter_count[(offsetof(OP_B3D_WORLD, emitter_count) == 0xccc) ? 1 : -1];
typedef char op_b3d_world_emitters[(offsetof(OP_B3D_WORLD, emitters) == 0xcd0) ? 1 : -1];
typedef char op_b3d_world_extent[(sizeof(OP_B3D_WORLD) == 0xcd4) ? 1 : -1];

void *op_allocate(unsigned int bytes);
/* Actual stream backend uses signed size/count arithmetic; four cdecl args. */
int op_stream_read(void *destination, int element_size, int count, OP_B3D_STREAM *stream);
int op_world_read_emitters(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header);
int op_world_read_sprite_refs(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header);
int op_world_read_palettes(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header);
int op_world_read_global_vertices(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header);
int op_world_read_local_vertices(OP_B3D_STREAM *stream, OP_B3D_WORLD *world, OP_B3D_HEADER *header);

#endif
