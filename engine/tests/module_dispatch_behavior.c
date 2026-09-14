#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mrs_checks, mrs_failures;
static void mrs_check(int ok, int line)
{
    ++mrs_checks;
    if (!ok)
    {
        ++mrs_failures;
        printf("line%d failed\n", line);
    }
}
#define MRS_CHECK(x) mrs_check(!!(x), __LINE__)
typedef struct MRS_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MRS_OWNED_NODE;
typedef struct MRS_OWNED_PAYLOAD
{
    unsigned int before;
    unsigned char bytes[8];
    unsigned int after;
} MRS_OWNED_PAYLOAD;
static MRS_OWNED_NODE mrs_nodes[5], mrs_expected_nodes[5];
static MRS_OWNED_PAYLOAD mrs_payload, mrs_expected_payload;
static OP_MODULE *mrs_expected_head, *mrs_expected_tail;
static int mrs_expected_initialized;
static int mrs_kind = 0, mrs_length, mrs_rotation, mrs_duplicates, mrs_mode, mrs_result_case, mrs_key_case,
           mrs_payload_case;
static int mrs_order[5], mrs_plan[5], mrs_planned, mrs_calls, mrs_query;
static unsigned int mrs_event_word, mrs_payload_word;
static const int mrs_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int mrs_results[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mrs_words[3] = {0, 0x3f800000u, 0xffffffffu};
static void mrs_verify(void)
{
    MRS_CHECK(memcmp(mrs_nodes, mrs_expected_nodes, sizeof(mrs_nodes)) == 0);
    MRS_CHECK(memcmp(&mrs_payload, &mrs_expected_payload, sizeof(mrs_payload)) == 0);
    MRS_CHECK(op_module_head == mrs_expected_head && op_module_tail == mrs_expected_tail &&
              op_module_initialized == mrs_expected_initialized);
}
static int mrs_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mrs_calls, next_node;
    MRS_CHECK(mrs_calls < mrs_planned && mrs_calls < 5);
    if (mrs_calls >= mrs_planned || mrs_calls >= 5)
        exit(2);
    MRS_CHECK(node == mrs_plan[position]);
    if (node != mrs_plan[position])
        exit(2);
    mrs_verify();
    MRS_CHECK(event == (mrs_kind == 0 ? 11u : mrs_kind == 1 ? 10u : mrs_event_word));
    MRS_CHECK(argument == (mrs_kind == 0   ? mrs_payload_word
                           : mrs_kind == 1 ? (unsigned int)(mrs_query ? mrs_query : mrs_expected_nodes[node].value.id)
                                           : 0u));
    MRS_CHECK(bits == (mrs_kind == 2 ? mrs_payload_word : 0u));
    ++mrs_calls;
    if (position == 0 && mrs_mode)
    {
        op_module_head = mrs_expected_head = &mrs_nodes[(node + 2) % 5].value;
        op_module_tail = mrs_expected_tail = &mrs_nodes[(node + 3) % 5].value;
        op_module_initialized = mrs_expected_initialized = INT_MIN;
        mrs_nodes[node].value.name[0] = mrs_expected_nodes[node].value.name[0] = 'Z';
        if (mrs_kind == 0)
            mrs_payload.bytes[2] = mrs_expected_payload.bytes[2] = 0x93;
        if (mrs_query == 0 && mrs_kind != 0 && (mrs_mode == 1 || mrs_mode == 2))
        {
            next_node = mrs_planned > 1 ? mrs_plan[1] : -1;
            if (mrs_kind == 1)
                mrs_nodes[node].value.next = mrs_expected_nodes[node].value.next =
                    next_node < 0 ? 0 : &mrs_nodes[next_node].value;
            else
                mrs_nodes[node].value.previous = mrs_expected_nodes[node].value.previous =
                    next_node < 0 ? 0 : &mrs_nodes[next_node].value;
        }
        if (mrs_mode == 3 && mrs_planned > 1)
        {
            next_node = mrs_plan[1];
            mrs_nodes[next_node].value.id = mrs_expected_nodes[next_node].value.id = INT_MIN;
        }
    }
    return mrs_results[(mrs_result_case + position) % 5];
}
static int mrs_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mrs_observe(0, a, b, c);
}
static int mrs_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mrs_observe(1, a, b, c);
}
static int mrs_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mrs_observe(2, a, b, c);
}
static int mrs_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mrs_observe(3, a, b, c);
}
static int mrs_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mrs_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mrs_callbacks[5] = {mrs_callback0, mrs_callback1, mrs_callback2, mrs_callback3,
                                              mrs_callback4};
