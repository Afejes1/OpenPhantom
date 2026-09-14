static int msha_active;
#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int msha_checks, msha_failures;
static void msha_check(int ok, int line)
{
    ++msha_checks;
    if (!ok)
    {
        ++msha_failures;
        printf("line%d failed\n", line);
    }
}
#define MSHA_CHECK(x) msha_check(!!(x), __LINE__)
typedef struct MSHA_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MSHA_OWNED_NODE;
static MSHA_OWNED_NODE msha_nodes[5], msha_expected_nodes[5];
static OP_MODULE *msha_expected_head, *msha_expected_tail;
static int msha_expected_initialized, msha_expected_open;
static int msha_length, msha_rotation, msha_mode, msha_release_mode, msha_result_case, msha_status_case, msha_init_case;
static int msha_order[5], msha_visits[5], msha_plan[5], msha_expected_callbacks[5], msha_released[5], msha_planned,
    msha_releases, msha_calls, msha_callback_on_current, msha_link_destination;
static const int msha_returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int msha_status_words[8] = {0,           1,           0x80000000u, 0x80000001u,
                                                  0xfffffffeu, 0xffffffffu, 0x12345670u, 0x12345671u};
static void msha_verify(void)
{
    MSHA_CHECK(memcmp(msha_nodes, msha_expected_nodes, sizeof(msha_nodes)) == 0);
    MSHA_CHECK(op_module_head == msha_expected_head && op_module_tail == msha_expected_tail);
    MSHA_CHECK(op_module_initialized == msha_expected_initialized && op_module_open == msha_expected_open);
}
static int msha_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int returned;
    MSHA_CHECK(msha_releases < msha_planned && msha_releases < 5);
    if (msha_releases >= msha_planned || msha_releases >= 5)
        exit(2);
    MSHA_CHECK(node == msha_plan[msha_releases] && !msha_released[node] && msha_expected_callbacks[msha_releases] &&
               !msha_callback_on_current);
    if (node != msha_plan[msha_releases] || msha_released[node] || !msha_expected_callbacks[msha_releases] ||
        msha_callback_on_current)
        exit(2);
    msha_verify();
    MSHA_CHECK(event == 2 && argument == 0 && bits == 0);
    msha_callback_on_current = 1;
    returned = msha_returns[(msha_result_case + msha_calls) % 5];
    ++msha_calls;
    if (msha_mode)
    {
        msha_nodes[node].value.status.raw = msha_expected_nodes[node].value.status.raw =
            msha_status_words[(msha_status_case + msha_calls + 3) % 8] ^ 0x00550000u;
        msha_nodes[node].value.name[0] = msha_expected_nodes[node].value.name[0] = 'S';
    }
    if (msha_calls == 1 && msha_mode)
    {
        op_module_head = msha_expected_head = &msha_nodes[(node + 2) % 5].value;
        op_module_tail = msha_expected_tail = &msha_nodes[(node + 3) % 5].value;
        op_module_initialized = msha_expected_initialized = INT_MIN;
        op_module_open = msha_expected_open = msha_returns[msha_result_case];
        if (msha_mode == 1 || msha_mode == 2)
            msha_nodes[node].value.previous = msha_expected_nodes[node].value.previous =
                msha_link_destination < 0 ? 0 : &msha_nodes[msha_link_destination].value;
    }
    return returned;
}
static int msha_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return msha_observe(0, a, b, c);
}
static int msha_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return msha_observe(1, a, b, c);
}
static int msha_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return msha_observe(2, a, b, c);
}
static int msha_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return msha_observe(3, a, b, c);
}
static int msha_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return msha_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK msha_callbacks[5] = {msha_callback0, msha_callback1, msha_callback2, msha_callback3,
                                               msha_callback4};
