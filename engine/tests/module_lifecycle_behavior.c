#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int msa_checks, msa_failures;
static void msa_check(int ok, int line)
{
    ++msa_checks;
    if (!ok)
    {
        ++msa_failures;
        printf("line%d failed\n", line);
    }
}
#define MSA_CHECK(x) msa_check(!!(x), __LINE__)
typedef struct MSA_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MSA_OWNED_NODE;
static MSA_OWNED_NODE msa_nodes[5], msa_expected_nodes[5];
static OP_MODULE *msa_expected_head, *msa_expected_tail;
static int msa_expected_initialized;
static int msa_resume = 0, msa_length, msa_rotation, msa_duplicates, msa_mode, msa_result_case, msa_key_case,
           msa_status_case;
static int msa_order[5], msa_visits[5], msa_plan[5], msa_planned, msa_calls, msa_query, msa_first_position,
    msa_link_destination;
static const int msa_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int msa_returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int msa_status_words[8] = {0,           2,           0x80000000u, 0x80000002u,
                                                 0xfffffffdu, 0xffffffffu, 0x12345670u, 0x12345672u};
static void msa_verify(void)
{
    MSA_CHECK(memcmp(msa_nodes, msa_expected_nodes, sizeof(msa_nodes)) == 0);
    MSA_CHECK(op_module_head == msa_expected_head && op_module_tail == msa_expected_tail &&
              op_module_initialized == msa_expected_initialized);
}
static int msa_eligible(unsigned int word)
{
    return msa_resume ? (word & 2) != 0 : (word & 2) == 0;
}
static int msa_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = msa_calls, returned;
    MSA_CHECK(msa_calls < msa_planned && msa_calls < 5);
    if (msa_calls >= msa_planned || msa_calls >= 5)
        exit(2);
    MSA_CHECK(node == msa_plan[position]);
    if (node != msa_plan[position])
        exit(2);
    msa_verify();
    MSA_CHECK(event == (msa_resume ? 9u : 3u));
    MSA_CHECK(argument == 0 && bits == 0);
    returned = msa_returns[(msa_result_case + position) % 5];
    ++msa_calls;
    if (msa_mode)
    {
        msa_nodes[node].value.status.raw = msa_expected_nodes[node].value.status.raw =
            msa_status_words[(msa_status_case + position + 3) % 8] ^ 0x00550000u;
        msa_nodes[node].value.name[0] = msa_expected_nodes[node].value.name[0] = 'L';
    }
    if (position == 0 && msa_mode)
    {
        op_module_head = msa_expected_head = &msa_nodes[(node + 2) % 5].value;
        op_module_tail = msa_expected_tail = &msa_nodes[(node + 3) % 5].value;
        op_module_initialized = msa_expected_initialized = INT_MIN;
        if (msa_query == 0 && (msa_mode == 1 || msa_mode == 2))
        {
            if (msa_resume)
                msa_nodes[node].value.previous = msa_expected_nodes[node].value.previous =
                    msa_link_destination < 0 ? 0 : &msa_nodes[msa_link_destination].value;
            else
                msa_nodes[node].value.next = msa_expected_nodes[node].value.next =
                    msa_link_destination < 0 ? 0 : &msa_nodes[msa_link_destination].value;
        }
    }
    if (returned == 0)
    {
        if (msa_resume)
            msa_expected_nodes[node].value.status.raw &= ~2u;
        else
            msa_expected_nodes[node].value.status.raw |= 2u;
    }
    return returned;
}
static int msa_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return msa_observe(0, a, b, c);
}
static int msa_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return msa_observe(1, a, b, c);
}
static int msa_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return msa_observe(2, a, b, c);
}
static int msa_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return msa_observe(3, a, b, c);
}
static int msa_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return msa_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK msa_callbacks[5] = {msa_callback0, msa_callback1, msa_callback2, msa_callback3,
                                              msa_callback4};