static void mrs_seed(void)
{
    int i, n, start;
    memset(mrs_nodes, 0x57, sizeof(mrs_nodes));
    memset(&mrs_payload, 0x68, sizeof(mrs_payload));
    for (i = 0; i < 5; ++i)
    {
        mrs_nodes[i].value.next = mrs_nodes[i].value.previous = 0;
        mrs_nodes[i].value.id = mrs_keys[i];
        mrs_nodes[i].value.callback_word = (unsigned int)mrs_callbacks[i];
        mrs_order[i] = (i + mrs_rotation) % 5;
    }
    for (i = 0; i < mrs_length; ++i)
    {
        n = mrs_order[i];
        mrs_nodes[n].value.previous = i ? &mrs_nodes[mrs_order[i - 1]].value : 0;
        mrs_nodes[n].value.next = i + 1 < mrs_length ? &mrs_nodes[mrs_order[i + 1]].value : 0;
    }
    if (mrs_duplicates && mrs_length > 1)
        mrs_nodes[mrs_order[1]].value.id = mrs_nodes[mrs_order[0]].value.id;
    op_module_head = mrs_expected_head = mrs_length ? &mrs_nodes[mrs_order[0]].value : 0;
    op_module_tail = mrs_expected_tail = mrs_length ? &mrs_nodes[mrs_order[mrs_length - 1]].value : 0;
    op_module_initialized = mrs_expected_initialized = 0x12345678;
    memcpy(mrs_expected_nodes, mrs_nodes, sizeof(mrs_nodes));
    memcpy(&mrs_expected_payload, &mrs_payload, sizeof(mrs_payload));
    mrs_query = mrs_keys[mrs_key_case];
    mrs_event_word = mrs_words[(mrs_payload_case + 1) % 3];
    mrs_payload_word = mrs_words[mrs_payload_case];
    mrs_planned = mrs_calls = 0;
    if (mrs_query)
    {
        for (i = 0; i < mrs_length; ++i)
            if (mrs_expected_nodes[mrs_order[i]].value.id == mrs_query)
            {
                mrs_plan[mrs_planned++] = mrs_order[i];
                break;
            }
    }
    else if (mrs_kind != 0)
    {
        for (i = 0; i < mrs_length; ++i)
        {
            start = mrs_kind == 1 ? i : mrs_length - 1 - i;
            mrs_plan[mrs_planned++] = mrs_order[start];
        }
        if (mrs_mode == 1 && mrs_planned > 0)
            mrs_planned = 1;
        if (mrs_mode == 2 && mrs_planned > 1)
        {
            for (i = 1; i + 1 < mrs_planned; ++i)
                mrs_plan[i] = mrs_plan[i + 1];
            --mrs_planned;
        }
    }
}
static int op_test_module_send_restore(void)
{
    int actual, expected;
    for (mrs_length = 0; mrs_length < 6; ++mrs_length)
        for (mrs_rotation = 0; mrs_rotation < 5; ++mrs_rotation)
            for (mrs_duplicates = 0; mrs_duplicates < 2; ++mrs_duplicates)
                for (mrs_key_case = 0; mrs_key_case < 6; ++mrs_key_case)
                    for (mrs_mode = 0; mrs_mode < 4; ++mrs_mode)
                        for (mrs_result_case = 0; mrs_result_case < 5; ++mrs_result_case)
                            for (mrs_payload_case = 0; mrs_payload_case < 3; ++mrs_payload_case)
                            {
                                mrs_seed();
                                expected = mrs_query ? (mrs_planned     ? mrs_results[mrs_result_case]
                                                        : mrs_kind == 2 ? 1
                                                                        : 0)
                                                     : 0;
                                actual = op_module_send_restore(mrs_query, mrs_payload_word);
                                MRS_CHECK(actual == expected);
                                MRS_CHECK(mrs_calls == mrs_planned);
                                mrs_verify();
                            }
    printf("module_send_restore: %d checks, %d failures\n", mrs_checks, mrs_failures);
    return mrs_failures != 0;
}

#undef MRS_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mbs_checks, mbs_failures;
static void mbs_check(int ok, int line)
{
    ++mbs_checks;
    if (!ok)
    {
        ++mbs_failures;
        printf("line%d failed\n", line);
    }
}
#define MBS_CHECK(x) mbs_check(!!(x), __LINE__)
typedef struct MBS_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MBS_OWNED_NODE;
typedef struct MBS_OWNED_PAYLOAD
{
    unsigned int before;
    unsigned char bytes[8];
    unsigned int after;
} MBS_OWNED_PAYLOAD;
static MBS_OWNED_NODE mbs_nodes[5], mbs_expected_nodes[5];
static MBS_OWNED_PAYLOAD mbs_payload, mbs_expected_payload;
static OP_MODULE *mbs_expected_head, *mbs_expected_tail;
static int mbs_expected_initialized;
static int mbs_kind = 1, mbs_length, mbs_rotation, mbs_duplicates, mbs_mode, mbs_result_case, mbs_key_case,
           mbs_payload_case;
static int mbs_order[5], mbs_plan[5], mbs_planned, mbs_calls, mbs_query;
static unsigned int mbs_event_word, mbs_payload_word;
static const int mbs_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int mbs_results[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mbs_words[3] = {0, 0x3f800000u, 0xffffffffu};
static void mbs_verify(void)
{
    MBS_CHECK(memcmp(mbs_nodes, mbs_expected_nodes, sizeof(mbs_nodes)) == 0);
    MBS_CHECK(memcmp(&mbs_payload, &mbs_expected_payload, sizeof(mbs_payload)) == 0);
    MBS_CHECK(op_module_head == mbs_expected_head && op_module_tail == mbs_expected_tail &&
              op_module_initialized == mbs_expected_initialized);
}
static int mbs_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mbs_calls, next_node;
    MBS_CHECK(mbs_calls < mbs_planned && mbs_calls < 5);
    if (mbs_calls >= mbs_planned || mbs_calls >= 5)
        exit(2);
    MBS_CHECK(node == mbs_plan[position]);
    if (node != mbs_plan[position])
        exit(2);
    mbs_verify();
    MBS_CHECK(event == (mbs_kind == 0 ? 11u : mbs_kind == 1 ? 10u : mbs_event_word));
    MBS_CHECK(argument == (mbs_kind == 0   ? mbs_payload_word
                           : mbs_kind == 1 ? (unsigned int)(mbs_query ? mbs_query : mbs_expected_nodes[node].value.id)
                                           : 0u));
    MBS_CHECK(bits == (mbs_kind == 2 ? mbs_payload_word : 0u));
    ++mbs_calls;
    if (position == 0 && mbs_mode)
    {
        op_module_head = mbs_expected_head = &mbs_nodes[(node + 2) % 5].value;
        op_module_tail = mbs_expected_tail = &mbs_nodes[(node + 3) % 5].value;
        op_module_initialized = mbs_expected_initialized = INT_MIN;
        mbs_nodes[node].value.name[0] = mbs_expected_nodes[node].value.name[0] = 'Z';
        if (mbs_kind == 0)
            mbs_payload.bytes[2] = mbs_expected_payload.bytes[2] = 0x93;
        if (mbs_query == 0 && mbs_kind != 0 && (mbs_mode == 1 || mbs_mode == 2))
        {
            next_node = mbs_planned > 1 ? mbs_plan[1] : -1;
            if (mbs_kind == 1)
                mbs_nodes[node].value.next = mbs_expected_nodes[node].value.next =
                    next_node < 0 ? 0 : &mbs_nodes[next_node].value;
            else
                mbs_nodes[node].value.previous = mbs_expected_nodes[node].value.previous =
                    next_node < 0 ? 0 : &mbs_nodes[next_node].value;
        }
        if (mbs_mode == 3 && mbs_planned > 1)
        {
            next_node = mbs_plan[1];
            mbs_nodes[next_node].value.id = mbs_expected_nodes[next_node].value.id = INT_MIN;
        }
    }
    return mbs_results[(mbs_result_case + position) % 5];
}
static int mbs_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mbs_observe(0, a, b, c);
}
static int mbs_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mbs_observe(1, a, b, c);
}
static int mbs_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mbs_observe(2, a, b, c);
}
static int mbs_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mbs_observe(3, a, b, c);
}
static int mbs_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mbs_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mbs_callbacks[5] = {mbs_callback0, mbs_callback1, mbs_callback2, mbs_callback3,
                                              mbs_callback4};
