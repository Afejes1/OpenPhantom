#include "api.h"
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
const char op_colormap_read_mode[] = "authored-read";
const char op_colormap_write_mode[] = "authored-write";
const char op_colormap_marker[] = "TEST";
typedef struct OWNED_MAP
{
    unsigned int before;
    OP_COLORMAP value;
    unsigned int after;
} OWNED_MAP;
static OWNED_MAP maps[3], expected_maps[3];
static int row, mutate, provider, position, plan_count;
static int plan[20];
static OP_FILE_HANDLE handle;
static unsigned int reported;
static OP_CMP_SERVICES services[2];
OP_CMP_SERVICES *op_colormap_services;
static char path[80], expected_path[80];
static void event(int wanted)
{
    if (position >= plan_count)
    {
        CHECK(0);
        return;
    }
    CHECK(plan[position] == wanted);
    ++position;
}
static void service(int which)
{
    CHECK(which == provider && op_colormap_services == &services[provider]);
    provider ^= 1;
    op_colormap_services = &services[provider];
}
static void verify_maps(void)
{
    CHECK(memcmp(maps, expected_maps, sizeof(maps)) == 0);
    CHECK(memcmp(path, expected_path, sizeof(path)) == 0);
}

enum
{
    OPEN = 1,
    BASENAME,
    COPY,
    HEADER,
    COMPARE,
    PALETTE,
    ALLOC_LIGHT,
    LIGHT,
    ALLOC_ALPHA,
    ALPHA,
    CLOSE,
    GRAY
};
static unsigned char light_pool[0x4300], expected_light[0x4300];
static unsigned char alpha_pool[0x10300], expected_alpha[0x10300];
static unsigned char *raw_light, *aligned_light, *raw_alpha, *aligned_alpha;
static int scenario, skew, name_case, header_good, alpha_enabled, closed;
static OP_CMP_HEADER input_header;
static char basename_value[48];
static const char *names[] = {"", "short", "abcdefghijklmnopqrstuvwxyzABCDE",
                              "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMN"};
static const void *header_local;
static int outside(const void *p, const void *base, unsigned int bytes)
{
    unsigned int a = (unsigned int)p, b = (unsigned int)base;
    return a < b || a >= b + bytes;
}
static int valid_header_local(const void *p)
{
    return p && outside(p, maps, sizeof(maps)) && outside(p, expected_maps, sizeof(expected_maps)) &&
           outside(p, path, sizeof(path)) && outside(p, expected_path, sizeof(expected_path)) &&
           outside(p, op_colormap_marker, sizeof(op_colormap_marker)) && outside(p, light_pool, sizeof(light_pool)) &&
           outside(p, alpha_pool, sizeof(alpha_pool)) && outside(p, expected_light, sizeof(expected_light)) &&
           outside(p, expected_alpha, sizeof(expected_alpha)) && outside(p, &input_header, sizeof(input_header)) &&
           outside(p, basename_value, sizeof(basename_value));
}

