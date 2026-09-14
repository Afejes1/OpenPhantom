#include "../src/window_state.h"
#include <stdio.h>
#include <string.h>
static int wsh_checks, wsh_failures;
static void wsh_check(int ok, int line)
{
    ++wsh_checks;
    if (!ok)
    {
        ++wsh_failures;
        printf("line%d failed\n", line);
    }
}
#define WSH_CHECK(x) wsh_check(!!(x), __LINE__)
OP_WINDOW_HANDLE_WORD op_window_handle, op_instance_handle;
OP_WINDOW_RECT op_window_rect;
typedef struct WSH_OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} WSH_OWNED_RECT;
static WSH_OWNED_RECT wsh_input, wsh_expected_input;
static OP_WINDOW_RECT wsh_expected_rect;
static unsigned int wsh_expected_window, wsh_expected_instance;
static const unsigned int wsh_bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void wsh_seed(unsigned int profile)
{
    unsigned int i;
    memset(&wsh_input, 0x57, sizeof(wsh_input));
    op_window_handle = wsh_bits[profile % 6];
    op_instance_handle = wsh_bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = wsh_bits[(profile / 36 + i) % 6];
        wsh_input.value.words[i] = wsh_bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&wsh_expected_input, &wsh_input, sizeof(wsh_input));
    memcpy(&wsh_expected_rect, &op_window_rect, sizeof(op_window_rect));
    wsh_expected_window = op_window_handle;
    wsh_expected_instance = op_instance_handle;
}
static void wsh_verify(void)
{
    WSH_CHECK(op_window_handle == wsh_expected_window);
    WSH_CHECK(op_instance_handle == wsh_expected_instance);
    WSH_CHECK(memcmp(&op_window_rect, &wsh_expected_rect, sizeof(op_window_rect)) == 0);
    WSH_CHECK(memcmp(&wsh_input, &wsh_expected_input, sizeof(wsh_input)) == 0);
}
static int op_test_window_set_handle(void)
{
    unsigned int profile, value;
    for (profile = 0; profile < 216; ++profile)
        for (value = 0; value < 6; ++value)
        {
            wsh_seed(profile);
            wsh_expected_window = wsh_bits[value];
            op_window_set_handle(wsh_bits[value]);
            wsh_verify();
        }
    printf("window_set_handle: %d checks, %d failures\n", wsh_checks, wsh_failures);
    return wsh_failures != 0;
}

#undef WSH_CHECK

#include "../src/window_state.h"
#include <stdio.h>
#include <string.h>
static int wgh_checks, wgh_failures;
static void wgh_check(int ok, int line)
{
    ++wgh_checks;
    if (!ok)
    {
        ++wgh_failures;
        printf("line%d failed\n", line);
    }
}
#define WGH_CHECK(x) wgh_check(!!(x), __LINE__)
typedef struct WGH_OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} WGH_OWNED_RECT;
static WGH_OWNED_RECT wgh_input, wgh_expected_input;
static OP_WINDOW_RECT wgh_expected_rect;
static unsigned int wgh_expected_window, wgh_expected_instance;
static const unsigned int wgh_bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void wgh_seed(unsigned int profile)
{
    unsigned int i;
    memset(&wgh_input, 0x57, sizeof(wgh_input));
    op_window_handle = wgh_bits[profile % 6];
    op_instance_handle = wgh_bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = wgh_bits[(profile / 36 + i) % 6];
        wgh_input.value.words[i] = wgh_bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&wgh_expected_input, &wgh_input, sizeof(wgh_input));
    memcpy(&wgh_expected_rect, &op_window_rect, sizeof(op_window_rect));
    wgh_expected_window = op_window_handle;
    wgh_expected_instance = op_instance_handle;
}
static void wgh_verify(void)
{
    WGH_CHECK(op_window_handle == wgh_expected_window);
    WGH_CHECK(op_instance_handle == wgh_expected_instance);
    WGH_CHECK(memcmp(&op_window_rect, &wgh_expected_rect, sizeof(op_window_rect)) == 0);
    WGH_CHECK(memcmp(&wgh_input, &wgh_expected_input, sizeof(wgh_input)) == 0);
}
static int op_test_window_get_handle(void)
{
    unsigned int profile;
    for (profile = 0; profile < 216; ++profile)
    {
        wgh_seed(profile);
        WGH_CHECK(op_window_get_handle() == wgh_expected_window);
        wgh_verify();
        op_window_handle ^= 0xffffffffu;
        wgh_expected_window ^= 0xffffffffu;
        WGH_CHECK(op_window_get_handle() == wgh_expected_window);
        wgh_verify();
    }
    printf("window_get_handle: %d checks, %d failures\n", wgh_checks, wgh_failures);
    return wgh_failures != 0;
}

