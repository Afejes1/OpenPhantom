static int cv_active, cv_mode, fctor_active;

#include "../src/canvas.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int cv_checks, cv_failures;
static void cv_check(int ok, int line)
{
    ++cv_checks;
    if (!ok)
    {
        ++cv_failures;
        printf("line%d failed\n", line);
    }
}
#define cv_CHECK(x) cv_check(!!(x), __LINE__)
const float op_canvas_two = 2.0f;
typedef struct cv_OWNED_CANVAS
{
    unsigned int before;
    OP_CANVAS value;
    unsigned int after;
} cv_OWNED_CANVAS;
static cv_OWNED_CANVAS cv_canvases[4], cv_expected[4];
static int cv_row, cv_stage;
static void cv_seed(void)
{
    memset(cv_canvases, 0x69, sizeof(cv_canvases));
    memcpy(cv_expected, cv_canvases, sizeof(cv_canvases));
    cv_stage = 0;
}
static void cv_verify(void)
{
    cv_CHECK(memcmp(cv_canvases, cv_expected, sizeof(cv_canvases)) == 0);
}

typedef struct cv_OWNED_RASTER
{
    unsigned int before;
    OP_RASTER value;
    unsigned int after;
} cv_OWNED_RASTER;
static cv_OWNED_RASTER cv_rasters[4], cv_raster_expected[4];
typedef struct cv_RECT_CASE
{
    int l, t, r, b;
    unsigned int cx, cy;
} cv_RECT_CASE;
typedef struct cv_DIM_CASE
{
    int w, h;
    unsigned int cx, cy;
} cv_DIM_CASE;
static cv_RECT_CASE cv_rectangles[] = {{0, 0, 639, 479, 0x43a00000u, 0x43700000u},
                                       {1, 3, 8, 10, 0x40a00000u, 0x40e00000u},
                                       {-10, -20, -3, -5, 0xc0c00000u, 0xc1400000u},
                                       {4, 8, 4, 8, 0x40900000u, 0x41080000u},
                                       {8, 3, 2, -4, 0x40b00000u, 0x00000000u},
                                       {-4, -7, -5, -8, 0xc0800000u, 0xc0e00000u},
                                       {1000001, -1000000, 1000002, -999997, 0x49742420u, 0xc97423e0u}};
static cv_DIM_CASE cv_dimensions[] = {{0, 0, 0x00000000u, 0x00000000u},
                                      {1, 2, 0x3f000000u, 0x3f800000u},
                                      {2, 3, 0x3f800000u, 0x3fc00000u},
                                      {3, 1, 0x3fc00000u, 0x3f000000u},
                                      {640, 480, 0x43a00000u, 0x43700000u},
                                      {-5, -7, 0xc0200000u, 0xc0600000u},
                                      {9999997, 9999999, 0x4a98967du, 0x4a98967fu}};
static int test_canvas_init(void)
{
    int n, p, null_raster;
    unsigned int f;
    OP_RASTER *raster;
    OP_CANVAS *e;
    static unsigned int flags[] = {0, 1, 2, 3, 0xffffffffu, 0x80000000u};
    for (cv_row = 0; cv_row < 4; ++cv_row)
        for (n = 0; n < 6; ++n)
            for (p = 0; p < 7; ++p)
                for (null_raster = 0; null_raster < 2; ++null_raster)
                {
                    f = flags[n];
                    if (!(f & 1) && null_raster)
                        continue;
                    cv_seed();
                    memset(cv_rasters, 0x57, sizeof(cv_rasters));
                    cv_rasters[cv_row].value.width = cv_dimensions[p].w;
                    cv_rasters[cv_row].value.height = cv_dimensions[p].h;
                    memcpy(cv_raster_expected, cv_rasters, sizeof(cv_rasters));
                    raster = null_raster ? 0 : &cv_rasters[cv_row].value;
                    e = &cv_expected[cv_row].value;
                    e->flags = f;
                    e->raster = raster;
                    e->field10 = 0x12345678;
                    e->field14 = 0x87654321;
                    if (f & 1)
                    {
                        e->left = cv_rectangles[p].l;
                        e->top = cv_rectangles[p].t;
                        e->right = cv_rectangles[p].r;
                        e->bottom = cv_rectangles[p].b;
                        memcpy(&e->center_x, &cv_rectangles[p].cx, 4);
                        memcpy(&e->center_y, &cv_rectangles[p].cy, 4);
                    }
                    else
                    {
                        e->left = e->top = 0;
                        e->right = cv_dimensions[p].w - 1;
                        e->bottom = cv_dimensions[p].h - 1;
                        memcpy(&e->center_x, &cv_dimensions[p].cx, 4);
                        memcpy(&e->center_y, &cv_dimensions[p].cy, 4);
                    }
                    cv_CHECK(op_canvas_init(&cv_canvases[cv_row].value, f, raster, 0x12345678, cv_rectangles[p].l,
                                            cv_rectangles[p].t, cv_rectangles[p].r, cv_rectangles[p].b,
                                            0x87654321) == 1);
                    cv_verify();
                    cv_CHECK(memcmp(cv_rasters, cv_raster_expected, sizeof(cv_rasters)) == 0);
                }
    printf("canvas_init: %d checks, %d failures\n", cv_checks, cv_failures);
    return cv_failures != 0;
}

