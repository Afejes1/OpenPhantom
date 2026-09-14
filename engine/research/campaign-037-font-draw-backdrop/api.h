#ifndef OP_CAMPAIGN037_API_H
#define OP_CAMPAIGN037_API_H
#include <stddef.h>
/* Recovered 72-byte record fields; domain meanings of word20/24 remain unknown.
   Pool is exactly 16 records; no research fixture guard belongs to its layout. */
typedef struct OP_FONT3D
{
    void *sprite, *backdrop, *font;
    unsigned int colours[4], align, word20, word24;
    float glyph_x, glyph_y, backdrop_width, backdrop_height, position_x, position_y, z_start, z_end;
} OP_FONT3D;
typedef struct OP_FONT_POOL
{
    OP_FONT3D rows[16];
} OP_FONT_POOL;
typedef char font3d_layout[(sizeof(void *) == 4 && sizeof(OP_FONT3D) == 72 && offsetof(OP_FONT3D, font) == 8 &&
                            offsetof(OP_FONT3D, colours) == 12 && offsetof(OP_FONT3D, align) == 28 &&
                            offsetof(OP_FONT3D, word20) == 32 && offsetof(OP_FONT3D, word24) == 36 &&
                            offsetof(OP_FONT3D, glyph_x) == 40 && offsetof(OP_FONT3D, backdrop_width) == 48 &&
                            offsetof(OP_FONT3D, position_x) == 56 && offsetof(OP_FONT3D, z_start) == 64 &&
                            offsetof(OP_FONT3D, z_end) == 68)
                               ? 1
                               : -1];
extern OP_FONT_POOL op_font_pool;
extern OP_FONT3D *op_current_font;
int op_release_sprite(void **);
void *op_acquire_sprite(char *);
void op_get_last_textbox(unsigned int *, unsigned int *, unsigned int *, unsigned int *);
unsigned int op_query_font_resource(void *);
void op_font_select(int index);
void op_font_set_colour(unsigned int colour);
void op_font_set_colours(unsigned int a, unsigned int b, unsigned int c, unsigned int d);
void op_font_get_colours(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d);
void op_font_set_align(int align);
void op_font_set_glyph_scale(float x, float y);
void op_font_set_position_scale(float x, float y);
void op_font_set_word20(unsigned int word);
int op_font_set_backdrop(char *name, float width, float height);
int op_font_set_zrange(float start, float range);
void op_font_last_textbox(unsigned int *a, unsigned int *b, unsigned int *c, unsigned int *d);
int op_font_set_word24(unsigned int word);
typedef struct OP_FONT_RESOURCE
{
    unsigned char prefix[16];
    void *material;
} OP_FONT_RESOURCE;
extern int op_system_font_slot;
extern char op_system_font_name[];
extern float op_font_screen_width, op_font_screen_height;
void *op_sprite_get_material(void *);
void *op_load_font_resource(char *, void *);
void op_free_font_resource(void *);
void op_set_font_scale(float, float);
int op_font_char_size(void *, char, float *, float *);
void op_draw_font_backdrop(void *, float, float, float, float);
int op_font_module(int);
int op_font_create(char *);
void op_font_destroy(int);
int op_font_measure_char(char, float *, float *);
float op_font_measure_string(char *);
void op_font_draw_backdrop(void);
#endif
