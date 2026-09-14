#include "../src/trig_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int msc_checks, msc_failures;
static void msc_check(int ok, int line)
{
    ++msc_checks;
    if (!ok)
    {
        ++msc_failures;
        printf("line%d failed\n", line);
    }
}
#define MSC_CHECK(x) msc_check(!!(x), __LINE__)
static float msc_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int msc_float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int msc_ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void msc_result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = msc_float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        MSC_CHECK(actual == expected);
        return;
    }
    MSC_CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = msc_ordered(actual);
    b = msc_ordered(expected);
    MSC_CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct MSC_OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} MSC_OWNED;
static MSC_OWNED msc_owned, msc_expected_owned;
static void msc_seed(void)
{
    memset(&msc_owned, 0x57, sizeof(msc_owned));
    msc_owned.first = 7.0f;
    msc_owned.second = -9.0f;
    memcpy(&msc_expected_owned, &msc_owned, sizeof(msc_owned));
}
static const unsigned int msc_vectors[][3] = {
    {0xc4340000u, 0xb4bbbd2eu, 0x3f800000u}, {0xc3b40000u, 0xb43bbd2eu, 0x3f800000u},
    {0xc3340000u, 0x33bbbd2eu, 0xbf800000u}, {0xc2b40000u, 0xbf800000u, 0xb33bbd2eu},
    {0xc2340000u, 0xbf3504f3u, 0x3f3504f3u}, {0xbf800000u, 0xbc8ef859u, 0x3f7ff605u},
    {0x80000000u, 0x80000000u, 0x3f800000u}, {0x00000000u, 0x00000000u, 0x3f800000u},
    {0x3f800000u, 0x3c8ef859u, 0x3f7ff605u}, {0x41f00000u, 0x3f000000u, 0x3f5db3d7u},
    {0x42340000u, 0x3f3504f3u, 0x3f3504f3u}, {0x42700000u, 0x3f5db3d8u, 0x3effffffu},
    {0x42b40000u, 0x3f800000u, 0xb33bbd2eu}, {0x43340000u, 0xb3bbbd2eu, 0xbf800000u},
    {0x43b3c000u, 0xbc0efacdu, 0x3f7ffd81u}, {0x43b40000u, 0x343bbd2eu, 0x3f800000u},
    {0x44340000u, 0x34bbbd2eu, 0x3f800000u}};
static int op_test_sincos_degrees(void)
{
    unsigned int i, msc_mode, old_control;
    float *sine, *cosine;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(msc_vectors) / sizeof(msc_vectors[0]); ++i)
        for (msc_mode = 0; msc_mode < 5; ++msc_mode)
        {
            msc_seed();
            sine = (msc_mode == 1 || msc_mode == 3 || msc_mode == 4) ? &msc_owned.first : 0;
            cosine = (msc_mode == 2 || msc_mode == 3) ? &msc_owned.second : (msc_mode == 4 ? &msc_owned.first : 0);
            op_sincos_degrees(msc_from_bits(msc_vectors[i][0]), sine, cosine);
            if (msc_mode == 1 || msc_mode == 3)
                msc_result_check(msc_owned.first, msc_vectors[i][1]);
            else if (msc_mode == 4)
                msc_result_check(msc_owned.first, msc_vectors[i][2]);
            else
                MSC_CHECK(msc_float_bits(msc_owned.first) == msc_float_bits(msc_expected_owned.first));
            if (msc_mode == 2 || msc_mode == 3)
                msc_result_check(msc_owned.second, msc_vectors[i][2]);
            else
                MSC_CHECK(msc_float_bits(msc_owned.second) == msc_float_bits(msc_expected_owned.second));
            MSC_CHECK(msc_owned.before == msc_expected_owned.before);
            MSC_CHECK(msc_owned.after == msc_expected_owned.after);
            MSC_CHECK(memcmp(msc_owned.tail, msc_expected_owned.tail, sizeof(msc_owned.tail)) == 0);
        }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("sincos_degrees: %d checks, %d failures\n", msc_checks, msc_failures);
    return msc_failures != 0;
}

#undef MSC_CHECK

#include "../src/trig_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int mco_checks, mco_failures;
static void mco_check(int ok, int line)
{
    ++mco_checks;
    if (!ok)
    {
        ++mco_failures;
        printf("line%d failed\n", line);
    }
}
#define MCO_CHECK(x) mco_check(!!(x), __LINE__)
static float mco_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mco_float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mco_ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void mco_result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = mco_float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        MCO_CHECK(actual == expected);
        return;
    }
    MCO_CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = mco_ordered(actual);
    b = mco_ordered(expected);
    MCO_CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct MCO_OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} MCO_OWNED;
