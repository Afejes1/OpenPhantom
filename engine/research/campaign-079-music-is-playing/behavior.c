#include "api.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static int checks, failures, calls, mutate;
static unsigned int before_gate, before_device, status_value, return_value;
unsigned int op_music_gate, op_music_device;
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
static unsigned int __stdcall command(unsigned int device, unsigned int message, unsigned int flags, void *data)
{
    OP_MUSIC_STATUS expected = {0, 0, 4, 0};
    CHECK(calls == 0);
    CHECK(device == before_device);
    CHECK(op_music_device == before_device);
    CHECK(op_music_gate == before_gate);
    CHECK(message == 0x814);
    CHECK(flags == 0x100);
    CHECK(data != 0);
    CHECK(data != (void *)&op_music_gate && data != (void *)&op_music_device);
    if (!data || data == (void *)&op_music_gate || data == (void *)&op_music_device)
        exit(2);
    CHECK(memcmp(data, &expected, sizeof(expected)) == 0);
    ((OP_MUSIC_STATUS *)data)->value = status_value;
    ((OP_MUSIC_STATUS *)data)->callback = 0x5a5a5a5au;
    ((OP_MUSIC_STATUS *)data)->item = 0x33333333u;
    ((OP_MUSIC_STATUS *)data)->track = 0x77777777u;

    ++calls;
    if (mutate)
    {
        op_music_gate = 0;
        op_music_device = 0xaabbccddu;
    }
    return return_value;
}
unsigned int(__stdcall *op_music_command)(unsigned int, unsigned int, unsigned int, void *) = command;
int main(void)
{
    static const unsigned int gates[4] = {0, 1, 0x80000000u, 0xffffffffu};
    static const unsigned int devices[5] = {0, 1, 65535, 0x80000000u, 0xffffffffu};
    static const unsigned int returns[5] = {0, 1, 0x80004005u, 0xffffffffu, 0x20d};
    static const unsigned int values[6] = {0, 1, 0x20c, 0x20d, 0x20e, 0xffffffffu};
    int g, d, r, v;
    int result;
    for (g = 0; g < 4; ++g)
        for (d = 0; d < 5; ++d)
            for (r = 0; r < 5; ++r)
                for (v = 0; v < 6; ++v)
                    for (mutate = 0; mutate < 2; ++mutate)
                    {
                        op_music_gate = before_gate = gates[g];
                        op_music_device = before_device = devices[d];
                        calls = 0;
                        return_value = returns[r];
                        status_value = values[v];
                        result = op_music_is_playing();
                        CHECK(result == (before_gate && status_value != 0x20d));
                        CHECK(calls == (before_gate != 0));
                        CHECK(op_music_gate == (before_gate && mutate ? 0 : before_gate));
                        CHECK(op_music_device == (before_gate && mutate ? 0xaabbccddu : before_device));
                        CHECK(op_music_command == command);
                    }
    printf("music_is_playing: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