static void verify(void)
{
    verify_maps();
    CHECK(memcmp(light_pool, expected_light, sizeof(light_pool)) == 0);
    CHECK(memcmp(alpha_pool, expected_alpha, sizeof(alpha_pool)) == 0);
}
static void predict_header(void)
{
    expected_maps[row].value.flags = input_header.flags;
    expected_maps[row].value.field24 = input_header.field24;
    expected_maps[row].value.field28 = input_header.field28;
    expected_maps[row].value.field2c = input_header.field2c;
}
static OP_FILE_HANDLE open_file(int which, const char *input, const char *mode)
{
    event(OPEN);
    service(which);
    CHECK(input == path && mode == op_colormap_read_mode);
    verify();
    if (mutate)
    {
        path[0] = expected_path[0] = 'Z';
        maps[row].value.tail[1] = expected_maps[row].value.tail[1] = 0x99;
    }
    return handle;
}
const char *op_resource_basename(const char *input)
{
    event(BASENAME);
    CHECK(input == path);
    verify();
    CHECK(path[0] == (mutate ? 'Z' : 'a'));
    return basename_value;
}
char *op_copy_keyframe_name(char *out, const char *input, unsigned int count)
{
    unsigned int i;
    int ended = 0;
    event(COPY);
    CHECK(out == maps[row].value.name && input == basename_value && count == 31);
    verify();
    if (out != maps[row].value.name || count != 31)
        return out;
    for (i = 0; i < 31; ++i)
    {
        char value = 0;
        if (!ended)
            value = basename_value[i];
        if (!value)
            ended = 1;
        out[i] = value;
        expected_maps[row].value.name[i] = value;
    }
    out[31] = 'X';
    expected_maps[row].value.name[31] = 0;
    return out;
}
int op_compare_marker(const char *input, const char *marker, unsigned int count)
{
    event(COMPARE);
    CHECK(marker == op_colormap_marker && count == 4);
    verify();
    CHECK(input == header_local && header_local != 0);
    if (input != header_local || !header_local)
        return -1;
    CHECK(memcmp(input, &input_header, 64) == 0);
    return header_good ? 0 : -3;
}
static void fill_bytes(unsigned char *out, unsigned char *expected, unsigned int count, unsigned int seed)
{
    unsigned int i;
    for (i = 0; i < count; ++i)
    {
        out[i] = (unsigned char)(i * 11 + seed);
        expected[i] = (unsigned char)(i * 11 + seed);
    }
}
static unsigned int read_file(int which, OP_FILE_HANDLE file, void *data, unsigned int bytes)
{
    int wanted;
    CHECK(file == handle && file != 0);
    if (position >= plan_count)
    {
        CHECK(0);
        return 0;
    }
    wanted = plan[position];
    event(wanted);
    service(which);
    verify();
    if (wanted == HEADER)
    {
        CHECK(bytes == 64);
        if (bytes != 64 || !valid_header_local(data))
        {
            CHECK(0);
            return 0;
        }
        header_local = data;
        memcpy(data, &input_header, 64);
        predict_header();
    }
    else if (wanted == PALETTE)
    {
        CHECK(data == maps[row].value.palette && bytes == 768);
        if (data != maps[row].value.palette || bytes != 768)
            return 0;
        fill_bytes((unsigned char *)data, expected_maps[row].value.palette, 768, 19);
    }
    else if (wanted == LIGHT)
    {
        CHECK(data == aligned_light && bytes == 0x4000);
        if (data != aligned_light || bytes != 0x4000)
            return 0;
        fill_bytes((unsigned char *)data, expected_light + (aligned_light - light_pool), bytes, 37);
        if (mutate)
        {
            maps[row].value.flags = expected_maps[row].value.flags ^= 1;
            maps[row].value.tail[2] = expected_maps[row].value.tail[2] = 0x88;
        }
    }
    else if (wanted == ALPHA)
    {
        CHECK(data == aligned_alpha && bytes == 0x10000);
        if (data != aligned_alpha || bytes != 0x10000)
            return 0;
        fill_bytes((unsigned char *)data, expected_alpha + (aligned_alpha - alpha_pool), bytes, 53);
    }
    else
        CHECK(0);
    return reported;
}
void *op_allocate(unsigned int bytes)
{
    int wanted;
    void *result;
    CHECK(position < plan_count);
    if (position >= plan_count)
        return 0;
    wanted = plan[position];
    event(wanted);
    verify();
    if (wanted == ALLOC_LIGHT)
    {
        CHECK(bytes == 0x4100);
        result = scenario == 2 ? 0 : raw_light;
        if (mutate)
        {
            maps[row].value.handle334 = expected_maps[row].value.handle334 = alpha_pool;
            maps[row].value.tail[3] = expected_maps[row].value.tail[3] = 0x21;
        }
        expected_maps[row].value.handle334 = result;
        if (result)
            expected_maps[row].value.light_table = aligned_light;
    }
    else if (wanted == ALLOC_ALPHA)
    {
        CHECK(bytes == 0x10100);
        result = scenario == 3 ? 0 : raw_alpha;
        if (mutate)
        {
            maps[row].value.handle33c = expected_maps[row].value.handle33c = light_pool;
            maps[row].value.tail[4] = expected_maps[row].value.tail[4] = 0x32;
        }
        expected_maps[row].value.handle33c = result;
        if (result)
            expected_maps[row].value.alpha_table = aligned_alpha;
    }
    else
    {
        CHECK(0);
        result = 0;
    }
    return result;
}
static int close_file(int which, OP_FILE_HANDLE file)
{
    event(CLOSE);
    service(which);
    CHECK(file == handle && file != 0 && !closed);
    verify();
    ++closed;
    maps[row].value.tail[5] = expected_maps[row].value.tail[5] = 0x54;
    return -9;
}
int op_colormap_build_gray(OP_COLORMAP *map)
{
    event(GRAY);
    CHECK(map == &maps[row].value && closed == (handle != 0));
    verify();
    if (map != &maps[row].value)
        return 0;
    map->tail[6] = expected_maps[row].value.tail[6] = 0x65;
    return mutate ? -8 : 0;
}
static OP_FILE_HANDLE open0(const char *p, const char *m)
{
    return open_file(0, p, m);
}
static OP_FILE_HANDLE open1(const char *p, const char *m)
{
    return open_file(1, p, m);
}
static int close0(OP_FILE_HANDLE f)
{
    return close_file(0, f);
}
static int close1(OP_FILE_HANDLE f)
{
    return close_file(1, f);
}
static unsigned int read0(OP_FILE_HANDLE f, void *p, unsigned int n)
{
    return read_file(0, f, p, n);
}
static unsigned int read1(OP_FILE_HANDLE f, void *p, unsigned int n)
{
    return read_file(1, f, p, n);
}
int main(void)
{
    int s, flag, r, h, i, fallback;
    unsigned int light_base, alpha_base;
    static const int residues[] = {0, 1, 255};
    static const unsigned int reports[] = {0, 1, 0xffffffffu};
    memset(services, 0x43, sizeof(services));
    services[0].open = open0;
    services[1].open = open1;
    services[0].close = close0;
    services[1].close = close1;
    services[0].read = read0;
    services[1].read = read1;
    light_base = ((unsigned int)(light_pool + 255)) & ~255u;
    alpha_base = ((unsigned int)(alpha_pool + 255)) & ~255u;
    for (row = 0; row < 3; ++row)
        for (scenario = 0; scenario < 5; ++scenario)
            for (s = 0; s < 3; ++s)
                for (flag = 0; flag < 2; ++flag)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        skew = residues[s];
                        name_case = (row + scenario + s + flag + mutate) % 4;
                        r = (row + scenario + s) % 3;
                        h = (scenario + row) % 2;
                        raw_light = (unsigned char *)(light_base + (unsigned int)skew);
                        raw_alpha = (unsigned char *)(alpha_base + (unsigned int)skew);
                        aligned_light = raw_light + (skew ? 256 - skew : 0);
                        aligned_alpha = raw_alpha + (skew ? 256 - skew : 0);
                        CHECK(raw_light >= light_pool && raw_light + 0x4100 <= light_pool + sizeof(light_pool));
                        CHECK(raw_alpha >= alpha_pool && raw_alpha + 0x10100 <= alpha_pool + sizeof(alpha_pool));
                        CHECK(((unsigned int)raw_light & 255) == (unsigned int)skew &&
                              ((unsigned int)raw_alpha & 255) == (unsigned int)skew);
                        memset(maps, 0x57, sizeof(maps));
                        maps[row].value.light_table = light_pool;
                        maps[row].value.handle334 = light_pool + 1;
                        maps[row].value.alpha_table = alpha_pool;
                        maps[row].value.handle33c = alpha_pool + 1;
                        memcpy(expected_maps, maps, sizeof(maps));
                        memset(light_pool, 0xa7, sizeof(light_pool));
                        memset(alpha_pool, 0xb6, sizeof(alpha_pool));
                        memcpy(expected_light, light_pool, sizeof(light_pool));
                        memcpy(expected_alpha, alpha_pool, sizeof(alpha_pool));
                        memset(path, 0, sizeof(path));
                        memcpy(path, "authored/input.name", 20);
                        memcpy(expected_path, path, sizeof(path));
                        memset(basename_value, 0, sizeof(basename_value));
                        for (i = 0; names[name_case][i]; ++i)
                            basename_value[i] = names[name_case][i];
                        memset(&input_header, 0x35, sizeof(input_header));
                        memcpy(input_header.marker, "TEST", 4);
                        input_header.version = (unsigned int)(scenario * 71);
                        input_header.flags = 0x80000200u | (unsigned int)flag;
                        input_header.field24 = 0x12345678;
                        input_header.field28 = 0xff800000u;
                        input_header.field2c = 0x80000000u;
                        handle = scenario == 0 ? 0 : (h ? 0x80000001u : 7);
                        reported = reports[r];
                        header_good = scenario != 1;
                        alpha_enabled = flag ^ mutate;
                        closed = 0;
                        plan_count = 0;
                        plan[plan_count++] = OPEN;
                        fallback = 0;
                        if (!handle)
                            fallback = 1;
                        else
                        {
                            plan[plan_count++] = BASENAME;
                            plan[plan_count++] = COPY;
                            plan[plan_count++] = HEADER;
                            plan[plan_count++] = COMPARE;
                            if (!header_good)
                                fallback = 1;
                            else
                            {
                                plan[plan_count++] = PALETTE;
                                plan[plan_count++] = ALLOC_LIGHT;
                                if (scenario == 2)
                                    fallback = 1;
                                else
                                {
                                    plan[plan_count++] = LIGHT;
                                    if (alpha_enabled)
                                    {
                                        plan[plan_count++] = ALLOC_ALPHA;
                                        if (scenario == 3)
                                            fallback = 1;
                                        else
                                            plan[plan_count++] = ALPHA;
                                    }
                                }
                            }
                            plan[plan_count++] = CLOSE;
                        }
                        if (fallback)
                            plan[plan_count++] = GRAY;
                        header_local = 0;
                        position = provider = 0;
                        op_colormap_services = &services[0];
                        CHECK(op_colormap_load_entry(path, &maps[row].value) == 1);
                        CHECK(position == plan_count && closed == (handle != 0));
                        verify();
                    }
    printf("colormap_load_entry: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
