#include "../src/colormap_io.h"
#include <stdio.h>
#include <string.h>
static int cmr_active, cmr_chain_mode, cmr_root_pending;
static void cm_use_legacy_services(void);
static int cm_legacy_gray(OP_COLORMAP *map);
static int cmr_checks, cmr_failures;
static void cmr_check(int ok, int line)
{
    ++cmr_checks;
    if (!ok)
    {
        ++cmr_failures;
        printf("line%d failed\n", line);
    }
}
#define CMR_CHECK(x) cmr_check(!!(x), __LINE__)
const char op_colormap_read_mode[] = "authored-read";
const char op_colormap_write_mode[] = "authored-write";
const char op_colormap_marker[] = "TEST";
typedef struct CMR_OWNED_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} CMR_OWNED_MAP;
static CMR_OWNED_MAP cmr_maps[3], cmr_expected_maps[3];
static int cmr_row, cmr_mutate, cmr_provider, cmr_position, cmr_plan_count;
static int cmr_plan[20];
static OP_FILE_HANDLE cmr_handle;
static unsigned int cmr_reported;
static OP_CMP_SERVICES cmr_services[2];
OP_CMP_SERVICES *op_colormap_services;
static char cmr_path[80], cmr_expected_path[80];
static void cmr_event(int wanted)
{
    if (cmr_position >= cmr_plan_count)
    {
        CMR_CHECK(0);
        return;
    }
    CMR_CHECK(cmr_plan[cmr_position] == wanted);
    ++cmr_position;
}
static void cmr_service(int which)
{
    CMR_CHECK(which == cmr_provider && op_colormap_services == &cmr_services[cmr_provider]);
    cmr_provider ^= 1;
    op_colormap_services = &cmr_services[cmr_provider];
}
static void cmr_verify_maps(void)
{
    CMR_CHECK(memcmp(cmr_maps, cmr_expected_maps, sizeof(cmr_maps)) == 0);
    CMR_CHECK(memcmp(cmr_path, cmr_expected_path, sizeof(cmr_path)) == 0);
}