#undef WGH_CHECK

#include "../src/window_state.h"
#include <stdio.h>
#include <string.h>
static int wsi_checks, wsi_failures;
static void wsi_check(int ok, int line)
{
    ++wsi_checks;
    if (!ok)
    {
        ++wsi_failures;
        printf("line%d failed\n", line);
    }
}
#define WSI_CHECK(x) wsi_check(!!(x), __LINE__)
typedef struct WSI_OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} WSI_OWNED_RECT;
static WSI_OWNED_RECT wsi_input, wsi_expected_input;
static OP_WINDOW_RECT wsi_expected_rect;
static unsigned int wsi_expected_window, wsi_expected_instance;
static const unsigned int wsi_bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void wsi_seed(unsigned int profile)
{
    unsigned int i;
    memset(&wsi_input, 0x57, sizeof(wsi_input));
    op_window_handle = wsi_bits[profile % 6];
    op_instance_handle = wsi_bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = wsi_bits[(profile / 36 + i) % 6];
        wsi_input.value.words[i] = wsi_bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&wsi_expected_input, &wsi_input, sizeof(wsi_input));
    memcpy(&wsi_expected_rect, &op_window_rect, sizeof(op_window_rect));
    wsi_expected_window = op_window_handle;
    wsi_expected_instance = op_instance_handle;
}
static void wsi_verify(void)
{
    WSI_CHECK(op_window_handle == wsi_expected_window);
    WSI_CHECK(op_instance_handle == wsi_expected_instance);
    WSI_CHECK(memcmp(&op_window_rect, &wsi_expected_rect, sizeof(op_window_rect)) == 0);
    WSI_CHECK(memcmp(&wsi_input, &wsi_expected_input, sizeof(wsi_input)) == 0);
}
static int op_test_window_set_instance(void)
{
    unsigned int profile, value;
    for (profile = 0; profile < 216; ++profile)
        for (value = 0; value < 6; ++value)
        {
            wsi_seed(profile);
            wsi_expected_instance = wsi_bits[value];
            op_window_set_instance(wsi_bits[value]);
            wsi_verify();
        }
    printf("window_set_instance: %d checks, %d failures\n", wsi_checks, wsi_failures);
    return wsi_failures != 0;
}

#undef WSI_CHECK

#include "../src/window_state.h"
#include <stdio.h>
#include <string.h>
static int wgi_checks, wgi_failures;
static void wgi_check(int ok, int line)
{
    ++wgi_checks;
    if (!ok)
    {
        ++wgi_failures;
        printf("line%d failed\n", line);
    }
}
#define WGI_CHECK(x) wgi_check(!!(x), __LINE__)
typedef struct WGI_OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} WGI_OWNED_RECT;
static WGI_OWNED_RECT wgi_input, wgi_expected_input;
static OP_WINDOW_RECT wgi_expected_rect;
static unsigned int wgi_expected_window, wgi_expected_instance;
static const unsigned int wgi_bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void wgi_seed(unsigned int profile)
{
    unsigned int i;
    memset(&wgi_input, 0x57, sizeof(wgi_input));
    op_window_handle = wgi_bits[profile % 6];
    op_instance_handle = wgi_bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = wgi_bits[(profile / 36 + i) % 6];
        wgi_input.value.words[i] = wgi_bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&wgi_expected_input, &wgi_input, sizeof(wgi_input));
    memcpy(&wgi_expected_rect, &op_window_rect, sizeof(op_window_rect));
    wgi_expected_window = op_window_handle;
    wgi_expected_instance = op_instance_handle;
}
static void wgi_verify(void)
{
    WGI_CHECK(op_window_handle == wgi_expected_window);
    WGI_CHECK(op_instance_handle == wgi_expected_instance);
    WGI_CHECK(memcmp(&op_window_rect, &wgi_expected_rect, sizeof(op_window_rect)) == 0);
    WGI_CHECK(memcmp(&wgi_input, &wgi_expected_input, sizeof(wgi_input)) == 0);
}
static int op_test_window_get_instance(void)
{
    unsigned int profile;
    for (profile = 0; profile < 216; ++profile)
    {
        wgi_seed(profile);
        WGI_CHECK(op_window_get_instance() == wgi_expected_instance);
        wgi_verify();
        op_instance_handle ^= 0xffffffffu;
        wgi_expected_instance ^= 0xffffffffu;
        WGI_CHECK(op_window_get_instance() == wgi_expected_instance);
        wgi_verify();
    }
    printf("window_get_instance: %d checks, %d failures\n", wgi_checks, wgi_failures);
    return wgi_failures != 0;
}

