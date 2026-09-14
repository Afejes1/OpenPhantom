#include "../src/module.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int min_checks, min_failures;
static void min_check(int ok, int line)
{
    ++min_checks;
    if (!ok)
    {
        ++min_failures;
        printf("line%d failed\n", line);
    }
}
#define MIN_CHECK(x) min_check(!!(x), __LINE__)
typedef struct MIN_OWNED_MODULE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MIN_OWNED_MODULE;
static MIN_OWNED_MODULE min_nodes[5], min_expected_nodes[5];
OP_MODULE *op_module_head, *op_module_tail;
int op_module_initialized;
static OP_MODULE *min_expected_head, *min_expected_tail;
static int min_expected_initialized;
static const int min_keys[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const char *min_names[] = {"alpha", "beta", "gamma", "delta", "omega"};
static int min_order[5], min_length, min_rotation, min_duplicates;
static void min_seed(void)
{
    int i, n;
    memset(min_nodes, 0x57, sizeof(min_nodes));
    for (i = 0; i < 5; ++i)
    {
        min_nodes[i].value.next = min_nodes[i].value.previous = 0;
        min_nodes[i].value.id = min_keys[i];
        memset(min_nodes[i].value.name, 0, sizeof(min_nodes[i].value.name));
        memcpy(min_nodes[i].value.name, min_names[i], strlen(min_names[i]) + 1);
        min_order[i] = (i + min_rotation) % 5;
    }
    for (i = 0; i < min_length; ++i)
    {
        n = min_order[i];
        min_nodes[n].value.previous = i ? &min_nodes[min_order[i - 1]].value : 0;
        min_nodes[n].value.next = i + 1 < min_length ? &min_nodes[min_order[i + 1]].value : 0;
    }
    if (min_duplicates && min_length > 1)
        min_nodes[min_order[1]].value.id = min_nodes[min_order[0]].value.id;
    memcpy(min_expected_nodes, min_nodes, sizeof(min_nodes));
    op_module_head = min_expected_head = min_length ? &min_nodes[min_order[0]].value : 0;
    op_module_tail = min_expected_tail = min_length ? &min_nodes[min_order[min_length - 1]].value : 0;
    op_module_initialized = min_expected_initialized = 0x12345678;
}
static void min_verify(void)
{
    MIN_CHECK(memcmp(min_nodes, min_expected_nodes, sizeof(min_nodes)) == 0);
    MIN_CHECK(op_module_head == min_expected_head && op_module_tail == min_expected_tail &&
              op_module_initialized == min_expected_initialized);
}

static int op_test_module_initialize_once(void)
{
    int k;
    for (min_length = 0; min_length < 6; ++min_length)
        for (min_rotation = 0; min_rotation < 5; ++min_rotation)
            for (min_duplicates = 0; min_duplicates < 2; ++min_duplicates)
                for (k = 0; k < 6; ++k)
                {
                    min_seed();
                    op_module_initialized = min_keys[k];
                    min_expected_initialized = min_keys[k] ? min_keys[k] : 1;
                    MIN_CHECK(op_module_initialize_once() == (min_keys[k] != 0));
                    min_verify();
                    MIN_CHECK(op_module_initialize_once() == 1);
                    min_verify();
                }
    printf("module_initialize_once: %d checks, %d failures\n", min_checks, min_failures);
    return min_failures != 0;
}

#undef MIN_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int mid_checks, mid_failures;
static void mid_check(int ok, int line)
{
    ++mid_checks;
    if (!ok)
    {
        ++mid_failures;
        printf("line%d failed\n", line);
    }
}
#define MID_CHECK(x) mid_check(!!(x), __LINE__)
typedef struct MID_OWNED_MODULE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MID_OWNED_MODULE;
static MID_OWNED_MODULE mid_nodes[5], mid_expected_nodes[5];
static OP_MODULE *mid_expected_head, *mid_expected_tail;
static int mid_expected_initialized;
static const int mid_keys[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const char *mid_names[] = {"alpha", "beta", "gamma", "delta", "omega"};
static int mid_order[5], mid_length, mid_rotation, mid_duplicates;
static void mid_seed(void)
{
    int i, n;
    memset(mid_nodes, 0x57, sizeof(mid_nodes));
    for (i = 0; i < 5; ++i)
    {
        mid_nodes[i].value.next = mid_nodes[i].value.previous = 0;
        mid_nodes[i].value.id = mid_keys[i];
        memset(mid_nodes[i].value.name, 0, sizeof(mid_nodes[i].value.name));
        memcpy(mid_nodes[i].value.name, mid_names[i], strlen(mid_names[i]) + 1);
        mid_order[i] = (i + mid_rotation) % 5;
    }
    for (i = 0; i < mid_length; ++i)
    {
        n = mid_order[i];
        mid_nodes[n].value.previous = i ? &mid_nodes[mid_order[i - 1]].value : 0;
        mid_nodes[n].value.next = i + 1 < mid_length ? &mid_nodes[mid_order[i + 1]].value : 0;
    }
    if (mid_duplicates && mid_length > 1)
        mid_nodes[mid_order[1]].value.id = mid_nodes[mid_order[0]].value.id;
    memcpy(mid_expected_nodes, mid_nodes, sizeof(mid_nodes));
    op_module_head = mid_expected_head = mid_length ? &mid_nodes[mid_order[0]].value : 0;
    op_module_tail = mid_expected_tail = mid_length ? &mid_nodes[mid_order[mid_length - 1]].value : 0;
    op_module_initialized = mid_expected_initialized = 0x12345678;
}
static void mid_verify(void)
{
    MID_CHECK(memcmp(mid_nodes, mid_expected_nodes, sizeof(mid_nodes)) == 0);
    MID_CHECK(op_module_head == mid_expected_head && op_module_tail == mid_expected_tail &&
              op_module_initialized == mid_expected_initialized);
}

static int op_test_module_find_by_id(void)
{
    int key_case, i;
    OP_MODULE *expected;
    for (mid_length = 0; mid_length < 6; ++mid_length)
        for (mid_rotation = 0; mid_rotation < 5; ++mid_rotation)
            for (mid_duplicates = 0; mid_duplicates < 2; ++mid_duplicates)
                for (key_case = 0; key_case < 6; ++key_case)
                {
                    mid_seed();
                    expected = 0;
                    for (i = 0; i < mid_length; ++i)
                        if (mid_expected_nodes[mid_order[i]].value.id == mid_keys[key_case])
                        {
                            expected = &mid_nodes[mid_order[i]].value;
                            break;
                        }
                    MID_CHECK(op_module_find_by_id(mid_keys[key_case]) == expected);
                    mid_verify();
                }
    printf("module_find_by_id: %d checks, %d failures\n", mid_checks, mid_failures);
    return mid_failures != 0;
}

#undef MID_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int mnm_active;
static int mnm_checks, mnm_failures;
static void mnm_check(int ok, int line)
{
    ++mnm_checks;
    if (!ok)
    {
        ++mnm_failures;
        printf("line%d failed\n", line);
    }
}
#define MNM_CHECK(x) mnm_check(!!(x), __LINE__)
typedef struct MNM_OWNED_MODULE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MNM_OWNED_MODULE;
static MNM_OWNED_MODULE mnm_nodes[5], mnm_expected_nodes[5];
static OP_MODULE *mnm_expected_head, *mnm_expected_tail;
static int mnm_expected_initialized;
static const int mnm_keys[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const char *mnm_names[] = {"alpha", "beta", "gamma", "delta", "omega"};
static int mnm_order[5], mnm_length, mnm_rotation, mnm_duplicates;
static void mnm_seed(void)
{
    int i, n;
    memset(mnm_nodes, 0x57, sizeof(mnm_nodes));
    for (i = 0; i < 5; ++i)
    {
        mnm_nodes[i].value.next = mnm_nodes[i].value.previous = 0;
        mnm_nodes[i].value.id = mnm_keys[i];
        memset(mnm_nodes[i].value.name, 0, sizeof(mnm_nodes[i].value.name));
        memcpy(mnm_nodes[i].value.name, mnm_names[i], strlen(mnm_names[i]) + 1);
        mnm_order[i] = (i + mnm_rotation) % 5;
    }
    for (i = 0; i < mnm_length; ++i)
    {
        n = mnm_order[i];
        mnm_nodes[n].value.previous = i ? &mnm_nodes[mnm_order[i - 1]].value : 0;
        mnm_nodes[n].value.next = i + 1 < mnm_length ? &mnm_nodes[mnm_order[i + 1]].value : 0;
    }
    if (mnm_duplicates && mnm_length > 1)
        mnm_nodes[mnm_order[1]].value.id = mnm_nodes[mnm_order[0]].value.id;
    memcpy(mnm_expected_nodes, mnm_nodes, sizeof(mnm_nodes));
    op_module_head = mnm_expected_head = mnm_length ? &mnm_nodes[mnm_order[0]].value : 0;
    op_module_tail = mnm_expected_tail = mnm_length ? &mnm_nodes[mnm_order[mnm_length - 1]].value : 0;
    op_module_initialized = mnm_expected_initialized = 0x12345678;
}
static void mnm_verify(void)
{
    MNM_CHECK(memcmp(mnm_nodes, mnm_expected_nodes, sizeof(mnm_nodes)) == 0);
    MNM_CHECK(op_module_head == mnm_expected_head && op_module_tail == mnm_expected_tail &&
              op_module_initialized == mnm_expected_initialized);
}

typedef struct MNM_OWNED_NAME
{
    unsigned int before;
    char text[16];
    unsigned int after;
} MNM_OWNED_NAME;
static MNM_OWNED_NAME mnm_input, mnm_expected_input;
static int mnm_input_case, mnm_mutate, mnm_calls, mnm_next_position, mnm_expected_result, mnm_matched;
static unsigned int mnm_count;
static int mnm_index_of(OP_MODULE *p)
{
    int i;
    for (i = 0; i < 5; ++i)
        if (p == &mnm_nodes[i].value)
            return i;
    return -1;
}
static int mnm_compare(const char *left, const char *right, unsigned int n)
{
    int index, result, next;
    MNM_CHECK(!mnm_matched);
    if (mnm_matched)
        return 0;
    MNM_CHECK(mnm_next_position >= 0 && mnm_next_position < mnm_length);
    if (mnm_next_position < 0 || mnm_next_position >= mnm_length)
        return 1;
    index = mnm_order[mnm_next_position];
    MNM_CHECK(left == mnm_input.text && right == mnm_nodes[index].value.name && n == mnm_count && n <= 16);
    if (left != mnm_input.text || right != mnm_nodes[index].value.name || n > 16)
        return 1;
    mnm_verify();
    MNM_CHECK(memcmp(&mnm_input, &mnm_expected_input, sizeof(mnm_input)) == 0);
    result = strncmp(mnm_input.text, mnm_nodes[index].value.name, n);
    ++mnm_calls;
    if (mnm_mutate)
    {
        op_module_head = mnm_expected_head = 0;
        op_module_tail = mnm_expected_tail = 0;
        if (result == 0)
        {
            mnm_nodes[index].value.id = mnm_expected_nodes[index].value.id = INT_MIN + index;
        }
        else if (mnm_next_position + 2 < mnm_length)
        {
            mnm_nodes[index].value.next = mnm_expected_nodes[index].value.next =
                &mnm_nodes[mnm_order[mnm_next_position + 2]].value;
        }
    }
    if (result == 0)
    {
        mnm_expected_result = mnm_expected_nodes[index].value.id;
        mnm_matched = 1;
    }
    else
    {
        next = mnm_index_of(mnm_expected_nodes[index].value.next);
        if (next < 0)
            mnm_next_position = mnm_length;
        else if (mnm_next_position + 2 < mnm_length && next == mnm_order[mnm_next_position + 2])
            mnm_next_position += 2;
        else
            ++mnm_next_position;
    }
    mnm_verify();
    return result;
}
static int op_test_module_find_by_name(void)
{
    int result;
    static const unsigned int counts[] = {0, 1, 3, 5, 15, 16};
    int count_case;
    mnm_active = 1;
    for (mnm_length = 0; mnm_length < 6; ++mnm_length)
        for (mnm_rotation = 0; mnm_rotation < 5; ++mnm_rotation)
            for (mnm_duplicates = 0; mnm_duplicates < 2; ++mnm_duplicates)
                for (mnm_input_case = 0; mnm_input_case < 6; ++mnm_input_case)
                    for (count_case = 0; count_case < 6; ++count_case)
                        for (mnm_mutate = 0; mnm_mutate < 2; ++mnm_mutate)
                        {
                            mnm_seed();
                            memset(&mnm_input, 0x63, sizeof(mnm_input));
                            memset(mnm_input.text, 0, sizeof(mnm_input.text));
                            if (mnm_input_case < 5)
                                memcpy(mnm_input.text, mnm_names[mnm_input_case],
                                       strlen(mnm_names[mnm_input_case]) + 1);
                            else
                                memcpy(mnm_input.text, "absent", 7);
                            memcpy(&mnm_expected_input, &mnm_input, sizeof(mnm_input));
                            mnm_count = counts[count_case];
                            mnm_calls = mnm_next_position = mnm_expected_result = mnm_matched = 0;
                            result = op_module_find_by_name(mnm_input.text, mnm_count);
                            MNM_CHECK(result == mnm_expected_result);
                            MNM_CHECK(mnm_calls <= mnm_length);
                            MNM_CHECK(mnm_matched || mnm_next_position == mnm_length);
                            if (mnm_length && mnm_count == 0)
                                MNM_CHECK(mnm_calls == 1);
                            mnm_verify();
                            MNM_CHECK(memcmp(&mnm_input, &mnm_expected_input, sizeof(mnm_input)) == 0);
                        }
    mnm_active = 0;
    printf("module_find_by_name: %d checks, %d failures\n", mnm_checks, mnm_failures);
    return mnm_failures != 0;
}

#undef MNM_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
static int mlc_checks, mlc_failures;
static void mlc_check(int ok, int line)
{
    ++mlc_checks;
    if (!ok)
    {
        ++mlc_failures;
        printf("line%d failed\n", line);
    }
}
#define MLC_CHECK(x) mlc_check(!!(x), __LINE__)
typedef struct MLC_OWNED_MODULE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MLC_OWNED_MODULE;
static MLC_OWNED_MODULE mlc_nodes[5], mlc_expected_nodes[5];
static OP_MODULE *mlc_expected_head, *mlc_expected_tail;
static int mlc_expected_initialized;
static const int mlc_keys[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const char *mlc_names[] = {"alpha", "beta", "gamma", "delta", "omega"};
static int mlc_order[5], mlc_length, mlc_rotation, mlc_duplicates;
static void mlc_seed(void)
{
    int i, n;
    memset(mlc_nodes, 0x57, sizeof(mlc_nodes));
    for (i = 0; i < 5; ++i)
    {
        mlc_nodes[i].value.next = mlc_nodes[i].value.previous = 0;
        mlc_nodes[i].value.id = mlc_keys[i];
        memset(mlc_nodes[i].value.name, 0, sizeof(mlc_nodes[i].value.name));
        memcpy(mlc_nodes[i].value.name, mlc_names[i], strlen(mlc_names[i]) + 1);
        mlc_order[i] = (i + mlc_rotation) % 5;
    }
    for (i = 0; i < mlc_length; ++i)
    {
        n = mlc_order[i];
        mlc_nodes[n].value.previous = i ? &mlc_nodes[mlc_order[i - 1]].value : 0;
        mlc_nodes[n].value.next = i + 1 < mlc_length ? &mlc_nodes[mlc_order[i + 1]].value : 0;
    }
    if (mlc_duplicates && mlc_length > 1)
        mlc_nodes[mlc_order[1]].value.id = mlc_nodes[mlc_order[0]].value.id;
    memcpy(mlc_expected_nodes, mlc_nodes, sizeof(mlc_nodes));
    op_module_head = mlc_expected_head = mlc_length ? &mlc_nodes[mlc_order[0]].value : 0;
    op_module_tail = mlc_expected_tail = mlc_length ? &mlc_nodes[mlc_order[mlc_length - 1]].value : 0;
    op_module_initialized = mlc_expected_initialized = 0x12345678;
}
static void mlc_verify(void)
{
    MLC_CHECK(memcmp(mlc_nodes, mlc_expected_nodes, sizeof(mlc_nodes)) == 0);
    MLC_CHECK(op_module_head == mlc_expected_head && op_module_tail == mlc_expected_tail &&
             op_module_initialized == mlc_expected_initialized);
}

static int mlc_active, mlc_key_case, mlc_mutate, mlc_calls, mlc_selected, mlc_selected_position, mlc_mode;
static const unsigned int mlc_words[] = {0, 1, 2, 3, 4, 7, 8, 9, 10, 11, 12, 15, 0x80000000u, 0x80000008u, 0xffffffffu};
static const unsigned int mlc_mapped[] = {0, 1, 2, 3, 0, 1, 2, 3, 4, 5, 6, 7, 4, 5, 6, 7};
static void mlc_select_expected(void)
{
    int i;
    mlc_selected = mlc_selected_position = -1;
    for (i = 0; i < mlc_length; ++i)
        if (mlc_expected_nodes[mlc_order[i]].value.id == mlc_keys[mlc_key_case])
        {
            mlc_selected = mlc_order[i];
            mlc_selected_position = i;
            break;
        }
}
static void mlc_predict_removal(void)
{
    if (mlc_selected < 0)
        return;
    if (mlc_selected_position > 0)
        mlc_expected_nodes[mlc_order[mlc_selected_position - 1]].value.next =
            mlc_selected_position + 1 < mlc_length ? &mlc_nodes[mlc_order[mlc_selected_position + 1]].value : 0;
    if (mlc_selected_position + 1 < mlc_length)
        mlc_expected_nodes[mlc_order[mlc_selected_position + 1]].value.previous =
            mlc_selected_position ? &mlc_nodes[mlc_order[mlc_selected_position - 1]].value : 0;
    if (mlc_selected_position == 0)
        mlc_expected_head = mlc_length > 1 ? &mlc_nodes[mlc_order[1]].value : 0;
    if (mlc_selected_position == mlc_length - 1)
        mlc_expected_tail = mlc_length > 1 ? &mlc_nodes[mlc_order[mlc_length - 2]].value : 0;
}
static void mlc_release(void *pointer)
{
    MLC_CHECK(mlc_active && mlc_calls == 0 && mlc_selected >= 0 && mlc_selected < 5);
    if (!mlc_active || mlc_selected < 0 || mlc_selected >= 5 || mlc_calls != 0)
        return;
    MLC_CHECK(pointer == &mlc_nodes[mlc_selected].value);
    if (pointer != &mlc_nodes[mlc_selected].value)
        return;
    mlc_verify();
    ++mlc_calls;
    if (mlc_mutate)
    {
        mlc_nodes[mlc_selected].value.name[0] = mlc_expected_nodes[mlc_selected].value.name[0] = 'R';
        op_module_initialized = mlc_expected_initialized = INT_MIN;
        op_module_head = mlc_expected_head = &mlc_nodes[(mlc_selected + 1) % 5].value;
    }
}
static int op_test_module_connected(void)
{
    int i, word_case, request_case, expected_return;
    unsigned int request, expected_flags;
    OP_MODULE *expected_remaining;
    mlc_active = 1;
    for (mlc_length = 0; mlc_length < 6; ++mlc_length)
        for (mlc_rotation = 0; mlc_rotation < 5; ++mlc_rotation)
            for (mlc_duplicates = 0; mlc_duplicates < 2; ++mlc_duplicates)
                for (mlc_key_case = 0; mlc_key_case < 6; ++mlc_key_case)
                {
                    for (word_case = 0; word_case < 15; ++word_case)
                    {
                        mlc_seed();
                        for (i = 0; i < 5; ++i)
                            mlc_nodes[i].value.status.raw = mlc_expected_nodes[i].value.status.raw =
                                mlc_words[(word_case + i) % 15];
                        mlc_select_expected();
                        for (request_case = 0; request_case < 34; ++request_case)
                        {
                            request = request_case < 32    ? (unsigned int)request_case
                                      : request_case == 32 ? 0x80000000u
                                                           : 0xffffffffu;
                            expected_flags =
                                mlc_selected < 0
                                    ? 0
                                    : (mlc_mapped[mlc_expected_nodes[mlc_selected].value.status.raw & 15] & request & 7);
                            MLC_CHECK(op_module_get_flags(mlc_keys[mlc_key_case], request) == expected_flags);
                            mlc_verify();
                        }
                    }
                    for (mlc_mode = 0; mlc_mode < 2; ++mlc_mode)
                        for (mlc_mutate = 0; mlc_mutate < 2; ++mlc_mutate)
                        {
                            mlc_seed();
                            mlc_select_expected();
                            expected_return = mlc_selected < 0 ? 1 : 0;
                            mlc_calls = 0;
                            mlc_predict_removal();
                            if (mlc_mode == 0)
                                MLC_CHECK(op_module_remove(mlc_keys[mlc_key_case]) == expected_return);
                            else
                                op_module_remove_void(mlc_keys[mlc_key_case]);
                            MLC_CHECK(mlc_calls == (mlc_selected < 0 ? 0 : 1));
                            mlc_verify();
                            if (!mlc_mutate)
                            {
                                expected_remaining = 0;
                                for (i = 0; i < mlc_length; ++i)
                                    if (i != mlc_selected_position &&
                                        mlc_expected_nodes[mlc_order[i]].value.id == mlc_keys[mlc_key_case])
                                    {
                                        expected_remaining = &mlc_nodes[mlc_order[i]].value;
                                        break;
                                    }
                                MLC_CHECK(op_module_find_by_id(mlc_keys[mlc_key_case]) == expected_remaining);
                                mlc_verify();
                            }
                        }
                }
    mlc_active = 0;
    printf("module connected: %d checks, %d failures\n", mlc_checks, mlc_failures);
    return mlc_failures != 0;
}

#undef MLC_CHECK