static void msa_seed(void)
{
    int i, n, selected;
    memset(msa_nodes, 0x57, sizeof(msa_nodes));
    for (i = 0; i < 5; ++i)
    {
        msa_nodes[i].value.next = msa_nodes[i].value.previous = 0;
        msa_nodes[i].value.id = msa_keys[i];
        msa_nodes[i].value.callback_word = (unsigned int)msa_callbacks[i];
        msa_nodes[i].value.status.raw = msa_status_words[(msa_status_case + i) % 8];
        msa_order[i] = (i + msa_rotation) % 5;
    }
    for (i = 0; i < msa_length; ++i)
    {
        n = msa_order[i];
        msa_nodes[n].value.previous = i ? &msa_nodes[msa_order[i - 1]].value : 0;
        msa_nodes[n].value.next = i + 1 < msa_length ? &msa_nodes[msa_order[i + 1]].value : 0;
    }
    if (msa_duplicates && msa_length > 1)
        msa_nodes[msa_order[1]].value.id = msa_nodes[msa_order[0]].value.id;
    op_module_head = msa_expected_head = msa_length ? &msa_nodes[msa_order[0]].value : 0;
    op_module_tail = msa_expected_tail = msa_length ? &msa_nodes[msa_order[msa_length - 1]].value : 0;
    op_module_initialized = msa_expected_initialized = 0x12345678;
    memcpy(msa_expected_nodes, msa_nodes, sizeof(msa_nodes));
    msa_query = msa_keys[msa_key_case];
    msa_planned = msa_calls = 0;
    msa_link_destination = msa_first_position = -1;
    if (msa_query)
    {
        selected = -1;
        for (i = 0; i < msa_length; ++i)
            if (msa_expected_nodes[msa_order[i]].value.id == msa_query)
            {
                selected = msa_order[i];
                break;
            }
        if (selected >= 0 && msa_eligible(msa_expected_nodes[selected].value.status.raw))
            msa_plan[msa_planned++] = selected;
    }
    else
    {
        for (i = 0; i < msa_length; ++i)
            msa_visits[i] = msa_order[msa_resume ? msa_length - 1 - i : i];
        for (i = 0; i < msa_length; ++i)
            if (msa_eligible(msa_expected_nodes[msa_visits[i]].value.status.raw))
            {
                msa_first_position = i;
                break;
            }
        if (msa_first_position >= 0)
        {
            if (msa_mode == 2 && msa_first_position + 2 < msa_length)
                msa_link_destination = msa_visits[msa_first_position + 2];
            for (i = msa_first_position; i < msa_length; ++i)
            {
                if (msa_mode == 1 && i > msa_first_position)
                    break;
                if (msa_mode == 2 && i == msa_first_position + 1)
                    continue;
                if (msa_eligible(msa_expected_nodes[msa_visits[i]].value.status.raw))
                    msa_plan[msa_planned++] = msa_visits[i];
            }
        }
    }
}
static int op_test_module_start_all(void)
{
    int expected, actual;
    for (msa_length = 0; msa_length < 6; ++msa_length)
        for (msa_rotation = 0; msa_rotation < 5; ++msa_rotation)
            for (msa_duplicates = 0; msa_duplicates < 2; ++msa_duplicates)
                for (msa_key_case = 0; msa_key_case < 6; ++msa_key_case)
                    for (msa_mode = 0; msa_mode < 4; ++msa_mode)
                        for (msa_result_case = 0; msa_result_case < 5; ++msa_result_case)
                            for (msa_status_case = 0; msa_status_case < 8; ++msa_status_case)
                            {
                                msa_seed();
                                expected = msa_query ? (msa_planned ? msa_returns[msa_result_case] : msa_resume) : 0;
                                actual = op_module_start_all(msa_query);
                                MSA_CHECK(actual == expected);
                                MSA_CHECK(msa_calls == msa_planned);
                                msa_verify();
                            }
    printf("module_start_all: %d checks, %d failures\n", msa_checks, msa_failures);
    return msa_failures != 0;
}

#undef MSA_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mop_checks, mop_failures;
static void mop_check(int ok, int line)
{
    ++mop_checks;
    if (!ok)
    {
        ++mop_failures;
        printf("line%d failed\n", line);
    }
}
#define MOP_CHECK(x) mop_check(!!(x), __LINE__)
typedef struct MOP_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MOP_OWNED_NODE;
static MOP_OWNED_NODE mop_nodes[5], mop_expected_nodes[5];
static OP_MODULE *mop_expected_head, *mop_expected_tail;
static int mop_expected_initialized, mop_expected_open;
static int mop_opening = 1, mop_length, mop_rotation, mop_mode, mop_result_case, mop_status_case, mop_init_case,
           mop_open_case;
