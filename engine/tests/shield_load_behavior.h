#include "../src/effects_save.h"
#include "../src/focused_accessors.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int lo_checks, lo_failures;
static void lo_check(int condition, int line)
{
    lo_checks++;
    if (!condition)
    {
        lo_failures++;
        printf("line %d failed\n", line);
    }
}
#define lo_CHECK(x) lo_check(!!(x), __LINE__)
static OP_SHIELD lo_expected[32];
static OP_SHIELD_SAVE *lo_payload;
static int *lo_count_address;
static int lo_count_value, lo_header_status, lo_row_status, lo_fail_after, lo_mutation, lo_named, lo_header_reads,
    lo_row_reads, lo_allocations, lo_texture_calls, lo_failed, lo_last_slot;
static void lo_finish_row(void)
{
    OP_SHIELD *s = &lo_expected[lo_last_slot];
    s->radius = lo_payload->radius;
    s->red = (unsigned char)((lo_payload->color >> 16) & 255);
    s->green = (unsigned char)((lo_payload->color >> 8) & 255);
    s->blue = (unsigned char)(lo_payload->color & 255);
    s->alpha = (unsigned char)(lo_payload->color >> 24);
}
int lo_op_save_read(void *memory, unsigned int bytes)
{
    OP_SHIELD_SAVE row;
    unsigned int elapsed_bits = 0x80000000, radius_bits = 0x7fc12345;
    lo_CHECK(memcmp(op_shields, lo_expected, sizeof(lo_expected)) == 0);
    if (!lo_header_reads)
    {
        lo_CHECK(bytes == 4);
        lo_count_address = (int *)memory;
        *lo_count_address = lo_count_value;
        lo_header_reads++;
        return lo_header_status;
    }
    lo_CHECK(bytes == 52);
    lo_CHECK(!lo_failed);
    memset(&row, 0, 52);
    row.slot = 1000 + lo_row_reads;
    row.visible = INT_MIN + lo_row_reads;
    memcpy(&row.elapsed, &elapsed_bits, 4);
    memcpy(&row.radius, &radius_bits, 4);
    row.color = 0x81234567u + (unsigned int)lo_row_reads;
    if (lo_named)
    {
        memcpy(row.name, "unit-row", 9);
        row.name[8] = (char)('a' + lo_row_reads % 26);
        row.name[9] = 0;
    }
    memcpy(memory, &row, 52);
    lo_payload = (OP_SHIELD_SAVE *)memory;
    lo_row_reads++;
    if (lo_mutation)
        *lo_count_address = 1;
    return lo_row_status;
}
int lo_op_shield_allocate(void *actor)
{
    OP_SHIELD *s;
    lo_CHECK(actor == 0);
    lo_CHECK(lo_row_reads == lo_allocations + 1);
    lo_CHECK(memcmp(op_shields, lo_expected, sizeof(lo_expected)) == 0);
    if (lo_allocations == lo_fail_after)
    {
        lo_allocations++;
        lo_failed = 1;
        return -7;
    }
    lo_last_slot = (lo_allocations * 7) % 32;
    lo_allocations++;
    op_shields[lo_last_slot].active = lo_expected[lo_last_slot].active = 1;
    op_shields[lo_last_slot].no_save = lo_expected[lo_last_slot].no_save = -9;
    s = &lo_expected[lo_last_slot];
    s->saved_actor = lo_payload->slot;
    s->visible = lo_payload->visible;
    s->no_save = 0;
    s->elapsed = lo_payload->elapsed;
    if (!lo_named)
        lo_finish_row();
    return lo_last_slot;
}
void lo_op_shield_set_texture(int slot, char *name)
{
    lo_CHECK(slot == lo_last_slot);
    lo_CHECK(name == lo_payload->name);
    lo_CHECK(name[0] == 'u');
    lo_CHECK(memcmp(op_shields, lo_expected, sizeof(lo_expected)) == 0);
    lo_texture_calls++;
    if (lo_mutation)
    {
        lo_payload->radius = -12.25f;
        lo_payload->color = 0xfedcba98;
        op_shields[slot].no_save = lo_expected[slot].no_save = 23;
        op_shields[slot].visible = lo_expected[slot].visible = -77;
    }
    lo_finish_row();
}
int lo_main(void)
{
    int counts[6] = {INT_MIN, -1, 0, 1, 3, 32}, statuses[3] = {0, 1, -7}, failures_at[4] = {-1, 0, 1, 31};
    int c, h, r, f, i, result, wanted_rows, successful;
    for (c = 0; c < 6; c++)
        for (h = 0; h < 3; h++)
            for (r = 0; r < 3; r++)
                for (f = 0; f < 4; f++)
                    for (lo_mutation = 0; lo_mutation < 2; lo_mutation++)
                        for (lo_named = 0; lo_named < 2; lo_named++)
                        {
                            memset(op_shields, 0, sizeof(op_shields));
                            for (i = 0; i < 32; i++)
                            {
                                op_shields[i].visible = 17 + i;
                                op_shields[i].saved_actor = -99;
                                op_shields[i].radius = 7.5f;
                                op_shields[i].red = 19;
                                op_shields[i].blue = 77;
                            }
                            memcpy(lo_expected, op_shields, sizeof(lo_expected));
                            lo_count_value = counts[c];
                            lo_header_status = statuses[h];
                            lo_row_status = statuses[r];
                            lo_fail_after = failures_at[f];
                            lo_header_reads = lo_row_reads = lo_allocations = lo_texture_calls = lo_failed = 0;
                            lo_payload = 0;
                            lo_count_address = 0;
                            lo_last_slot = 0;
                            result = op_shield_load();
                            wanted_rows =
                                lo_header_status && lo_count_value > 0 ? (lo_mutation ? 1 : lo_count_value) : 0;
                            if (lo_fail_after >= 0 && wanted_rows > lo_fail_after)
                                wanted_rows = lo_fail_after + 1;
                            successful = wanted_rows - (lo_failed ? 1 : 0);
                            lo_CHECK(result == (!lo_header_status || lo_failed));
                            lo_CHECK(lo_header_reads == 1);
                            lo_CHECK(lo_row_reads == wanted_rows);
                            lo_CHECK(lo_allocations == wanted_rows);
                            lo_CHECK(lo_texture_calls == (lo_named ? successful : 0));
                            lo_CHECK(memcmp(op_shields, lo_expected, sizeof(lo_expected)) == 0);
                        }
    printf("shield load: %d checks, %d failures\n", lo_checks, lo_failures);
    return lo_failures != 0;
}

#undef lo_CHECK
