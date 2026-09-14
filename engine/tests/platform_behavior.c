#include "../src/platform_helpers.h"
static int ph_active;
static void c840_heap_free(void *ptr);
int op_platform_highres;
double op_platform_millisecond_scale, op_platform_second_scale;
unsigned int(__stdcall *op_platform_time_get_time)(void);
int(__stdcall *op_platform_query_counter)(__int64 *);

#include "../src/platform_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int phad_checks, phad_failures;
static void phad_check(int ok, int line)
{
    ++phad_checks;
    if (!ok)
    {
        ++phad_failures;
        printf("line%d failed\n", line);
    }
}
#define PHAD_CHECK(x) phad_check(!!(x), __LINE__)
static unsigned char phad_owned[128], phad_expected_owned[128];
static void *phad_input, *phad_output;
static unsigned int phad_requested;
static int phad_calls;
static void phad_verify(void)
{
    PHAD_CHECK(memcmp(phad_owned, phad_expected_owned, sizeof(phad_owned)) == 0);
}
static void *phad_heap_alloc(unsigned int size)
{
    PHAD_CHECK(phad_calls++ == 0);
    PHAD_CHECK(size == phad_requested);
    phad_verify();
    return phad_output;
}
static void phad_heap_free(void *ptr)
{
    PHAD_CHECK(phad_calls++ == 0);
    PHAD_CHECK(ptr == phad_input);
    phad_verify();
}
static void *phad_heap_realloc(void *ptr, unsigned int size)
{
    PHAD_CHECK(phad_calls++ == 0);
    PHAD_CHECK(ptr == phad_input);
    PHAD_CHECK(size == phad_requested);
    phad_verify();
    return phad_output;
}
static int op_test_platform_alloc_debug(void)
{
    unsigned int i, j, k;
    static const unsigned int phad_sizes[] = {0, 1, 7, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    ph_active = 0;
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 9; ++j)
            for (k = 0; k < 9; ++k)
            {
                memset(phad_owned, (int)(i + j + k), sizeof(phad_owned));
                memcpy(phad_expected_owned, phad_owned, sizeof(phad_owned));
                phad_input = j ? phad_owned + 7 * j : 0;
                phad_output = k ? phad_owned + 7 * k : 0;
                phad_requested = phad_sizes[i];
                phad_calls = 0;
                PHAD_CHECK(op_platform_alloc_debug(phad_requested) == phad_output);
                PHAD_CHECK(phad_calls == 1);
                phad_verify();
            }
    printf("platform_alloc_debug: %d checks, %d failures\n", phad_checks, phad_failures);
    return phad_failures != 0;
}

#undef PHAD_CHECK

#include "../src/platform_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int phfd_checks, phfd_failures;
static void phfd_check(int ok, int line)
{
    ++phfd_checks;
    if (!ok)
    {
        ++phfd_failures;
        printf("line%d failed\n", line);
    }
}
#define PHFD_CHECK(x) phfd_check(!!(x), __LINE__)
static unsigned char phfd_owned[128], phfd_expected_owned[128];
static void *phfd_input, *phfd_output;
static unsigned int phfd_requested;
static int phfd_calls;
static void phfd_verify(void)
{
    PHFD_CHECK(memcmp(phfd_owned, phfd_expected_owned, sizeof(phfd_owned)) == 0);
}
static void *phfd_heap_alloc(unsigned int size)
{
    PHFD_CHECK(phfd_calls++ == 0);
    PHFD_CHECK(size == phfd_requested);
    phfd_verify();
    return phfd_output;
}
static void phfd_heap_free(void *ptr)
{
    PHFD_CHECK(phfd_calls++ == 0);
    PHFD_CHECK(ptr == phfd_input);
    phfd_verify();
}
static void *phfd_heap_realloc(void *ptr, unsigned int size)
{
    PHFD_CHECK(phfd_calls++ == 0);
    PHFD_CHECK(ptr == phfd_input);
    PHFD_CHECK(size == phfd_requested);
    phfd_verify();
    return phfd_output;
}
static int op_test_platform_free_debug(void)
{
    unsigned int i, j, k;
    static const unsigned int phfd_sizes[] = {0, 1, 7, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    ph_active = 1;
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 9; ++j)
            for (k = 0; k < 9; ++k)
            {
                memset(phfd_owned, (int)(i + j + k), sizeof(phfd_owned));
                memcpy(phfd_expected_owned, phfd_owned, sizeof(phfd_owned));
                phfd_input = j ? phfd_owned + 7 * j : 0;
                phfd_output = k ? phfd_owned + 7 * k : 0;
                phfd_requested = phfd_sizes[i];
                phfd_calls = 0;
                op_platform_free_debug(phfd_input);
                PHFD_CHECK(phfd_calls == 1);
                phfd_verify();
            }
    printf("platform_free_debug: %d checks, %d failures\n", phfd_checks, phfd_failures);
    return phfd_failures != 0;
}

