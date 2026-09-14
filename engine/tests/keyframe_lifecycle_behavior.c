#include "../src/face_keyframe.h"
#include "../src/material_hooks.h"
#include <stdio.h>
#include <string.h>
static int kc_active, kc_mode;

#include <stdio.h>
#include <string.h>
#include <limits.h>
static int ke_checks, ke_failures;
static void ke_check(int ok, int line)
{
    ++ke_checks;
    if (!ok)
    {
        ++ke_failures;
        printf("line %d failed\n", line);
    }
}
#define ke_CHECK(x) ke_check(!!(x), __LINE__)
typedef struct ke_OWNED_FRAME
{
    unsigned int before;
    OP_KEYFRAME value;
    unsigned int after;
} ke_OWNED_FRAME;
static ke_OWNED_FRAME ke_frames[3], ke_expected[3];
static int ke_stage;
static void ke_seed(void)
{
    memset(ke_frames, 0x69, sizeof(ke_frames));
    memcpy(ke_expected, ke_frames, sizeof(ke_frames));
    ke_stage = 0;
}
static void ke_verify(void)
{
    ke_CHECK(memcmp(ke_frames, ke_expected, sizeof(ke_frames)) == 0);
}
typedef struct ke_OWNED_NODES
{
    unsigned int before;
    OP_KEYFRAME_NODE rows[4];
    unsigned int after;
} ke_OWNED_NODES;
static ke_OWNED_NODES ke_nodes[3], ke_expected_nodes[3];
static unsigned int ke_tokens[10];
static void *ke_plan[5];
static int ke_plan_node[5], ke_plan_count, ke_profile;
static void ke_verify_all(void)
{
    ke_verify();
    ke_CHECK(memcmp(ke_nodes, ke_expected_nodes, sizeof(ke_nodes)) == 0);
}
void kc_entry_release(void *p)
{
    int index;
    if (ke_stage >= ke_plan_count || p != ke_plan[ke_stage])
    {
        ke_CHECK(0);
        return;
    }
    ke_CHECK(1);
    ke_verify_all();
    index = ke_plan_node[ke_stage];
    if (index >= 0)
    {
        if (ke_stage == 0)
        {
            if (ke_profile == 1)
                ke_frames[0].value.node_count = ke_expected[0].value.node_count = 0;
            if (ke_profile == 2)
                ke_frames[0].value.node_count = ke_expected[0].value.node_count = 4;
            if (ke_profile == 3)
                ke_frames[0].value.nodes = ke_expected[0].value.nodes = ke_nodes[1].rows;
            if (ke_profile == 4)
            {
                ke_nodes[0].rows[0].payload = &ke_tokens[8];
                ke_nodes[0].rows[1].payload = ke_expected_nodes[0].rows[1].payload = &ke_tokens[9];
            }
            if (ke_profile == 5)
                ke_nodes[0].rows[1].payload = ke_expected_nodes[0].rows[1].payload = 0;
        }
        ke_expected_nodes[0].rows[index].payload = 0;
    }
    else
    {
        ke_frames[0].value.nodes = ke_nodes[2].rows;
        ke_expected[0].value.nodes = 0;
    }
    ++ke_stage;
}
static void ke_prepare(int count, int mask, int has_array)
{
    int i;
    ke_seed();
    memset(ke_nodes, 0x57, sizeof(ke_nodes));
    for (i = 0; i < 4; ++i)
    {
        ke_nodes[0].rows[i].payload = (mask & (1 << i)) ? &ke_tokens[i] : 0;
        ke_nodes[1].rows[i].payload = &ke_tokens[i + 4];
        ke_nodes[2].rows[i].payload = &ke_tokens[i + 5];
    }
    memcpy(ke_expected_nodes, ke_nodes, sizeof(ke_nodes));
    ke_frames[0].value.node_count = (unsigned int)count;
    ke_frames[0].value.nodes = has_array ? ke_nodes[0].rows : 0;
    memcpy(ke_expected, ke_frames, sizeof(ke_frames));
    ke_plan_count = 0;
}
static void ke_add(int index, void *p)
{
    ke_plan_node[ke_plan_count] = index;
    ke_plan[ke_plan_count++] = p;
}
static void ke_run(void)
{
    op_keyframe_free_entry(&ke_frames[0].value);
    ke_CHECK(ke_stage == ke_plan_count);
    ke_verify_all();
}
static int kc_test_keyframe_free_entry(void)
{
    int count, mask, i;
    ke_profile = 0;
    for (count = 0; count <= 4; ++count)
        for (mask = 0; mask < 16; ++mask)
        {
            ke_prepare(count, mask, 1);
            for (i = 0; i < count; ++i)
                if (mask & (1 << i))
                    ke_add(i, &ke_tokens[i]);
            ke_add(-1, ke_nodes[0].rows);
            ke_run();
        }
    for (count = 0; count <= 4; ++count)
    {
        ke_prepare(count, 15, 0);
        ke_run();
    }
    for (ke_profile = 1; ke_profile <= 5; ++ke_profile)
    {
        ke_prepare(ke_profile == 2 ? 1 : 3, 15, 1);
        ke_add(0, &ke_tokens[0]);
        if (ke_profile != 1)
        {
            if (ke_profile != 5)
                ke_add(1, ke_profile == 4 ? &ke_tokens[9] : &ke_tokens[1]);
            ke_add(2, &ke_tokens[2]);
            if (ke_profile == 2)
                ke_add(3, &ke_tokens[3]);
        }
        ke_add(-1, ke_profile == 3 ? ke_nodes[1].rows : ke_nodes[0].rows);
        ke_run();
    }
    printf("keyframe_free_entry: %d checks, %d failures\n", ke_checks, ke_failures);
    return ke_failures != 0;
}

