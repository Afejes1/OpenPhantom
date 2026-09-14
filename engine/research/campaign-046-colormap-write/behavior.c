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
    COPY = 1,
    OPEN,
    HEADER,
    PALETTE,
    LIGHT,
    ALPHA,
    CLOSE
};
static unsigned char lights[2][16384], expected_lights[2][16384];
static unsigned char alphas[2][65536], expected_alphas[2][65536];
static OP_CMP_HEADER expected_header;
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
           outside(p, op_colormap_marker, sizeof(op_colormap_marker)) && outside(p, lights, sizeof(lights)) &&
           outside(p, alphas, sizeof(alphas)) && outside(p, expected_lights, sizeof(expected_lights)) &&
           outside(p, expected_alphas, sizeof(expected_alphas)) &&
           outside(p, &expected_header, sizeof(expected_header));
}

static void verify(void)
{
    verify_maps();
    CHECK(memcmp(lights, expected_lights, sizeof(lights)) == 0);
    CHECK(memcmp(alphas, expected_alphas, sizeof(alphas)) == 0);
}
char *op_copy_keyframe_name(char *out, const char *input, unsigned int count)
{
    event(COPY);
    CHECK(input == op_colormap_marker && count == 4);
    verify();
    if (count != 4 || !valid_header_local(out))
    {
        CHECK(0);
        return out;
    }
    header_local = out;
    memcpy(out, "TEST", 4);
    return out;
}
static OP_FILE_HANDLE open_file(int which, const char *input, const char *mode)
{
    event(OPEN);
    service(which);
    CHECK(input == path && mode == op_colormap_write_mode);
    verify();
    if (mutate)
    {
        maps[row].value.flags = expected_maps[row].value.flags ^= 0x200;
        maps[row].value.field24 = expected_maps[row].value.field24 = 0x11223344;
        maps[row].value.field28 = expected_maps[row].value.field28 = 0x55667788;
        maps[row].value.field2c = expected_maps[row].value.field2c = 0x99aabbcc;
        maps[row].value.palette[0] = expected_maps[row].value.palette[0] = 0x19;
        maps[row].value.light_table = expected_maps[row].value.light_table = lights[1];
    }
    return handle;
}
static unsigned int write_file(int which, OP_FILE_HANDLE file, const void *data, unsigned int bytes)
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
        CHECK(data == header_local && header_local != 0);
        if (bytes != 64 || data != header_local || !header_local)
            return 0;
        CHECK(memcmp(data, &expected_header, 64) == 0);
        if (mutate)
            maps[row].value.palette[1] = expected_maps[row].value.palette[1] = 0x3e;
    }
    else if (wanted == PALETTE)
    {
        CHECK(bytes == 768 && data == maps[row].value.palette);
        if (data != maps[row].value.palette || bytes != 768)
            return 0;
        CHECK(memcmp(data, expected_maps[row].value.palette, 768) == 0);
        if (mutate)
            maps[row].value.light_table = expected_maps[row].value.light_table = lights[0];
    }
    else if (wanted == LIGHT)
    {
        CHECK(bytes == 16384 && data == expected_maps[row].value.light_table);
        if (bytes != 16384 || data != expected_maps[row].value.light_table)
            return 0;
        CHECK(memcmp(data, data == lights[0] ? expected_lights[0] : expected_lights[1], 16384) == 0);
        if (mutate)
        {
            maps[row].value.flags = expected_maps[row].value.flags ^= 1;
            maps[row].value.alpha_table = expected_maps[row].value.alpha_table = alphas[1];
        }
    }
    else if (wanted == ALPHA)
    {
        CHECK(bytes == 65536 && data == expected_maps[row].value.alpha_table);
        if (bytes != 65536 || data != expected_maps[row].value.alpha_table)
            return 0;
        CHECK(memcmp(data, data == alphas[0] ? expected_alphas[0] : expected_alphas[1], 65536) == 0);
    }
    else
        CHECK(0);
    return reported;
}
static int close_file(int which, OP_FILE_HANDLE file)
{
    event(CLOSE);
    service(which);
    CHECK(file == handle && file != 0);
    verify();
    maps[row].value.tail[0] = expected_maps[row].value.tail[0] = 0x7d;
    return -7;
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
static unsigned int write0(OP_FILE_HANDLE f, const void *p, unsigned int n)
{
    return write_file(0, f, p, n);
}
static unsigned int write1(OP_FILE_HANDLE f, const void *p, unsigned int n)
{
    return write_file(1, f, p, n);
}
int main(void)
{
    unsigned int flags;
    int h, r, i, j, result;
    static const unsigned int handles[] = {0, 7, 0x80000001u};
    static const unsigned int reports[] = {0, 1, 0xffffffffu};
    memset(services, 0x43, sizeof(services));
    services[0].open = open0;
    services[1].open = open1;
    services[0].close = close0;
    services[1].close = close1;
    services[0].write = write0;
    services[1].write = write1;
    for (row = 0; row < 3; ++row)
        for (flags = 0; flags < 2; ++flags)
            for (mutate = 0; mutate < 2; ++mutate)
                for (h = 0; h < 3; ++h)
                    for (r = 0; r < 3; ++r)
                    {
                        memset(maps, 0x57, sizeof(maps));
                        memset(path, 0, sizeof(path));
                        memcpy(path, "authored/output.name", 21);
                        memcpy(expected_path, path, sizeof(path));
                        for (i = 0; i < 2; ++i)
                        {
                            for (j = 0; j < 16384; ++j)
                                lights[i][j] = (unsigned char)(j * 13 + i);
                            for (j = 0; j < 65536; ++j)
                                alphas[i][j] = (unsigned char)(j * 7 + i);
                        }
                        memcpy(expected_lights, lights, sizeof(lights));
                        memcpy(expected_alphas, alphas, sizeof(alphas));
                        maps[row].value.flags = flags | 0x80000000u;
                        maps[row].value.field24 = 0x01020304;
                        maps[row].value.field28 = 0xff800000u;
                        maps[row].value.field2c = 0x80000000u;
                        maps[row].value.light_table = lights[0];
                        maps[row].value.alpha_table = alphas[0];
                        memcpy(expected_maps, maps, sizeof(maps));
                        memset(&expected_header, 0, sizeof(expected_header));
                        memcpy(expected_header.marker, "TEST", 4);
                        expected_header.version = 30;
                        expected_header.flags = maps[row].value.flags;
                        expected_header.field24 = maps[row].value.field24;
                        expected_header.field28 = maps[row].value.field28;
                        expected_header.field2c = maps[row].value.field2c;
                        handle = handles[h];
                        reported = reports[r];
                        plan_count = 0;
                        plan[plan_count++] = COPY;
                        plan[plan_count++] = OPEN;
                        if (handle)
                        {
                            plan[plan_count++] = HEADER;
                            plan[plan_count++] = PALETTE;
                            plan[plan_count++] = LIGHT;
                            if (flags ^ (unsigned int)mutate)
                                plan[plan_count++] = ALPHA;
                            plan[plan_count++] = CLOSE;
                        }
                        header_local = 0;
                        position = provider = 0;
                        op_colormap_services = &services[0];
                        result = op_colormap_write(path, &maps[row].value);
                        CHECK(result == (handle != 0));
                        CHECK(position == plan_count);
                        verify();
                    }
    printf("colormap_write: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