static void mbs_seed(void)
{
    int i, n, start;
    memset(mbs_nodes, 0x57, sizeof(mbs_nodes));
    memset(&mbs_payload, 0x68, sizeof(mbs_payload));
    for (i = 0; i < 5; ++i)
    {
        mbs_nodes[i].value.next = mbs_nodes[i].value.previous = 0;
        mbs_nodes[i].value.id = mbs_keys[i];
        mbs_nodes[i].value.callback_word = (unsigned int)mbs_callbacks[i];
        mbs_order[i] = (i + mbs_rotation) % 5;
    }
    for (i = 0; i < mbs_length; ++i)
    {
        n = mbs_order[i];
        mbs_nodes[n].value.previous = i ? &mbs_nodes[mbs_order[i - 1]].value : 0;
        mbs_nodes[n].value.next = i + 1 < mbs_length ? &mbs_nodes[mbs_order[i + 1]].value : 0;
    }
    if (mbs_duplicates && mbs_length > 1)
        mbs_nodes[mbs_order[1]].value.id = mbs_nodes[mbs_order[0]].value.id;
    op_module_head = mbs_expected_head = mbs_length ? &mbs_nodes[mbs_order[0]].value : 0;
    op_module_tail = mbs_expected_tail = mbs_length ? &mbs_nodes[mbs_order[mbs_length - 1]].value : 0;
    op_module_initialized = mbs_expected_initialized = 0x12345678;
    memcpy(mbs_expected_nodes, mbs_nodes, sizeof(mbs_nodes));
    memcpy(&mbs_expected_payload, &mbs_payload, sizeof(mbs_payload));
    mbs_query = mbs_keys[mbs_key_case];
    mbs_event_word = mbs_words[(mbs_payload_case + 1) % 3];
    mbs_payload_word = mbs_words[mbs_payload_case];
    mbs_planned = mbs_calls = 0;
    if (mbs_query)
    {
        for (i = 0; i < mbs_length; ++i)
            if (mbs_expected_nodes[mbs_order[i]].value.id == mbs_query)
            {
                mbs_plan[mbs_planned++] = mbs_order[i];
                break;
            }
    }
    else if (mbs_kind != 0)
    {
        for (i = 0; i < mbs_length; ++i)
        {
            start = mbs_kind == 1 ? i : mbs_length - 1 - i;
            mbs_plan[mbs_planned++] = mbs_order[start];
        }
        if (mbs_mode == 1 && mbs_planned > 0)
            mbs_planned = 1;
        if (mbs_mode == 2 && mbs_planned > 1)
        {
            for (i = 1; i + 1 < mbs_planned; ++i)
                mbs_plan[i] = mbs_plan[i + 1];
            --mbs_planned;
        }
    }
}
static int op_test_module_broadcast_save(void)
{
    int actual, expected;
    for (mbs_length = 0; mbs_length < 6; ++mbs_length)
        for (mbs_rotation = 0; mbs_rotation < 5; ++mbs_rotation)
            for (mbs_duplicates = 0; mbs_duplicates < 2; ++mbs_duplicates)
                for (mbs_key_case = 0; mbs_key_case < 6; ++mbs_key_case)
                    for (mbs_mode = 0; mbs_mode < 4; ++mbs_mode)
                        for (mbs_result_case = 0; mbs_result_case < 5; ++mbs_result_case)
                            for (mbs_payload_case = 0; mbs_payload_case < 3; ++mbs_payload_case)
                            {
                                mbs_seed();
                                expected = mbs_query ? (mbs_planned     ? mbs_results[mbs_result_case]
                                                        : mbs_kind == 2 ? 1
                                                                        : 0)
                                                     : 0;
                                actual = op_module_broadcast_save(mbs_query);
                                MBS_CHECK(actual == expected);
                                MBS_CHECK(mbs_calls == mbs_planned);
                                mbs_verify();
                            }
    printf("module_broadcast_save: %d checks, %d failures\n", mbs_checks, mbs_failures);
    return mbs_failures != 0;
}

#undef MBS_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mbe_checks, mbe_failures;
static void mbe_check(int ok, int line)
{
    ++mbe_checks;
    if (!ok)
    {
        ++mbe_failures;
        printf("line%d failed\n", line);
    }
}
#define MBE_CHECK(x) mbe_check(!!(x), __LINE__)
typedef struct MBE_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MBE_OWNED_NODE;
typedef struct MBE_OWNED_PAYLOAD
{
    unsigned int before;
    unsigned char bytes[8];
    unsigned int after;
} MBE_OWNED_PAYLOAD;
static MBE_OWNED_NODE mbe_nodes[5], mbe_expected_nodes[5];
static MBE_OWNED_PAYLOAD mbe_payload, mbe_expected_payload;
static OP_MODULE *mbe_expected_head, *mbe_expected_tail;
static int mbe_expected_initialized;
static int mbe_kind = 2, mbe_length, mbe_rotation, mbe_duplicates, mbe_mode, mbe_result_case, mbe_key_case,
           mbe_payload_case;