static MCO_OWNED mco_owned, mco_expected_owned;
static void mco_seed(void)
{
    memset(&mco_owned, 0x57, sizeof(mco_owned));
    mco_owned.first = 7.0f;
    mco_owned.second = -9.0f;
    memcpy(&mco_expected_owned, &mco_owned, sizeof(mco_owned));
}
static const unsigned int mco_vectors[][2] = {
    {0xc4340000u, 0x3f800000u}, {0xc3b40000u, 0x3f800000u}, {0xc3340000u, 0xbf800000u}, {0xc2b40000u, 0x32510b46u},
    {0xc2340000u, 0x3f3504f3u}, {0xbf800000u, 0x3f7ff605u}, {0x80000000u, 0x3f800000u}, {0x00000000u, 0x3f800000u},
    {0x3f800000u, 0x3f7ff605u}, {0x41f00000u, 0x3f5db3d7u}, {0x42340000u, 0x3f3504f3u}, {0x42700000u, 0x3f000000u},
    {0x42b40000u, 0x32510b46u}, {0x43340000u, 0xbf800000u}, {0x43b3c000u, 0x3f7ffd81u}, {0x43b40000u, 0x3f800000u},
    {0x44340000u, 0x3f800000u}};
#if _MSC_VER != 1100
/* Modern float multiplication rounds the radian argument before promotion. */
static const unsigned int mco_modern_expected[] = {
    0x3f800000u, 0x3f800000u, 0xbf800000u, 0xb33bbd2eu, 0x3f3504f3u, 0x3f7ff605u, 0x3f800000u, 0x3f800000u, 0x3f7ff605u,
    0x3f5db3d7u, 0x3f3504f3u, 0x3effffffu, 0xb33bbd2eu, 0xbf800000u, 0x3f7ffd81u, 0x3f800000u, 0x3f800000u};
#endif
static int op_test_cosine_degrees(void)
{
    unsigned int i, old_control;
    float mco_input;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(mco_vectors) / sizeof(mco_vectors[0]); ++i)
    {
        mco_seed();
        mco_input = mco_from_bits(mco_vectors[i][0]);
#if _MSC_VER == 1100
        mco_result_check(op_cosine_degrees(mco_input), mco_vectors[i][1]);
#else
        mco_result_check(op_cosine_degrees(mco_input), mco_modern_expected[i]);
#endif
        MCO_CHECK(mco_float_bits(mco_input) == mco_vectors[i][0]);
        MCO_CHECK(memcmp(&mco_owned, &mco_expected_owned, sizeof(mco_owned)) == 0);
    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("cosine_degrees: %d checks, %d failures\n", mco_checks, mco_failures);
    return mco_failures != 0;
}

#undef MCO_CHECK

#include "../src/trig_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int mas_checks, mas_failures;
static void mas_check(int ok, int line)
{
    ++mas_checks;
    if (!ok)
    {
        ++mas_failures;
        printf("line%d failed\n", line);
    }
}
#define MAS_CHECK(x) mas_check(!!(x), __LINE__)
static float mas_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mas_float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mas_ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void mas_result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = mas_float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        MAS_CHECK(actual == expected);
        return;
    }
    MAS_CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = mas_ordered(actual);
    b = mas_ordered(expected);
    MAS_CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct MAS_OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} MAS_OWNED;
static MAS_OWNED mas_owned, mas_expected_owned;
static void mas_seed(void)
{
    memset(&mas_owned, 0x57, sizeof(mas_owned));
    mas_owned.first = 7.0f;
    mas_owned.second = -9.0f;
    memcpy(&mas_expected_owned, &mas_owned, sizeof(mas_owned));
}
static const unsigned int mas_vectors[][2] = {
    {0xbf800000u, 0xc2b40071u}, {0xbf600000u, 0xc2742ea7u}, {0xbf000000u, 0xc1f00096u}, {0xbe000000u, 0xc0e5c950u},
    {0x80000000u, 0x80000000u}, {0x00000000u, 0x00000000u}, {0x3e000000u, 0x40e5c950u}, {0x3f000000u, 0x41f00096u},
    {0x3f600000u, 0x42742ea7u}, {0x3f800000u, 0x42b40071u}};
static int op_test_asine_degrees(void)
{
    unsigned int i, old_control;
    float mas_input;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(mas_vectors) / sizeof(mas_vectors[0]); ++i)
    {
        mas_seed();
        mas_input = mas_from_bits(mas_vectors[i][0]);
        mas_result_check(op_asine_degrees(mas_input), mas_vectors[i][1]);
        MAS_CHECK(mas_float_bits(mas_input) == mas_vectors[i][0]);
        MAS_CHECK(memcmp(&mas_owned, &mas_expected_owned, sizeof(mas_owned)) == 0);
    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("asine_degrees: %d checks, %d failures\n", mas_checks, mas_failures);
    return mas_failures != 0;
}