enum
{
    CMR_OPEN = 1,
    CMR_BASENAME,
    CMR_COPY,
    CMR_HEADER,
    CMR_COMPARE,
    CMR_PALETTE,
    CMR_ALLOC_LIGHT,
    CMR_LIGHT,
    CMR_ALLOC_ALPHA,
    CMR_ALPHA,
    CMR_CLOSE,
    CMR_GRAY
};
static unsigned char cmr_light_pool[0x4300], cmr_expected_light[0x4300];
static unsigned char cmr_alpha_pool[0x10300], cmr_expected_alpha[0x10300];
static unsigned char *cmr_raw_light, *cmr_aligned_light, *cmr_raw_alpha, *cmr_aligned_alpha;
static int cmr_scenario, cmr_skew, cmr_name_case, cmr_header_good, cmr_alpha_enabled, cmr_closed;
static OP_CMP_HEADER cmr_input_header;
static char cmr_basename_value[48];
static const char *cmr_names[] = {"", "short", "abcdefghijklmnopqrstuvwxyzABCDE",
                                  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN"};
static const void *cmr_header_local;
static int cmr_outside(const void *p, const void *base, unsigned int bytes)
{
    unsigned int a = (unsigned int)p, b = (unsigned int)base;
    return a < b || a >= b + bytes;
}
static int cmr_valid_header_local(const void *p)
{
    return p && cmr_outside(p, cmr_maps, sizeof(cmr_maps)) &&
           cmr_outside(p, cmr_expected_maps, sizeof(cmr_expected_maps)) && cmr_outside(p, cmr_path, sizeof(cmr_path)) &&
           cmr_outside(p, cmr_expected_path, sizeof(cmr_expected_path)) &&
           cmr_outside(p, op_colormap_marker, sizeof(op_colormap_marker)) &&
           cmr_outside(p, cmr_light_pool, sizeof(cmr_light_pool)) &&
           cmr_outside(p, cmr_alpha_pool, sizeof(cmr_alpha_pool)) &&
           cmr_outside(p, cmr_expected_light, sizeof(cmr_expected_light)) &&
           cmr_outside(p, cmr_expected_alpha, sizeof(cmr_expected_alpha)) &&
           cmr_outside(p, &cmr_input_header, sizeof(cmr_input_header)) &&
           cmr_outside(p, cmr_basename_value, sizeof(cmr_basename_value));
}

static void cmr_verify(void)
{
    cmr_verify_maps();
    CMR_CHECK(memcmp(cmr_light_pool, cmr_expected_light, sizeof(cmr_light_pool)) == 0);
    CMR_CHECK(memcmp(cmr_alpha_pool, cmr_expected_alpha, sizeof(cmr_alpha_pool)) == 0);
}
static void cmr_predict_header(void)
{
    cmr_expected_maps[cmr_row].value.flags = cmr_input_header.flags;
    cmr_expected_maps[cmr_row].value.field24 = cmr_input_header.field24;
    cmr_expected_maps[cmr_row].value.field28 = cmr_input_header.field28;
    cmr_expected_maps[cmr_row].value.field2c = cmr_input_header.field2c;
}
static OP_FILE_HANDLE cmr_open_file(int which, const char *input, const char *mode)
{
    cmr_event(CMR_OPEN);
    cmr_service(which);
    CMR_CHECK(input == cmr_path && mode == op_colormap_read_mode);
    cmr_verify();
    if (cmr_mutate)
    {
        cmr_path[0] = cmr_expected_path[0] = 'Z';
        cmr_maps[cmr_row].value.tail[1] = cmr_expected_maps[cmr_row].value.tail[1] = 0x99;
    }
    return cmr_handle;
}
const char *op_resource_basename(const char *input)
{
    CMR_CHECK(cmr_active);
    if (!cmr_active)
        return "";
    cmr_event(CMR_BASENAME);
    CMR_CHECK(input == cmr_path);
    cmr_verify();
    CMR_CHECK(cmr_path[0] == (cmr_mutate ? 'Z' : 'a'));
    return cmr_basename_value;
}
static char *cmr_copy(char *out, const char *input, unsigned int count)
{
    unsigned int i;
    int ended = 0;
    cmr_event(CMR_COPY);
    CMR_CHECK(out == cmr_maps[cmr_row].value.name && input == cmr_basename_value && count == 31);
    cmr_verify();
    if (out != cmr_maps[cmr_row].value.name || count != 31)
        return out;
    for (i = 0; i < 31; ++i)
    {
        char value = 0;
        if (!ended)
            value = cmr_basename_value[i];
        if (!value)
            ended = 1;
        out[i] = value;
        cmr_expected_maps[cmr_row].value.name[i] = value;
    }
    out[31] = 'X';
    cmr_expected_maps[cmr_row].value.name[31] = 0;
    return out;
}
int op_compare_marker(const char *input, const char *marker, unsigned int count)
{
    CMR_CHECK(cmr_active);
    if (!cmr_active)
        return -1;
    cmr_event(CMR_COMPARE);
    CMR_CHECK(marker == op_colormap_marker && count == 4);
    cmr_verify();
    CMR_CHECK(input == cmr_header_local && cmr_header_local != 0);
    if (input != cmr_header_local || !cmr_header_local)
        return -1;
    CMR_CHECK(memcmp(input, &cmr_input_header, 64) == 0);
    return cmr_header_good ? 0 : -3;
}
static void cmr_fill_bytes(unsigned char *out, unsigned char *expected, unsigned int count, unsigned int seed)
{
    unsigned int i;
    for (i = 0; i < count; ++i)
    {
        out[i] = (unsigned char)(i * 11 + seed);
        expected[i] = (unsigned char)(i * 11 + seed);
    }
}
static unsigned int cmr_read_file(int which, OP_FILE_HANDLE file, void *data, unsigned int bytes)
{
    int wanted;
    CMR_CHECK(file == cmr_handle && file != 0);
    if (cmr_position >= cmr_plan_count)
    {
        CMR_CHECK(0);
        return 0;
    }
    wanted = cmr_plan[cmr_position];
    cmr_event(wanted);
    cmr_service(which);
    cmr_verify();
    if (wanted == CMR_HEADER)
    {
        CMR_CHECK(bytes == 64);
        if (bytes != 64 || !cmr_valid_header_local(data))
        {
            CMR_CHECK(0);
            return 0;
        }
        cmr_header_local = data;
        memcpy(data, &cmr_input_header, 64);
        cmr_predict_header();
    }
    else if (wanted == CMR_PALETTE)
    {
        CMR_CHECK(data == cmr_maps[cmr_row].value.palette && bytes == 768);
        if (data != cmr_maps[cmr_row].value.palette || bytes != 768)
            return 0;
        cmr_fill_bytes((unsigned char *)data, cmr_expected_maps[cmr_row].value.palette, 768, 19);
    }
    else if (wanted == CMR_LIGHT)
    {
        CMR_CHECK(data == cmr_aligned_light && bytes == 0x4000);
        if (data != cmr_aligned_light || bytes != 0x4000)
            return 0;
        cmr_fill_bytes((unsigned char *)data, cmr_expected_light + (cmr_aligned_light - cmr_light_pool), bytes, 37);
        if (cmr_mutate)
        {
            cmr_maps[cmr_row].value.flags = cmr_expected_maps[cmr_row].value.flags ^= 1;
            cmr_maps[cmr_row].value.tail[2] = cmr_expected_maps[cmr_row].value.tail[2] = 0x88;
        }
    }
    else if (wanted == CMR_ALPHA)
    {
        CMR_CHECK(data == cmr_aligned_alpha && bytes == 0x10000);
        if (data != cmr_aligned_alpha || bytes != 0x10000)
            return 0;
        cmr_fill_bytes((unsigned char *)data, cmr_expected_alpha + (cmr_aligned_alpha - cmr_alpha_pool), bytes, 53);
    }
    else
        CMR_CHECK(0);
    return cmr_reported;
}
static void *cmr_allocate(unsigned int bytes)
{
    int wanted;
    void *result;
    if (cmr_root_pending)
    {
        CMR_CHECK(cmr_chain_mode && bytes == 844 && cmr_position == 0);
        cmr_verify();
        cmr_root_pending = 0;
        memset(&cmr_expected_maps[cmr_row].value, 0, 844);
        return &cmr_maps[cmr_row].value;
    }
    CMR_CHECK(cmr_position < cmr_plan_count);
    if (cmr_position >= cmr_plan_count)
        return 0;
    wanted = cmr_plan[cmr_position];
    cmr_event(wanted);
    cmr_verify();
    if (wanted == CMR_ALLOC_LIGHT)
    {
        CMR_CHECK(bytes == 0x4100);
        result = cmr_scenario == 2 ? 0 : cmr_raw_light;
        if (cmr_mutate)
        {
            cmr_maps[cmr_row].value.handle334 = cmr_expected_maps[cmr_row].value.handle334 = cmr_alpha_pool;
            cmr_maps[cmr_row].value.tail[3] = cmr_expected_maps[cmr_row].value.tail[3] = 0x21;
        }
        cmr_expected_maps[cmr_row].value.handle334 = result;
        if (result)
            cmr_expected_maps[cmr_row].value.light_table = cmr_aligned_light;
    }
    else if (wanted == CMR_ALLOC_ALPHA)
    {
        CMR_CHECK(bytes == 0x10100);
        result = cmr_scenario == 3 ? 0 : cmr_raw_alpha;
        if (cmr_mutate)
        {
            cmr_maps[cmr_row].value.handle33c = cmr_expected_maps[cmr_row].value.handle33c = cmr_light_pool;
            cmr_maps[cmr_row].value.tail[4] = cmr_expected_maps[cmr_row].value.tail[4] = 0x32;
        }
        cmr_expected_maps[cmr_row].value.handle33c = result;
        if (result)
            cmr_expected_maps[cmr_row].value.alpha_table = cmr_aligned_alpha;
    }
    else
    {
        CMR_CHECK(0);
        result = 0;
    }
    return result;
}
static int cmr_close_file(int which, OP_FILE_HANDLE file)
{
    cmr_event(CMR_CLOSE);
    cmr_service(which);
    CMR_CHECK(file == cmr_handle && file != 0 && !cmr_closed);
    cmr_verify();
    ++cmr_closed;
    cmr_maps[cmr_row].value.tail[5] = cmr_expected_maps[cmr_row].value.tail[5] = 0x54;
    return -9;
}
static int cmr_gray(OP_COLORMAP *map)
{
    cmr_event(CMR_GRAY);
    CMR_CHECK(map == &cmr_maps[cmr_row].value && cmr_closed == (cmr_handle != 0));
    cmr_verify();
    if (map != &cmr_maps[cmr_row].value)
        return 0;
    map->tail[6] = cmr_expected_maps[cmr_row].value.tail[6] = 0x65;
    return cmr_mutate ? -8 : 0;
}
static OP_FILE_HANDLE cmr_open0(const char *p, const char *m)
{
    return cmr_open_file(0, p, m);
}
static OP_FILE_HANDLE cmr_open1(const char *p, const char *m)
{
    return cmr_open_file(1, p, m);
}
static int cmr_close0(OP_FILE_HANDLE f)
{
    return cmr_close_file(0, f);
}
static int cmr_close1(OP_FILE_HANDLE f)
{
    return cmr_close_file(1, f);
}
static unsigned int cmr_read0(OP_FILE_HANDLE f, void *p, unsigned int n)
{
    return cmr_read_file(0, f, p, n);
}
static unsigned int cmr_read1(OP_FILE_HANDLE f, void *p, unsigned int n)
{
    return cmr_read_file(1, f, p, n);
}

static void *cmr_release_plan[3];
static int cmr_release_kind[3], cmr_release_count, cmr_release_position;
static void cmr_release(void *p)
{
    int kind;
    if (!cmr_chain_mode || cmr_release_position >= cmr_release_count || p != cmr_release_plan[cmr_release_position])
    {
        CMR_CHECK(0);
        return;
    }
    cmr_verify();
    kind = cmr_release_kind[cmr_release_position++];
    if (kind == 0)
        cmr_expected_maps[cmr_row].value.handle334 = 0;
    else if (kind == 1)
        cmr_expected_maps[cmr_row].value.handle33c = 0;
    else
        CMR_CHECK(p == &cmr_maps[cmr_row].value);
}
static void cmr_free_loaded(OP_COLORMAP *map)
{
    cmr_release_count = cmr_release_position = 0;
    if (cmr_expected_maps[cmr_row].value.handle334)
    {
        cmr_release_plan[cmr_release_count] = cmr_expected_maps[cmr_row].value.handle334;
        cmr_release_kind[cmr_release_count++] = 0;
    }
    if ((cmr_expected_maps[cmr_row].value.flags & 1) && cmr_expected_maps[cmr_row].value.handle33c)
    {
        cmr_release_plan[cmr_release_count] = cmr_expected_maps[cmr_row].value.handle33c;
        cmr_release_kind[cmr_release_count++] = 1;
    }
    cmr_release_plan[cmr_release_count] = map;
    cmr_release_kind[cmr_release_count++] = 2;
    op_rd_colormap_free(map);
    CMR_CHECK(cmr_release_position == cmr_release_count);
    cmr_verify();
}

static int op_test_colormap_reader(void)
{
    int s, flag, r, h, i, fallback;
    unsigned int light_base, alpha_base;
    OP_COLORMAP *loaded;
    static const int residues[] = {0, 1, 255};
    static const unsigned int reports[] = {0, 1, 0xffffffffu};
    cmr_active = 1;
    memset(cmr_services, 0x43, sizeof(cmr_services));
    cmr_services[0].open = cmr_open0;
    cmr_services[1].open = cmr_open1;
    cmr_services[0].close = cmr_close0;
    cmr_services[1].close = cmr_close1;
    cmr_services[0].read = cmr_read0;
    cmr_services[1].read = cmr_read1;
    light_base = ((unsigned int)(cmr_light_pool + 255)) & ~255u;
    alpha_base = ((unsigned int)(cmr_alpha_pool + 255)) & ~255u;
    for (cmr_chain_mode = 0; cmr_chain_mode < 2; ++cmr_chain_mode)
        for (cmr_row = 0; cmr_row < 3; ++cmr_row)
            for (cmr_scenario = 0; cmr_scenario < 5; ++cmr_scenario)
                for (s = 0; s < 3; ++s)
                    for (flag = 0; flag < 2; ++flag)
                        for (cmr_mutate = 0; cmr_mutate < 2; ++cmr_mutate)
                        {
                            cmr_skew = residues[s];
                            cmr_name_case = (cmr_row + cmr_scenario + s + flag + cmr_mutate) % 4;
                            r = (cmr_row + cmr_scenario + s) % 3;
                            h = (cmr_scenario + cmr_row) % 2;
                            cmr_raw_light = (unsigned char *)(light_base + (unsigned int)cmr_skew);
                            cmr_raw_alpha = (unsigned char *)(alpha_base + (unsigned int)cmr_skew);
                            cmr_aligned_light = cmr_raw_light + (cmr_skew ? 256 - cmr_skew : 0);
                            cmr_aligned_alpha = cmr_raw_alpha + (cmr_skew ? 256 - cmr_skew : 0);
                            CMR_CHECK(cmr_raw_light >= cmr_light_pool &&
                                      cmr_raw_light + 0x4100 <= cmr_light_pool + sizeof(cmr_light_pool));
                            CMR_CHECK(cmr_raw_alpha >= cmr_alpha_pool &&
                                      cmr_raw_alpha + 0x10100 <= cmr_alpha_pool + sizeof(cmr_alpha_pool));
                            CMR_CHECK(((unsigned int)cmr_raw_light & 255) == (unsigned int)cmr_skew &&
                                      ((unsigned int)cmr_raw_alpha & 255) == (unsigned int)cmr_skew);
                            memset(cmr_maps, 0x57, sizeof(cmr_maps));
                            cmr_maps[cmr_row].value.light_table = cmr_light_pool;
                            cmr_maps[cmr_row].value.handle334 = cmr_light_pool + 1;
                            cmr_maps[cmr_row].value.alpha_table = cmr_alpha_pool;
                            cmr_maps[cmr_row].value.handle33c = cmr_alpha_pool + 1;
                            memcpy(cmr_expected_maps, cmr_maps, sizeof(cmr_maps));
                            memset(cmr_light_pool, 0xa7, sizeof(cmr_light_pool));
                            memset(cmr_alpha_pool, 0xb6, sizeof(cmr_alpha_pool));
                            memcpy(cmr_expected_light, cmr_light_pool, sizeof(cmr_light_pool));
                            memcpy(cmr_expected_alpha, cmr_alpha_pool, sizeof(cmr_alpha_pool));
                            memset(cmr_path, 0, sizeof(cmr_path));
                            memcpy(cmr_path, "authored/input.name", 20);
                            memcpy(cmr_expected_path, cmr_path, sizeof(cmr_path));
                            memset(cmr_basename_value, 0, sizeof(cmr_basename_value));
                            for (i = 0; cmr_names[cmr_name_case][i]; ++i)
                                cmr_basename_value[i] = cmr_names[cmr_name_case][i];
                            memset(&cmr_input_header, 0x35, sizeof(cmr_input_header));
                            memcpy(cmr_input_header.marker, "TEST", 4);
                            cmr_input_header.version = (unsigned int)(cmr_scenario * 71);
                            cmr_input_header.flags = 0x80000200u | (unsigned int)flag;
                            cmr_input_header.field24 = 0x12345678;
                            cmr_input_header.field28 = 0xff800000u;
                            cmr_input_header.field2c = 0x80000000u;
                            cmr_handle = cmr_scenario == 0 ? 0 : (h ? 0x80000001u : 7);
                            cmr_reported = reports[r];
                            cmr_header_good = cmr_scenario != 1;
                            cmr_alpha_enabled = flag ^ cmr_mutate;
                            cmr_closed = 0;
                            cmr_plan_count = 0;
                            cmr_plan[cmr_plan_count++] = CMR_OPEN;
                            fallback = 0;
                            if (!cmr_handle)
                                fallback = 1;
                            else
                            {
                                cmr_plan[cmr_plan_count++] = CMR_BASENAME;
                                cmr_plan[cmr_plan_count++] = CMR_COPY;
                                cmr_plan[cmr_plan_count++] = CMR_HEADER;
                                cmr_plan[cmr_plan_count++] = CMR_COMPARE;
                                if (!cmr_header_good)
                                    fallback = 1;
                                else
                                {
                                    cmr_plan[cmr_plan_count++] = CMR_PALETTE;
                                    cmr_plan[cmr_plan_count++] = CMR_ALLOC_LIGHT;
                                    if (cmr_scenario == 2)
                                        fallback = 1;
                                    else
                                    {
                                        cmr_plan[cmr_plan_count++] = CMR_LIGHT;
                                        if (cmr_alpha_enabled)
                                        {
                                            cmr_plan[cmr_plan_count++] = CMR_ALLOC_ALPHA;
                                            if (cmr_scenario == 3)
                                                fallback = 1;
                                            else
                                                cmr_plan[cmr_plan_count++] = CMR_ALPHA;
                                        }
                                    }
                                }
                                cmr_plan[cmr_plan_count++] = CMR_CLOSE;
                            }
                            if (fallback)
                                cmr_plan[cmr_plan_count++] = CMR_GRAY;
                            cmr_header_local = 0;
                            cmr_position = cmr_provider = 0;
                            op_colormap_services = &cmr_services[0];
                            cmr_root_pending = cmr_chain_mode;
                            if (cmr_chain_mode)
                            {
                                loaded = op_rd_colormap_load(cmr_path);
                                CMR_CHECK(loaded == &cmr_maps[cmr_row].value && !cmr_root_pending);
                            }
                            else
                            {
                                loaded = 0;
                                CMR_CHECK(op_colormap_load_entry(cmr_path, &cmr_maps[cmr_row].value) == 1);
                            }
                            CMR_CHECK(cmr_position == cmr_plan_count && cmr_closed == (cmr_handle != 0));
                            cmr_verify();
                            if (cmr_chain_mode && loaded == &cmr_maps[cmr_row].value)
                                cmr_free_loaded(loaded);
                        }
    cmr_active = 0;
    printf("colormap_load_entry: %d checks, %d failures\n", cmr_checks, cmr_failures);
    return cmr_failures != 0;
}

int op_colormap_build_gray(OP_COLORMAP *map)
{
    if (cmr_active)
        return cmr_gray(map);
    return cm_legacy_gray(map);
}
#undef CMR_CHECK

#include "../src/colormap_io.h"
#include <stdio.h>
#include <string.h>
static int cmw_active;
static int cmw_checks, cmw_failures;
static void cmw_check(int ok, int line)
{
    ++cmw_checks;
    if (!ok)
    {
        ++cmw_failures;
        printf("line%d failed\n", line);
    }
}
#define CMW_CHECK(x) cmw_check(!!(x), __LINE__)
typedef struct CMW_OWNED_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} CMW_OWNED_MAP;
static CMW_OWNED_MAP cmw_maps[3], cmw_expected_maps[3];
static int cmw_row, cmw_mutate, cmw_provider, cmw_position, cmw_plan_count;
static int cmw_plan[20];
static OP_FILE_HANDLE cmw_handle;
static unsigned int cmw_reported;
static OP_CMP_SERVICES cmw_services[2];
static char cmw_path[80], cmw_expected_path[80];
static void cmw_event(int wanted)
{
    if (cmw_position >= cmw_plan_count)
    {
        CMW_CHECK(0);
        return;
    }
    CMW_CHECK(cmw_plan[cmw_position] == wanted);
    ++cmw_position;
}
static void cmw_service(int which)
{
    CMW_CHECK(which == cmw_provider && op_colormap_services == &cmw_services[cmw_provider]);
    cmw_provider ^= 1;
    op_colormap_services = &cmw_services[cmw_provider];
}
static void cmw_verify_maps(void)
{
    CMW_CHECK(memcmp(cmw_maps, cmw_expected_maps, sizeof(cmw_maps)) == 0);
    CMW_CHECK(memcmp(cmw_path, cmw_expected_path, sizeof(cmw_path)) == 0);
}

