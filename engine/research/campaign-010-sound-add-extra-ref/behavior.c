#include "api.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int op_sound_initialized;
int op_sound_ref_count;
int op_sound_extra_count;
void *op_sound_registry;
op_sound_ref_node *op_sound_extra_head;
static op_sound_ref_node nodes[4];
static char registries[2];
static int checks, failures, alloc_index, insert_calls;
static op_sound_ref_node *expected_node;
static int mutate_head;
static char expected_name[52];
static char *source_name;
static int expected_length;

static void check(int condition, const char *message)
{
    ++checks;
    if (!condition) {
        ++failures;
        printf("FAIL: %s\n", message);
    }
}

static int owned_node(op_sound_ref_node *value)
{
    return value == &nodes[0] || value == &nodes[1] ||
           value == &nodes[2] || value == &nodes[3];
}

void *op_sound_allocate_node(int size)
{
    check(size == 64, "allocation size");
    check(alloc_index >= 0 && alloc_index < 4, "allocation bound");
    op_sound_registry = &registries[1];
    if (alloc_index < 0 || alloc_index >= 4)
        return 0;
    return &nodes[alloc_index++];
}

int op_sound_insert_name(void *registry, const char *name,
                         op_sound_ref_node *value)
{
    int i;

    ++insert_calls;
    check(owned_node(value), "insert owned node");
    if (!owned_node(value))
        return -1;
    check(value == expected_node, "insert expected node");
    check(registry == &registries[1], "insert live registry");
    check(name == value->name, "insert name address");
    check(value->flags == 0 && value->resource == 0 && value->next == 0,
          "fields zero before insert");
    check(name != source_name, "independent name storage");
    check(memcmp(value->name, expected_name, expected_length + 1) == 0, "bounded copied name");
    source_name[0] = (char)(source_name[0] == 'z' ? 'y' : 'z');
    check(memcmp(value->name, expected_name, expected_length + 1) == 0, "copy survives source mutation");
    for (i = expected_length + 1; i < 52; ++i)
        check((unsigned char)value->name[i] == 0xa5,
              "name tail remains untouched");
    op_sound_ref_count += 10;
    op_sound_extra_count += 20;
    if (mutate_head) {
        nodes[3].next = 0;
        op_sound_extra_head = &nodes[3];
    }
    value->resource = &registries[0];
    return -1;
}

static void run_case(const char *name, int existing, int change_head)
{
    op_sound_ref_node *result;
    op_sound_ref_node expected;
    char local_name[52];
    int i;

    if(existing < 0 || existing > 3 || (change_head && existing == 3)) { check(0,"owned list case bounds"); return; }
    expected_length = (int)strlen(name);
    if(expected_length > 51) { check(0,"input name bound"); return; }
    memcpy(expected_name,name,expected_length+1);
    memcpy(local_name,name,expected_length+1);
    source_name=local_name;

    memset(nodes, 0xa5, sizeof(nodes));
    for (i = 0; i < existing; ++i)
        nodes[i].next = i + 1 < existing ? &nodes[i + 1] : 0;
    alloc_index = existing;
    expected_node = &nodes[existing];
    op_sound_extra_head = existing ? &nodes[0] : 0;
    op_sound_initialized = -1;
    op_sound_ref_count = 3;
    op_sound_extra_count = 4;
    op_sound_registry = &registries[0];
    insert_calls = 0;
    mutate_head = change_head;

    result = op_sound_add_extra_ref(local_name);
    check(result == expected_node, "returned allocated node");
    check(insert_calls == 1, "one insertion callback");
    check(op_sound_ref_count == 14 && op_sound_extra_count == 25,
          "live counter increments");

    memset(&expected, 0xa5, sizeof(expected));
    expected.flags = 0;
    memcpy(expected.name, expected_name, expected_length + 1);
    expected.resource = &registries[0];
    expected.next = 0;
    if(result != expected_node) return;
    check(memcmp(result, &expected, sizeof(expected)) == 0,
          "complete allocated record");
    if (change_head) {
        check(op_sound_extra_head == &nodes[3], "callback head retained");
        check(nodes[3].next == result, "appended to callback head");
    } else if (existing) {
        check(op_sound_extra_head == &nodes[0], "existing head retained");
        check(nodes[existing - 1].next == result, "appended at tail");
    } else {
        check(op_sound_extra_head == result, "empty-list head assigned");
    }
}

int main(void)
{
    char long_name[52];
    int i;

    check(sizeof(op_sound_ref_node) == 64, "node size");
    check(offsetof(op_sound_ref_node, name) == 4, "name offset");
    check(offsetof(op_sound_ref_node, resource) == 0x38, "resource offset");
    check(offsetof(op_sound_ref_node, next) == 0x3c, "next offset");

    op_sound_initialized = 0;
    alloc_index = 0;
    insert_calls = 0;
    check(op_sound_add_extra_ref(0) == 0, "initialization guard");
    check(alloc_index == 0 && insert_calls == 0, "guard has no callbacks");

    run_case("", 0, 0);
    run_case("", 0, 1);
    run_case("ordinary", 1, 0);
    for (i = 0; i < 51; ++i)
        long_name[i] = (char)('a' + i % 26);
    long_name[51] = 0;
    run_case(long_name, 3, 0);

    printf("Sound add extra ref: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