#undef ke_CHECK

#include <stdio.h>
#include <string.h>
#include <limits.h>
static int ml_checks, ml_failures;
static void ml_check(int ok, int line)
{
    ++ml_checks;
    if (!ok)
    {
        ++ml_failures;
        printf("line %d failed\n", line);
    }
}
#define ml_CHECK(x) ml_check(!!(x), __LINE__)
OP_MATERIAL_LOAD_HOOK op_material_load_hook;
OP_MATERIAL_UNLOAD_HOOK op_material_unload_hook;
static int ml_invoked;
static OP_MATERIAL *ml_load_a(char *name, void *a, void *b)
{
    (void)name;
    (void)a;
    (void)b;
    ++ml_invoked;
    return 0;
}
static OP_MATERIAL *ml_load_b(char *name, void *a, void *b)
{
    (void)name;
    (void)a;
    (void)b;
    ++ml_invoked;
    return 0;
}
static void ml_unload_a(OP_MATERIAL *material)
{
    (void)material;
    ++ml_invoked;
}
static void ml_unload_b(OP_MATERIAL *material)
{
    (void)material;
    ++ml_invoked;
}
static int kc_test_material_set_load_hook(void)
{
    int old, new, repeat;
    OP_MATERIAL_LOAD_HOOK choices[3];
    choices[0] = 0;
    choices[1] = ml_load_a;
    choices[2] = ml_load_b;
    for (old = 0; old < 3; ++old)
        for (new = 0; new < 3; ++new)
            for (repeat = 0; repeat < 2; ++repeat)
            {
                ml_invoked = 0;
                op_material_load_hook = choices[old];
                op_material_unload_hook = repeat ? ml_unload_a : ml_unload_b;
                ml_CHECK(op_material_set_load_hook(choices[new]) == choices[old]);
                ml_CHECK(op_material_load_hook == choices[new]);
                ml_CHECK(op_material_unload_hook == (repeat ? ml_unload_a : ml_unload_b));
                if (repeat)
                {
                    ml_CHECK(op_material_set_load_hook(choices[new]) == choices[new]);
                    ml_CHECK(op_material_load_hook == choices[new]);
                }
                ml_CHECK(ml_invoked == 0);
            }
    printf("material_set_load_hook: %d checks, %d failures\n", ml_checks, ml_failures);
    return ml_failures != 0;
}

#undef ml_CHECK

#include <stdio.h>
#include <string.h>
#include <limits.h>
static int mu_checks, mu_failures;
static void mu_check(int ok, int line)
{
    ++mu_checks;
    if (!ok)
    {
        ++mu_failures;
        printf("line %d failed\n", line);
    }
}
#define mu_CHECK(x) mu_check(!!(x), __LINE__)