#undef PHFD_CHECK

#include "../src/platform_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int phrd_checks, phrd_failures;
static void phrd_check(int ok, int line)
{
    ++phrd_checks;
    if (!ok)
    {
        ++phrd_failures;
        printf("line%d failed\n", line);
    }
}
#define PHRD_CHECK(x) phrd_check(!!(x), __LINE__)
static unsigned char phrd_owned[128], phrd_expected_owned[128];
static void *phrd_input, *phrd_output;
static unsigned int phrd_requested;
static int phrd_calls;
static void phrd_verify(void)
{
    PHRD_CHECK(memcmp(phrd_owned, phrd_expected_owned, sizeof(phrd_owned)) == 0);
}
static void *phrd_heap_alloc(unsigned int size)
{
    PHRD_CHECK(phrd_calls++ == 0);
    PHRD_CHECK(size == phrd_requested);
    phrd_verify();
    return phrd_output;
}
static void phrd_heap_free(void *ptr)
{
    PHRD_CHECK(phrd_calls++ == 0);
    PHRD_CHECK(ptr == phrd_input);
    phrd_verify();
}
static void *phrd_heap_realloc(void *ptr, unsigned int size)
{
    PHRD_CHECK(phrd_calls++ == 0);
    PHRD_CHECK(ptr == phrd_input);
    PHRD_CHECK(size == phrd_requested);
    phrd_verify();
    return phrd_output;
}
static int op_test_platform_realloc_debug(void)
{
    unsigned int i, j, k;
    static const unsigned int phrd_sizes[] = {0, 1, 7, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    ph_active = 2;
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 9; ++j)
            for (k = 0; k < 9; ++k)
            {
                memset(phrd_owned, (int)(i + j + k), sizeof(phrd_owned));
                memcpy(phrd_expected_owned, phrd_owned, sizeof(phrd_owned));
                phrd_input = j ? phrd_owned + 7 * j : 0;
                phrd_output = k ? phrd_owned + 7 * k : 0;
                phrd_requested = phrd_sizes[i];
                phrd_calls = 0;
                PHRD_CHECK(op_platform_realloc_debug(phrd_input, phrd_requested) == phrd_output);
                PHRD_CHECK(phrd_calls == 1);
                phrd_verify();
            }
    printf("platform_realloc_debug: %d checks, %d failures\n", phrd_checks, phrd_failures);
    return phrd_failures != 0;
}

#undef PHRD_CHECK

#include "../src/platform_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int phli_checks, phli_failures;
static void phli_check(int ok, int line)
{
    ++phli_checks;
    if (!ok)
    {
        ++phli_failures;
        printf("line%d failed\n", line);
    }
}
#define PHLI_CHECK(x) phli_check(!!(x), __LINE__)
static unsigned char phli_owned[128], phli_expected_owned[128];
static void *phli_input, *phli_output;
static unsigned int phli_requested;
static int phli_calls;
static void phli_verify(void)
{
    PHLI_CHECK(memcmp(phli_owned, phli_expected_owned, sizeof(phli_owned)) == 0);
}
static void *phli_heap_alloc(unsigned int size)
{
    PHLI_CHECK(phli_calls++ == 0);
    PHLI_CHECK(size == phli_requested);
    phli_verify();
    return phli_output;
}
static void phli_heap_free(void *ptr)
{
    PHLI_CHECK(phli_calls++ == 0);
    PHLI_CHECK(ptr == phli_input);
    phli_verify();
}
static void *phli_heap_realloc(void *ptr, unsigned int size)
{
    PHLI_CHECK(phli_calls++ == 0);
    PHLI_CHECK(ptr == phli_input);
    PHLI_CHECK(size == phli_requested);
    phli_verify();
    return phli_output;
}
static int op_test_platform_lock_identity(void)
{
    unsigned int i, j, k;
    static const unsigned int phli_sizes[] = {0, 1, 7, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    ph_active = 3;
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 9; ++j)
            for (k = 0; k < 9; ++k)
            {
                memset(phli_owned, (int)(i + j + k), sizeof(phli_owned));
                memcpy(phli_expected_owned, phli_owned, sizeof(phli_owned));
                phli_input = j ? phli_owned + 7 * j : 0;
                phli_output = k ? phli_owned + 7 * k : 0;
                phli_requested = phli_sizes[i];
                phli_calls = 0;
                PHLI_CHECK(op_platform_lock_identity(phli_input) == phli_input);
                PHLI_CHECK(phli_calls == 0);
                phli_verify();
            }
    printf("platform_lock_identity: %d checks, %d failures\n", phli_checks, phli_failures);
    return phli_failures != 0;
}