static int mbe_order[5], mbe_plan[5], mbe_planned, mbe_calls, mbe_query;
static unsigned int mbe_event_word, mbe_payload_word;
static const int mbe_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int mbe_results[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mbe_words[3] = {0, 0x3f800000u, 0xffffffffu};
static void mbe_verify(void)
{
    MBE_CHECK(memcmp(mbe_nodes, mbe_expected_nodes, sizeof(mbe_nodes)) == 0);
    MBE_CHECK(memcmp(&mbe_payload, &mbe_expected_payload, sizeof(mbe_payload)) == 0);
    MBE_CHECK(op_module_head == mbe_expected_head && op_module_tail == mbe_expected_tail &&
              op_module_initialized == mbe_expected_initialized);
}
static int mbe_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mbe_calls, next_node;
    MBE_CHECK(mbe_calls < mbe_planned && mbe_calls < 5);
    if (mbe_calls >= mbe_planned || mbe_calls >= 5)
        exit(2);
    MBE_CHECK(node == mbe_plan[position]);
    if (node != mbe_plan[position])
        exit(2);
    mbe_verify();
    MBE_CHECK(event == (mbe_kind == 0 ? 11u : mbe_kind == 1 ? 10u : mbe_event_word));
    MBE_CHECK(argument == (mbe_kind == 0   ? mbe_payload_word
                           : mbe_kind == 1 ? (unsigned int)(mbe_query ? mbe_query : mbe_expected_nodes[node].value.id)
                                           : 0u));
    MBE_CHECK(bits == (mbe_kind == 2 ? mbe_payload_word : 0u));
    ++mbe_calls;
    if (position == 0 && mbe_mode)
    {
        op_module_head = mbe_expected_head = &mbe_nodes[(node + 2) % 5].value;
        op_module_tail = mbe_expected_tail = &mbe_nodes[(node + 3) % 5].value;
        op_module_initialized = mbe_expected_initialized = INT_MIN;
        mbe_nodes[node].value.name[0] = mbe_expected_nodes[node].value.name[0] = 'Z';
        if (mbe_kind == 0)
            mbe_payload.bytes[2] = mbe_expected_payload.bytes[2] = 0x93;
        if (mbe_query == 0 && mbe_kind != 0 && (mbe_mode == 1 || mbe_mode == 2))
        {
            next_node = mbe_planned > 1 ? mbe_plan[1] : -1;
            if (mbe_kind == 1)
                mbe_nodes[node].value.next = mbe_expected_nodes[node].value.next =
                    next_node < 0 ? 0 : &mbe_nodes[next_node].value;
            else
                mbe_nodes[node].value.previous = mbe_expected_nodes[node].value.previous =
                    next_node < 0 ? 0 : &mbe_nodes[next_node].value;
        }
        if (mbe_mode == 3 && mbe_planned > 1)
        {
            next_node = mbe_plan[1];
            mbe_nodes[next_node].value.id = mbe_expected_nodes[next_node].value.id = INT_MIN;
        }
    }
    return mbe_results[(mbe_result_case + position) % 5];
}
static int mbe_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mbe_observe(0, a, b, c);
}
static int mbe_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mbe_observe(1, a, b, c);
}
static int mbe_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mbe_observe(2, a, b, c);
}
static int mbe_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mbe_observe(3, a, b, c);
}
static int mbe_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mbe_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mbe_callbacks[5] = {mbe_callback0, mbe_callback1, mbe_callback2, mbe_callback3,
                                              mbe_callback4};
static void mbe_seed(void)
{
    int i, n, start;
    memset(mbe_nodes, 0x57, sizeof(mbe_nodes));
    memset(&mbe_payload, 0x68, sizeof(mbe_payload));
    for (i = 0; i < 5; ++i)
    {
        mbe_nodes[i].value.next = mbe_nodes[i].value.previous = 0;
        mbe_nodes[i].value.id = mbe_keys[i];
        mbe_nodes[i].value.callback_word = (unsigned int)mbe_callbacks[i];
        mbe_order[i] = (i + mbe_rotation) % 5;
    }
    for (i = 0; i < mbe_length; ++i)
    {
        n = mbe_order[i];
        mbe_nodes[n].value.previous = i ? &mbe_nodes[mbe_order[i - 1]].value : 0;
        mbe_nodes[n].value.next = i + 1 < mbe_length ? &mbe_nodes[mbe_order[i + 1]].value : 0;
    }
    if (mbe_duplicates && mbe_length > 1)
        mbe_nodes[mbe_order[1]].value.id = mbe_nodes[mbe_order[0]].value.id;
    op_module_head = mbe_expected_head = mbe_length ? &mbe_nodes[mbe_order[0]].value : 0;
    op_module_tail = mbe_expected_tail = mbe_length ? &mbe_nodes[mbe_order[mbe_length - 1]].value : 0;
    op_module_initialized = mbe_expected_initialized = 0x12345678;
    memcpy(mbe_expected_nodes, mbe_nodes, sizeof(mbe_nodes));
    memcpy(&mbe_expected_payload, &mbe_payload, sizeof(mbe_payload));
    mbe_query = mbe_keys[mbe_key_case];
    mbe_event_word = mbe_words[(mbe_payload_case + 1) % 3];
    mbe_payload_word = mbe_words[mbe_payload_case];
    mbe_planned = mbe_calls = 0;
    if (mbe_query)
    {
        for (i = 0; i < mbe_length; ++i)
            if (mbe_expected_nodes[mbe_order[i]].value.id == mbe_query)
            {
                mbe_plan[mbe_planned++] = mbe_order[i];
                break;
            }
    }
    else if (mbe_kind != 0)
    {
        for (i = 0; i < mbe_length; ++i)
        {
            start = mbe_kind == 1 ? i : mbe_length - 1 - i;
            mbe_plan[mbe_planned++] = mbe_order[start];
        }
        if (mbe_mode == 1 && mbe_planned > 0)
            mbe_planned = 1;
        if (mbe_mode == 2 && mbe_planned > 1)
        {
            for (i = 1; i + 1 < mbe_planned; ++i)
                mbe_plan[i] = mbe_plan[i + 1];
            --mbe_planned;
        }
    }
}
static int op_test_module_broadcast_event(void)
{
    int actual, expected;
    for (mbe_length = 0; mbe_length < 6; ++mbe_length)
        for (mbe_rotation = 0; mbe_rotation < 5; ++mbe_rotation)
            for (mbe_duplicates = 0; mbe_duplicates < 2; ++mbe_duplicates)
                for (mbe_key_case = 0; mbe_key_case < 6; ++mbe_key_case)
                    for (mbe_mode = 0; mbe_mode < 4; ++mbe_mode)
                        for (mbe_result_case = 0; mbe_result_case < 5; ++mbe_result_case)
                            for (mbe_payload_case = 0; mbe_payload_case < 3; ++mbe_payload_case)
                            {
                                mbe_seed();
                                expected = mbe_query ? (mbe_planned     ? mbe_results[mbe_result_case]
                                                        : mbe_kind == 2 ? 1
                                                                        : 0)
                                                     : 0;
                                actual = op_module_broadcast_event(mbe_query, mbe_event_word, mbe_payload_word);
                                MBE_CHECK(actual == expected);
                                MBE_CHECK(mbe_calls == mbe_planned);
                                mbe_verify();
                            }
    printf("module_broadcast_event: %d checks, %d failures\n", mbe_checks, mbe_failures);
    return mbe_failures != 0;
}

