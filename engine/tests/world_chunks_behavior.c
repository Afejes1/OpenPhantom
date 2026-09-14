/* Authored tags and owned fixtures only. Backend calls log lengths; they never
 * dereference a skip buffer or attempt a high-bit allocation. */
#include "../src/b3d_chunk_readers.h"
#include <stdio.h>
#include <string.h>

typedef struct WC_STREAM
{
    unsigned int before;
    OP_B3D_STREAM value;
    unsigned int after;
} WC_STREAM;
typedef struct WC_HEADER
{
    unsigned int before;
    OP_B3D_HEADER value;
    unsigned int after;
} WC_HEADER;
typedef struct WC_OUTPUT
{
    unsigned int before, value, after;
} WC_OUTPUT;
typedef struct WC_LOCAL
{
    char tag[8];
    unsigned int payload;
} WC_LOCAL;
typedef char wc_local_size[sizeof(WC_LOCAL) == 12 ? 1 : -1];
typedef char wc_local_payload[offsetof(WC_LOCAL, payload) == 8 ? 1 : -1];
static WC_STREAM wc_stream, wc_expected_stream;
static WC_HEADER wc_header, wc_expected_header;
static WC_OUTPUT wc_output, wc_expected_output;
static unsigned char wc_owned[4][8], wc_expected_owned[4][8];
static unsigned char wc_unused_world[32], wc_expected_unused_world[32];
static char wc_tags[23][8], wc_expected_tags[23][8], wc_unknown[8], wc_expected_unknown[8];
const char *op_chunk_tags[23];
static const char *wc_expected_table[23];
static int wc_checks, wc_failures, wc_stage, wc_read_result, wc_selected, wc_mutate_table;
static int wc_header_reads, wc_compares, wc_backend_events, wc_backend_read_result;
static unsigned int wc_payload, wc_skip_bytes;
static void *wc_local, *wc_allocation, *wc_captured_allocation;

