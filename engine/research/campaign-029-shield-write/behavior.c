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
static OP_ACTOR actors[2];
static int selected, profile, mutation, header_count, rows, next_slot;
static void expected_record(unsigned char *bytes, int slot)
{
    OP_SHIELD *s = &expected[slot];
    unsigned int color;
    memset(bytes, 0, 52);
    memcpy(bytes, &slot, 4);
    memcpy(bytes + 4, &s->visible, 4);
    memcpy(bytes + 8, &s->elapsed, 4);
    memcpy(bytes + 12, s->name, 31);
    memcpy(bytes + 44, &s->radius, 4);
    color = (unsigned int)s->blue + ((unsigned int)s->green * 256u) + ((unsigned int)s->red * 65536u) +
            ((unsigned int)s->alpha * 16777216u);
    memcpy(bytes + 48, &color, 4);
}
void op_save_write(void *memory, unsigned int bytes)
{
    unsigned char record[52];
    int slot, alias;
    unsigned int byte;
    if (next_slot == -1)
    {
        CHECK(bytes == 4);
        CHECK(*(int *)memory == header_count);
        CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
        next_slot = 0;
        if (mutation)
        {
            *(int *)memory = header_count ? 0 : 32;
            if (selected < 31)
            {
                op_shields[selected + 1].active = expected[selected + 1].active = 1;
                op_shields[selected + 1].stopping = expected[selected + 1].stopping = 0;
                op_shields[selected + 1].no_save = expected[selected + 1].no_save = 0;
            }
        }
        return;
    }
    CHECK(bytes == 52);
    CHECK(rows < 32);
    slot = next_slot;
    while (slot < 32 && (!expected[slot].active || expected[slot].stopping || expected[slot].no_save))
        slot++;
    CHECK(slot < 32);
    if (slot >= 32)
        return;
    expected_record(record, slot);
    CHECK(memcmp(memory, record, 52) == 0);
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    alias = 0;
    for (byte = 0; byte < sizeof(op_shields); byte++)
        if (memory == (void *)((unsigned char *)op_shields + byte))
            alias = 1;
    CHECK(!alias);
    if (mutation)
    {
        ((unsigned char *)memory)[12] ^= 0x33;
        if (slot + 1 < 32)
        {
            op_shields[slot + 1].visible = expected[slot + 1].visible = -777;
            op_shields[slot + 1].alpha = expected[slot + 1].alpha = 0x81;
        }
        op_shields[slot].radius = expected[slot].radius = 23.5f;
    }
    rows++;
    next_slot = slot + 1;
}
int main(void)
{
    int i, j, result, expected_rows;
    unsigned int elapsed_bits = 0x80000000, radius_bits = 0x7fc12345;
    for (selected = 0; selected < 32; selected++)
        for (profile = 0; profile < 8; profile++)
            for (mutation = 0; mutation < 2; mutation++)
            {
                memset(actors, 0, sizeof(actors));
                memset(op_shields, 0, sizeof(op_shields));
                for (i = 0; i < 32; i++)
                {
                    op_shields[i].visible = INT_MIN + i;
                    op_shields[i].actor = &actors[i % 2];
                    op_shields[i].elapsed = (float)i + 0.25f;
                    op_shields[i].radius = (float)i - 16.5f;
                    op_shields[i].red = (unsigned char)(i * 7);
                    op_shields[i].green = (unsigned char)(255 - i);
                    op_shields[i].blue = (unsigned char)i;
                    op_shields[i].alpha = (unsigned char)(128 + i);
                    for (j = 0; j < 80; j++)
                        op_shields[i].name[j] = (char)('A' + (i + j) % 26);
                }
                op_shields[selected].active = profile & 1 ? -3 : 0;
                op_shields[selected].stopping = profile & 2 ? INT_MIN : 0;
                op_shields[selected].no_save = profile & 4 ? -1 : 0;
                header_count = (profile & 3) == 1 ? 1 : 0;
                memcpy(expected, op_shields, sizeof(expected));
                rows = 0;
                next_slot = -1;
                result = op_shield_write();
                CHECK(result == 0);
                CHECK(next_slot >= 0);
                expected_rows = 0;
                if (header_count)
                {
                    for (i = 0; i < 32; i++)
                        if (expected[i].active && !expected[i].stopping && !expected[i].no_save)
                            expected_rows++;
                }
                CHECK(rows == expected_rows);
                CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
            }
    /* Dense rows exercise the complete scan and the count/no_save discrepancy. */
    selected = 31;
    mutation = 0;
    memset(op_shields, 0, sizeof(op_shields));
    for (i = 0; i < 32; i++)
    {
        op_shields[i].active = 1;
        op_shields[i].no_save = i & 1;
        op_shields[i].visible = i;
        op_shields[i].alpha = 255;
        memcpy(&op_shields[i].elapsed, &elapsed_bits, 4);
        memcpy(&op_shields[i].radius, &radius_bits, 4);
        memset(op_shields[i].name, 'Q', 80);
    }
    memcpy(expected, op_shields, sizeof(expected));
    header_count = 32;
    rows = 0;
    next_slot = -1;
    CHECK(op_shield_write() == 0);
    CHECK(rows == 16);
    CHECK(memcmp(op_shields, expected, sizeof(expected)) == 0);
    printf("shield write: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