static int mu_invoked;
static OP_MATERIAL *mu_load_a(char *name, void *a, void *b)
{
    (void)name;
    (void)a;
    (void)b;
    ++mu_invoked;
    return 0;
}
static OP_MATERIAL *mu_load_b(char *name, void *a, void *b)
{
    (void)name;
    (void)a;
    (void)b;
    ++mu_invoked;
    return 0;
}
static void mu_unload_a(OP_MATERIAL *material)
{
    (void)material;
    ++mu_invoked;
}
static void mu_unload_b(OP_MATERIAL *material)
{
    (void)material;
    ++mu_invoked;
}
static int kc_test_material_set_unload_hook(void)
{
    int old, new, repeat;
    OP_MATERIAL_UNLOAD_HOOK choices[3];
    choices[0] = 0;
    choices[1] = mu_unload_a;
    choices[2] = mu_unload_b;
    for (old = 0; old < 3; ++old)
        for (new = 0; new < 3; ++new)
            for (repeat = 0; repeat < 2; ++repeat)
            {
                mu_invoked = 0;
                op_material_unload_hook = choices[old];
                op_material_load_hook = repeat ? mu_load_a : mu_load_b;
                mu_CHECK(op_material_set_unload_hook(choices[new]) == choices[old]);
                mu_CHECK(op_material_unload_hook == choices[new]);
                mu_CHECK(op_material_load_hook == (repeat ? mu_load_a : mu_load_b));
                if (repeat)
                {
                    mu_CHECK(op_material_set_unload_hook(choices[new]) == choices[new]);
                    mu_CHECK(op_material_unload_hook == choices[new]);
                }
                mu_CHECK(mu_invoked == 0);
            }
    printf("material_set_unload_hook: %d checks, %d failures\n", mu_checks, mu_failures);
    return mu_failures != 0;
}

#undef mu_CHECK

typedef struct KC_FRAME
{
    unsigned int before;
    OP_KEYFRAME value;
    unsigned int after;
} KC_FRAME;
typedef struct KC_NODES
{
    unsigned int before;
    OP_KEYFRAME_NODE rows[4];
    unsigned int after;
} KC_NODES;
static KC_FRAME kc_frames[3], kc_expected[3];
static KC_NODES kc_nodes, kc_expected_nodes;
static unsigned int kc_tokens[4];
static int kc_stage, kc_count, kc_mask, kc_allocate_ok, kc_load_result, kc_custom_unload, kc_hook_result, kc_checks,
    kc_failures, kc_plan_count, kc_next;