enum
{
    CMW_COPY = 1,
    CMW_OPEN,
    CMW_HEADER,
    CMW_PALETTE,
    CMW_LIGHT,
    CMW_ALPHA,
    CMW_CLOSE
};
static unsigned char cmw_lights[2][16384], cmw_expected_lights[2][16384];
static unsigned char cmw_alphas[2][65536], cmw_expected_alphas[2][65536];
static OP_CMP_HEADER cmw_expected_header;
static const void *cmw_header_local;
static int cmw_outside(const void *p, const void *base, unsigned int bytes)
{
    unsigned int a = (unsigned int)p, b = (unsigned int)base;
    return a < b || a >= b + bytes;
}
static int cmw_valid_header_local(const void *p)
{
    return p && cmw_outside(p, cmw_maps, sizeof(cmw_maps)) &&
           cmw_outside(p, cmw_expected_maps, sizeof(cmw_expected_maps)) && cmw_outside(p, cmw_path, sizeof(cmw_path)) &&
           cmw_outside(p, cmw_expected_path, sizeof(cmw_expected_path)) &&
           cmw_outside(p, op_colormap_marker, sizeof(op_colormap_marker)) &&
           cmw_outside(p, cmw_lights, sizeof(cmw_lights)) && cmw_outside(p, cmw_alphas, sizeof(cmw_alphas)) &&
           cmw_outside(p, cmw_expected_lights, sizeof(cmw_expected_lights)) &&
           cmw_outside(p, cmw_expected_alphas, sizeof(cmw_expected_alphas)) &&
           cmw_outside(p, &cmw_expected_header, sizeof(cmw_expected_header));
}

