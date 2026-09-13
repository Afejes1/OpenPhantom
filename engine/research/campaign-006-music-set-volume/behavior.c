#include "api.h"
#include <stdio.h>
#include <string.h>

float op_music_volume;
int op_music_initialized;
int op_music_lock_depth;
static int checks;
static int failures;
static int calls;
static int original_depth;
static int lock_delta;
static int apply_delta;
static int unlock_delta;
static unsigned long expected_bits;
static unsigned long callback_cache_bits;
static void *returned_buffer;
static char buffer_storage;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static unsigned long bits(float value)
{
    unsigned long result;
    memcpy(&result, &value, sizeof result);
    return result;
}

static float make_float(unsigned long value)
{
    float result;
    memcpy(&result, &value, sizeof result);
    return result;
}

static void __stdcall lock_stub(void)
{
    check(calls == 0, "lock first");
    check(op_music_lock_depth == original_depth, "lock sees old depth");
    check(bits(op_music_volume) == callback_cache_bits,
          "lock sees cached volume before final store");
    ++calls;
    op_music_lock_depth += lock_delta;
    op_music_volume = make_float(callback_cache_bits);
    op_music_initialized = 0;
}

void *op_music_grab_buffer(void)
{
    check(calls == 1, "grab follows lock and increment");
    check(op_music_lock_depth == original_depth + lock_delta + 1,
          "grab sees current incremented depth");
    ++calls;
    return returned_buffer;
}

void op_music_apply_volume(void *buffer, float volume)
{
    check(calls == 2, "apply follows grab");
    check(buffer == returned_buffer, "returned buffer forwarded including null");
    check(bits(volume) == expected_bits, "original input bits forwarded");
    check(bits(op_music_volume) == callback_cache_bits,
          "cache not updated before apply");
    ++calls;
    op_music_lock_depth += apply_delta;
    op_music_volume = make_float(0x3f000000UL);
}

static void __stdcall unlock_stub(void)
{
    check(calls == 3, "unlock follows apply");
    check(op_music_lock_depth == original_depth + lock_delta + 1 + apply_delta,
          "unlock sees apply-mutated depth");
    check(bits(op_music_volume) == 0x3f000000UL,
          "unlock occurs before final cache store");
    ++calls;
    op_music_lock_depth += unlock_delta;
    op_music_volume = make_float(0x3f400000UL);
}

op_music_lock_fn op_music_lock = lock_stub;
op_music_lock_fn op_music_unlock = unlock_stub;

static void skipped(unsigned long cached, unsigned long input, int initialized,
                    const char *description)
{
    calls = 0;
    op_music_volume = make_float(cached);
    op_music_initialized = initialized;
    op_music_lock_depth = 17;
    op_music_set_volume(make_float(input));
    check(calls == 0, description);
    check(bits(op_music_volume) == cached && op_music_lock_depth == 17,
          "skipped path preserves cache bits and depth");
}

static void active(unsigned long input, void *buffer, int result_depth)
{
    calls = 0;
    original_depth = 100;
    op_music_lock_depth = original_depth;
    op_music_initialized = -7;
    op_music_volume = make_float(0x3f800000UL);
    expected_bits = input;
    callback_cache_bits = 0x3f800000UL;
    returned_buffer = buffer;
    lock_delta = 3;
    apply_delta = 5;
    unlock_delta = 7;
    op_music_set_volume(make_float(input));
    check(calls == 4, "all callbacks run for changed initialized volume");
    check(op_music_lock_depth == result_depth, "final decrement reloads depth");
    check(bits(op_music_volume) == input, "original input cached after unlock");
    check(op_music_initialized == 0, "callback initialized mutation retained");
}

int main(void)
{
    skipped(0x00000000UL, 0x80000000UL, 1, "signed zeros compare equal");
    skipped(0x3f800000UL, 0x3f800000UL, 1, "ordinary equality skips");
    skipped(0x7fc12345UL, 0x3f800000UL, 1, "cached quiet NaN is unordered and skips");
    skipped(0x3f800000UL, 0x7fc54321UL, 1, "input quiet NaN is unordered and skips");
    skipped(0x3f800000UL, 0x40000000UL, 0, "uninitialized changed value skips");
    active(0x40000000UL, &buffer_storage, 115);
    active(0xff800000UL, 0, 115);
    printf("Music set volume: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}