#undef MAS_CHECK

#include "../src/trig_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int mta_checks, mta_failures;
static void mta_check(int ok, int line)
{
    ++mta_checks;
    if (!ok)
    {
        ++mta_failures;
        printf("line%d failed\n", line);
    }
}
#define MTA_CHECK(x) mta_check(!!(x), __LINE__)
static float mta_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mta_float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mta_ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void mta_result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = mta_float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        MTA_CHECK(actual == expected);
        return;
    }
    MTA_CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = mta_ordered(actual);
    b = mta_ordered(expected);
    MTA_CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct MTA_OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} MTA_OWNED;
static MTA_OWNED mta_owned, mta_expected_owned;
static void mta_seed(void)
{
    memset(&mta_owned, 0x57, sizeof(mta_owned));
    mta_owned.first = 7.0f;
    mta_owned.second = -9.0f;
    memcpy(&mta_expected_owned, &mta_owned, sizeof(mta_owned));
}
static const unsigned int mta_vectors[][2] = {
    {0xc2700000u, 0xbfddb3d7u}, {0xc2340000u, 0xbf800000u}, {0xbf800000u, 0xbc8efdedu},
    {0x80000000u, 0x80000000u}, {0x00000000u, 0x00000000u}, {0x3f800000u, 0x3c8efdedu},
    {0x41f00000u, 0x3f13cd3au}, {0x42340000u, 0x3f800000u}, {0x42700000u, 0x3fddb3d7u}};
static unsigned int mta_index, mta_input_bits;
static int mta_calls, mta_mutate;
float op_wrap_degrees(float mta_input)
{
    MTA_CHECK(mta_calls++ == 0);
    MTA_CHECK(mta_float_bits(mta_input) == mta_input_bits);
    MTA_CHECK(memcmp(&mta_owned, &mta_expected_owned, sizeof(mta_owned)) == 0);
    if (mta_mutate)
    {
        mta_expected_owned.tail[7] = 0xa6;
        mta_owned.tail[7] = 0xa6;
    }
    return mta_from_bits(mta_vectors[mta_index][0]);
}
static int op_test_tangent_degrees(void)
{
    unsigned int i, m, old_control;
    static const unsigned int inputs[] = {0, 0x80000000u, 0x447a0000u, 0xc47a0000u, 0x3f800000u};
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(inputs) / sizeof(inputs[0]); ++i)
        for (mta_index = 0; mta_index < sizeof(mta_vectors) / sizeof(mta_vectors[0]); ++mta_index)
            for (m = 0; m < 2; ++m)
            {
                mta_seed();
                mta_input_bits = inputs[i];
                mta_calls = 0;
                mta_mutate = (int)m;
                mta_result_check(op_tangent_degrees(mta_from_bits(mta_input_bits)), mta_vectors[mta_index][1]);
                MTA_CHECK(mta_calls == 1);
                MTA_CHECK(memcmp(&mta_owned, &mta_expected_owned, sizeof(mta_owned)) == 0);
            }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("tangent_degrees: %d checks, %d failures\n", mta_checks, mta_failures);
    return mta_failures != 0;
}

#undef MTA_CHECK

#include "../src/trig_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int mac_checks, mac_failures;
static void mac_check(int ok, int line)
{
    ++mac_checks;
    if (!ok)
    {
        ++mac_failures;
        printf("line%d failed\n", line);
    }
}
#define MAC_CHECK(x) mac_check(!!(x), __LINE__)
static float mac_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mac_float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mac_ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void mac_result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = mac_float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        MAC_CHECK(actual == expected);
        return;
    }
    MAC_CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = mac_ordered(actual);
    b = mac_ordered(expected);
    MAC_CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct MAC_OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} MAC_OWNED;
static MAC_OWNED mac_owned, mac_expected_owned;
static void mac_seed(void)
{
    memset(&mac_owned, 0x57, sizeof(mac_owned));
    mac_owned.first = 7.0f;
    mac_owned.second = -9.0f;
    memcpy(&mac_expected_owned, &mac_owned, sizeof(mac_owned));
}
static const unsigned int mac_vectors[][2] = {
    {0xbf800000u, 0x43340071u}, {0xbf600000u, 0x43170be2u}, {0xbf000000u, 0x42f00096u}, {0xbe000000u, 0x42c25d06u},
    {0x80000000u, 0x42b40071u}, {0x00000000u, 0x42b40071u}, {0x3e000000u, 0x42a5a3dcu}, {0x3f000000u, 0x42700096u},
    {0x3f600000u, 0x41e7a475u}, {0x3f800000u, 0x00000000u}};
