#include "../src/effects_save.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>
static int sw_checks, sw_failures;
static void sw_check(int condition, int line)
{
    sw_checks++;
    if (!condition)
    {
        sw_failures++;
        printf("line %d failed\n", line);
    }
}
#define sw_CHECK(x) sw_check(!!(x), __LINE__)
static OP_SHIELD sw_expected[32];
static OP_ATTACHED_ACTOR sw_actors[2];
static int sw_selected, sw_profile, sw_mutation, sw_header_count, sw_rows, sw_next_slot;
static void sw_expected_record(unsigned char *bytes, int slot)
{
    OP_SHIELD *s = &sw_expected[slot];
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
void sw_op_save_write(void *memory, unsigned int bytes)
{
    unsigned char record[52];
    int slot, alias;
    unsigned int byte;
    if (sw_next_slot == -1)
    {
        sw_CHECK(bytes == 4);
        sw_CHECK(*(int *)memory == sw_header_count);
        sw_CHECK(memcmp(op_shields, sw_expected, sizeof(sw_expected)) == 0);
        sw_next_slot = 0;
        if (sw_mutation)
        {
            *(int *)memory = sw_header_count ? 0 : 32;
            if (sw_selected < 31)
            {
                op_shields[sw_selected + 1].active = sw_expected[sw_selected + 1].active = 1;
                op_shields[sw_selected + 1].stopping = sw_expected[sw_selected + 1].stopping = 0;
                op_shields[sw_selected + 1].no_save = sw_expected[sw_selected + 1].no_save = 0;
            }
        }
        return;
    }
    sw_CHECK(bytes == 52);
    sw_CHECK(sw_rows < 32);
    slot = sw_next_slot;
    while (slot < 32 && (!sw_expected[slot].active || sw_expected[slot].stopping || sw_expected[slot].no_save))
        slot++;
    sw_CHECK(slot < 32);
    if (slot >= 32)
        return;
    sw_expected_record(record, slot);
    sw_CHECK(memcmp(memory, record, 52) == 0);
    sw_CHECK(memcmp(op_shields, sw_expected, sizeof(sw_expected)) == 0);
    alias = 0;
    for (byte = 0; byte < sizeof(op_shields); byte++)
        if (memory == (void *)((unsigned char *)op_shields + byte))
            alias = 1;
    sw_CHECK(!alias);
    if (sw_mutation)
    {
        ((unsigned char *)memory)[12] ^= 0x33;
        if (slot + 1 < 32)
        {
            op_shields[slot + 1].visible = sw_expected[slot + 1].visible = -777;
            op_shields[slot + 1].alpha = sw_expected[slot + 1].alpha = 0x81;
        }
        op_shields[slot].radius = sw_expected[slot].radius = 23.5f;
    }
    sw_rows++;
    sw_next_slot = slot + 1;
}
int sw_main(void)
{
    int i, j, result, expected_rows;
    unsigned int elapsed_bits = 0x80000000, radius_bits = 0x7fc12345;
    for (sw_selected = 0; sw_selected < 32; sw_selected++)
        for (sw_profile = 0; sw_profile < 8; sw_profile++)
            for (sw_mutation = 0; sw_mutation < 2; sw_mutation++)
            {
                memset(sw_actors, 0, sizeof(sw_actors));
                memset(op_shields, 0, sizeof(op_shields));
                for (i = 0; i < 32; i++)
                {
                    op_shields[i].visible = INT_MIN + i;
                    op_shields[i].attached = &sw_actors[i % 2];
                    op_shields[i].elapsed = (float)i + 0.25f;
                    op_shields[i].radius = (float)i - 16.5f;
                    op_shields[i].red = (unsigned char)(i * 7);
                    op_shields[i].green = (unsigned char)(255 - i);
                    op_shields[i].blue = (unsigned char)i;
                    op_shields[i].alpha = (unsigned char)(128 + i);
                    for (j = 0; j < 80; j++)
                        op_shields[i].name[j] = (char)('A' + (i + j) % 26);
                }
                op_shields[sw_selected].active = sw_profile & 1 ? -3 : 0;
                op_shields[sw_selected].stopping = sw_profile & 2 ? INT_MIN : 0;
                op_shields[sw_selected].no_save = sw_profile & 4 ? -1 : 0;
                sw_header_count = (sw_profile & 3) == 1 ? 1 : 0;
                memcpy(sw_expected, op_shields, sizeof(sw_expected));
                sw_rows = 0;
                sw_next_slot = -1;
                result = op_shield_write();
                sw_CHECK(result == 0);
                sw_CHECK(sw_next_slot >= 0);
                expected_rows = 0;
                if (sw_header_count)
                {
                    for (i = 0; i < 32; i++)
                        if (sw_expected[i].active && !sw_expected[i].stopping && !sw_expected[i].no_save)
                            expected_rows++;
                }
                sw_CHECK(sw_rows == expected_rows);
                sw_CHECK(memcmp(op_shields, sw_expected, sizeof(sw_expected)) == 0);
            }
    /* Dense sw_rows exercise the complete scan and the count/no_save discrepancy. */
    sw_selected = 31;
    sw_mutation = 0;
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
    memcpy(sw_expected, op_shields, sizeof(sw_expected));
    sw_header_count = 32;
    sw_rows = 0;
    sw_next_slot = -1;
    sw_CHECK(op_shield_write() == 0);
    sw_CHECK(sw_rows == 16);
    sw_CHECK(memcmp(op_shields, sw_expected, sizeof(sw_expected)) == 0);
    printf("shield write: %d checks, %d failures\n", sw_checks, sw_failures);
    return sw_failures != 0;
}

#undef sw_CHECK