static int mop_order[5], mop_visits[5], mop_plan[5], mop_planned, mop_calls, mop_link_destination;
static const int mop_returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mop_status_words[8] = {0,           2,           0x80000000u, 0x80000002u,
                                                 0xfffffffdu, 0xffffffffu, 0x12345670u, 0x12345672u};
static void mop_verify(void)
{
    MOP_CHECK(memcmp(mop_nodes, mop_expected_nodes, sizeof(mop_nodes)) == 0);
    MOP_CHECK(op_module_head == mop_expected_head && op_module_tail == mop_expected_tail);
    MOP_CHECK(op_module_initialized == mop_expected_initialized && op_module_open == mop_expected_open);
}
static int mop_eligible(unsigned int word)
{
    return mop_opening ? (word & 2) == 0 : (word & 2) != 0;
}
static int mop_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mop_calls, returned;
    MOP_CHECK(mop_calls < mop_planned && mop_calls < 5);
    if (mop_calls >= mop_planned || mop_calls >= 5)
        exit(2);
    MOP_CHECK(node == mop_plan[position]);
    if (node != mop_plan[position])
        exit(2);
    mop_verify();
    MOP_CHECK(event == (mop_opening ? 3u : 4u) && argument == 0 && bits == 0);
    returned = mop_returns[(mop_result_case + position) % 5];
    ++mop_calls;
    if (mop_mode)
    {
        mop_nodes[node].value.status.raw = mop_expected_nodes[node].value.status.raw =
            mop_status_words[(mop_status_case + position + 3) % 8] ^ 0x00550000u;
        mop_nodes[node].value.name[0] = mop_expected_nodes[node].value.name[0] = 'Q';
    }
    if (position == 0 && mop_mode)
    {
        op_module_head = mop_expected_head = &mop_nodes[(node + 2) % 5].value;
        op_module_tail = mop_expected_tail = &mop_nodes[(node + 3) % 5].value;
        op_module_initialized = mop_expected_initialized = INT_MIN;
        op_module_open = mop_expected_open = mop_returns[mop_result_case];
        if (mop_mode == 1 || mop_mode == 2)
        {
            if (mop_opening)
                mop_nodes[node].value.previous = mop_expected_nodes[node].value.previous =
                    mop_link_destination < 0 ? 0 : &mop_nodes[mop_link_destination].value;
            else
                mop_nodes[node].value.next = mop_expected_nodes[node].value.next =
                    mop_link_destination < 0 ? 0 : &mop_nodes[mop_link_destination].value;
        }
    }
    if (mop_opening && returned == 0)
        mop_expected_nodes[node].value.status.raw |= 2u;
    return returned;
}
static int mop_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mop_observe(0, a, b, c);
}
static int mop_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mop_observe(1, a, b, c);
}
static int mop_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mop_observe(2, a, b, c);
}
static int mop_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mop_observe(3, a, b, c);
}
static int mop_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mop_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mop_callbacks[5] = {mop_callback0, mop_callback1, mop_callback2, mop_callback3,
                                              mop_callback4};