#undef MBE_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int msu_checks, msu_failures;
static void msu_check(int ok, int line)
{
    ++msu_checks;
    if (!ok)
    {
        ++msu_failures;
        printf("line%d failed\n", line);
    }
}
#define MSU_CHECK(x) msu_check(!!(x), __LINE__)
typedef struct MSU_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MSU_OWNED_NODE;
static MSU_OWNED_NODE msu_nodes[5], msu_expected_nodes[5];
static OP_MODULE *msu_expected_head, *msu_expected_tail;
static int msu_expected_initialized;
static int msu_resume = 0, msu_length, msu_rotation, msu_duplicates, msu_mode, msu_result_case, msu_key_case,
           msu_status_case;
static int msu_order[5], msu_visits[5], msu_plan[5], msu_planned, msu_calls, msu_query, msu_first_position,
    msu_link_destination;
static const int msu_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int msu_returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int msu_status_words[8] = {0,           8,           0x80000000u, 0x80000008u,
                                                 0xfffffff7u, 0xffffffffu, 0x12345670u, 0x12345678u};
static void msu_verify(void)
{
    MSU_CHECK(memcmp(msu_nodes, msu_expected_nodes, sizeof(msu_nodes)) == 0);
    MSU_CHECK(op_module_head == msu_expected_head && op_module_tail == msu_expected_tail &&
              op_module_initialized == msu_expected_initialized);
}
static int msu_eligible(unsigned int word)
{
    return msu_resume ? (word & 8) != 0 : (word & 8) == 0;
}
static int msu_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = msu_calls, returned;
    MSU_CHECK(msu_calls < msu_planned && msu_calls < 5);
    if (msu_calls >= msu_planned || msu_calls >= 5)
        exit(2);
    MSU_CHECK(node == msu_plan[position]);
    if (node != msu_plan[position])
        exit(2);
    msu_verify();
    MSU_CHECK(event == (msu_resume ? 9u : 8u));
    MSU_CHECK(argument == 0 && bits == 0);
    returned = msu_returns[(msu_result_case + position) % 5];
    ++msu_calls;
    if (msu_mode)
    {
        msu_nodes[node].value.status.raw = msu_expected_nodes[node].value.status.raw =
            msu_status_words[(msu_status_case + position + 3) % 8] ^ 0x00550000u;
        msu_nodes[node].value.name[0] = msu_expected_nodes[node].value.name[0] = 'L';
    }
    if (position == 0 && msu_mode)
    {
        op_module_head = msu_expected_head = &msu_nodes[(node + 2) % 5].value;
        op_module_tail = msu_expected_tail = &msu_nodes[(node + 3) % 5].value;
        op_module_initialized = msu_expected_initialized = INT_MIN;
        if (msu_query == 0 && (msu_mode == 1 || msu_mode == 2))
        {
            if (msu_resume)
                msu_nodes[node].value.previous = msu_expected_nodes[node].value.previous =
                    msu_link_destination < 0 ? 0 : &msu_nodes[msu_link_destination].value;
            else
                msu_nodes[node].value.next = msu_expected_nodes[node].value.next =
                    msu_link_destination < 0 ? 0 : &msu_nodes[msu_link_destination].value;
        }
    }
    if (returned == 0)
    {
        if (msu_resume)
            msu_expected_nodes[node].value.status.raw &= ~8u;
        else
            msu_expected_nodes[node].value.status.raw |= 8u;
    }
    return returned;
}
static int msu_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return msu_observe(0, a, b, c);
}
static int msu_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return msu_observe(1, a, b, c);
}
static int msu_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return msu_observe(2, a, b, c);
}
static int msu_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return msu_observe(3, a, b, c);
}
static int msu_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return msu_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK msu_callbacks[5] = {msu_callback0, msu_callback1, msu_callback2, msu_callback3,
                                              msu_callback4};