static void msha_release(void *memory)
{
    int node;
    MSHA_CHECK(msha_releases < msha_planned && msha_releases < 5);
    if (msha_releases >= msha_planned || msha_releases >= 5)
        exit(2);
    node = msha_plan[msha_releases];
    MSHA_CHECK(memory == &msha_nodes[node].value && !msha_released[node]);
    if (memory != &msha_nodes[node].value || msha_released[node])
        exit(2);
    MSHA_CHECK(msha_callback_on_current == msha_expected_callbacks[msha_releases]);
    msha_verify();
    msha_released[node] = 1;
    ++msha_releases;
    msha_callback_on_current = 0;
    if (msha_release_mode)
    {
        msha_nodes[node].value.previous = msha_expected_nodes[node].value.previous = &msha_nodes[(node + 2) % 5].value;
        msha_nodes[node].value.name[1] = msha_expected_nodes[node].value.name[1] = 'R';
        op_module_initialized = msha_expected_initialized = INT_MAX;
    }
}
static void msha_seed(void)
{
    int i, n, first = -1;
    memset(msha_nodes, 0x57, sizeof(msha_nodes));
    memset(msha_released, 0, sizeof(msha_released));
    for (i = 0; i < 5; ++i)
    {
        msha_nodes[i].value.next = msha_nodes[i].value.previous = 0;
        msha_nodes[i].value.id = msha_returns[i];
        msha_nodes[i].value.callback_word = (unsigned int)msha_callbacks[i];
        msha_nodes[i].value.status.raw = msha_status_words[(msha_status_case + i) % 8];
        msha_order[i] = (i + msha_rotation) % 5;
    }
    for (i = 0; i < msha_length; ++i)
    {
        n = msha_order[i];
        msha_nodes[n].value.previous = i ? &msha_nodes[msha_order[i - 1]].value : 0;
        msha_nodes[n].value.next = i + 1 < msha_length ? &msha_nodes[msha_order[i + 1]].value : 0;
    }
    op_module_head = msha_expected_head = msha_length ? &msha_nodes[msha_order[0]].value : 0;
    op_module_tail = msha_expected_tail = msha_length ? &msha_nodes[msha_order[msha_length - 1]].value : 0;
    op_module_initialized = msha_expected_initialized = msha_returns[msha_init_case];
    op_module_open = msha_expected_open = 0x76543210;
    memcpy(msha_expected_nodes, msha_nodes, sizeof(msha_nodes));
    msha_planned = msha_releases = msha_calls = msha_callback_on_current = 0;
    msha_link_destination = -1;
    if (!op_module_initialized)
        return;
    for (i = 0; i < msha_length; ++i)
        msha_visits[i] = msha_order[msha_length - 1 - i];
    for (i = 0; i < msha_length; ++i)
        if (msha_expected_nodes[msha_visits[i]].value.status.raw & 1)
        {
            first = i;
            break;
        }
    if (msha_mode == 2 && first >= 0 && first + 2 < msha_length)
        msha_link_destination = msha_visits[first + 2];
    for (i = 0; i < msha_length; ++i)
    {
        if (first >= 0 && msha_mode == 1 && i > first)
            break;
        if (first >= 0 && msha_mode == 2 && i == first + 1)
            continue;
        msha_plan[msha_planned] = msha_visits[i];
        msha_expected_callbacks[msha_planned] = (msha_expected_nodes[msha_visits[i]].value.status.raw & 1) != 0;
        ++msha_planned;
    }
}
static int op_test_module_shutdown_all(void)
{
    int i, expected_calls;
    msha_active = 1;
    for (msha_length = 0; msha_length < 6; ++msha_length)
        for (msha_rotation = 0; msha_rotation < 5; ++msha_rotation)
            for (msha_mode = 0; msha_mode < 4; ++msha_mode)
                for (msha_release_mode = 0; msha_release_mode < 2; ++msha_release_mode)
                    for (msha_result_case = 0; msha_result_case < 5; ++msha_result_case)
                        for (msha_status_case = 0; msha_status_case < 8; ++msha_status_case)
                            for (msha_init_case = 0; msha_init_case < 5; ++msha_init_case)
                            {
                                msha_seed();
                                expected_calls = 0;
                                for (i = 0; i < msha_planned; ++i)
                                    expected_calls += msha_expected_callbacks[i];
                                op_module_shutdown_all();
                                msha_expected_initialized = 0;
                                MSHA_CHECK(msha_releases == msha_planned && msha_calls == expected_calls &&
                                           !msha_callback_on_current);
                                msha_verify();
                            }
    printf("module_shutdown_all: %d checks, %d failures\n", msha_checks, msha_failures);
    msha_active = 0;
    return msha_failures != 0;
}