static void mop_seed(void)
{
    int i, n, first = -1;
    memset(mop_nodes, 0x57, sizeof(mop_nodes));
    for (i = 0; i < 5; ++i)
    {
        mop_nodes[i].value.next = mop_nodes[i].value.previous = 0;
        mop_nodes[i].value.id = mop_returns[i];
        mop_nodes[i].value.callback_word = (unsigned int)mop_callbacks[i];
        mop_nodes[i].value.status.raw = mop_status_words[(mop_status_case + i) % 8];
        mop_order[i] = (i + mop_rotation) % 5;
    }
    for (i = 0; i < mop_length; ++i)
    {
        n = mop_order[i];
        mop_nodes[n].value.previous = i ? &mop_nodes[mop_order[i - 1]].value : 0;
        mop_nodes[n].value.next = i + 1 < mop_length ? &mop_nodes[mop_order[i + 1]].value : 0;
    }
    op_module_head = mop_expected_head = mop_length ? &mop_nodes[mop_order[0]].value : 0;
    op_module_tail = mop_expected_tail = mop_length ? &mop_nodes[mop_order[mop_length - 1]].value : 0;
    op_module_initialized = mop_expected_initialized = mop_returns[mop_init_case];
    op_module_open = mop_expected_open = mop_returns[mop_open_case];
    memcpy(mop_expected_nodes, mop_nodes, sizeof(mop_nodes));
    mop_planned = mop_calls = 0;
    mop_link_destination = -1;
    if (mop_opening ? (!op_module_initialized || op_module_open) : !op_module_open)
        return;
    if (mop_opening)
        mop_expected_open = 1;
    for (i = 0; i < mop_length; ++i)
        mop_visits[i] = mop_order[mop_opening ? mop_length - 1 - i : i];
    for (i = 0; i < mop_length; ++i)
        if (mop_eligible(mop_expected_nodes[mop_visits[i]].value.status.raw))
        {
            first = i;
            break;
        }
    if (first < 0)
        return;
    if (mop_mode == 2 && first + 2 < mop_length)
        mop_link_destination = mop_visits[first + 2];
    for (i = first; i < mop_length; ++i)
    {
        if (mop_mode == 1 && i > first)
            break;
        if (mop_mode == 2 && i == first + 1)
            continue;
        if (mop_eligible(mop_expected_nodes[mop_visits[i]].value.status.raw))
            mop_plan[mop_planned++] = mop_visits[i];
    }
}
static int op_test_module_open_all(void)
{
    int expected, actual;
    for (mop_length = 0; mop_length < 6; ++mop_length)
        for (mop_rotation = 0; mop_rotation < 5; ++mop_rotation)
            for (mop_mode = 0; mop_mode < 4; ++mop_mode)
                for (mop_result_case = 0; mop_result_case < 5; ++mop_result_case)
                    for (mop_status_case = 0; mop_status_case < 8; ++mop_status_case)
                        for (mop_init_case = 0; mop_init_case < 5; ++mop_init_case)
                            for (mop_open_case = 0; mop_open_case < 5; ++mop_open_case)
                            {
                                mop_seed();
                                expected = (mop_returns[mop_init_case] && !mop_returns[mop_open_case]) ? 0 : 1;
                                actual = op_module_open_all();
                                MOP_CHECK(actual == expected);
                                MOP_CHECK(mop_calls == mop_planned);
                                mop_verify();
                            }
    printf("module_open_all: %d checks, %d failures\n", mop_checks, mop_failures);
    return mop_failures != 0;
}

#undef MOP_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mcl_checks, mcl_failures;
static void mcl_check(int ok, int line)
{
    ++mcl_checks;
    if (!ok)
    {
        ++mcl_failures;
        printf("line%d failed\n", line);
    }
}
#define MCL_CHECK(x) mcl_check(!!(x), __LINE__)
typedef struct MCL_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MCL_OWNED_NODE;
static MCL_OWNED_NODE mcl_nodes[5], mcl_expected_nodes[5];
static OP_MODULE *mcl_expected_head, *mcl_expected_tail;
static int mcl_expected_initialized, mcl_expected_open;
static int mcl_opening = 0, mcl_length, mcl_rotation, mcl_mode, mcl_result_case, mcl_status_case, mcl_init_case,
           mcl_open_case;
static int mcl_order[5], mcl_visits[5], mcl_plan[5], mcl_planned, mcl_calls, mcl_link_destination;
static const int mcl_returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mcl_status_words[8] = {0,           2,           0x80000000u, 0x80000002u,
                                                 0xfffffffdu, 0xffffffffu, 0x12345670u, 0x12345672u};