static void msu_seed(void)
{
    int i, n, selected;
    memset(msu_nodes, 0x57, sizeof(msu_nodes));
    for (i = 0; i < 5; ++i)
    {
        msu_nodes[i].value.next = msu_nodes[i].value.previous = 0;
        msu_nodes[i].value.id = msu_keys[i];
        msu_nodes[i].value.callback_word = (unsigned int)msu_callbacks[i];
        msu_nodes[i].value.status.raw = msu_status_words[(msu_status_case + i) % 8];
        msu_order[i] = (i + msu_rotation) % 5;
    }
    for (i = 0; i < msu_length; ++i)
    {
        n = msu_order[i];
        msu_nodes[n].value.previous = i ? &msu_nodes[msu_order[i - 1]].value : 0;
        msu_nodes[n].value.next = i + 1 < msu_length ? &msu_nodes[msu_order[i + 1]].value : 0;
    }
    if (msu_duplicates && msu_length > 1)
        msu_nodes[msu_order[1]].value.id = msu_nodes[msu_order[0]].value.id;
    op_module_head = msu_expected_head = msu_length ? &msu_nodes[msu_order[0]].value : 0;
    op_module_tail = msu_expected_tail = msu_length ? &msu_nodes[msu_order[msu_length - 1]].value : 0;
    op_module_initialized = msu_expected_initialized = 0x12345678;
    memcpy(msu_expected_nodes, msu_nodes, sizeof(msu_nodes));
    msu_query = msu_keys[msu_key_case];
    msu_planned = msu_calls = 0;
    msu_link_destination = msu_first_position = -1;
    if (msu_query)
    {
        selected = -1;
        for (i = 0; i < msu_length; ++i)
            if (msu_expected_nodes[msu_order[i]].value.id == msu_query)
            {
                selected = msu_order[i];
                break;
            }
        if (selected >= 0 && msu_eligible(msu_expected_nodes[selected].value.status.raw))
            msu_plan[msu_planned++] = selected;
    }
    else
    {
        for (i = 0; i < msu_length; ++i)
            msu_visits[i] = msu_order[msu_resume ? msu_length - 1 - i : i];
        for (i = 0; i < msu_length; ++i)
            if (msu_eligible(msu_expected_nodes[msu_visits[i]].value.status.raw))
            {
                msu_first_position = i;
                break;
            }
        if (msu_first_position >= 0)
        {
            if (msu_mode == 2 && msu_first_position + 2 < msu_length)
                msu_link_destination = msu_visits[msu_first_position + 2];
            for (i = msu_first_position; i < msu_length; ++i)
            {
                if (msu_mode == 1 && i > msu_first_position)
                    break;
                if (msu_mode == 2 && i == msu_first_position + 1)
                    continue;
                if (msu_eligible(msu_expected_nodes[msu_visits[i]].value.status.raw))
                    msu_plan[msu_planned++] = msu_visits[i];
            }
        }
    }
}
static int op_test_module_suspend_all(void)
{
    int expected, actual;
    for (msu_length = 0; msu_length < 6; ++msu_length)
        for (msu_rotation = 0; msu_rotation < 5; ++msu_rotation)
            for (msu_duplicates = 0; msu_duplicates < 2; ++msu_duplicates)
                for (msu_key_case = 0; msu_key_case < 6; ++msu_key_case)
                    for (msu_mode = 0; msu_mode < 4; ++msu_mode)
                        for (msu_result_case = 0; msu_result_case < 5; ++msu_result_case)
                            for (msu_status_case = 0; msu_status_case < 8; ++msu_status_case)
                            {
                                msu_seed();
                                expected = msu_query ? (msu_planned ? msu_returns[msu_result_case] : msu_resume) : 0;
                                actual = op_module_suspend_all(msu_query);
                                MSU_CHECK(actual == expected);
                                MSU_CHECK(msu_calls == msu_planned);
                                msu_verify();
                            }
    printf("module_suspend_all: %d checks, %d failures\n", msu_checks, msu_failures);
    return msu_failures != 0;
}

#undef MSU_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mre_checks, mre_failures;
static void mre_check(int ok, int line)
{
    ++mre_checks;
    if (!ok)
    {
        ++mre_failures;
        printf("line%d failed\n", line);
    }
}
#define MRE_CHECK(x) mre_check(!!(x), __LINE__)
typedef struct MRE_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MRE_OWNED_NODE;
static MRE_OWNED_NODE mre_nodes[5], mre_expected_nodes[5];
static OP_MODULE *mre_expected_head, *mre_expected_tail;
static int mre_expected_initialized;
static int mre_resume = 1, mre_length, mre_rotation, mre_duplicates, mre_mode, mre_result_case, mre_key_case,
           mre_status_case;
static int mre_order[5], mre_visits[5], mre_plan[5], mre_planned, mre_calls, mre_query, mre_first_position,
    mre_link_destination;
static const int mre_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int mre_returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mre_status_words[8] = {0,           8,           0x80000000u, 0x80000008u,
                                                 0xfffffff7u, 0xffffffffu, 0x12345670u, 0x12345678u};
static void mre_verify(void)
{
    MRE_CHECK(memcmp(mre_nodes, mre_expected_nodes, sizeof(mre_nodes)) == 0);
    MRE_CHECK(op_module_head == mre_expected_head && op_module_tail == mre_expected_tail &&
              op_module_initialized == mre_expected_initialized);
}
static int mre_eligible(unsigned int word)
{
    return mre_resume ? (word & 8) != 0 : (word & 8) == 0;
}
static int mre_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mre_calls, returned;
    MRE_CHECK(mre_calls < mre_planned && mre_calls < 5);
    if (mre_calls >= mre_planned || mre_calls >= 5)
        exit(2);
    MRE_CHECK(node == mre_plan[position]);
    if (node != mre_plan[position])
        exit(2);
    mre_verify();
    MRE_CHECK(event == (mre_resume ? 9u : 8u));
    MRE_CHECK(argument == 0 && bits == 0);
    returned = mre_returns[(mre_result_case + position) % 5];
    ++mre_calls;
    if (mre_mode)
    {
        mre_nodes[node].value.status.raw = mre_expected_nodes[node].value.status.raw =
            mre_status_words[(mre_status_case + position + 3) % 8] ^ 0x00550000u;
        mre_nodes[node].value.name[0] = mre_expected_nodes[node].value.name[0] = 'L';
    }
    if (position == 0 && mre_mode)
    {
        op_module_head = mre_expected_head = &mre_nodes[(node + 2) % 5].value;
        op_module_tail = mre_expected_tail = &mre_nodes[(node + 3) % 5].value;
        op_module_initialized = mre_expected_initialized = INT_MIN;
        if (mre_query == 0 && (mre_mode == 1 || mre_mode == 2))
        {
            if (mre_resume)
                mre_nodes[node].value.previous = mre_expected_nodes[node].value.previous =
                    mre_link_destination < 0 ? 0 : &mre_nodes[mre_link_destination].value;
            else
                mre_nodes[node].value.next = mre_expected_nodes[node].value.next =
                    mre_link_destination < 0 ? 0 : &mre_nodes[mre_link_destination].value;
        }
    }
    if (returned == 0)
    {
        if (mre_resume)
            mre_expected_nodes[node].value.status.raw &= ~8u;
        else
            mre_expected_nodes[node].value.status.raw |= 8u;
    }
    return returned;
}
static int mre_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mre_observe(0, a, b, c);
}
static int mre_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mre_observe(1, a, b, c);
}
static int mre_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mre_observe(2, a, b, c);
}
static int mre_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mre_observe(3, a, b, c);
}
static int mre_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mre_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mre_callbacks[5] = {mre_callback0, mre_callback1, mre_callback2, mre_callback3,
                                              mre_callback4};