static void *kc_plan[6];
static int kc_plan_index[6];
static char kc_name[] = "authored-connected-keyframe";
static void kc_check(int ok, int line)
{
    ++kc_checks;
    if (!ok)
    {
        ++kc_failures;
        printf("keyframe connected line%d failed\n", line);
    }
}
#define KC_CHECK(x) kc_check(!!(x), __LINE__)
static void kc_verify(void)
{
    KC_CHECK(memcmp(kc_frames, kc_expected, sizeof(kc_frames)) == 0);
    KC_CHECK(memcmp(&kc_nodes, &kc_expected_nodes, sizeof(kc_nodes)) == 0);
}
static OP_KEYFRAME *kc_load_hook(char *name)
{
    KC_CHECK(kc_stage == 0 && name == kc_name);
    kc_verify();
    op_keyframe_set_load_hook(0);
    kc_stage = 5;
    return kc_hook_result ? &kc_frames[2].value : 0;
}
static void kc_unload_hook(OP_KEYFRAME *entry)
{
    KC_CHECK(kc_stage == 2 && entry == &kc_frames[0].value);
    kc_verify();
    entry->node_count = kc_expected[0].value.node_count = 99;
    op_keyframe_set_unload_hook(0);
    kc_stage = 4;
}
static void kc_seed(void)
{
    int i;
    memset(kc_frames, 0x69, sizeof(kc_frames));
    memset(&kc_nodes, 0x57, sizeof(kc_nodes));
    for (i = 0; i < 4; ++i)
        kc_nodes.rows[i].payload = (kc_mask & (1 << i)) ? &kc_tokens[i] : 0;
    memcpy(kc_expected, kc_frames, sizeof(kc_frames));
    memcpy(&kc_expected_nodes, &kc_nodes, sizeof(kc_nodes));
    op_keyframe_set_load_hook(0);
    op_keyframe_set_unload_hook(0);
    kc_stage = kc_next = kc_plan_count = 0;
    for (i = 0; i < kc_count; ++i)
        if (kc_mask & (1 << i))
        {
            kc_plan_index[kc_plan_count] = i;
            kc_plan[kc_plan_count++] = &kc_tokens[i];
        }
    kc_plan_index[kc_plan_count] = -1;
    kc_plan[kc_plan_count++] = kc_nodes.rows;
    kc_plan_index[kc_plan_count] = -2;
    kc_plan[kc_plan_count++] = &kc_frames[0].value;
}
static void *kc_allocate(unsigned int bytes)
{
    KC_CHECK(kc_mode == 2 && kc_stage == 0 && bytes == 132);
    kc_verify();
    op_keyframe_set_load_hook(kc_load_hook);
    kc_stage = 1;
    return kc_allocate_ok ? &kc_frames[0].value : 0;
}
int op_keyframe_load_entry(char *name, OP_KEYFRAME *entry)
{
    KC_CHECK(kc_mode == 2 && kc_stage == 1 && name == kc_name && entry == &kc_frames[0].value);
    kc_verify();
    entry->node_count = kc_expected[0].value.node_count = (unsigned int)kc_count;
    entry->nodes = kc_expected[0].value.nodes = kc_nodes.rows;
    op_keyframe_set_load_hook(0);
    op_keyframe_set_unload_hook(kc_custom_unload ? kc_unload_hook : 0);
    kc_stage = 2;
    return kc_load_result;
}
static void kc_release(void *p)
{
    int index;
    if (kc_mode == 1)
    {
        kc_entry_release(p);
        return;
    }
    if (kc_stage != 2 || kc_next >= kc_plan_count || p != kc_plan[kc_next])
    {
        KC_CHECK(0);
        return;
    }
    KC_CHECK(1);
    kc_verify();
    index = kc_plan_index[kc_next];
    if (index >= 0)
    {
        kc_nodes.rows[index].payload = &kc_tokens[(index + 1) % 4];
        kc_expected_nodes.rows[index].payload = 0;
    }
    else if (index == -1)
    {
        kc_frames[0].value.nodes = (OP_KEYFRAME_NODE *)&kc_frames[2].value;
        kc_expected[0].value.nodes = 0;
    }
    else
        kc_stage = 3;
    ++kc_next;
}
static int op_test_keyframe_lifecycle(void)
{
    int a, r, u, h;
    OP_KEYFRAME *result;
    static int results[] = {0, 1, -1};
    kc_active = 1;
    kc_mode = 1;
    kc_test_keyframe_free_entry();
    kc_test_material_set_load_hook();
    kc_test_material_set_unload_hook();
    kc_mode = 2;
    for (kc_count = 0; kc_count <= 4; ++kc_count)
        for (kc_mask = 0; kc_mask < 16; ++kc_mask)
            for (a = 0; a < 2; ++a)
                for (r = 0; r < 3; ++r)
                    for (u = 0; u < 2; ++u)
                    {
                        kc_allocate_ok = a;
                        kc_load_result = results[r];
                        kc_custom_unload = u;
                        kc_seed();
                        result = op_keyframe_load(kc_name);
                        KC_CHECK(result == (a && results[r] ? &kc_frames[0].value : 0));
                        KC_CHECK(kc_stage == (!a ? 1 : (results[r] ? 2 : (u ? 4 : 3))));
                        kc_verify();
                        if (a && results[r])
                            op_keyframe_free(result);
                        KC_CHECK(kc_stage == (!a ? 1 : (u ? 4 : 3)));
                        KC_CHECK(kc_next == (!a || u ? 0 : kc_plan_count));
                        KC_CHECK(op_keyframe_load_hook == (!a ? kc_load_hook : 0));
                        KC_CHECK(op_keyframe_unload_hook == 0);
                        kc_verify();
                    }
    for (h = 0; h < 2; ++h)
    {
        kc_count = 0;
        kc_mask = 0;
        kc_seed();
        kc_hook_result = h;
        op_keyframe_set_load_hook(kc_load_hook);
        result = op_keyframe_load(kc_name);
        KC_CHECK(result == (h ? &kc_frames[2].value : 0));
        KC_CHECK(kc_stage == 5 && kc_next == 0 && op_keyframe_load_hook == 0);
        kc_verify();
    }
    kc_seed();
    op_keyframe_set_unload_hook(kc_unload_hook);
    op_keyframe_free(0);
    KC_CHECK(kc_stage == 0 && kc_next == 0 && op_keyframe_unload_hook == kc_unload_hook);
    kc_verify();
    op_keyframe_set_unload_hook(0);
    kc_active = 0;
    printf("keyframe lifecycle connected: %d checks, %d failures\n", kc_checks + ke_checks + ml_checks + mu_checks,
           kc_failures + ke_failures + ml_failures + mu_failures);
    return kc_failures + ke_failures + ml_failures + mu_failures != 0;
}