static void mcl_verify(void)
{
    MCL_CHECK(memcmp(mcl_nodes, mcl_expected_nodes, sizeof(mcl_nodes)) == 0);
    MCL_CHECK(op_module_head == mcl_expected_head && op_module_tail == mcl_expected_tail);
    MCL_CHECK(op_module_initialized == mcl_expected_initialized && op_module_open == mcl_expected_open);
}
static int mcl_eligible(unsigned int word)
{
    return mcl_opening ? (word & 2) == 0 : (word & 2) != 0;
}
static int mcl_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mcl_calls, returned;
    MCL_CHECK(mcl_calls < mcl_planned && mcl_calls < 5);
    if (mcl_calls >= mcl_planned || mcl_calls >= 5)
        exit(2);
    MCL_CHECK(node == mcl_plan[position]);
    if (node != mcl_plan[position])
        exit(2);
    mcl_verify();
    MCL_CHECK(event == (mcl_opening ? 3u : 4u) && argument == 0 && bits == 0);
    returned = mcl_returns[(mcl_result_case + position) % 5];
    ++mcl_calls;
    if (mcl_mode)
    {
        mcl_nodes[node].value.status.raw = mcl_expected_nodes[node].value.status.raw =
            mcl_status_words[(mcl_status_case + position + 3) % 8] ^ 0x00550000u;
        mcl_nodes[node].value.name[0] = mcl_expected_nodes[node].value.name[0] = 'Q';
    }
    if (position == 0 && mcl_mode)
    {
        op_module_head = mcl_expected_head = &mcl_nodes[(node + 2) % 5].value;
        op_module_tail = mcl_expected_tail = &mcl_nodes[(node + 3) % 5].value;
        op_module_initialized = mcl_expected_initialized = INT_MIN;
        op_module_open = mcl_expected_open = mcl_returns[mcl_result_case];
        if (mcl_mode == 1 || mcl_mode == 2)
        {
            if (mcl_opening)
                mcl_nodes[node].value.previous = mcl_expected_nodes[node].value.previous =
                    mcl_link_destination < 0 ? 0 : &mcl_nodes[mcl_link_destination].value;
            else
                mcl_nodes[node].value.next = mcl_expected_nodes[node].value.next =
                    mcl_link_destination < 0 ? 0 : &mcl_nodes[mcl_link_destination].value;
        }
    }
    if (mcl_opening && returned == 0)
        mcl_expected_nodes[node].value.status.raw |= 2u;
    return returned;
}
static int mcl_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mcl_observe(0, a, b, c);
}
static int mcl_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mcl_observe(1, a, b, c);
}
static int mcl_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mcl_observe(2, a, b, c);
}
static int mcl_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mcl_observe(3, a, b, c);
}
static int mcl_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mcl_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mcl_callbacks[5] = {mcl_callback0, mcl_callback1, mcl_callback2, mcl_callback3,
                                              mcl_callback4};
static void mcl_seed(void)
{
    int i, n, first = -1;
    memset(mcl_nodes, 0x57, sizeof(mcl_nodes));
    for (i = 0; i < 5; ++i)
    {
        mcl_nodes[i].value.next = mcl_nodes[i].value.previous = 0;
        mcl_nodes[i].value.id = mcl_returns[i];
        mcl_nodes[i].value.callback_word = (unsigned int)mcl_callbacks[i];
        mcl_nodes[i].value.status.raw = mcl_status_words[(mcl_status_case + i) % 8];
        mcl_order[i] = (i + mcl_rotation) % 5;
    }
    for (i = 0; i < mcl_length; ++i)
    {
        n = mcl_order[i];
        mcl_nodes[n].value.previous = i ? &mcl_nodes[mcl_order[i - 1]].value : 0;
        mcl_nodes[n].value.next = i + 1 < mcl_length ? &mcl_nodes[mcl_order[i + 1]].value : 0;
    }
    op_module_head = mcl_expected_head = mcl_length ? &mcl_nodes[mcl_order[0]].value : 0;
    op_module_tail = mcl_expected_tail = mcl_length ? &mcl_nodes[mcl_order[mcl_length - 1]].value : 0;
    op_module_initialized = mcl_expected_initialized = mcl_returns[mcl_init_case];
    op_module_open = mcl_expected_open = mcl_returns[mcl_open_case];
    memcpy(mcl_expected_nodes, mcl_nodes, sizeof(mcl_nodes));
    mcl_planned = mcl_calls = 0;
    mcl_link_destination = -1;
    if (mcl_opening ? (!op_module_initialized || op_module_open) : !op_module_open)
        return;
    if (mcl_opening)
        mcl_expected_open = 1;
    for (i = 0; i < mcl_length; ++i)
        mcl_visits[i] = mcl_order[mcl_opening ? mcl_length - 1 - i : i];
    for (i = 0; i < mcl_length; ++i)
        if (mcl_eligible(mcl_expected_nodes[mcl_visits[i]].value.status.raw))
        {
            first = i;
            break;
        }
    if (first < 0)
        return;
    if (mcl_mode == 2 && first + 2 < mcl_length)
        mcl_link_destination = mcl_visits[first + 2];
    for (i = first; i < mcl_length; ++i)
    {
        if (mcl_mode == 1 && i > first)
            break;
        if (mcl_mode == 2 && i == first + 1)
            continue;
        if (mcl_eligible(mcl_expected_nodes[mcl_visits[i]].value.status.raw))
            mcl_plan[mcl_planned++] = mcl_visits[i];
    }
}
static int op_test_module_close_all(void)
{

    for (mcl_length = 0; mcl_length < 6; ++mcl_length)
        for (mcl_rotation = 0; mcl_rotation < 5; ++mcl_rotation)
            for (mcl_mode = 0; mcl_mode < 4; ++mcl_mode)
                for (mcl_result_case = 0; mcl_result_case < 5; ++mcl_result_case)
                    for (mcl_status_case = 0; mcl_status_case < 8; ++mcl_status_case)
                        for (mcl_init_case = 0; mcl_init_case < 5; ++mcl_init_case)
                            for (mcl_open_case = 0; mcl_open_case < 5; ++mcl_open_case)
                            {
                                mcl_seed();
                                op_module_close_all();
                                mcl_expected_open = 0;
                                MCL_CHECK(mcl_calls == mcl_planned);
                                mcl_verify();
                            }
    printf("module_close_all: %d checks, %d failures\n", mcl_checks, mcl_failures);
    return mcl_failures != 0;
}