static void mre_seed(void)
{
    int i, n, selected;
    memset(mre_nodes, 0x57, sizeof(mre_nodes));
    for (i = 0; i < 5; ++i)
    {
        mre_nodes[i].value.next = mre_nodes[i].value.previous = 0;
        mre_nodes[i].value.id = mre_keys[i];
        mre_nodes[i].value.callback_word = (unsigned int)mre_callbacks[i];
        mre_nodes[i].value.status.raw = mre_status_words[(mre_status_case + i) % 8];
        mre_order[i] = (i + mre_rotation) % 5;
    }
    for (i = 0; i < mre_length; ++i)
    {
        n = mre_order[i];
        mre_nodes[n].value.previous = i ? &mre_nodes[mre_order[i - 1]].value : 0;
        mre_nodes[n].value.next = i + 1 < mre_length ? &mre_nodes[mre_order[i + 1]].value : 0;
    }
    if (mre_duplicates && mre_length > 1)
        mre_nodes[mre_order[1]].value.id = mre_nodes[mre_order[0]].value.id;
    op_module_head = mre_expected_head = mre_length ? &mre_nodes[mre_order[0]].value : 0;
    op_module_tail = mre_expected_tail = mre_length ? &mre_nodes[mre_order[mre_length - 1]].value : 0;
    op_module_initialized = mre_expected_initialized = 0x12345678;
    memcpy(mre_expected_nodes, mre_nodes, sizeof(mre_nodes));
    mre_query = mre_keys[mre_key_case];
    mre_planned = mre_calls = 0;
    mre_link_destination = mre_first_position = -1;
    if (mre_query)
    {
        selected = -1;
        for (i = 0; i < mre_length; ++i)
            if (mre_expected_nodes[mre_order[i]].value.id == mre_query)
            {
                selected = mre_order[i];
                break;
            }
        if (selected >= 0 && mre_eligible(mre_expected_nodes[selected].value.status.raw))
            mre_plan[mre_planned++] = selected;
    }
    else
    {
        for (i = 0; i < mre_length; ++i)
            mre_visits[i] = mre_order[mre_resume ? mre_length - 1 - i : i];
        for (i = 0; i < mre_length; ++i)
            if (mre_eligible(mre_expected_nodes[mre_visits[i]].value.status.raw))
            {
                mre_first_position = i;
                break;
            }
        if (mre_first_position >= 0)
        {
            if (mre_mode == 2 && mre_first_position + 2 < mre_length)
                mre_link_destination = mre_visits[mre_first_position + 2];
            for (i = mre_first_position; i < mre_length; ++i)
            {
                if (mre_mode == 1 && i > mre_first_position)
                    break;
                if (mre_mode == 2 && i == mre_first_position + 1)
                    continue;
                if (mre_eligible(mre_expected_nodes[mre_visits[i]].value.status.raw))
                    mre_plan[mre_planned++] = mre_visits[i];
            }
        }
    }
}
static int op_test_module_resume_all(void)
{
    int expected, actual;
    for (mre_length = 0; mre_length < 6; ++mre_length)
        for (mre_rotation = 0; mre_rotation < 5; ++mre_rotation)
            for (mre_duplicates = 0; mre_duplicates < 2; ++mre_duplicates)
                for (mre_key_case = 0; mre_key_case < 6; ++mre_key_case)
                    for (mre_mode = 0; mre_mode < 4; ++mre_mode)
                        for (mre_result_case = 0; mre_result_case < 5; ++mre_result_case)
                            for (mre_status_case = 0; mre_status_case < 8; ++mre_status_case)
                            {
                                mre_seed();
                                expected = mre_query ? (mre_planned ? mre_returns[mre_result_case] : mre_resume) : 0;
                                actual = op_module_resume_all(mre_query);
                                MRE_CHECK(actual == expected);
                                MRE_CHECK(mre_calls == mre_planned);
                                mre_verify();
                            }
    printf("module_resume_all: %d checks, %d failures\n", mre_checks, mre_failures);
    return mre_failures != 0;
}

#undef MRE_CHECK

#include "../src/module.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
static int mst_checks, mst_failures;
static void mst_check(int ok, int line)
{
    ++mst_checks;
    if (!ok)
    {
        ++mst_failures;
        printf("line%d failed\n", line);
    }
}
#define MST_CHECK(x) mst_check(!!(x), __LINE__)
typedef struct MST_OWNED_NODE
{
    unsigned int before;
    OP_MODULE value;
    unsigned int after;
} MST_OWNED_NODE;
static MST_OWNED_NODE mst_nodes[5], mst_expected_nodes[5];
static OP_MODULE *mst_expected_head, *mst_expected_tail;
static int mst_expected_initialized;
static int mst_length, mst_rotation, mst_duplicates, mst_mode, mst_result_case, mst_key_case, mst_status_case;
static int mst_order[5], mst_visits[5], mst_plan[5], mst_planned, mst_calls, mst_query, mst_first_position,
    mst_link_destination;
static const int mst_keys[6] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static const int mst_returns[5] = {0, 1, -1, INT_MIN, INT_MAX};
static const unsigned int mst_status_words[8] = {0,           2,           0x80000000u, 0x80000002u,
                                                 0xfffffffdu, 0xffffffffu, 0x12345670u, 0x12345672u};