static void wc_check_at(int value, int line)
{
    ++wc_checks;
    if (!value)
    {
        ++wc_failures;
        printf("world chunks failed at line %d\n", line);
    }
}
#define WC_CHECK(value) wc_check_at(!!(value), __LINE__)
static void wc_check_state(void)
{
    WC_CHECK(memcmp(&wc_stream, &wc_expected_stream, sizeof(wc_stream)) == 0);
    WC_CHECK(memcmp(&wc_header, &wc_expected_header, sizeof(wc_header)) == 0);
    WC_CHECK(memcmp(&wc_output, &wc_expected_output, sizeof(wc_output)) == 0);
    WC_CHECK(memcmp(wc_owned, wc_expected_owned, sizeof(wc_owned)) == 0);
    WC_CHECK(memcmp(wc_unused_world, wc_expected_unused_world, sizeof(wc_unused_world)) == 0);
    WC_CHECK(memcmp(wc_tags, wc_expected_tags, sizeof(wc_tags)) == 0);
    WC_CHECK(memcmp(wc_unknown, wc_expected_unknown, sizeof(wc_unknown)) == 0);
    WC_CHECK(memcmp(op_chunk_tags, wc_expected_table, sizeof(op_chunk_tags)) == 0);
}
static void wc_initialize(void)
{
    int i, j;
    memset(&wc_stream, 0x11, sizeof(wc_stream));
    memset(&wc_header, 0x22, sizeof(wc_header));
    memset(&wc_output, 0x33, sizeof(wc_output));
    memset(wc_owned, 0x44, sizeof(wc_owned));
    memset(wc_unused_world, 0x55, sizeof(wc_unused_world));
    wc_stream.value.handle = wc_owned[0];
    wc_stream.value.mode = 4;
    wc_header.value.skip_bytes = 37;
    wc_header.value.payload_size = 0x87654321U;
    wc_expected_stream = wc_stream;
    wc_expected_header = wc_header;
    wc_expected_output = wc_output;
    memcpy(wc_expected_owned, wc_owned, sizeof(wc_owned));
    memcpy(wc_expected_unused_world, wc_unused_world, sizeof(wc_unused_world));
    for (i = 0; i < 23; ++i)
    {
        for (j = 0; j < 8; ++j)
            wc_tags[i][j] = (char)(1 + i * 8 + j);
        op_chunk_tags[i] = wc_tags[i];
        wc_expected_table[i] = wc_tags[i];
    }
    for (j = 0; j < 8; ++j)
        wc_unknown[j] = (char)(0xe0 + j);
    memcpy(wc_expected_tags, wc_tags, sizeof(wc_tags));
    memcpy(wc_expected_unknown, wc_unknown, sizeof(wc_unknown));
    wc_stage = 0;
    wc_read_result = 1;
    wc_selected = wc_mutate_table = 0;
    wc_header_reads = wc_compares = wc_backend_events = 0;
    wc_payload = 77;
    wc_backend_read_result = -23;
    wc_allocation = wc_owned[1];
    wc_captured_allocation = wc_allocation;
}
static void wc_mutate_backend_state(int mode, void *handle)
{
    wc_stream.value.mode = mode;
    wc_expected_stream.value.mode = mode;
    wc_stream.value.handle = handle;
    wc_expected_stream.value.handle = handle;
    wc_header.value.skip_bytes = 0x13579bdfU;
    wc_expected_header.value.skip_bytes = 0x13579bdfU;
    wc_header.value.payload_size = 0xa1b2c3d4U;
    wc_expected_header.value.payload_size = 0xa1b2c3d4U;
}
static void *world_chunks_allocate(unsigned int bytes)
{
    WC_CHECK(world_chunks_active && wc_stage == 1);
    WC_CHECK(wc_backend_events++ == 0);
    wc_check_state();
    WC_CHECK(bytes == wc_skip_bytes);
    wc_mutate_backend_state(-5, wc_owned[1]);
    return wc_allocation;
}
static int world_chunks_read(void *destination, int size, int count, OP_B3D_STREAM *stream)
{
    WC_LOCAL local;
    WC_CHECK(world_chunks_active);
    WC_CHECK(stream == &wc_stream.value);
    wc_check_state();
    if (wc_stage == 0)
    {
        WC_CHECK(wc_header_reads++ == 0);
        WC_CHECK(size == 12 && count == 1);
        WC_CHECK(wc_output.value == 0);
        wc_local = destination;
        wc_output.value = 0xabcdef01U;
        wc_expected_output.value = 0xabcdef01U;
        if (wc_read_result != 0)
        {
            memcpy(local.tag, wc_selected < 23 ? wc_tags[wc_selected] : wc_unknown, 8);
            local.payload = wc_payload;
            memcpy(destination, &local, sizeof(local));
        }
        return wc_read_result;
    }
    WC_CHECK(wc_stage == 1);
    WC_CHECK(wc_backend_events++ == 1);
    WC_CHECK(destination == wc_captured_allocation);
    WC_CHECK(size == 1 && (unsigned int)count == wc_skip_bytes);
    WC_CHECK(stream->mode == -5 && stream->handle == wc_owned[1]);
    wc_allocation = wc_owned[2]; /* Candidate must release its captured local. */
    wc_mutate_backend_state(12, wc_owned[2]);
    return wc_backend_read_result;
}
int op_compare_tag(const char *left, const char *right, unsigned int limit)
{
    int call;
    if(mnm_active)return mnm_compare(left,right,limit);
    call = wc_compares++;
    WC_CHECK(world_chunks_active && wc_stage == 0);
    wc_check_state();
    WC_CHECK(limit == 8 && left == (const char *)wc_local);
    WC_CHECK(call >= 0 && call < 23);
    if (call < 0 || call >= 23)
        return 1;
    WC_CHECK(right == wc_expected_table[call]);
    if (wc_mutate_table && call == 0)
    {
        op_chunk_tags[1] = wc_tags[wc_selected];
        wc_expected_table[1] = wc_tags[wc_selected];
    }
    wc_output.value = 0x12345678U;
    wc_expected_output.value = 0x12345678U;
    return strncmp(left, right, limit);
}
int op_stream_seek(void *handle, long offset, int origin)
{
    if(sv_active)return sv_seek(handle,offset,origin);
    WC_CHECK(world_chunks_active && wc_stage == 1);
    WC_CHECK(wc_backend_events++ == 0);
    wc_check_state();
    WC_CHECK(handle == wc_owned[0]);
    WC_CHECK((unsigned int)offset == wc_skip_bytes && origin == 1);
    wc_mutate_backend_state(7, wc_owned[3]);
    return -19;
}
void op_release(void *memory)
{
    if(t930_active){t930_release(memory);return;}
    if(t920_active){t920_release(memory);return;}
    if(s73o_active){s73o_release(memory);return;}
    if(s73p_active){s73p_release(memory);return;}
    if(s73c_active){s73c_release(memory);return;}
    if(msha_active){msha_release(memory);return;}
    if(video_route){video_release(memory);return;}
    if(mlc_active){mlc_release(memory);return;}
    if(dcy_active){dcy_release(memory);return;}
    if(dcl_active){dcl_release(memory);return;}
    if(sfl_active){sfl_release(memory);return;}
    if(pfc_active){pfc_raw_release(memory);return;}
    if(cmr_active){cmr_release(memory);return;}
    if (cv_active)
    {
        cv_release(memory);
        return;
    }
    if (fctor_active)
    {
        fctor_release(memory);
        return;
    }
    if (cc_active)
    {
        cc_release(memory);
        return;
    }
    if (lc_active)
    {
        lc_release(memory);
        return;
    }
    if (colormap_active)
    {
        colormap_release_root(memory);
        return;
    }
    if (md_active)
    {
        md_release(memory);
        return;
    }
    if (kc_active)
    {
        kc_release(memory);
        return;
    }
    if (fk_active)
    {
        fk_release(memory);
        return;
    }
    if (shield_lifecycle_active)
    {
        shield_lifecycle_release(memory);
        return;
    }
    WC_CHECK(world_chunks_active && wc_stage == 1);
    WC_CHECK(wc_backend_events++ == 2);
    wc_check_state();
    WC_CHECK(memory == wc_captured_allocation);
    WC_CHECK(wc_stream.value.mode == 12 && wc_stream.value.handle == wc_owned[2]);
    wc_mutate_backend_state(-11, wc_owned[3]);
}
static void wc_read_header(int expected_index)
{
    int result;
    wc_expected_output.value = 0; /* Required store before read callback. */
    result = op_world_read_chunk_header(&wc_stream.value, &wc_output.value);
    WC_CHECK(result == expected_index);
    WC_CHECK(wc_header_reads == 1);
    WC_CHECK(wc_backend_events == 0);
    if (wc_read_result == 0)
    {
        WC_CHECK(wc_compares == 0);
        /* Callback mutation survives the failure return. */
        wc_expected_output.value = 0xabcdef01U;
    }
    else
    {
        WC_CHECK(wc_compares == (expected_index < 23 ? expected_index + 1 : 23));
        wc_expected_output.value = wc_payload;
    }
    wc_check_state();
}
static void wc_skip(int wrapper, int mode, unsigned int bytes, int null_allocation, int read_result)
{
    int result;
    wc_stage = 1;
    wc_backend_events = 0;
    wc_skip_bytes = bytes;
    wc_header.value.skip_bytes = bytes;
    wc_expected_header.value.skip_bytes = bytes;
    wc_stream.value.mode = mode;
    wc_expected_stream.value.mode = mode;
    wc_allocation = null_allocation ? 0 : wc_owned[1];
    wc_captured_allocation = wc_allocation;
    wc_backend_read_result = read_result;
    if (wrapper)
        result = op_world_load_via_skip(&wc_stream.value, wrapper == 2 ? 0 : wc_unused_world, &wc_header.value);
    else
        result = op_world_skip_chunk(&wc_stream.value, bytes);
    WC_CHECK(result == 1);
    WC_CHECK(wc_backend_events == (mode == 0 ? 1 : 3));
    wc_check_state();
}
int op_test_world_chunks(void)
{
    static const unsigned int lengths[4] = {0, 37, 0x80000000U, 0xffffffffU};
    static const int modes[3] = {0, 4, -1};
    int i, j, wrapper, old_reads, old_allocates;
    old_reads = world_readers_read_calls;
    old_allocates = world_readers_allocate_calls;
    wc_checks = wc_failures = 0;
    world_chunks_active = 1;
    for (i = 0; i < 23; ++i)
    {
        wc_initialize();
        wc_selected = i;
        wc_payload = 0x80000000U + (unsigned int)i;
        wc_read_result = i == 7 ? -9 : 1;
        wc_read_header(i);
    }
    wc_initialize();
    wc_selected = 23;
    wc_read_header(23);
    wc_initialize();
    wc_read_result = 0;
    wc_read_header(-1);
    wc_initialize();
    wc_mutate_table = 1;
    wc_selected = 5;
    wc_read_header(1);
    wc_initialize();
    memset(wc_tags[0], 'A', 8);
    memset(wc_tags[1], 'B', 8);
    wc_tags[0][0] = wc_tags[1][0] = 'Q';
    wc_tags[0][1] = wc_tags[1][1] = 0;
    memcpy(wc_expected_tags, wc_tags, sizeof(wc_tags));
    wc_selected = 1;
    wc_read_result = -1;
    wc_payload = 0xffffffffU;
    wc_read_header(0);
    for (wrapper = 0; wrapper < 3; ++wrapper)
        for (i = 0; i < 4; ++i)
            for (j = 0; j < 3; ++j)
            {
                wc_initialize();
                wc_skip(wrapper, modes[j], lengths[i], i == 2, i == 0 ? 0 : -23);
            }
    /* Real header -> caller stores size -> real wrapper -> real skip. */
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 3; ++j)
        {
            wc_initialize();
            wc_selected = 3;
            wc_payload = lengths[i];
            wc_read_header(3);
            wc_skip(i == 0 ? 2 : 1, modes[j], wc_output.value, i == 2, i == 0 ? 0 : -23);
        }
    world_chunks_active = 0;
    WC_CHECK(world_readers_read_calls == old_reads);
    WC_CHECK(world_readers_allocate_calls == old_allocates);
    WC_CHECK(world_names_active == 0);
    printf("world chunks integration: %d checks, %d failures\n", wc_checks, wc_failures);
    return wc_failures;
}
#undef WC_CHECK
