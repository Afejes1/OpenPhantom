#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

int op_music_initialized;
int op_music_cached_sequence;
int op_music_lock_depth;

static int checks;
static int failures;
static int events[8];
static int event_count;
static unsigned int supplied_count;
static void *supplied_player;
static unsigned long expected_radius_bits;
static int expected_sequence;
static int backend_result;
static int initial_depth;
static int lock_delta;
static int backend_delta;
static int unlock_delta;
static int replacement_called;
static int mutate_preconditions;
static char player_storage;

static void check(int condition, const char *description)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", description);
    }
}

static unsigned long float_bits(float value)
{
    unsigned long bits;
    memcpy(&bits, &value, sizeof(bits));
    return bits;
}

static void event(int id)
{
    if (event_count >= 0 && event_count < 8)
        events[event_count] = id;
    else
        check(0, "event array bound");
    ++event_count;
}

void *op_effective_player(void)
{
    event(1);
    if (mutate_preconditions) { op_music_initialized = 0; op_music_cached_sequence = 22; }
    return supplied_player;
}

unsigned int op_count_nearby(void *player, float radius)
{
    event(2);
    check(player == supplied_player, "count receives effective player");
    check(float_bits(radius) == expected_radius_bits, "count receives exact radius bits");
    if (mutate_preconditions) { op_music_initialized = -9; op_music_cached_sequence = -77; }
    return supplied_count;
}

static void __stdcall replacement_unlock(void)
{
    event(5);
    check(op_music_lock_depth == initial_depth + lock_delta + 1 + backend_delta,
          "replacement unlock sees backend-mutated depth");
    ++replacement_called;
    op_music_lock_depth += unlock_delta;
    op_music_cached_sequence = 909;
}

static void __stdcall old_unlock(void)
{
    check(0, "backend replaces unlock callback");
}

static void __stdcall lock_stub(void)
{
    event(3);
    check(op_music_cached_sequence == expected_sequence,
          "cache is written before lock");
    check(op_music_lock_depth == initial_depth, "lock sees initial depth");
    op_music_lock_depth += lock_delta;
}

op_music_callback op_music_lock = lock_stub;
op_music_callback op_music_unlock = old_unlock;

int op_backend_sequence(int sequence)
{
    event(4);
    check(sequence == expected_sequence, "backend receives sequence");
    check(op_music_lock_depth == initial_depth + lock_delta + 1,
          "backend sees post-lock increment");
    op_music_lock_depth += backend_delta;
    op_music_initialized = -17;
    op_music_unlock = replacement_unlock;
    return backend_result;
}

static void reset_case(void)
{
    memset(events, 0, sizeof(events));
    event_count = 0;
    replacement_called = 0;
    mutate_preconditions = 0;
    op_music_initialized = 1;
    op_music_cached_sequence = 10;
    op_music_lock_depth = 20;
    initial_depth = 20;
    lock_delta = 3;
    backend_delta = 5;
    unlock_delta = 7;
    supplied_player = &player_storage;
    supplied_count = 5;
    expected_radius_bits = 0x3f000000UL;
    expected_sequence = 22;
    backend_result = -123;
    op_music_unlock = old_unlock;
}

static void expect_no_calls(int result, int old_cache, int old_depth,
                            const char *description)
{
    check(result == 0, description);
    check(event_count == 0, "rejected path makes no callbacks");
    check(op_music_cached_sequence == old_cache, "rejected path preserves cache");
    check(op_music_lock_depth == old_depth, "rejected path preserves depth");
    check(op_music_lock == lock_stub && op_music_unlock == old_unlock, "rejected callbacks unchanged");
}

static void threshold_case(unsigned int mode, unsigned int count,
                           unsigned int threshold, int should_run)
{
    int result;
    reset_case();
    supplied_count = count;
    result = op_music_set_sequence(22, mode, threshold, 0.5f);
    if (should_run) {
        check(result == -123, "successful path preserves backend result");
        check(event_count == 5, "successful gated path makes five callbacks");
        check(events[0] == 1 && events[1] == 2 && events[2] == 3 &&
              events[3] == 4 && events[4] == 5, "successful callback order");
        check(replacement_called == 1, "live unlock replacement is called");
        check(op_music_lock_depth == 35, "final depth uses all live mutations");
        check(op_music_cached_sequence == 909, "unlock cache mutation is retained");
        check(op_music_initialized == -17, "backend initialized mutation retained");
    } else {
        check(result == 0, "threshold rejection returns zero");
        check(event_count == 2 && events[0] == 1 && events[1] == 2,
              "threshold rejection stops after count");
        check(op_music_cached_sequence == 10, "threshold rejection preserves cache");
        check(op_music_lock_depth == 20, "threshold rejection preserves depth");
    }
}

int main(void)
{
    int result;

    reset_case();
    op_music_initialized = 0;
    expect_no_calls(op_music_set_sequence(22, 1, 5, 0.5f), 10, 20,
                    "uninitialized rejects");

    reset_case();
    expect_no_calls(op_music_set_sequence(10, 1, 5, 0.5f), 10, 20,
                    "same cached sequence rejects");

    reset_case();
    supplied_player = 0;
    result = op_music_set_sequence(22, 1, 5, 0.5f);
    check(result == 0 && event_count == 1 && events[0] == 1,
          "null player rejects after lookup");
    check(op_music_cached_sequence == 10 && op_music_lock_depth == 20,
          "null player preserves state");

    threshold_case(1, 4, 5, 0);
    threshold_case(1, 5, 5, 1);
    threshold_case(1, 6, 5, 1);
    threshold_case(2, 4, 5, 1);
    threshold_case(2, 5, 5, 1);
    threshold_case(2, 6, 5, 0);
    threshold_case(UINT_MAX, 0, UINT_MAX, 1);
    threshold_case(1, UINT_MAX, 0, 1);
    threshold_case(1, 0, UINT_MAX, 0);
    threshold_case(2, UINT_MAX, 0, 0);
    threshold_case(2, 0, UINT_MAX, 1);
    threshold_case(3, UINT_MAX, 0, 1);

    reset_case();
    result = op_music_set_sequence(22, 0, UINT_MAX, 0.5f);
    check(result == -123, "mode zero succeeds");
    check(event_count == 3 && events[0] == 3 && events[1] == 4 && events[2] == 5,
          "mode zero skips player and count");

    check(op_music_lock_depth == 35 && op_music_cached_sequence == 909 && op_music_initialized == -17,
          "mode zero complete callback state retained");
    check(op_music_lock == lock_stub && op_music_unlock == replacement_unlock,
          "successful callback pointer state retained");

    reset_case();
    mutate_preconditions = 1;
    backend_result = 0;
    expected_radius_bits = 0x7fc12345UL;
    {
        float special_radius;
        memcpy(&special_radius, &expected_radius_bits, 4);
        result = op_music_set_sequence(22, 1, 5, special_radius);
    }
    check(result == 0 && event_count == 5 && replacement_called == 1,
          "ordinary backend zero result still completes callback sequence");
    check(events[0] == 1 && events[1] == 2 && events[2] == 3 && events[3] == 4 && events[4] == 5,
          "guards are not rechecked after player/count mutations");
    check(op_music_lock_depth == 35 && op_music_cached_sequence == 909 && op_music_initialized == -17,
          "mutated preconditions and zero backend return retain complete final state");
    check(op_music_lock == lock_stub && op_music_unlock == replacement_unlock,
          "zero result preserves callback pointer state");

    printf("Music set sequence: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