static void mst_verify(void)
{
    MST_CHECK(memcmp(mst_nodes, mst_expected_nodes, sizeof(mst_nodes)) == 0);
    MST_CHECK(op_module_head == mst_expected_head && op_module_tail == mst_expected_tail &&
              op_module_initialized == mst_expected_initialized);
}
static int mst_eligible(unsigned int word)
{
    return (word & 2) != 0;
}
static int mst_observe(int node, unsigned int event, unsigned int argument, unsigned int bits)
{
    int position = mst_calls, returned;
    MST_CHECK(mst_calls < mst_planned && mst_calls < 5);
    if (mst_calls >= mst_planned || mst_calls >= 5)
        exit(2);
    MST_CHECK(node == mst_plan[position]);
    if (node != mst_plan[position])
        exit(2);
    mst_verify();
    MST_CHECK(event == 4u);
    MST_CHECK(argument == 0 && bits == 0);
    returned = mst_returns[(mst_result_case + position) % 5];
    ++mst_calls;
    if (mst_mode)
    {
        mst_nodes[node].value.status.raw = mst_expected_nodes[node].value.status.raw =
            mst_status_words[(mst_status_case + position + 3) % 8] ^ 0x00550000u;
        mst_nodes[node].value.name[0] = mst_expected_nodes[node].value.name[0] = 'L';
    }
    if (position == 0 && mst_mode)
    {
        op_module_head = mst_expected_head = &mst_nodes[(node + 2) % 5].value;
        op_module_tail = mst_expected_tail = &mst_nodes[(node + 3) % 5].value;
        op_module_initialized = mst_expected_initialized = INT_MIN;
        if (mst_query == 0 && (mst_mode == 1 || mst_mode == 2))
        {
            mst_nodes[node].value.next = mst_expected_nodes[node].value.next =
                mst_link_destination < 0 ? 0 : &mst_nodes[mst_link_destination].value;
        }
    }
    if (returned == 0)
    {
        mst_expected_nodes[node].value.status.raw &= ~2u;
    }
    return returned;
}
static int mst_callback0(unsigned int a, unsigned int b, unsigned int c)
{
    return mst_observe(0, a, b, c);
}
static int mst_callback1(unsigned int a, unsigned int b, unsigned int c)
{
    return mst_observe(1, a, b, c);
}
static int mst_callback2(unsigned int a, unsigned int b, unsigned int c)
{
    return mst_observe(2, a, b, c);
}
static int mst_callback3(unsigned int a, unsigned int b, unsigned int c)
{
    return mst_observe(3, a, b, c);
}
static int mst_callback4(unsigned int a, unsigned int b, unsigned int c)
{
    return mst_observe(4, a, b, c);
}
static OP_MODULE_CALLBACK mst_callbacks[5] = {mst_callback0, mst_callback1, mst_callback2, mst_callback3,
                                              mst_callback4};
static void mst_seed(void)
{
    int i, n, selected;
    memset(mst_nodes, 0x57, sizeof(mst_nodes));
    for (i = 0; i < 5; ++i)
    {
        mst_nodes[i].value.next = mst_nodes[i].value.previous = 0;
        mst_nodes[i].value.id = mst_keys[i];
        mst_nodes[i].value.callback_word = (unsigned int)mst_callbacks[i];
        mst_nodes[i].value.status.raw = mst_status_words[(mst_status_case + i) % 8];
        mst_order[i] = (i + mst_rotation) % 5;
    }
    for (i = 0; i < mst_length; ++i)
    {
        n = mst_order[i];
        mst_nodes[n].value.previous = i ? &mst_nodes[mst_order[i - 1]].value : 0;
        mst_nodes[n].value.next = i + 1 < mst_length ? &mst_nodes[mst_order[i + 1]].value : 0;
    }
    if (mst_duplicates && mst_length > 1)
        mst_nodes[mst_order[1]].value.id = mst_nodes[mst_order[0]].value.id;
    op_module_head = mst_expected_head = mst_length ? &mst_nodes[mst_order[0]].value : 0;
    op_module_tail = mst_expected_tail = mst_length ? &mst_nodes[mst_order[mst_length - 1]].value : 0;
    op_module_initialized = mst_expected_initialized = 0x12345678;
    memcpy(mst_expected_nodes, mst_nodes, sizeof(mst_nodes));
    mst_query = mst_keys[mst_key_case];
    mst_planned = mst_calls = 0;
    mst_link_destination = mst_first_position = -1;
    if (mst_query)
    {
        selected = -1;
        for (i = 0; i < mst_length; ++i)
            if (mst_expected_nodes[mst_order[i]].value.id == mst_query)
            {
                selected = mst_order[i];
                break;
            }
        if (selected >= 0 && mst_eligible(mst_expected_nodes[selected].value.status.raw))
            mst_plan[mst_planned++] = selected;
    }
    else
    {
        for (i = 0; i < mst_length; ++i)
            mst_visits[i] = mst_order[i];
        for (i = 0; i < mst_length; ++i)
            if (mst_eligible(mst_expected_nodes[mst_visits[i]].value.status.raw))
            {
                mst_first_position = i;
                break;
            }
        if (mst_first_position >= 0)
        {
            if (mst_mode == 2 && mst_first_position + 2 < mst_length)
                mst_link_destination = mst_visits[mst_first_position + 2];
            for (i = mst_first_position; i < mst_length; ++i)
            {
                if (mst_mode == 1 && i > mst_first_position)
                    break;
                if (mst_mode == 2 && i == mst_first_position + 1)
                    continue;
                if (mst_eligible(mst_expected_nodes[mst_visits[i]].value.status.raw))
                    mst_plan[mst_planned++] = mst_visits[i];
            }
        }
    }
}
static int op_test_module_stop_all(void)
{
    int expected, actual;
    for (mst_length = 0; mst_length < 6; ++mst_length)
        for (mst_rotation = 0; mst_rotation < 5; ++mst_rotation)
            for (mst_duplicates = 0; mst_duplicates < 2; ++mst_duplicates)
                for (mst_key_case = 0; mst_key_case < 6; ++mst_key_case)
                    for (mst_mode = 0; mst_mode < 4; ++mst_mode)
                        for (mst_result_case = 0; mst_result_case < 5; ++mst_result_case)
                            for (mst_status_case = 0; mst_status_case < 8; ++mst_status_case)
                            {
                                mst_seed();
                                expected = mst_query ? (mst_planned ? mst_returns[mst_result_case] : 1) : 0;
                                actual = op_module_stop_all(mst_query);
                                MST_CHECK(actual == expected);
                                MST_CHECK(mst_calls == mst_planned);
                                mst_verify();
                            }
    printf("module_stop_all: %d checks, %d failures\n", mst_checks, mst_failures);
    return mst_failures != 0;
}

#undef MST_CHECK
