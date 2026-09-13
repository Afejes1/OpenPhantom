#include "api.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int checks, failures;
static void check(int condition, int line)
{
    checks++;
    if (!condition)
    {
        failures++;
        printf("line %d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
OP_SHIELD op_shields[32];
static OP_SHIELD expected[32];
static OP_SHIELD_SAVE *payload;
static int *count_address;
static int count_value, header_status, row_status, fail_after, mutation, named, header_reads, row_reads, allocations,
    texture_calls, failed, last_slot;
static void finish_row(void)
{
    OP_SHIELD *s = &expected[last_slot];
    s->radius = payload->radius;
    s->red = (unsigned char)((payload->color >> 16) & 255);
    s->green = (unsigned char)((payload->color >> 8) & 255);
    s->blue = (unsigned char)(payload->color & 255);
    s->alpha = (unsigned char)(payload->color >> 24);
}
int op_save_read(void *memory, unsigned int bytes)
{
    OP_SHIELD_SAVE row;
    unsigned int elapsed_bits = 0x80000000, radius_bits = 0x7fc12345;
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    if (!header_reads)
    {
        CHECK(bytes == 4);
        count_address = (int *)memory;
        *count_address = count_value;
        header_reads++;
        return header_status;
    }
    CHECK(bytes == 52);
    CHECK(!failed);
    memset(&row, 0, 52);
    row.slot = 1000 + row_reads;
    row.visible = INT_MIN + row_reads;
    memcpy(&row.elapsed, &elapsed_bits, 4);
    memcpy(&row.radius, &radius_bits, 4);
    row.color = 0x81234567u + (unsigned int)row_reads;
    if (named)
    {
        memcpy(row.name, "unit-row", 9);
        row.name[8] = (char)('a' + row_reads % 26);
        row.name[9] = 0;
    }
    memcpy(memory, &row, 52);
    payload = (OP_SHIELD_SAVE *)memory;
    row_reads++;
    if (mutation)
        *count_address = 1;
    return row_status;
}
int op_shield_allocate(void *actor)
{
    OP_SHIELD *s;
    CHECK(actor == 0);
    CHECK(row_reads == allocations + 1);
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    if (allocations == fail_after)
    {
        allocations++;
        failed = 1;
        return -7;
    }
    last_slot = (allocations * 7) % 32;
    allocations++;
    op_shields[last_slot].active = expected[last_slot].active = 1;
    op_shields[last_slot].no_save = expected[last_slot].no_save = -9;
    s = &expected[last_slot];
    s->saved_actor = payload->slot;
    s->visible = payload->visible;
    s->no_save = 0;
    s->elapsed = payload->elapsed;
    if (!named)
        finish_row();
    return last_slot;
}
void op_shield_set_texture(int slot, char *name)
{
    CHECK(slot == last_slot);
    CHECK(name == payload->name);
    CHECK(name[0] == 'u');
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    texture_calls++;
    if (mutation)
    {
        payload->radius = -12.25f;
        payload->color = 0xfedcba98;
        op_shields[slot].no_save = expected[slot].no_save = 23;
        op_shields[slot].visible = expected[slot].visible = -77;
    }
    finish_row();
}
int main(void)
{
    int counts[6] = {INT_MIN, -1, 0, 1, 3, 32}, statuses[3] = {0, 1, -7}, failures_at[4] = {-1, 0, 1, 31};
    int c, h, r, f, i, result, wanted_rows, successful;
    for (c = 0; c < 6; c++)
        for (h = 0; h < 3; h++)
            for (r = 0; r < 3; r++)
                for (f = 0; f < 4; f++)
                    for (mutation = 0; mutation < 2; mutation++)
                        for (named = 0; named < 2; named++)
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
                            memcpy(expected, op_shields, sizeof(expected));
                            count_value = counts[c];
                            header_status = statuses[h];
                            row_status = statuses[r];
                            fail_after = failures_at[f];
                            header_reads = row_reads = allocations = texture_calls = failed = 0;
                            payload = 0;
                            count_address = 0;
                            last_slot = 0;
                            result = op_shield_load();
                            wanted_rows = header_status && count_value > 0 ? (mutation ? 1 : count_value) : 0;
                            if (fail_after >= 0 && wanted_rows > fail_after)
                                wanted_rows = fail_after + 1;
                            successful = wanted_rows - (failed ? 1 : 0);
                            CHECK(result == (!header_status || failed));
                            CHECK(header_reads == 1);
                            CHECK(row_reads == wanted_rows);
                            CHECK(allocations == wanted_rows);
                            CHECK(texture_calls == (named ? successful : 0));
                            CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
                        }
    printf("shield load: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