#undef cv_CHECK

#include "../src/canvas.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int cf_checks, cf_failures;
static void cf_check(int ok, int line)
{
    ++cf_checks;
    if (!ok)
    {
        ++cf_failures;
        printf("line%d failed\n", line);
    }
}
#define cf_CHECK(x) cf_check(!!(x), __LINE__)
typedef struct cf_OWNED_CANVAS
{
    unsigned int before;
    OP_CANVAS value;
    unsigned int after;
} cf_OWNED_CANVAS;
static cf_OWNED_CANVAS cf_canvases[4], cf_expected[4];
static int cf_row, cf_stage;
static void cf_seed(void)
{
    memset(cf_canvases, 0x69, sizeof(cf_canvases));
    memcpy(cf_expected, cf_canvases, sizeof(cf_canvases));
    cf_stage = 0;
}
static void cf_verify(void)
{
    cf_CHECK(memcmp(cf_canvases, cf_expected, sizeof(cf_canvases)) == 0);
}

void cf_free_inner(OP_CANVAS *canvas)
{
    if (cf_row < 0 || cf_row >= 4 || canvas != &cf_canvases[cf_row].value)
    {
        cf_CHECK(0);
        return;
    }
    cf_CHECK(cf_stage == 0);
    cf_verify();
    canvas->field10 = cf_expected[cf_row].value.field10 = 42;
    cf_stage = 1;
}
void cf_release(void *p)
{
    cf_CHECK(cf_row >= 0 && cf_stage == 1 && p == &cf_canvases[cf_row].value);
    cf_verify();
    cf_stage = 2;
}
static int test_canvas_free(void)
{
    for (cf_row = -1; cf_row < 4; ++cf_row)
    {
        cf_seed();
        op_canvas_free(cf_row < 0 ? 0 : &cf_canvases[cf_row].value);
        cf_CHECK(cf_stage == (cf_row < 0 ? 0 : 2));
        cf_verify();
    }
    printf("canvas_free: %d checks, %d failures\n", cf_checks, cf_failures);
    return cf_failures != 0;
}

#undef cf_CHECK