#undef PHLI_CHECK

#include "../src/platform_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int ptk_checks, ptk_failures;
static void ptk_check(int ok, int line)
{
    ++ptk_checks;
    if (!ok)
    {
        ++ptk_failures;
        printf("line%d failed\n", line);
    }
}
#define PTK_CHECK(x) ptk_check(!!(x), __LINE__)
typedef struct PTK_VECTOR
{
    __int64 counter;
    double scale;
    int ticks;
    unsigned int seconds;
} PTK_VECTOR;
static const PTK_VECTOR ptk_vectors[] = {{0i64, 0.5, 0, 0x00000000u},
                                         {1i64, 0.5, 0, 0x3f000000u},
                                         {-1i64, 0.5, 0, 0xbf000000u},
                                         {15i64, 0.5, 7, 0x40f00000u},
                                         {-15i64, 0.5, -7, 0xc0f00000u},
                                         {16777219i64, 0.25, 4194304, 0x4a800002u},
                                         {-16777219i64, 0.25, -4194304, 0xca800002u},
                                         {4294967296i64, 2.3283064365386963e-10, 1, 0x3f800000u},
                                         {-4294967296i64, 2.3283064365386963e-10, -1, 0xbf800000u}};
static const unsigned int ptk_times[] = {0, 1, 7, 1000, 0x7fffffffu, 0x80000000u, 0xffffffffu};

static unsigned char ptk_owned[64], ptk_expected_owned[64];
static unsigned int ptk_time_index, ptk_vector_index;
static int ptk_calls, ptk_mode, ptk_mutate, ptk_initial_flag, ptk_expected_flag;
static double ptk_expected_ms, ptk_expected_seconds;
static void ptk_verify(void)
{
    PTK_CHECK(op_platform_highres == ptk_expected_flag);
    PTK_CHECK(memcmp(&op_platform_millisecond_scale, &ptk_expected_ms, 8) == 0);
    PTK_CHECK(memcmp(&op_platform_second_scale, &ptk_expected_seconds, 8) == 0);
    PTK_CHECK(memcmp(ptk_owned, ptk_expected_owned, sizeof(ptk_owned)) == 0);
}
static void ptk_change(void)
{
    if (ptk_mutate)
    {
        ptk_expected_flag = ptk_initial_flag ? 0 : -1;
        ptk_expected_ms = ptk_vectors[ptk_vector_index].scale;
        ptk_expected_seconds = ptk_vectors[ptk_vector_index].scale;
        op_platform_highres = ptk_expected_flag;
        op_platform_millisecond_scale = ptk_expected_ms;
        op_platform_second_scale = ptk_expected_seconds;
    }
}
static unsigned int __stdcall ptk_read_time(void)
{
    PTK_CHECK(ptk_calls++ == 0);
    PTK_CHECK(ptk_initial_flag == 0);
    ptk_verify();
    ptk_change();
    return ptk_times[ptk_time_index];
}
static int __stdcall ptk_read_counter(__int64 *ptr)
{
    PTK_CHECK(ptk_calls++ == 0);
    PTK_CHECK(ptk_initial_flag != 0);
    PTK_CHECK(ptr != 0);
    ptk_verify();
    if (!ptr)
        exit(2);
    *ptr = ptk_vectors[ptk_vector_index].counter;
    ptk_change();
    return ptk_mode;
}

