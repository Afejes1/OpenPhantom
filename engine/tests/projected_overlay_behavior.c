#include "../src/projected_overlay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
OP_PROJECTION op_projections[255];
int op_projection_dirty;
float op_elapsed;
const float op_projection_zero = 0.0f;
#include "projection_expire_behavior.h"
#include "projection_remove_behavior.h"
#include "projection_clear_behavior.h"
#include "projection_sort_behavior.h"
#include "projection_compare_behavior.h"
static int po_connected, po_backend_calls, po_checks, po_failures;
static void po_check(int ok, int line)
{
    po_checks++;
    if (!ok)
    {
        po_failures++;
        printf("projected overlay integration line %d failed\n", line);
    }
}
#define PO_CHECK(x) po_check(!!(x), __LINE__)
void op_projection_qsort(void *p, unsigned int count, unsigned int stride, int (*compare)(const void *, const void *))
{
    if (!po_connected)
    {
        po_sort_observer(p, count, stride, compare);
        return;
    }
    PO_CHECK(p == op_projections);
    PO_CHECK(count == 255);
    PO_CHECK(stride == 68);
    PO_CHECK(compare == op_projection_compare);
    PO_CHECK(op_projection_dirty != 0);
    po_backend_calls++;
    qsort(p, count, stride, compare);
}
static OP_PROJECTION po_by_sequence[255], po_expected[255], po_zero;
static const unsigned int po_keys[7] = {0U, 1U, 2U, 0x7FFFFFFFU, 0x80000000U, 0xFFFFFFFEU, 0xFFFFFFFFU};
static int po_test_chain(void)
{
    int rotation, seq, key, slot, out, cleared, before_calls;
    po_connected = 1;
    memset(&po_zero, 0, sizeof(po_zero));
    for (rotation = 0; rotation < 17; rotation++)
    {
        memset(op_projections, 0xA5, sizeof(op_projections));
        for (seq = 0; seq < 255; seq++)
        {
            slot = (seq * 73 + rotation) % 255;
            op_projections[slot].surface = po_keys[seq % 7];
            op_projections[slot].sequence = (unsigned int)(seq + 1);
            op_projections[slot].created = 8.0f;
            op_projections[slot].lifetime = (seq % 3) == 0 ? 0.0f : ((seq % 3) == 1 ? 2.0f : 4.0f);
            op_projections[slot].group_token = (seq % 5) + 1;
            po_by_sequence[seq] = op_projections[slot];
        }
        out = 0;
        for (key = 0; key < 7; key++)
            for (seq = key; seq < 255; seq += 7)
                po_expected[out++] = po_by_sequence[seq];
        PO_CHECK(out == 255);
        op_projection_dirty = -1;
        op_elapsed = 10.0f;
        po_backend_calls = 0;
        op_projection_sort();
        PO_CHECK(po_backend_calls == 1);
        PO_CHECK(op_projection_dirty == 0);
        PO_CHECK(memcmp(po_expected, op_projections, sizeof(po_expected)) == 0);
        op_projection_expire();
        PO_CHECK(op_projection_dirty == 0);
        PO_CHECK(memcmp(po_expected, op_projections, sizeof(po_expected)) == 0);
        op_elapsed = 11.0f;
        cleared = 0;
        for (slot = 0; slot < 255; slot++)
            if (po_expected[slot].surface != 0 && (po_expected[slot].sequence - 1) % 3 == 1)
            {
                memset(&po_expected[slot], 0, 68);
                cleared++;
            }
        op_projection_expire();
        PO_CHECK(cleared > 0);
        PO_CHECK(op_projection_dirty == 1);
        PO_CHECK(memcmp(po_expected, op_projections, sizeof(po_expected)) == 0);
        for (slot = 0; slot < 255; slot++)
            if (po_expected[slot].group_token == 3)
                memset(&po_expected[slot], 0, 68);
        op_projection_remove(3);
        PO_CHECK(op_projection_dirty == 1);
        PO_CHECK(memcmp(po_expected, op_projections, sizeof(po_expected)) == 0);
        /* Expected sort: all zeroed records first, then surviving original key/sequence order. */
        out = 0;
        for (slot = 0; slot < 255; slot++)
            if (memcmp(&po_expected[slot], &po_zero, 68) == 0)
                out++;
        memset(po_by_sequence, 0, sizeof(po_by_sequence));
        for (slot = 0; slot < 255; slot++)
            if (memcmp(&po_expected[slot], &po_zero, 68) != 0)
                po_by_sequence[out++] = po_expected[slot];
        PO_CHECK(out == 255);
        op_projection_sort();
        PO_CHECK(po_backend_calls == 2);
        PO_CHECK(op_projection_dirty == 0);
        PO_CHECK(memcmp(po_by_sequence, op_projections, sizeof(po_by_sequence)) == 0);
        before_calls = po_backend_calls;
        op_projection_sort();
        PO_CHECK(po_backend_calls == before_calls);
        op_projection_dirty = -7;
        op_projection_clear();
        PO_CHECK(op_projection_dirty == -7);
        memset(po_expected, 0, sizeof(po_expected));
        PO_CHECK(memcmp(po_expected, op_projections, sizeof(po_expected)) == 0);
        PO_CHECK(op_elapsed == 11.0f);
        op_projection_sort();
        PO_CHECK(po_backend_calls == 3);
        PO_CHECK(op_projection_dirty == 0);
        PO_CHECK(memcmp(po_expected, op_projections, sizeof(po_expected)) == 0);
    }
    po_connected = 0;
    return po_failures;
}
static int op_test_projected_overlay(void)
{
    int result = 0;
    result += po_expire_main();
    result += po_remove_main();
    result += po_clear_main();
    result += po_sort_main();
    result += po_compare_main();
    result += po_test_chain();
    printf("projected overlay connected lifecycle: %d checks, %d failures\n", po_checks, po_failures);
    return result;
}