#undef WGI_CHECK

#include "../src/window_state.h"
#include <stdio.h>
#include <string.h>
static int wsr_checks, wsr_failures;
static void wsr_check(int ok, int line)
{
    ++wsr_checks;
    if (!ok)
    {
        ++wsr_failures;
        printf("line%d failed\n", line);
    }
}
#define WSR_CHECK(x) wsr_check(!!(x), __LINE__)
typedef struct WSR_OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} WSR_OWNED_RECT;
static WSR_OWNED_RECT wsr_input, wsr_expected_input;
static OP_WINDOW_RECT wsr_expected_rect;
static unsigned int wsr_expected_window, wsr_expected_instance;
static const unsigned int wsr_bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void wsr_seed(unsigned int profile)
{
    unsigned int i;
    memset(&wsr_input, 0x57, sizeof(wsr_input));
    op_window_handle = wsr_bits[profile % 6];
    op_instance_handle = wsr_bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = wsr_bits[(profile / 36 + i) % 6];
        wsr_input.value.words[i] = wsr_bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&wsr_expected_input, &wsr_input, sizeof(wsr_input));
    memcpy(&wsr_expected_rect, &op_window_rect, sizeof(op_window_rect));
    wsr_expected_window = op_window_handle;
    wsr_expected_instance = op_instance_handle;
}
static void wsr_verify(void)
{
    WSR_CHECK(op_window_handle == wsr_expected_window);
    WSR_CHECK(op_instance_handle == wsr_expected_instance);
    WSR_CHECK(memcmp(&op_window_rect, &wsr_expected_rect, sizeof(op_window_rect)) == 0);
    WSR_CHECK(memcmp(&wsr_input, &wsr_expected_input, sizeof(wsr_input)) == 0);
}
static int op_test_window_set_rect(void)
{
    unsigned int profile;
    int alias;
    for (profile = 0; profile < 216; ++profile)
        for (alias = 0; alias < 2; ++alias)
        {
            wsr_seed(profile);
            if (!alias)
                memcpy(&wsr_expected_rect, &wsr_expected_input.value, sizeof(wsr_expected_rect));
            op_window_set_rect(alias ? &op_window_rect : &wsr_input.value);
            wsr_verify();
        }
    printf("window_set_rect: %d checks, %d failures\n", wsr_checks, wsr_failures);
    return wsr_failures != 0;
}

#undef WSR_CHECK

#include "../src/window_state.h"
#include <stdio.h>
#include <string.h>
static int wgr_checks, wgr_failures;
static void wgr_check(int ok, int line)
{
    ++wgr_checks;
    if (!ok)
    {
        ++wgr_failures;
        printf("line%d failed\n", line);
    }
}
#define WGR_CHECK(x) wgr_check(!!(x), __LINE__)
typedef struct WGR_OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} WGR_OWNED_RECT;
static WGR_OWNED_RECT wgr_input, wgr_expected_input;
static OP_WINDOW_RECT wgr_expected_rect;
static unsigned int wgr_expected_window, wgr_expected_instance;
static const unsigned int wgr_bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void wgr_seed(unsigned int profile)
{
    unsigned int i;
    memset(&wgr_input, 0x57, sizeof(wgr_input));
    op_window_handle = wgr_bits[profile % 6];
    op_instance_handle = wgr_bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = wgr_bits[(profile / 36 + i) % 6];
        wgr_input.value.words[i] = wgr_bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&wgr_expected_input, &wgr_input, sizeof(wgr_input));
    memcpy(&wgr_expected_rect, &op_window_rect, sizeof(op_window_rect));
    wgr_expected_window = op_window_handle;
    wgr_expected_instance = op_instance_handle;
}
static void wgr_verify(void)
{
    WGR_CHECK(op_window_handle == wgr_expected_window);
    WGR_CHECK(op_instance_handle == wgr_expected_instance);
    WGR_CHECK(memcmp(&op_window_rect, &wgr_expected_rect, sizeof(op_window_rect)) == 0);
    WGR_CHECK(memcmp(&wgr_input, &wgr_expected_input, sizeof(wgr_input)) == 0);
}
static int op_test_window_get_rect(void)
{
    unsigned int profile, index;
    OP_WINDOW_RECT *result;
    for (profile = 0; profile < 216; ++profile)
    {
        wgr_seed(profile);
        result = op_window_get_rect();
        WGR_CHECK(result == &op_window_rect);
        wgr_verify();
        if (result != &op_window_rect)
            return 1;
        index = profile % 4;
        wgr_expected_rect.words[index] = wgr_bits[(profile + 2) % 6];
        result->words[index] = wgr_bits[(profile + 2) % 6];
        wgr_verify();
        WGR_CHECK(op_window_get_rect() == &op_window_rect);
        wgr_verify();
    }
    printf("window_get_rect: %d checks, %d failures\n", wgr_checks, wgr_failures);
    return wgr_failures != 0;
}

