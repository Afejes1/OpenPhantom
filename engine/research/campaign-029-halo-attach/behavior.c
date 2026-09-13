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
OP_HALO_COLOR op_halo_colors[5];
char op_halo_name_a[] = "unit-a", op_halo_name_b[] = "unit-b", op_halo_name_c[] = "unit-c";
static char names[4][13] = {"unit-alpha", "unit-bravo", "unit-charlie", "unit-delta"};
static OP_ACTOR actor, expected_actor;
static OP_DEFINITION definitions[2], expected_definitions[2];
static OP_HALO_COLOR expected_table[5];
static int matched, mutation, comparisons, adds;
static unsigned int selected_color;
static int fold(int c)
{
    return c >= 'A' && c <= 'Z' ? c + 32 : c;
}
int op_compare_names(char *a, char *b)
{
    int equal = 1, i;
    CHECK(a == definitions[0].name);
    CHECK(b == op_halo_colors[comparisons].name);
    CHECK(adds == 0);
    for (i = 0; a[i] || b[i]; i++)
    {
        if (fold((unsigned char)a[i]) != fold((unsigned char)b[i]))
        {
            equal = 0;
            break;
        }
    }
    if (mutation)
    {
        actor.definition = &definitions[1];
        expected_actor.definition = &definitions[1];
        op_halo_colors[comparisons].color ^= 0x12345678;
        expected_table[comparisons].color ^= 0x12345678;
    }
    comparisons++;
    return equal ? 0 : (comparisons & 1 ? -3 : 7);
}
void op_halo_add(OP_ACTOR *a, unsigned int key, int field8, int fieldc, char *name, unsigned int color, int mode)
{
    CHECK(a == &actor);
    CHECK(key == (unsigned int)(9 + adds));
    CHECK(field8 == 0);
    CHECK(fieldc == 2);
    CHECK(color == selected_color);
    CHECK(mode == (adds >= 4));
    CHECK(name == (adds < 4 ? op_halo_name_a : adds == 4 ? op_halo_name_b : op_halo_name_c));
    if (mutation && adds == 0)
    {
        op_halo_colors[0].color = 0x7890;
        expected_table[0].color = 0x7890;
        actor.flags ^= 0x80;
        expected_actor.flags ^= 0x80;
    }
    adds++;
}
int main(void)
{
    int i, k, upper;
    for (matched = -1; matched < 4; matched++)
        for (mutation = 0; mutation < 2; mutation++)
            for (upper = 0; upper < 2; upper++)
            {
                memset(&actor, 0, sizeof(actor));
                memset(definitions, 0, sizeof(definitions));
                actor.definition = &definitions[0];
                actor.flags = 0x8123;
                strcpy(definitions[0].name, matched < 0 ? "unit-absent" : names[matched]);
                strcpy(definitions[1].name, "different");
                if (upper)
                    for (k = 0; definitions[0].name[k]; k++)
                        if (definitions[0].name[k] >= 'a' && definitions[0].name[k] <= 'z')
                            definitions[0].name[k] -= 32;
                for (i = 0; i < 4; i++)
                {
                    op_halo_colors[i].name = names[i];
                    op_halo_colors[i].color = 0x23450000u + (unsigned int)i;
                }
                op_halo_colors[4].name = 0;
                op_halo_colors[4].color = 0xabcdef;
                selected_color = matched < 0 ? 0xffffff : op_halo_colors[matched].color ^ (mutation ? 0x12345678 : 0);
                expected_actor = actor;
                memcpy(expected_definitions, definitions, sizeof(definitions));
                memcpy(expected_table, op_halo_colors, sizeof(expected_table));
                comparisons = adds = 0;
                op_halo_attach(&actor);
                CHECK(comparisons == (matched < 0 ? 4 : matched + 1));
                CHECK(adds == 6);
                CHECK(memcmp(&actor, &expected_actor, sizeof(actor)) == 0);
                CHECK(memcmp(definitions, expected_definitions, sizeof(definitions)) == 0);
                CHECK(memcmp(op_halo_colors, expected_table, sizeof(expected_table)) == 0);
            }
    /* Empty table still produces six default-color attachments. */
    matched = -1;
    mutation = 0;
    op_halo_colors[0].name = 0;
    selected_color = 0xffffff;
    comparisons = adds = 0;
    actor.definition = &definitions[0];
    expected_actor = actor;
    memcpy(expected_definitions, definitions, sizeof(definitions));
    memcpy(expected_table, op_halo_colors, sizeof(expected_table));
    op_halo_attach(&actor);
    CHECK(comparisons == 0);
    CHECK(adds == 6);
    CHECK(memcmp(&actor, &expected_actor, sizeof(actor)) == 0);
    CHECK(memcmp(definitions, expected_definitions, sizeof(definitions)) == 0);
    CHECK(memcmp(op_halo_colors, expected_table, sizeof(expected_table)) == 0);
    printf("halo attach: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