#undef MSHA_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mgen_checks, mgen_failures;
static void mgen_check(int ok, int line)
{
    ++mgen_checks;
    if (!ok)
    {
        ++mgen_failures;
        printf("line%d failed\n", line);
    }
}
#define MGEN_CHECK(x) mgen_check(!!(x), __LINE__)
typedef struct MGEN_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MGEN_OWNED_NODE;
typedef struct MGEN_OWNED_PAYLOAD
{
    unsigned int before;
    unsigned char bytes[8];
    unsigned int after;
} MGEN_OWNED_PAYLOAD;
static MGEN_OWNED_NODE mgen_nodes[5], mgen_expected_nodes[5];
static MGEN_OWNED_PAYLOAD mgen_payload, mgen_expected_payload;
static OP_MODULE *mgen_expected_head, *mgen_expected_tail;
static int mgen_expected_initialized;
static int mgen_kind = 2, mgen_length, mgen_rotation, mgen_duplicates, mgen_mode, mgen_result_case, mgen_key_case,
           mgen_event_case;
static int mgen_order[5], mgen_plan[5], mgen_planned, mgen_calls, mgen_query;
static unsigned int mgen_event_word;
static const int mgen_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int mgen_results[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mgen_words[8] = {0, 1, 3, 4, 6, 7, 0x80000000u, 0xffffffffu};
static void mgen_verify(void)
{
    MGEN_CHECK(memcmp(mgen_nodes, mgen_expected_nodes, sizeof(mgen_nodes)) == 0);
    MGEN_CHECK(memcmp(&mgen_payload, &mgen_expected_payload, sizeof(mgen_payload)) == 0);
    MGEN_CHECK(op_module_head == mgen_expected_head && op_module_tail == mgen_expected_tail &&
               op_module_initialized == mgen_expected_initialized);
}
static int mgen_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mgen_calls, next_node;
    MGEN_CHECK(mgen_calls < mgen_planned && mgen_calls < 5);
    if (mgen_calls >= mgen_planned || mgen_calls >= 5)
        exit(2);
    MGEN_CHECK(node == mgen_plan[position]);
    if (node != mgen_plan[position])
        exit(2);
    mgen_verify();
    MGEN_CHECK(event == mgen_event_word && argument == 0 && bits == 0);
    ++mgen_calls;
    if (position == 0 && mgen_mode)
    {
        op_module_head = mgen_expected_head = &mgen_nodes[(node + 2) % 5].value;
        op_module_tail = mgen_expected_tail = &mgen_nodes[(node + 3) % 5].value;
        op_module_initialized = mgen_expected_initialized = INT_MIN;
        mgen_nodes[node].value.name[0] = mgen_expected_nodes[node].value.name[0] = 'Z';
        if (mgen_kind == 0)
            mgen_payload.bytes[2] = mgen_expected_payload.bytes[2] = 0x93;
        if (mgen_query == 0 && mgen_kind != 0 && (mgen_mode == 1 || mgen_mode == 2))
        {
            next_node = mgen_planned > 1 ? mgen_plan[1] : -1;
            if (mgen_kind == 1)
                mgen_nodes[node].value.next = mgen_expected_nodes[node].value.next =
                    next_node < 0 ? 0 : &mgen_nodes[next_node].value;
            else
                mgen_nodes[node].value.previous = mgen_expected_nodes[node].value.previous =
                    next_node < 0 ? 0 : &mgen_nodes[next_node].value;
        }
        if (mgen_mode == 3 && mgen_planned > 1)
        {
            next_node = mgen_plan[1];
            mgen_nodes[next_node].value.id = mgen_expected_nodes[next_node].value.id = INT_MIN;
        }
    }
    return mgen_results[(mgen_result_case + position) % 5];
}
static int mgen_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mgen_observe(0, a, b, c);
}
static int mgen_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mgen_observe(1, a, b, c);
}
static int mgen_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mgen_observe(2, a, b, c);
}
static int mgen_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mgen_observe(3, a, b, c);
}
static int mgen_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mgen_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mgen_callbacks[5] = {mgen_callback0, mgen_callback1, mgen_callback2, mgen_callback3,
                                               mgen_callback4};