#undef MCL_CHECK

static int mii_active;
#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mii_checks, mii_failures;
static void mii_check(int ok, int line)
{
    ++mii_checks;
    if (!ok)
    {
        ++mii_failures;
        printf("line%d failed\n", line);
    }
}
#define MII_CHECK(x) mii_check(!!(x), __LINE__)
typedef struct MII_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MII_OWNED_NODE;
static MII_OWNED_NODE mii_nodes[4], mii_expected_nodes[4];
static OP_MODULE *mii_expected_head, *mii_expected_tail;
static int mii_expected_initialized, mii_expected_result;
static int mii_fail_allocation, mii_endpoint_case, mii_allocator_mode, mii_callback_mode, mii_status_case,
    mii_name_case, mii_result_case;
static int mii_allocations, mii_calls, mii_copies;
static char mii_names[5][24] = {"", "small", "123456789012345", "1234567890123456", "12345678901234567890123"};
static char mii_original_names[5][24];
static const int mii_returns[6] = {0, 1, 2, -1, INT_MIN, INT_MAX};
static const unsigned int mii_status_words[8] = {0, 1, 14, 15, 0x80000000u, 0xffffffffu, 0x12345678u, 0x76543210u};
static void mii_verify(void)
{
    MII_CHECK(memcmp(mii_nodes, mii_expected_nodes, sizeof(mii_nodes)) == 0);
    MII_CHECK(memcmp(mii_names, mii_original_names, sizeof(mii_names)) == 0);
    MII_CHECK(op_module_head == mii_expected_head && op_module_tail == mii_expected_tail &&
              op_module_initialized == mii_expected_initialized);
}
static int mii_callback(unsigned int, unsigned int, unsigned int);
static void mii_expect_header(void)
{
    mii_expected_nodes[0].value.id = (int)&mii_nodes[0].value;
    mii_expected_nodes[0].value.status.raw &= ~14u;
    mii_expected_nodes[0].value.callback_word = (unsigned int)mii_callback;
}
static char *mii_copy(char *destination, const char *source, unsigned int count)
{
    int i, ended = 0, tail_index;
    MII_CHECK(mii_allocations == 1 && mii_copies == 0 && mii_calls == 0);
    MII_CHECK(destination == mii_nodes[0].value.name && source == mii_names[mii_name_case] && count == 16);
    if (mii_allocations != 1 || mii_copies || mii_calls || destination != mii_nodes[0].value.name ||
        source != mii_names[mii_name_case] || count != 16)
        exit(2);
    mii_verify();
    ++mii_copies;
    strncpy(destination, source, count);
    for (i = 0; i < 16; ++i)
    {
        if (!mii_names[mii_name_case][i])
            ended = 1;
        mii_expected_nodes[0].value.name[i] = (char)(ended ? 0 : mii_names[mii_name_case][i]);
    }
    mii_verify();
    mii_expected_nodes[0].value.previous = mii_expected_tail;
    mii_expected_nodes[0].value.next = 0;
    if (mii_expected_tail)
    {
        tail_index = mii_expected_tail == &mii_nodes[1].value ? 1 : mii_expected_tail == &mii_nodes[2].value ? 2 : 3;
        mii_expected_nodes[tail_index].value.next = &mii_nodes[0].value;
    }
    mii_expected_tail = &mii_nodes[0].value;
    if (!mii_expected_head)
        mii_expected_head = &mii_nodes[0].value;
    return destination;
}
static void *mii_allocate(unsigned int bytes)
{
    MII_CHECK(mii_allocations == 0 && mii_calls == 0 && bytes == 36);
    if (mii_allocations || mii_calls || bytes != 36)
        exit(2);
    mii_verify();
    ++mii_allocations;
    if (mii_allocator_mode)
    {
        op_module_head = mii_expected_head = &mii_nodes[2].value;
        op_module_tail = mii_expected_tail = &mii_nodes[3].value;
        op_module_initialized = mii_expected_initialized = INT_MIN;
    }
    if (mii_fail_allocation)
        return 0;
    mii_expect_header();
    return &mii_nodes[0].value;
}
static int mii_callback(unsigned int event, unsigned int argument, unsigned int bits)
{
    MII_CHECK(mii_allocations == 1 && mii_calls == 0 && !mii_fail_allocation);
    if (mii_allocations != 1 || mii_calls || mii_fail_allocation)
        exit(2);
    mii_verify();
    MII_CHECK(event == 1 && argument == 0 && bits == 0);
    ++mii_calls;
    if (mii_callback_mode)
    {
        mii_nodes[0].value.status.raw = mii_expected_nodes[0].value.status.raw =
            mii_status_words[(mii_status_case + 3) % 8] ^ 0x00550000u;
        mii_nodes[0].value.id = mii_expected_nodes[0].value.id = mii_returns[(mii_result_case + 3) % 6];
        mii_nodes[0].value.name[0] = mii_expected_nodes[0].value.name[0] = 'M';
        op_module_head = mii_expected_head = &mii_nodes[3].value;
        op_module_tail = mii_expected_tail = &mii_nodes[2].value;
        op_module_initialized = mii_expected_initialized = -1;
    }
    if (mii_returns[mii_result_case] == 0 || mii_returns[mii_result_case] == 2)
        mii_expected_nodes[0].value.status.raw |= 1u;
    else if (mii_returns[mii_result_case] == 1)
        mii_expected_nodes[0].value.status.raw &= ~1u;
    mii_expected_result = mii_expected_nodes[0].value.id;
    return mii_returns[mii_result_case];
}
static void mii_seed(void)
{
    int i;
    memset(mii_nodes, 0x57, sizeof(mii_nodes));
    for (i = 0; i < 4; ++i)
    {
        mii_nodes[i].value.next = mii_nodes[i].value.previous = 0;
        mii_nodes[i].value.id = mii_returns[i];
        mii_nodes[i].value.status.raw = mii_status_words[(mii_status_case + i) % 8];
        mii_nodes[i].value.callback_word = 0x5678u;
    }
    op_module_head = mii_expected_head = (mii_endpoint_case & 1) ? &mii_nodes[1].value : 0;
    op_module_tail = mii_expected_tail = (mii_endpoint_case & 2) ? &mii_nodes[1].value : 0;
    op_module_initialized = mii_expected_initialized = 0x12345678;
    memcpy(mii_expected_nodes, mii_nodes, sizeof(mii_nodes));
    memcpy(mii_original_names, mii_names, sizeof(mii_names));
    mii_allocations = mii_calls = mii_copies = 0;
    mii_expected_result = 0;
}
static int op_test_module_install(void)
{
    int actual;
    mii_active = 1;
    for (mii_fail_allocation = 0; mii_fail_allocation < 2; ++mii_fail_allocation)
        for (mii_endpoint_case = 0; mii_endpoint_case < 4; ++mii_endpoint_case)
            for (mii_allocator_mode = 0; mii_allocator_mode < 2; ++mii_allocator_mode)
                for (mii_callback_mode = 0; mii_callback_mode < 2; ++mii_callback_mode)
                    for (mii_status_case = 0; mii_status_case < 8; ++mii_status_case)
                        for (mii_name_case = 0; mii_name_case < 5; ++mii_name_case)
                            for (mii_result_case = 0; mii_result_case < 6; ++mii_result_case)
                            {
                                mii_seed();
                                actual = op_module_install((unsigned int)mii_callback, mii_names[mii_name_case]);
                                MII_CHECK(actual == mii_expected_result);
                                MII_CHECK(mii_allocations == 1 && mii_calls == !mii_fail_allocation &&
                                          mii_copies == !mii_fail_allocation);
                                mii_verify();
                            }
    printf("module_install: %d checks, %d failures\n", mii_checks, mii_failures);
    mii_active = 0;
    return mii_failures != 0;
}

#undef MII_CHECK