static int cn_checks, cn_failures, cn_stage, cn_allocate_ok, cn_releases;
static OP_CANVAS cn_after;
static void cn_check(int ok, int line)
{
    ++cn_checks;
    if (!ok)
    {
        ++cn_failures;
        printf("canvas chain line%d failed\n", line);
    }
}
#define CN_CHECK(x) cn_check(!!(x), __LINE__)
static void cn_verify(void)
{
    CN_CHECK(memcmp(cv_canvases, cv_expected, sizeof(cv_canvases)) == 0);
    CN_CHECK(memcmp(cv_rasters, cv_raster_expected, sizeof(cv_rasters)) == 0);
}
static void *cv_allocate(unsigned int bytes)
{
    CN_CHECK(cv_mode == 1 && cn_stage == 0 && bytes == 40);
    cn_verify();
    cn_stage = 1;
    if (cn_allocate_ok)
        cv_expected[cv_row].value = cn_after;
    return cn_allocate_ok ? &cv_canvases[cv_row].value : 0;
}
/* Authored observation backend. The original inner body is empty, but its four-byte tail is unaccepted. */
void op_canvas_free_inner(OP_CANVAS *canvas)
{
    if (cv_mode == 0)
    {
        cf_free_inner(canvas);
        return;
    }
    if (canvas != &cv_canvases[cv_row].value)
    {
        CN_CHECK(0);
        return;
    }
    CN_CHECK(cn_stage == 1);
    cn_verify();
    cn_stage = 2;
}
static void cv_release(void *p)
{
    if (cv_mode == 0)
    {
        cf_release(p);
        return;
    }
    CN_CHECK(cn_stage == 2 && p == &cv_canvases[cv_row].value);
    cn_verify();
    ++cn_releases;
    cn_stage = 3;
}
static int op_test_canvas(void)
{
    int n, p, null_raster;
    unsigned int f;
    OP_RASTER *raster;
    OP_CANVAS *result;
    static unsigned int flags[] = {0, 1, 2, 3, 0xffffffffu, 0x80000000u};
    cv_active = 1;
    test_canvas_init();
    cv_mode = 0;
    test_canvas_free();
    cv_mode = 1;
    for (cv_row = 0; cv_row < 4; ++cv_row)
        for (n = 0; n < 6; ++n)
            for (p = 0; p < 7; ++p)
                for (null_raster = 0; null_raster < 2; ++null_raster)
                    for (cn_allocate_ok = 0; cn_allocate_ok < 2; ++cn_allocate_ok)
                    {
                        f = flags[n];
                        if (!(f & 1) && null_raster)
                            continue;
                        cv_seed();
                        memset(cv_rasters, 0x57, sizeof(cv_rasters));
                        cv_rasters[cv_row].value.width = cv_dimensions[p].w;
                        cv_rasters[cv_row].value.height = cv_dimensions[p].h;
                        memcpy(cv_raster_expected, cv_rasters, sizeof(cv_rasters));
                        raster = null_raster ? 0 : &cv_rasters[cv_row].value;
                        cn_after = cv_expected[cv_row].value;
                        cn_after.flags = f;
                        cn_after.raster = raster;
                        cn_after.field10 = 0x12345678;
                        cn_after.field14 = 0x87654321;
                        if (f & 1)
                        {
                            cn_after.left = cv_rectangles[p].l;
                            cn_after.top = cv_rectangles[p].t;
                            cn_after.right = cv_rectangles[p].r;
                            cn_after.bottom = cv_rectangles[p].b;
                            memcpy(&cn_after.center_x, &cv_rectangles[p].cx, 4);
                            memcpy(&cn_after.center_y, &cv_rectangles[p].cy, 4);
                        }
                        else
                        {
                            cn_after.left = cn_after.top = 0;
                            cn_after.right = cv_dimensions[p].w - 1;
                            cn_after.bottom = cv_dimensions[p].h - 1;
                            memcpy(&cn_after.center_x, &cv_dimensions[p].cx, 4);
                            memcpy(&cn_after.center_y, &cv_dimensions[p].cy, 4);
                        }
                        cn_stage = 0;
                        cn_releases = 0;
                        result = op_canvas_new(f, raster, 0x12345678, cv_rectangles[p].l, cv_rectangles[p].t,
                                               cv_rectangles[p].r, cv_rectangles[p].b, 0x87654321);
                        CN_CHECK(result == (cn_allocate_ok ? &cv_canvases[cv_row].value : 0));
                        CN_CHECK(cn_stage == 1);
                        cn_verify();
                        op_canvas_free(result);
                        CN_CHECK(cn_stage == (cn_allocate_ok ? 3 : 1) && cn_releases == cn_allocate_ok);
                        cn_verify();
                    }
    cv_active = 0;
    printf("canvas lifecycle connected: %d checks, %d failures\n", cn_checks + cv_checks + cf_checks,
           cn_failures + cv_failures + cf_failures);
    return cn_failures + cv_failures + cf_failures != 0;
}
#include "../src/face_keyframe.h"
typedef struct FCTOR_FACE
{
    unsigned int before;
    OP_RD_FACE value;
    unsigned int after;
} FCTOR_FACE;
static FCTOR_FACE fctor_faces[4], fctor_expected[4];
static unsigned int fctor_tokens[2];
static int fctor_checks, fctor_failures, fctor_row, fctor_allocate_ok, fctor_stage, fctor_nr, fctor_plan_count;
static void *fctor_plan[3];
static void fctor_check(int ok, int line)
{
    ++fctor_checks;
    if (!ok)
    {
        ++fctor_failures;
        printf("face allocation line%d failed\n", line);
    }
}
#define FCTOR_CHECK(x) fctor_check(!!(x), __LINE__)
static void fctor_verify(void)
{
    FCTOR_CHECK(memcmp(fctor_faces, fctor_expected, sizeof(fctor_faces)) == 0);
}
static void fctor_predict_init(void)
{
    int i;
    unsigned int zero = 0, minusone = 0xffffffffu;
    static unsigned int offsets[] = {0, 4, 20, 24, 28, 32, 40, 44, 48, 56, 60, 64, 68, 72};
    for (i = 0; i < 14; ++i)
        memcpy((unsigned char *)&fctor_expected[fctor_row].value + offsets[i], &zero, 4);
    memcpy((unsigned char *)&fctor_expected[fctor_row].value + 36, &minusone, 4);
}
static void *fctor_allocate(unsigned int bytes)
{
    FCTOR_CHECK(fctor_stage == 0 && bytes == 76);
    fctor_verify();
    fctor_stage = 1;
    if (fctor_allocate_ok)
        fctor_predict_init();
    return fctor_allocate_ok ? &fctor_faces[fctor_row].value : 0;
}
static void fctor_release(void *p)
{
    if (fctor_nr >= fctor_plan_count || p != fctor_plan[fctor_nr])
    {
        FCTOR_CHECK(0);
        return;
    }
    FCTOR_CHECK(fctor_stage == 1);
    fctor_verify();
    ++fctor_nr;
}
static int op_test_face_constructor(void)
{
    int fill, mask;
    OP_RD_FACE *face;
    fctor_active = 1;
    for (fctor_row = 0; fctor_row < 4; ++fctor_row)
        for (fill = 0; fill < 3; ++fill)
            for (fctor_allocate_ok = 0; fctor_allocate_ok < 2; ++fctor_allocate_ok)
                for (mask = 0; mask < 4; ++mask)
                {
                    memset(fctor_faces, 0x69, sizeof(fctor_faces));
                    memset(&fctor_faces[fctor_row].value, fill == 0 ? 0 : (fill == 1 ? 0xff : 0x57), 76);
                    memcpy(fctor_expected, fctor_faces, sizeof(fctor_faces));
                    fctor_stage = 0;
                    fctor_nr = 0;
                    fctor_plan_count = 0;
                    face = op_face_new();
                    FCTOR_CHECK(face == (fctor_allocate_ok ? &fctor_faces[fctor_row].value : 0));
                    FCTOR_CHECK(fctor_stage == 1);
                    fctor_verify();
                    if (face)
                    {
                        if (mask & 1)
                        {
                            face->array18 = fctor_expected[fctor_row].value.array18 = &fctor_tokens[0];
                            fctor_plan[fctor_plan_count++] = &fctor_tokens[0];
                        }
                        if (mask & 2)
                        {
                            face->array1c = fctor_expected[fctor_row].value.array1c = &fctor_tokens[1];
                            fctor_plan[fctor_plan_count++] = &fctor_tokens[1];
                        }
                        fctor_plan[fctor_plan_count++] = face;
                    }
                    op_face_free(face);
                    FCTOR_CHECK(fctor_nr == fctor_plan_count);
                    fctor_verify();
                }
    fctor_active = 0;
    printf("face constructor connected: %d checks, %d failures\n", fctor_checks, fctor_failures);
    return fctor_failures != 0;
}