static void cmw_verify(void)
{
    cmw_verify_maps();
    CMW_CHECK(memcmp(cmw_lights, cmw_expected_lights, sizeof(cmw_lights)) == 0);
    CMW_CHECK(memcmp(cmw_alphas, cmw_expected_alphas, sizeof(cmw_alphas)) == 0);
}
static char *cmw_copy(char *out, const char *input, unsigned int count)
{
    cmw_event(CMW_COPY);
    CMW_CHECK(input == op_colormap_marker && count == 4);
    cmw_verify();
    if (count != 4 || !cmw_valid_header_local(out))
    {
        CMW_CHECK(0);
        return out;
    }
    cmw_header_local = out;
    memcpy(out, "TEST", 4);
    return out;
}
static OP_FILE_HANDLE cmw_open_file(int which, const char *input, const char *mode)
{
    cmw_event(CMW_OPEN);
    cmw_service(which);
    CMW_CHECK(input == cmw_path && mode == op_colormap_write_mode);
    cmw_verify();
    if (cmw_mutate)
    {
        cmw_maps[cmw_row].value.flags = cmw_expected_maps[cmw_row].value.flags ^= 0x200;
        cmw_maps[cmw_row].value.field24 = cmw_expected_maps[cmw_row].value.field24 = 0x11223344;
        cmw_maps[cmw_row].value.field28 = cmw_expected_maps[cmw_row].value.field28 = 0x55667788;
        cmw_maps[cmw_row].value.field2c = cmw_expected_maps[cmw_row].value.field2c = 0x99aabbcc;
        cmw_maps[cmw_row].value.palette[0] = cmw_expected_maps[cmw_row].value.palette[0] = 0x19;
        cmw_maps[cmw_row].value.light_table = cmw_expected_maps[cmw_row].value.light_table = cmw_lights[1];
    }
    return cmw_handle;
}
static unsigned int cmw_write_file(int which, OP_FILE_HANDLE file, const void *data, unsigned int bytes)
{
    int wanted;
    CMW_CHECK(file == cmw_handle && file != 0);
    if (cmw_position >= cmw_plan_count)
    {
        CMW_CHECK(0);
        return 0;
    }
    wanted = cmw_plan[cmw_position];
    cmw_event(wanted);
    cmw_service(which);
    cmw_verify();
    if (wanted == CMW_HEADER)
    {
        CMW_CHECK(bytes == 64);
        CMW_CHECK(data == cmw_header_local && cmw_header_local != 0);
        if (bytes != 64 || data != cmw_header_local || !cmw_header_local)
            return 0;
        CMW_CHECK(memcmp(data, &cmw_expected_header, 64) == 0);
        if (cmw_mutate)
            cmw_maps[cmw_row].value.palette[1] = cmw_expected_maps[cmw_row].value.palette[1] = 0x3e;
    }
    else if (wanted == CMW_PALETTE)
    {
        CMW_CHECK(bytes == 768 && data == cmw_maps[cmw_row].value.palette);
        if (data != cmw_maps[cmw_row].value.palette || bytes != 768)
            return 0;
        CMW_CHECK(memcmp(data, cmw_expected_maps[cmw_row].value.palette, 768) == 0);
        if (cmw_mutate)
            cmw_maps[cmw_row].value.light_table = cmw_expected_maps[cmw_row].value.light_table = cmw_lights[0];
    }
    else if (wanted == CMW_LIGHT)
    {
        CMW_CHECK(bytes == 16384 && data == cmw_expected_maps[cmw_row].value.light_table);
        if (bytes != 16384 || data != cmw_expected_maps[cmw_row].value.light_table)
            return 0;
        CMW_CHECK(memcmp(data, data == cmw_lights[0] ? cmw_expected_lights[0] : cmw_expected_lights[1], 16384) == 0);
        if (cmw_mutate)
        {
            cmw_maps[cmw_row].value.flags = cmw_expected_maps[cmw_row].value.flags ^= 1;
            cmw_maps[cmw_row].value.alpha_table = cmw_expected_maps[cmw_row].value.alpha_table = cmw_alphas[1];
        }
    }
    else if (wanted == CMW_ALPHA)
    {
        CMW_CHECK(bytes == 65536 && data == cmw_expected_maps[cmw_row].value.alpha_table);
        if (bytes != 65536 || data != cmw_expected_maps[cmw_row].value.alpha_table)
            return 0;
        CMW_CHECK(memcmp(data, data == cmw_alphas[0] ? cmw_expected_alphas[0] : cmw_expected_alphas[1], 65536) == 0);
    }
    else
        CMW_CHECK(0);
    return cmw_reported;
}
static int cmw_close_file(int which, OP_FILE_HANDLE file)
{
    cmw_event(CMW_CLOSE);
    cmw_service(which);
    CMW_CHECK(file == cmw_handle && file != 0);
    cmw_verify();
    cmw_maps[cmw_row].value.tail[0] = cmw_expected_maps[cmw_row].value.tail[0] = 0x7d;
    return -7;
}
static OP_FILE_HANDLE cmw_open0(const char *p, const char *m)
{
    return cmw_open_file(0, p, m);
}
static OP_FILE_HANDLE cmw_open1(const char *p, const char *m)
{
    return cmw_open_file(1, p, m);
}
static int cmw_close0(OP_FILE_HANDLE f)
{
    return cmw_close_file(0, f);
}
static int cmw_close1(OP_FILE_HANDLE f)
{
    return cmw_close_file(1, f);
}
static unsigned int cmw_write0(OP_FILE_HANDLE f, const void *p, unsigned int n)
{
    return cmw_write_file(0, f, p, n);
}
static unsigned int cmw_write1(OP_FILE_HANDLE f, const void *p, unsigned int n)
{
    return cmw_write_file(1, f, p, n);
}
static int op_test_colormap_writer(void)
{
    unsigned int flags;
    int h, r, i, j, result;
    static const unsigned int handles[] = {0, 7, 0x80000001u};
    static const unsigned int reports[] = {0, 1, 0xffffffffu};
    cmw_active = 1;
    memset(cmw_services, 0x43, sizeof(cmw_services));
    cmw_services[0].open = cmw_open0;
    cmw_services[1].open = cmw_open1;
    cmw_services[0].close = cmw_close0;
    cmw_services[1].close = cmw_close1;
    cmw_services[0].write = cmw_write0;
    cmw_services[1].write = cmw_write1;
    for (cmw_row = 0; cmw_row < 3; ++cmw_row)
        for (flags = 0; flags < 2; ++flags)
            for (cmw_mutate = 0; cmw_mutate < 2; ++cmw_mutate)
                for (h = 0; h < 3; ++h)
                    for (r = 0; r < 3; ++r)
                    {
                        memset(cmw_maps, 0x57, sizeof(cmw_maps));
                        memset(cmw_path, 0, sizeof(cmw_path));
                        memcpy(cmw_path, "authored/output.name", 21);
                        memcpy(cmw_expected_path, cmw_path, sizeof(cmw_path));
                        for (i = 0; i < 2; ++i)
                        {
                            for (j = 0; j < 16384; ++j)
                                cmw_lights[i][j] = (unsigned char)(j * 13 + i);
                            for (j = 0; j < 65536; ++j)
                                cmw_alphas[i][j] = (unsigned char)(j * 7 + i);
                        }
                        memcpy(cmw_expected_lights, cmw_lights, sizeof(cmw_lights));
                        memcpy(cmw_expected_alphas, cmw_alphas, sizeof(cmw_alphas));
                        cmw_maps[cmw_row].value.flags = flags | 0x80000000u;
                        cmw_maps[cmw_row].value.field24 = 0x01020304;
                        cmw_maps[cmw_row].value.field28 = 0xff800000u;
                        cmw_maps[cmw_row].value.field2c = 0x80000000u;
                        cmw_maps[cmw_row].value.light_table = cmw_lights[0];
                        cmw_maps[cmw_row].value.alpha_table = cmw_alphas[0];
                        memcpy(cmw_expected_maps, cmw_maps, sizeof(cmw_maps));
                        memset(&cmw_expected_header, 0, sizeof(cmw_expected_header));
                        memcpy(cmw_expected_header.marker, "TEST", 4);
                        cmw_expected_header.version = 30;
                        cmw_expected_header.flags = cmw_maps[cmw_row].value.flags;
                        cmw_expected_header.field24 = cmw_maps[cmw_row].value.field24;
                        cmw_expected_header.field28 = cmw_maps[cmw_row].value.field28;
                        cmw_expected_header.field2c = cmw_maps[cmw_row].value.field2c;
                        cmw_handle = handles[h];
                        cmw_reported = reports[r];
                        cmw_plan_count = 0;
                        cmw_plan[cmw_plan_count++] = CMW_COPY;
                        cmw_plan[cmw_plan_count++] = CMW_OPEN;
                        if (cmw_handle)
                        {
                            cmw_plan[cmw_plan_count++] = CMW_HEADER;
                            cmw_plan[cmw_plan_count++] = CMW_PALETTE;
                            cmw_plan[cmw_plan_count++] = CMW_LIGHT;
                            if (flags ^ (unsigned int)cmw_mutate)
                                cmw_plan[cmw_plan_count++] = CMW_ALPHA;
                            cmw_plan[cmw_plan_count++] = CMW_CLOSE;
                        }
                        cmw_header_local = 0;
                        cmw_position = cmw_provider = 0;
                        op_colormap_services = &cmw_services[0];
                        result = op_colormap_write(cmw_path, &cmw_maps[cmw_row].value);
                        CMW_CHECK(result == (cmw_handle != 0));
                        CMW_CHECK(cmw_position == cmw_plan_count);
                        cmw_verify();
                    }
    cmw_active = 0;
    printf("colormap_write: %d checks, %d failures\n", cmw_checks, cmw_failures);
    return cmw_failures != 0;
}

#undef CMW_CHECK