static void mgen_seed(void)
{
    int i, n, start;
    memset(mgen_nodes, 0x57, sizeof(mgen_nodes));
    memset(&mgen_payload, 0x68, sizeof(mgen_payload));
    for (i = 0; i < 5; ++i)
    {
        mgen_nodes[i].value.next = mgen_nodes[i].value.previous = 0;
        mgen_nodes[i].value.id = mgen_keys[i];
        mgen_nodes[i].value.callback_word = (unsigned int)mgen_callbacks[i];
        mgen_nodes[i].value.status.raw = mgen_words[(mgen_event_case + i) % 8];
        mgen_order[i] = (i + mgen_rotation) % 5;
    }
    for (i = 0; i < mgen_length; ++i)
    {
        n = mgen_order[i];
        mgen_nodes[n].value.previous = i ? &mgen_nodes[mgen_order[i - 1]].value : 0;
        mgen_nodes[n].value.next = i + 1 < mgen_length ? &mgen_nodes[mgen_order[i + 1]].value : 0;
    }
    if (mgen_duplicates && mgen_length > 1)
        mgen_nodes[mgen_order[1]].value.id = mgen_nodes[mgen_order[0]].value.id;
    op_module_head = mgen_expected_head = mgen_length ? &mgen_nodes[mgen_order[0]].value : 0;
    op_module_tail = mgen_expected_tail = mgen_length ? &mgen_nodes[mgen_order[mgen_length - 1]].value : 0;
    op_module_initialized = mgen_expected_initialized = 0x12345678;
    memcpy(mgen_expected_nodes, mgen_nodes, sizeof(mgen_nodes));
    memcpy(&mgen_expected_payload, &mgen_payload, sizeof(mgen_payload));
    mgen_query = mgen_keys[mgen_key_case];
    mgen_event_word = mgen_words[mgen_event_case];
    mgen_kind = mgen_event_word == 4 || mgen_event_word == 6 ? 1 : 2;
    mgen_planned = mgen_calls = 0;
    if (mgen_query)
    {
        for (i = 0; i < mgen_length; ++i)
            if (mgen_expected_nodes[mgen_order[i]].value.id == mgen_query)
            {
                mgen_plan[mgen_planned++] = mgen_order[i];
                break;
            }
    }
    else if (mgen_kind != 0)
    {
        for (i = 0; i < mgen_length; ++i)
        {
            start = mgen_kind == 1 ? i : mgen_length - 1 - i;
            mgen_plan[mgen_planned++] = mgen_order[start];
        }
        if (mgen_mode == 1 && mgen_planned > 0)
            mgen_planned = 1;
        if (mgen_mode == 2 && mgen_planned > 1)
        {
            for (i = 1; i + 1 < mgen_planned; ++i)
                mgen_plan[i] = mgen_plan[i + 1];
            --mgen_planned;
        }
    }
}
static int op_test_module_broadcast(void)
{
    int actual, expected;
    for (mgen_length = 0; mgen_length < 6; ++mgen_length)
        for (mgen_rotation = 0; mgen_rotation < 5; ++mgen_rotation)
            for (mgen_duplicates = 0; mgen_duplicates < 2; ++mgen_duplicates)
                for (mgen_key_case = 0; mgen_key_case < 6; ++mgen_key_case)
                    for (mgen_mode = 0; mgen_mode < 4; ++mgen_mode)
                        for (mgen_result_case = 0; mgen_result_case < 5; ++mgen_result_case)
                            for (mgen_event_case = 0; mgen_event_case < 8; ++mgen_event_case)
                            {
                                mgen_seed();
                                expected = mgen_query ? (mgen_planned ? mgen_results[mgen_result_case] : 1) : 0;
                                actual = op_save_lifecycle(mgen_query, mgen_event_word);
                                MGEN_CHECK(actual == expected);
                                MGEN_CHECK(mgen_calls == mgen_planned);
                                mgen_verify();
                            }
    printf("module_broadcast: %d checks, %d failures\n", mgen_checks, mgen_failures);
    return mgen_failures != 0;
}

#undef MGEN_CHECK