static void ptk_seed(void)
{
    op_platform_time_get_time = ptk_read_time;
    op_platform_query_counter = ptk_read_counter;
    ptk_calls = 0;
    op_platform_highres = ptk_expected_flag = ptk_initial_flag;
    op_platform_millisecond_scale = ptk_expected_ms = ptk_mutate ? 2.0 : ptk_vectors[ptk_vector_index].scale;
    op_platform_second_scale = ptk_expected_seconds = ptk_mutate ? 4.0 : ptk_vectors[ptk_vector_index].scale;
    memset(ptk_owned, 0x57, sizeof(ptk_owned));
    memcpy(ptk_expected_owned, ptk_owned, sizeof(ptk_owned));
}
static int op_test_platform_ticks(void)
{
    unsigned int f, m, r, old_control;
    static const int flags[] = {0, 1, -1, 0x40000000};
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (f = 0; f < 4; ++f)
        for (m = 0; m < 2; ++m)
            for (r = 0; r < 3; ++r)
                for (ptk_time_index = 0; ptk_time_index < 7; ++ptk_time_index)
                    for (ptk_vector_index = 0; ptk_vector_index < 9; ++ptk_vector_index)
                    {
                        ptk_initial_flag = flags[f];
                        ptk_mutate = (int)m;
                        ptk_mode = (int)r - 1;
                        ptk_seed();
                        PTK_CHECK((unsigned int)op_platform_ticks() ==
                                  (ptk_initial_flag ? (unsigned int)ptk_vectors[ptk_vector_index].ticks
                                                    : ptk_times[ptk_time_index]));
                        PTK_CHECK(ptk_calls == 1);
                        ptk_verify();
                        PTK_CHECK(op_platform_time_get_time == ptk_read_time);
                        PTK_CHECK(op_platform_query_counter == ptk_read_counter);
                    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("platform_ticks: %d checks, %d failures\n", ptk_checks, ptk_failures);
    return ptk_failures != 0;
}

#undef PTK_CHECK

#include "../src/platform_helpers.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static int psec_checks, psec_failures;
static void psec_check(int ok, int line)
{
    ++psec_checks;
    if (!ok)
    {
        ++psec_failures;
        printf("line%d failed\n", line);
    }
}
#define PSEC_CHECK(x) psec_check(!!(x), __LINE__)
typedef struct PSEC_VECTOR
{
    __int64 counter;
    double scale;
    int ticks;
    unsigned int seconds;
} PSEC_VECTOR;
static const PSEC_VECTOR psec_vectors[] = {{0i64, 0.5, 0, 0x00000000u},
                                           {1i64, 0.5, 0, 0x3f000000u},
                                           {-1i64, 0.5, 0, 0xbf000000u},
                                           {15i64, 0.5, 7, 0x40f00000u},
                                           {-15i64, 0.5, -7, 0xc0f00000u},
                                           {16777219i64, 0.25, 4194304, 0x4a800002u},
                                           {-16777219i64, 0.25, -4194304, 0xca800002u},
                                           {4294967296i64, 2.3283064365386963e-10, 1, 0x3f800000u},
                                           {-4294967296i64, 2.3283064365386963e-10, -1, 0xbf800000u}};
static const unsigned int psec_times[] = {0, 1, 7, 1000, 0x7fffffffu, 0x80000000u, 0xffffffffu};
static const unsigned int psec_time_seconds[] = {0x00000000u, 0x3a83126fu, 0x3be56042u, 0x3f800000u,
                                                 0x4a03126fu, 0x4a03126fu, 0x4a83126fu};
static unsigned char psec_owned[64], psec_expected_owned[64];
static unsigned int psec_time_index, psec_vector_index;
static int psec_calls, psec_mode, psec_mutate, psec_initial_flag, psec_expected_flag;
static double psec_expected_ms, psec_expected_seconds;
static void psec_verify(void)
{
    PSEC_CHECK(op_platform_highres == psec_expected_flag);
    PSEC_CHECK(memcmp(&op_platform_millisecond_scale, &psec_expected_ms, 8) == 0);
    PSEC_CHECK(memcmp(&op_platform_second_scale, &psec_expected_seconds, 8) == 0);
    PSEC_CHECK(memcmp(psec_owned, psec_expected_owned, sizeof(psec_owned)) == 0);
}
static void psec_change(void)
{
    if (psec_mutate)
    {
        psec_expected_flag = psec_initial_flag ? 0 : -1;
        psec_expected_ms = psec_vectors[psec_vector_index].scale;
        psec_expected_seconds = psec_vectors[psec_vector_index].scale;
        op_platform_highres = psec_expected_flag;
        op_platform_millisecond_scale = psec_expected_ms;
        op_platform_second_scale = psec_expected_seconds;
    }
}
static unsigned int __stdcall psec_read_time(void)
{
    PSEC_CHECK(psec_calls++ == 0);
    PSEC_CHECK(psec_initial_flag == 0);
    psec_verify();
    psec_change();
    return psec_times[psec_time_index];
}
static int __stdcall psec_read_counter(__int64 *ptr)
{
    PSEC_CHECK(psec_calls++ == 0);
    PSEC_CHECK(psec_initial_flag != 0);
    PSEC_CHECK(ptr != 0);
    psec_verify();
    if (!ptr)
        exit(2);
    *ptr = psec_vectors[psec_vector_index].counter;
    psec_change();
    return psec_mode;
}
static unsigned int psec_float_bits(float value)
{
    unsigned int bits;
    memcpy(&bits, &value, 4);
    return bits;
}
static void psec_seed(void)
{
    op_platform_time_get_time = psec_read_time;
    op_platform_query_counter = psec_read_counter;
    psec_calls = 0;
    op_platform_highres = psec_expected_flag = psec_initial_flag;
    op_platform_millisecond_scale = psec_expected_ms = psec_mutate ? 2.0 : psec_vectors[psec_vector_index].scale;
    op_platform_second_scale = psec_expected_seconds = psec_mutate ? 4.0 : psec_vectors[psec_vector_index].scale;
    memset(psec_owned, 0x57, sizeof(psec_owned));
    memcpy(psec_expected_owned, psec_owned, sizeof(psec_owned));
}
static int op_test_platform_seconds(void)
{
    unsigned int f, m, r, old_control;
    static const int flags[] = {0, 1, -1, 0x40000000};
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    for (f = 0; f < 4; ++f)
        for (m = 0; m < 2; ++m)
            for (r = 0; r < 3; ++r)
                for (psec_time_index = 0; psec_time_index < 7; ++psec_time_index)
                    for (psec_vector_index = 0; psec_vector_index < 9; ++psec_vector_index)
                    {
                        psec_initial_flag = flags[f];
                        psec_mutate = (int)m;
                        psec_mode = (int)r - 1;
                        psec_seed();
                        PSEC_CHECK(psec_float_bits(op_platform_seconds()) ==
                                   (psec_initial_flag ? psec_vectors[psec_vector_index].seconds
                                                      : psec_time_seconds[psec_time_index]));
                        PSEC_CHECK(psec_calls == 1);
                        psec_verify();
                        PSEC_CHECK(op_platform_time_get_time == psec_read_time);
                        PSEC_CHECK(op_platform_query_counter == psec_read_counter);
                    }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("platform_seconds: %d checks, %d failures\n", psec_checks, psec_failures);
    return psec_failures != 0;
}

#undef PSEC_CHECK

static int phcy_stage, phcy_checks, phcy_failures;
static unsigned char phcy_storage[128], phcy_expected[128];
static void *phcy_first, *phcy_second;
static unsigned int phcy_size;
static void phcy_check(int ok)
{
    ++phcy_checks;
    if (!ok)
        ++phcy_failures;
}
static void phcy_verify(void)
{
    phcy_check(memcmp(phcy_storage, phcy_expected, sizeof(phcy_storage)) == 0);
}
static void *phcy_alloc(unsigned int size)
{
    phcy_check(phcy_stage == 0 && size == phcy_size);
    phcy_verify();
    phcy_stage = 1;
    return phcy_first;
}
static void *phcy_realloc(void *ptr, unsigned int size)
{
    phcy_check(phcy_stage == 1 && ptr == phcy_first && size == phcy_size);
    phcy_verify();
    phcy_stage = 2;
    return phcy_second;
}
static void phcy_free(void *ptr)
{
    phcy_check(phcy_stage == 2 && ptr == phcy_second);
    phcy_verify();
    phcy_stage = 3;
}
void *op_platform_heap_alloc(unsigned int size)
{
    if (ph_active == 0)
    {
        return phad_heap_alloc(size);
    }
    if (ph_active == 1)
    {
        return phfd_heap_alloc(size);
    }
    if (ph_active == 2)
    {
        return phrd_heap_alloc(size);
    }
    if (ph_active == 3)
    {
        return phli_heap_alloc(size);
    }
    return phcy_alloc(size);
}
void op_platform_heap_free(void *ptr)
{
    if(ph_active==5) { c840_heap_free(ptr);return; }
    if (ph_active == 0)
    {
        phad_heap_free(ptr);
        return;
    }
    if (ph_active == 1)
    {
        phfd_heap_free(ptr);
        return;
    }
    if (ph_active == 2)
    {
        phrd_heap_free(ptr);
        return;
    }
    if (ph_active == 3)
    {
        phli_heap_free(ptr);
        return;
    }
    phcy_free(ptr);
}
void *op_platform_heap_realloc(void *ptr, unsigned int size)
{
    if (ph_active == 0)
    {
        return phad_heap_realloc(ptr, size);
    }
    if (ph_active == 1)
    {
        return phfd_heap_realloc(ptr, size);
    }
    if (ph_active == 2)
    {
        return phrd_heap_realloc(ptr, size);
    }
    if (ph_active == 3)
    {
        return phli_heap_realloc(ptr, size);
    }
    return phcy_realloc(ptr, size);
}
static int op_test_platform_heap_connected(void)
{
    unsigned int i, j, k;
    void *ptr;
    static const unsigned int sizes[] = {0, 1, 7, 0x7fffffffu, 0x80000000u, 0xffffffffu};
    ph_active = 4;
    for (i = 0; i < 6; ++i)
        for (j = 0; j < 5; ++j)
            for (k = 0; k < 5; ++k)
            {
                memset(phcy_storage, 0x57, sizeof(phcy_storage));
                memcpy(phcy_expected, phcy_storage, sizeof(phcy_storage));
                phcy_first = j ? phcy_storage + j * 7 : 0;
                phcy_second = k ? phcy_storage + k * 11 : 0;
                phcy_size = sizes[i];
                phcy_stage = 0;
                ptr = op_platform_alloc_debug(phcy_size);
                phcy_check(ptr == phcy_first);
                phcy_check(op_platform_lock_identity(ptr) == phcy_first);
                phcy_check(phcy_stage == 1);
                phcy_verify();
                ptr = op_platform_realloc_debug(ptr, phcy_size);
                phcy_check(ptr == phcy_second);
                phcy_check(op_platform_lock_identity(ptr) == phcy_second);
                phcy_check(phcy_stage == 2);
                phcy_verify();
                op_platform_free_debug(ptr);
                phcy_check(phcy_stage == 3);
                phcy_verify();
            }
    printf("platform heap connected: %d checks, %d failures\n", phcy_checks, phcy_failures);
    return phcy_failures != 0;
}
static int op_test_platform_timer_connected(void)
{
    unsigned int f, m, old_control, want;
    int before_checks, before_failures;
    static const int flags[] = {0, 1, -1, 0x40000000};
    old_control = _controlfp(0, 0);
    _controlfp(_MCW_EM | _PC_53 | _RC_NEAR, _MCW_EM | _MCW_PC | _MCW_RC);
    before_checks = ptk_checks;
    before_failures = ptk_failures;
    for (f = 0; f < 4; ++f)
        for (m = 0; m < 2; ++m)
            for (ptk_time_index = 0; ptk_time_index < 7; ++ptk_time_index)
                for (ptk_vector_index = 0; ptk_vector_index < 9; ++ptk_vector_index)
                {
                    ptk_initial_flag = flags[f];
                    ptk_mutate = (int)m;
                    ptk_mode = 1;
                    ptk_seed();
                    want = ptk_initial_flag ? (unsigned int)ptk_vectors[ptk_vector_index].ticks
                                            : ptk_times[ptk_time_index];
                    ptk_check((unsigned int)op_platform_ticks() == want, __LINE__);
                    ptk_check(ptk_calls == 1, __LINE__);
                    ptk_verify();
                    ptk_initial_flag = ptk_expected_flag;
                    ptk_calls = 0;
                    want = ptk_initial_flag ? ptk_vectors[ptk_vector_index].seconds : psec_time_seconds[ptk_time_index];
                    ptk_check(psec_float_bits(op_platform_seconds()) == want, __LINE__);
                    ptk_check(ptk_calls == 1, __LINE__);
                    ptk_verify();
                    ptk_check(op_platform_time_get_time == ptk_read_time, __LINE__);
                    ptk_check(op_platform_query_counter == ptk_read_counter, __LINE__);
                }
    _controlfp(old_control, _MCW_EM | _MCW_PC | _MCW_RC);
    printf("platform timer connected: %d checks, %d failures\n", ptk_checks - before_checks,
           ptk_failures - before_failures);
    return ptk_failures != before_failures;
}