static int op_test_scalar_angle(void)
{
    unsigned int i, old_control;
    float mac_input;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(mac_vectors) / sizeof(mac_vectors[0]); ++i)
    {
        mac_seed();
        mac_input = mac_from_bits(mac_vectors[i][0]);
        mac_result_check(op_scalar_angle(mac_input), mac_vectors[i][1]);
        MAC_CHECK(mac_float_bits(mac_input) == mac_vectors[i][0]);
        MAC_CHECK(memcmp(&mac_owned, &mac_expected_owned, sizeof(mac_owned)) == 0);
    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("scalar_angle: %d checks, %d failures\n", mac_checks, mac_failures);
    return mac_failures != 0;
}

#undef MAC_CHECK

#include "../src/trig_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int mac2_checks, mac2_failures;
static void mac2_check(int ok, int line)
{
    ++mac2_checks;
    if (!ok)
    {
        ++mac2_failures;
        printf("line%d failed\n", line);
    }
}
#define MAC2_CHECK(x) mac2_check(!!(x), __LINE__)
static float mac2_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mac2_float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mac2_ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void mac2_result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = mac2_float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        MAC2_CHECK(actual == expected);
        return;
    }
    MAC2_CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = mac2_ordered(actual);
    b = mac2_ordered(expected);
    MAC2_CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct MAC2_OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} MAC2_OWNED;
static MAC2_OWNED mac2_owned, mac2_expected_owned;
static void mac2_seed(void)
{
    memset(&mac2_owned, 0x57, sizeof(mac2_owned));
    mac2_owned.first = 7.0f;
    mac2_owned.second = -9.0f;
    memcpy(&mac2_expected_owned, &mac2_owned, sizeof(mac2_owned));
}
static const unsigned int mac2_vectors[][2] = {
    {0xbf800000u, 0x43340071u}, {0xbf600000u, 0x43170be2u}, {0xbf000000u, 0x42f00096u}, {0xbe000000u, 0x42c25d06u},
    {0x80000000u, 0x42b40071u}, {0x00000000u, 0x42b40071u}, {0x3e000000u, 0x42a5a3dcu}, {0x3f000000u, 0x42700096u},
    {0x3f600000u, 0x41e7a475u}, {0x3f800000u, 0x00000000u}};
static int op_test_acos_degrees_copy2(void)
{
    unsigned int i, old_control;
    float mac2_input;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(mac2_vectors) / sizeof(mac2_vectors[0]); ++i)
    {
        mac2_seed();
        mac2_input = mac2_from_bits(mac2_vectors[i][0]);
        mac2_result_check(op_acos_degrees_copy2(mac2_input), mac2_vectors[i][1]);
        MAC2_CHECK(mac2_float_bits(mac2_input) == mac2_vectors[i][0]);
        MAC2_CHECK(memcmp(&mac2_owned, &mac2_expected_owned, sizeof(mac2_owned)) == 0);
    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("acos_degrees_copy2: %d checks, %d failures\n", mac2_checks, mac2_failures);
    return mac2_failures != 0;
}

#undef MAC2_CHECK

