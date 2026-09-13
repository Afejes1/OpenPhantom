#include "api.h"
#include <limits.h>
#include <stdio.h>

int op_music_cached_ids[2];
static int checks;
static int failures;
static int callback_calls;
static int read_result;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

int op_music_read_bytes(void *data, int size)
{
    int *ids = (int *)data;
    ++callback_calls;
    check(data == op_music_cached_ids, "cache base pointer passed");
    check(size == 8, "exact cache byte count");
    ids[0] = 71;
    ids[1] = 72;
    return read_result;
}

static void reject(int version)
{
    op_music_cached_ids[0] = 11;
    op_music_cached_ids[1] = 12;
    callback_calls = 0;
    check(op_music_restore_cached_ids(version) == 1, "unsupported version rejected");
    check(callback_calls == 0, "unsupported version performs no read");
    check(op_music_cached_ids[0] == 11 && op_music_cached_ids[1] == 12,
          "unsupported version preserves cache");
}

static void accept(int result, int expected)
{
    op_music_cached_ids[0] = 21;
    op_music_cached_ids[1] = 22;
    callback_calls = 0;
    read_result = result;
    check(op_music_restore_cached_ids(2) == expected, "normalized read result");
    check(callback_calls == 1, "supported version reads once");
    check(op_music_cached_ids[0] == 71 && op_music_cached_ids[1] == 72,
          "read mutations remain on both success and failure");
}

int main(void)
{
    reject(0);
    reject(1);
    reject(3);
    reject(-1);
    reject(INT_MIN);
    reject(INT_MAX);
    accept(0, 1);
    accept(1, 0);
    accept(-9, 0);
    printf("Music restore IDs: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}