#undef WGR_CHECK

#include "../src/window_state.h"
#include <stdio.h>
#include <string.h>
static int wcy_checks, wcy_failures;
static void wcy_check(int ok, int line)
{
    ++wcy_checks;
    if (!ok)
    {
        ++wcy_failures;
        printf("line%d failed\n", line);
    }
}
#define WCY_CHECK(x) wcy_check(!!(x), __LINE__)
typedef struct WCY_OWNED_RECT
{
    unsigned int before;
    OP_WINDOW_RECT value;
    unsigned int after;
} WCY_OWNED_RECT;
static WCY_OWNED_RECT wcy_input, wcy_expected_input;
static OP_WINDOW_RECT wcy_expected_rect;
static unsigned int wcy_expected_window, wcy_expected_instance;
static const unsigned int wcy_bits[] = {0, 1, 0x7fffffffu, 0x80000000u, 0xffffffffu, 0x12345678u};
static void wcy_seed(unsigned int profile)
{
    unsigned int i;
    memset(&wcy_input, 0x57, sizeof(wcy_input));
    op_window_handle = wcy_bits[profile % 6];
    op_instance_handle = wcy_bits[(profile / 6) % 6];
    for (i = 0; i < 4; ++i)
    {
        op_window_rect.words[i] = wcy_bits[(profile / 36 + i) % 6];
        wcy_input.value.words[i] = wcy_bits[(profile / 36 + i + 2) % 6];
    }
    memcpy(&wcy_expected_input, &wcy_input, sizeof(wcy_input));
    memcpy(&wcy_expected_rect, &op_window_rect, sizeof(op_window_rect));
    wcy_expected_window = op_window_handle;
    wcy_expected_instance = op_instance_handle;
}
static void wcy_verify(void)
{
    WCY_CHECK(op_window_handle == wcy_expected_window);
    WCY_CHECK(op_instance_handle == wcy_expected_instance);
    WCY_CHECK(memcmp(&op_window_rect, &wcy_expected_rect, sizeof(op_window_rect)) == 0);
    WCY_CHECK(memcmp(&wcy_input, &wcy_expected_input, sizeof(wcy_input)) == 0);
}
static int op_test_window_connected(void)
{
    unsigned int profile, value, index;
    OP_WINDOW_RECT *live;
    for (profile = 0; profile < 216; ++profile)
        for (value = 0; value < 6; ++value)
        {
            wcy_seed(profile);
            wcy_expected_window = wcy_bits[value];
            op_window_set_handle(wcy_bits[value]);
            WCY_CHECK(op_window_get_handle() == wcy_expected_window);
            wcy_verify();
            wcy_expected_instance = wcy_bits[(value + 1) % 6];
            op_window_set_instance(wcy_bits[(value + 1) % 6]);
            WCY_CHECK(op_window_get_instance() == wcy_expected_instance);
            WCY_CHECK(op_window_get_handle() == wcy_expected_window);
            wcy_verify();
            memcpy(&wcy_expected_rect, &wcy_expected_input.value, sizeof(wcy_expected_rect));
            op_window_set_rect(&wcy_input.value);
            live = op_window_get_rect();
            WCY_CHECK(live == &op_window_rect);
            wcy_verify();
            if (live != &op_window_rect)
                return 1;
            index = profile % 4;
            wcy_expected_rect.words[index] = wcy_bits[(value + 2) % 6];
            live->words[index] = wcy_bits[(value + 2) % 6];
            wcy_verify();
            op_window_set_rect(op_window_get_rect());
            WCY_CHECK(op_window_get_rect() == &op_window_rect);
            WCY_CHECK(op_window_get_handle() == wcy_expected_window);
            WCY_CHECK(op_window_get_instance() == wcy_expected_instance);
            wcy_verify();
        }
    printf("window connected: %d checks, %d failures\n", wcy_checks, wcy_failures);
    return wcy_failures != 0;
}

#undef WCY_CHECK