#include "../src/trig_helpers.h"
#include <stdio.h>
#include <string.h>
#include <float.h>
static int mac3_checks, mac3_failures;
static void mac3_check(int ok, int line)
{
    ++mac3_checks;
    if (!ok)
    {
        ++mac3_failures;
        printf("line%d failed\n", line);
    }
}
#define MAC3_CHECK(x) mac3_check(!!(x), __LINE__)
static float mac3_from_bits(unsigned int value)
{
    float result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mac3_float_bits(float value)
{
    unsigned int result;
    memcpy(&result, &value, 4);
    return result;
}
static unsigned int mac3_ordered(unsigned int value)
{
    return value & 0x80000000u ? ~value : value | 0x80000000u;
}
static void mac3_result_check(float value, unsigned int expected)
{
    unsigned int actual, a, b;
    actual = mac3_float_bits(value);
    if ((expected & 0x7fffffffu) == 0)
    {
        MAC3_CHECK(actual == expected);
        return;
    }
    MAC3_CHECK((actual & 0x7f800000u) != 0x7f800000u);
    a = mac3_ordered(actual);
    b = mac3_ordered(expected);
    MAC3_CHECK((a > b ? a - b : b - a) <= 4);
}
typedef struct MAC3_OWNED
{
    unsigned int before;
    float first, second;
    unsigned char tail[32];
    unsigned int after;
} MAC3_OWNED;
static MAC3_OWNED mac3_owned, mac3_expected_owned;
static void mac3_seed(void)
{
    memset(&mac3_owned, 0x57, sizeof(mac3_owned));
    mac3_owned.first = 7.0f;
    mac3_owned.second = -9.0f;
    memcpy(&mac3_expected_owned, &mac3_owned, sizeof(mac3_owned));
}
static const unsigned int mac3_vectors[][2] = {
    {0xbf800000u, 0x43340071u}, {0xbf600000u, 0x43170be2u}, {0xbf000000u, 0x42f00096u}, {0xbe000000u, 0x42c25d06u},
    {0x80000000u, 0x42b40071u}, {0x00000000u, 0x42b40071u}, {0x3e000000u, 0x42a5a3dcu}, {0x3f000000u, 0x42700096u},
    {0x3f600000u, 0x41e7a475u}, {0x3f800000u, 0x00000000u}};
static int op_test_acos_degrees_copy3(void)
{
    unsigned int i, old_control;
    float mac3_input;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(mac3_vectors) / sizeof(mac3_vectors[0]); ++i)
    {
        mac3_seed();
        mac3_input = mac3_from_bits(mac3_vectors[i][0]);
        mac3_result_check(op_acos_degrees_copy3(mac3_input), mac3_vectors[i][1]);
        MAC3_CHECK(mac3_float_bits(mac3_input) == mac3_vectors[i][0]);
        MAC3_CHECK(memcmp(&mac3_owned, &mac3_expected_owned, sizeof(mac3_owned)) == 0);
    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("acos_degrees_copy3: %d checks, %d failures\n", mac3_checks, mac3_failures);
    return mac3_failures != 0;
}

#undef MAC3_CHECK

#include "../src/geometry.h"
typedef struct MCY_VECTOR
{
    unsigned int before;
    OP_VECTOR3 value;
    unsigned int after;
} MCY_VECTOR;
static int mcy_checks, mcy_failures;
static void mcy_check(int ok)
{
    ++mcy_checks;
    if (!ok)
        ++mcy_failures;
}
static int op_test_math_connected(void)
{
    static const unsigned int cases[][3] = {
        {0xbf800000u, 0, 0xc2b400e1u}, {0xbf600000u, 0, 0xc2742f88u}, {0xbf000000u, 0, 0xc1f00259u},
        {0xbe000000u, 0, 0xc0e5d05bu}, {0x80000000u, 0, 0xba614c8bu}, {0x00000000u, 0, 0xba614c8bu},
        {0x3e000000u, 0, 0x40e5c246u}, {0x3f000000u, 0, 0x41effed4u}, {0x3f600000u, 0, 0x42742dc6u},
        {0x3f800000u, 0, 0x00000000u}, {0xbf800000u, 1, 0x00000000u}, {0xbf600000u, 1, 0x4247d957u},
        {0xbf000000u, 1, 0x4167a0f0u}, {0xbe000000u, 1, 0x3f64f980u}, {0x80000000u, 1, 0xba614c8bu},
        {0x00000000u, 1, 0xba614c8bu}, {0x3e000000u, 1, 0x3f64f980u}, {0x3f000000u, 1, 0x4167a0f0u},
        {0x3f600000u, 1, 0x4247d957u}, {0x3f800000u, 1, 0x00000000u}};
    unsigned int i, old_control;
    MCY_VECTOR a, b, expected_a, expected_b;
    float result, expected, delta;
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i)
    {
        memset(&a, 0x57, sizeof(a));
        memset(&b, 0x68, sizeof(b));
        a.value.x = mac_from_bits(cases[i][0]);
        a.value.y = a.value.z = 0;
        b.value.x = 1;
        b.value.y = b.value.z = 0;
        expected_a = a;
        expected_b = b;
        result = op_vector_angle(&a.value, cases[i][1] ? &a.value : &b.value);
        expected = mac_from_bits(cases[i][2]);
        delta = result - expected;
        mcy_check((mac_float_bits(result) & 0x7f800000u) != 0x7f800000u);
        if (cases[i][2] == 0)
            mcy_check(mac_float_bits(result) == 0);
        else
            mcy_check(delta >= -0.00002f && delta <= 0.00002f);
        mcy_check(memcmp(&a, &expected_a, sizeof(a)) == 0);
        mcy_check(memcmp(&b, &expected_b, sizeof(b)) == 0);
    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("math vector connected: %d checks, %d failures\n", mcy_checks, mcy_failures);
    return mcy_failures != 0;
}
