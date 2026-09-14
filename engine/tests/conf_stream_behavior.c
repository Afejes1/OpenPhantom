#include "../src/conf_stream.h"
#include <stdio.h>
#include <limits.h>
static int cfor_checks, cfor_failures;
static void cfor_check(int ok, int line)
{
    ++cfor_checks;
    if (!ok)
    {
        ++cfor_failures;
        printf("line%d failed\n", line);
    }
}
#define CFOR_CHECK(x) cfor_check(!!(x), __LINE__)
typedef struct CFOR_OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} CFOR_OWNED_SERVICES;
static CFOR_OWNED_SERVICES cfor_tables[2], cfor_expected_tables[2];
OP_CONF_SERVICES *op_conf_services;
int op_conf_read_active, op_conf_read_handle, op_conf_write_handle;
char op_conf_write_name[136], op_conf_read_mode[16], op_conf_not_open[140];
static unsigned char cfor_payload[40], cfor_expected_payload[40];
static char cfor_expected_name[136], cfor_expected_mode[16], cfor_expected_closed[140];
static OP_CONF_SERVICES *cfor_expected_services;
static int cfor_expected_active, cfor_expected_input, cfor_expected_output;
static const int cfor_values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int cfor_table_case, cfor_handle_case, cfor_active_case, cfor_mutate, cfor_stage;
static void cfor_verify(void)
{
    CFOR_CHECK(memcmp(cfor_tables, cfor_expected_tables, sizeof(cfor_tables)) == 0);
    CFOR_CHECK(op_conf_services == cfor_expected_services);
    CFOR_CHECK(op_conf_read_active == cfor_expected_active && op_conf_read_handle == cfor_expected_input &&
               op_conf_write_handle == cfor_expected_output);
    CFOR_CHECK(memcmp(op_conf_write_name, cfor_expected_name, sizeof(cfor_expected_name)) == 0);
    CFOR_CHECK(memcmp(op_conf_read_mode, cfor_expected_mode, sizeof(cfor_expected_mode)) == 0);
    CFOR_CHECK(memcmp(op_conf_not_open, cfor_expected_closed, sizeof(cfor_expected_closed)) == 0);
    CFOR_CHECK(memcmp(cfor_payload, cfor_expected_payload, sizeof(cfor_payload)) == 0);
}
static void cfor_snapshot(void)
{
    memcpy(cfor_expected_tables, cfor_tables, sizeof(cfor_tables));
    cfor_expected_services = op_conf_services;
    cfor_expected_active = op_conf_read_active;
    cfor_expected_input = op_conf_read_handle;
    cfor_expected_output = op_conf_write_handle;
    memcpy(cfor_expected_name, op_conf_write_name, sizeof(cfor_expected_name));
    memcpy(cfor_expected_mode, op_conf_read_mode, sizeof(cfor_expected_mode));
    memcpy(cfor_expected_closed, op_conf_not_open, sizeof(cfor_expected_closed));
    memcpy(cfor_expected_payload, cfor_payload, sizeof(cfor_payload));
}
static void cfor_seed(void)
{
    memset(cfor_tables, 0x57, sizeof(cfor_tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(cfor_payload, 0x71, sizeof(cfor_payload));
    cfor_payload[17] = 0;
    op_conf_services = &cfor_tables[cfor_table_case].value;
    op_conf_read_active = cfor_values[cfor_active_case];
    op_conf_read_handle = op_conf_write_handle = cfor_values[cfor_handle_case];
    cfor_stage = 0;
}
static void cfor_mutate_state(void)
{
    if (cfor_mutate)
    {
        op_conf_services = cfor_expected_services = &cfor_tables[1 - cfor_table_case].value;
        op_conf_read_active = cfor_expected_active = -17;
        op_conf_read_handle = cfor_expected_input = -19;
        op_conf_write_handle = cfor_expected_output = -23;
        op_conf_write_name[1] = cfor_expected_name[1] = 'W';
    }
}

static int cfor_answer, cfor_path_case;
static const char *cfor_path;
int op_conf_open(const char *input, const char *mode)
{
    CFOR_CHECK(cfor_stage == 0 && input == cfor_path && mode == op_conf_read_mode);
    cfor_verify();
    cfor_mutate_state();
    if (cfor_mutate)
        cfor_payload[0] = cfor_expected_payload[0] = 'P';
    ++cfor_stage;
    return cfor_values[cfor_answer];
}
static int op_test_conf_open_read(void)
{
    for (cfor_table_case = 0; cfor_table_case < 2; ++cfor_table_case)
        for (cfor_handle_case = 0; cfor_handle_case < 6; ++cfor_handle_case)
            for (cfor_active_case = 0; cfor_active_case < 6; ++cfor_active_case)
                for (cfor_answer = 0; cfor_answer < 6; ++cfor_answer)
                    for (cfor_path_case = 0; cfor_path_case < 2; ++cfor_path_case)
                        for (cfor_mutate = 0; cfor_mutate < 2; ++cfor_mutate)
                        {
                            cfor_seed();
                            cfor_path = cfor_path_case ? (char *)cfor_payload : 0;
                            cfor_snapshot();
                            CFOR_CHECK(op_conf_open_read(cfor_path) == cfor_values[cfor_answer]);
                            CFOR_CHECK(cfor_stage == 1);
                            cfor_verify();
                        }
    printf("conf_open_read: %d checks, %d failures\n", cfor_checks, cfor_failures);
    return cfor_failures != 0;
}

#undef CFOR_CHECK

#include "../src/conf_stream.h"
#include <stdio.h>
#include <limits.h>
static int cfcw_active;
static int cfcw_checks, cfcw_failures;
static void cfcw_check(int ok, int line)
{
    ++cfcw_checks;
    if (!ok)
    {
        ++cfcw_failures;
        printf("line%d failed\n", line);
    }
}
#define CFCW_CHECK(x) cfcw_check(!!(x), __LINE__)
typedef struct CFCW_OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} CFCW_OWNED_SERVICES;
static CFCW_OWNED_SERVICES cfcw_tables[2], cfcw_expected_tables[2];
static unsigned char cfcw_payload[40], cfcw_expected_payload[40];
static char cfcw_expected_name[136], cfcw_expected_mode[16], cfcw_expected_closed[140];
static OP_CONF_SERVICES *cfcw_expected_services;
static int cfcw_expected_active, cfcw_expected_input, cfcw_expected_output;
static const int cfcw_values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int cfcw_table_case, cfcw_handle_case, cfcw_active_case, cfcw_mutate, cfcw_stage;
static void cfcw_verify(void)
{
    CFCW_CHECK(memcmp(cfcw_tables, cfcw_expected_tables, sizeof(cfcw_tables)) == 0);
    CFCW_CHECK(op_conf_services == cfcw_expected_services);
    CFCW_CHECK(op_conf_read_active == cfcw_expected_active && op_conf_read_handle == cfcw_expected_input &&
               op_conf_write_handle == cfcw_expected_output);
    CFCW_CHECK(memcmp(op_conf_write_name, cfcw_expected_name, sizeof(cfcw_expected_name)) == 0);
    CFCW_CHECK(memcmp(op_conf_read_mode, cfcw_expected_mode, sizeof(cfcw_expected_mode)) == 0);
    CFCW_CHECK(memcmp(op_conf_not_open, cfcw_expected_closed, sizeof(cfcw_expected_closed)) == 0);
    CFCW_CHECK(memcmp(cfcw_payload, cfcw_expected_payload, sizeof(cfcw_payload)) == 0);
}
static void cfcw_snapshot(void)
{
    memcpy(cfcw_expected_tables, cfcw_tables, sizeof(cfcw_tables));
    cfcw_expected_services = op_conf_services;
    cfcw_expected_active = op_conf_read_active;
    cfcw_expected_input = op_conf_read_handle;
    cfcw_expected_output = op_conf_write_handle;
    memcpy(cfcw_expected_name, op_conf_write_name, sizeof(cfcw_expected_name));
    memcpy(cfcw_expected_mode, op_conf_read_mode, sizeof(cfcw_expected_mode));
    memcpy(cfcw_expected_closed, op_conf_not_open, sizeof(cfcw_expected_closed));
    memcpy(cfcw_expected_payload, cfcw_payload, sizeof(cfcw_payload));
}
static void cfcw_seed(void)
{
    memset(cfcw_tables, 0x57, sizeof(cfcw_tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(cfcw_payload, 0x71, sizeof(cfcw_payload));
    cfcw_payload[17] = 0;
    op_conf_services = &cfcw_tables[cfcw_table_case].value;
    op_conf_read_active = cfcw_values[cfcw_active_case];
    op_conf_read_handle = op_conf_write_handle = cfcw_values[cfcw_handle_case];
    cfcw_stage = 0;
}
static void cfcw_mutate_state(void)
{
    if (cfcw_mutate)
    {
        op_conf_services = cfcw_expected_services = &cfcw_tables[1 - cfcw_table_case].value;
        op_conf_read_active = cfcw_expected_active = -17;
        op_conf_read_handle = cfcw_expected_input = -19;
        op_conf_write_handle = cfcw_expected_output = -23;
        op_conf_write_name[1] = cfcw_expected_name[1] = 'W';
    }
}

static int cfcw_answer, cfcw_name_case;
static const unsigned int cfcw_lengths[] = {0, 1, 7, 126, 127, 130};
static int cfcw_close_common(int handle, int which)
{
    CFCW_CHECK(cfcw_stage == 0 && which == cfcw_table_case && handle == cfcw_values[cfcw_handle_case] && handle != 0);
    cfcw_verify();
    cfcw_mutate_state();
    ++cfcw_stage;
    cfcw_expected_output = 0;
    return cfcw_values[cfcw_answer];
}
static int cfcw_close0(int handle)
{
    return cfcw_close_common(handle, 0);
}
static int cfcw_close1(int handle)
{
    return cfcw_close_common(handle, 1);
}
static char *cfcw_copy(char *out, const char *in, unsigned int cfcw_count)
{
    unsigned int i;
    CFCW_CHECK(cfcw_stage == 1 && out == op_conf_write_name && in == op_conf_not_open && cfcw_count == 127);
    if (cfcw_stage != 1 || out != op_conf_write_name || in != op_conf_not_open || cfcw_count != 127)
        return 0;
    cfcw_verify();
    for (i = 0; i < 127; ++i)
        if (i < cfcw_lengths[cfcw_name_case])
            cfcw_expected_name[i] = 'N';
        else
            cfcw_expected_name[i] = 0;
    strncpy(op_conf_write_name, op_conf_not_open, 127);
    if (cfcw_mutate)
    {
        op_conf_write_name[127] = cfcw_expected_name[127] = 'Z';
        op_conf_write_handle = cfcw_expected_output = 91;
    }
    cfcw_verify();
    cfcw_expected_name[127] = 0;
    ++cfcw_stage;
    return op_conf_not_open;
}
static int op_test_conf_close_write(void)
{
    cfcw_active = 1;
    for (cfcw_table_case = 0; cfcw_table_case < 2; ++cfcw_table_case)
        for (cfcw_handle_case = 0; cfcw_handle_case < 6; ++cfcw_handle_case)
            for (cfcw_active_case = 0; cfcw_active_case < 6; ++cfcw_active_case)
                for (cfcw_answer = 0; cfcw_answer < 6; ++cfcw_answer)
                    for (cfcw_name_case = 0; cfcw_name_case < 6; ++cfcw_name_case)
                        for (cfcw_mutate = 0; cfcw_mutate < 2; ++cfcw_mutate)
                        {
                            cfcw_seed();
                            cfcw_tables[0].value.close = cfcw_close0;
                            cfcw_tables[1].value.close = cfcw_close1;
                            memset(op_conf_not_open, 'N', sizeof(op_conf_not_open));
                            op_conf_not_open[cfcw_lengths[cfcw_name_case]] = 0;
                            if (!cfcw_values[cfcw_handle_case])
                                op_conf_services = 0;
                            cfcw_snapshot();
                            op_conf_close_write();
                            CFCW_CHECK(cfcw_stage == (cfcw_values[cfcw_handle_case] ? 2 : 0));
                            cfcw_verify();
                        }
    cfcw_active = 0;
    printf("conf_close_write: %d checks, %d failures\n", cfcw_checks, cfcw_failures);
    return cfcw_failures != 0;
}

#undef CFCW_CHECK

#include "../src/conf_stream.h"
#include <stdio.h>
#include <limits.h>
static int cfws_checks, cfws_failures;
static void cfws_check(int ok, int line)
{
    ++cfws_checks;
    if (!ok)
    {
        ++cfws_failures;
        printf("line%d failed\n", line);
    }
}
#define CFWS_CHECK(x) cfws_check(!!(x), __LINE__)
typedef struct CFWS_OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} CFWS_OWNED_SERVICES;
static CFWS_OWNED_SERVICES cfws_tables[2], cfws_expected_tables[2];
static unsigned char cfws_payload[40], cfws_expected_payload[40];
static char cfws_expected_name[136], cfws_expected_mode[16], cfws_expected_closed[140];
static OP_CONF_SERVICES *cfws_expected_services;
static int cfws_expected_active, cfws_expected_input, cfws_expected_output;
static const int cfws_values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int cfws_table_case, cfws_handle_case, cfws_active_case, cfws_mutate, cfws_stage;
static void cfws_verify(void)
{
    CFWS_CHECK(memcmp(cfws_tables, cfws_expected_tables, sizeof(cfws_tables)) == 0);
    CFWS_CHECK(op_conf_services == cfws_expected_services);
    CFWS_CHECK(op_conf_read_active == cfws_expected_active && op_conf_read_handle == cfws_expected_input &&
               op_conf_write_handle == cfws_expected_output);
    CFWS_CHECK(memcmp(op_conf_write_name, cfws_expected_name, sizeof(cfws_expected_name)) == 0);
    CFWS_CHECK(memcmp(op_conf_read_mode, cfws_expected_mode, sizeof(cfws_expected_mode)) == 0);
    CFWS_CHECK(memcmp(op_conf_not_open, cfws_expected_closed, sizeof(cfws_expected_closed)) == 0);
    CFWS_CHECK(memcmp(cfws_payload, cfws_expected_payload, sizeof(cfws_payload)) == 0);
}
static void cfws_snapshot(void)
{
    memcpy(cfws_expected_tables, cfws_tables, sizeof(cfws_tables));
    cfws_expected_services = op_conf_services;
    cfws_expected_active = op_conf_read_active;
    cfws_expected_input = op_conf_read_handle;
    cfws_expected_output = op_conf_write_handle;
    memcpy(cfws_expected_name, op_conf_write_name, sizeof(cfws_expected_name));
    memcpy(cfws_expected_mode, op_conf_read_mode, sizeof(cfws_expected_mode));
    memcpy(cfws_expected_closed, op_conf_not_open, sizeof(cfws_expected_closed));
    memcpy(cfws_expected_payload, cfws_payload, sizeof(cfws_payload));
}
static void cfws_seed(void)
{
    memset(cfws_tables, 0x57, sizeof(cfws_tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(cfws_payload, 0x71, sizeof(cfws_payload));
    cfws_payload[17] = 0;
    op_conf_services = &cfws_tables[cfws_table_case].value;
    op_conf_read_active = cfws_values[cfws_active_case];
    op_conf_read_handle = op_conf_write_handle = cfws_values[cfws_handle_case];
    cfws_stage = 0;
}
static void cfws_mutate_state(void)
{
    if (cfws_mutate)
    {
        op_conf_services = cfws_expected_services = &cfws_tables[1 - cfws_table_case].value;
        op_conf_read_active = cfws_expected_active = -17;
        op_conf_read_handle = cfws_expected_input = -19;
        op_conf_write_handle = cfws_expected_output = -23;
        op_conf_write_name[1] = cfws_expected_name[1] = 'W';
    }
}

static const unsigned int cfws_string_lengths[] = {0, 1, 3, 8, 16, 17};
static int cfws_count_case, cfws_pointer_case, cfws_answer_case;
static unsigned int cfws_count, cfws_response;
static void *cfws_argument;
static unsigned int cfws_io_common(int handle, const void *data, unsigned int n, int which)
{
    CFWS_CHECK(cfws_stage == 0 && which == cfws_table_case && handle == cfws_values[cfws_handle_case] &&
               data == cfws_argument && n == cfws_count);
    cfws_verify();
    cfws_mutate_state();
    if (cfws_mutate)
    {
        cfws_payload[0] = cfws_expected_payload[0] = 0;
        cfws_payload[3] = cfws_expected_payload[3] = 'D';
    }
    ++cfws_stage;
    return cfws_response;
}
static unsigned int cfws_write0(int h, const void *p, unsigned int n)
{
    return cfws_io_common(h, p, n, 0);
}
static unsigned int cfws_write1(int h, const void *p, unsigned int n)
{
    return cfws_io_common(h, p, n, 1);
}
static int op_test_conf_write_string(void)
{
    int expected, enabled;
    for (cfws_table_case = 0; cfws_table_case < 2; ++cfws_table_case)
        for (cfws_handle_case = 0; cfws_handle_case < 6; ++cfws_handle_case)
            for (cfws_active_case = 0; cfws_active_case < 6; ++cfws_active_case)
                for (cfws_count_case = 0; cfws_count_case < 6; ++cfws_count_case)
                    for (cfws_pointer_case = 0; cfws_pointer_case < 2; ++cfws_pointer_case)
                        for (cfws_answer_case = 0; cfws_answer_case < 4; ++cfws_answer_case)
                            for (cfws_mutate = 0; cfws_mutate < 2; ++cfws_mutate)
                            {
                                cfws_seed();
                                cfws_tables[0].value.write = cfws_write0;
                                cfws_tables[1].value.write = cfws_write1;
                                cfws_count = cfws_string_lengths[cfws_count_case];
                                cfws_payload[cfws_count] = 0;
                                cfws_argument = cfws_pointer_case ? cfws_payload : 0;
                                cfws_response = cfws_answer_case == 0   ? cfws_count
                                                : cfws_answer_case == 1 ? cfws_count + 1u
                                                : cfws_answer_case == 2 ? cfws_count - 1u
                                                                        : 0x80000000u;
                                enabled = cfws_values[cfws_handle_case] != 0 && cfws_pointer_case != 0;
                                if (!enabled)
                                    op_conf_services = 0;
                                cfws_snapshot();
                                expected = enabled && cfws_response == cfws_count;
                                CFWS_CHECK(op_conf_write_string((const char *)cfws_argument) == expected);
                                CFWS_CHECK(cfws_stage == enabled);
                                cfws_verify();
                            }
    printf("conf_write_string: %d checks, %d failures\n", cfws_checks, cfws_failures);
    return cfws_failures != 0;
}

#undef CFWS_CHECK

#include "../src/conf_stream.h"
#include <stdio.h>
#include <limits.h>
static int cfwb_checks, cfwb_failures;
static void cfwb_check(int ok, int line)
{
    ++cfwb_checks;
    if (!ok)
    {
        ++cfwb_failures;
        printf("line%d failed\n", line);
    }
}
#define CFWB_CHECK(x) cfwb_check(!!(x), __LINE__)
typedef struct CFWB_OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} CFWB_OWNED_SERVICES;
static CFWB_OWNED_SERVICES cfwb_tables[2], cfwb_expected_tables[2];
static unsigned char cfwb_payload[40], cfwb_expected_payload[40];
static char cfwb_expected_name[136], cfwb_expected_mode[16], cfwb_expected_closed[140];
static OP_CONF_SERVICES *cfwb_expected_services;
static int cfwb_expected_active, cfwb_expected_input, cfwb_expected_output;
static const int cfwb_values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int cfwb_table_case, cfwb_handle_case, cfwb_active_case, cfwb_mutate, cfwb_stage;
static void cfwb_verify(void)
{
    CFWB_CHECK(memcmp(cfwb_tables, cfwb_expected_tables, sizeof(cfwb_tables)) == 0);
    CFWB_CHECK(op_conf_services == cfwb_expected_services);
    CFWB_CHECK(op_conf_read_active == cfwb_expected_active && op_conf_read_handle == cfwb_expected_input &&
               op_conf_write_handle == cfwb_expected_output);
    CFWB_CHECK(memcmp(op_conf_write_name, cfwb_expected_name, sizeof(cfwb_expected_name)) == 0);
    CFWB_CHECK(memcmp(op_conf_read_mode, cfwb_expected_mode, sizeof(cfwb_expected_mode)) == 0);
    CFWB_CHECK(memcmp(op_conf_not_open, cfwb_expected_closed, sizeof(cfwb_expected_closed)) == 0);
    CFWB_CHECK(memcmp(cfwb_payload, cfwb_expected_payload, sizeof(cfwb_payload)) == 0);
}
static void cfwb_snapshot(void)
{
    memcpy(cfwb_expected_tables, cfwb_tables, sizeof(cfwb_tables));
    cfwb_expected_services = op_conf_services;
    cfwb_expected_active = op_conf_read_active;
    cfwb_expected_input = op_conf_read_handle;
    cfwb_expected_output = op_conf_write_handle;
    memcpy(cfwb_expected_name, op_conf_write_name, sizeof(cfwb_expected_name));
    memcpy(cfwb_expected_mode, op_conf_read_mode, sizeof(cfwb_expected_mode));
    memcpy(cfwb_expected_closed, op_conf_not_open, sizeof(cfwb_expected_closed));
    memcpy(cfwb_expected_payload, cfwb_payload, sizeof(cfwb_payload));
}
static void cfwb_seed(void)
{
    memset(cfwb_tables, 0x57, sizeof(cfwb_tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(cfwb_payload, 0x71, sizeof(cfwb_payload));
    cfwb_payload[17] = 0;
    op_conf_services = &cfwb_tables[cfwb_table_case].value;
    op_conf_read_active = cfwb_values[cfwb_active_case];
    op_conf_read_handle = op_conf_write_handle = cfwb_values[cfwb_handle_case];
    cfwb_stage = 0;
}
static void cfwb_mutate_state(void)
{
    if (cfwb_mutate)
    {
        op_conf_services = cfwb_expected_services = &cfwb_tables[1 - cfwb_table_case].value;
        op_conf_read_active = cfwb_expected_active = -17;
        op_conf_read_handle = cfwb_expected_input = -19;
        op_conf_write_handle = cfwb_expected_output = -23;
        op_conf_write_name[1] = cfwb_expected_name[1] = 'W';
    }
}

static const unsigned int cfwb_counts[] = {0, 1, 8, 17, 0x7fffffffu, 0xffffffffu};

static int cfwb_count_case, cfwb_pointer_case, cfwb_answer_case;
static unsigned int cfwb_count, cfwb_response;
static void *cfwb_argument;
static unsigned int cfwb_io_common(int handle, const void *data, unsigned int n, int which)
{
    CFWB_CHECK(cfwb_stage == 0 && which == cfwb_table_case && handle == cfwb_values[cfwb_handle_case] &&
               data == cfwb_argument && n == cfwb_count);
    cfwb_verify();
    cfwb_mutate_state();
    if (cfwb_mutate)
    {
        cfwb_payload[0] = cfwb_expected_payload[0] = 0;
        cfwb_payload[3] = cfwb_expected_payload[3] = 'D';
    }
    ++cfwb_stage;
    return cfwb_response;
}
static unsigned int cfwb_write0(int h, const void *p, unsigned int n)
{
    return cfwb_io_common(h, p, n, 0);
}
static unsigned int cfwb_write1(int h, const void *p, unsigned int n)
{
    return cfwb_io_common(h, p, n, 1);
}
static int op_test_conf_write_bytes(void)
{
    int expected, enabled;
    for (cfwb_table_case = 0; cfwb_table_case < 2; ++cfwb_table_case)
        for (cfwb_handle_case = 0; cfwb_handle_case < 6; ++cfwb_handle_case)
            for (cfwb_active_case = 0; cfwb_active_case < 6; ++cfwb_active_case)
                for (cfwb_count_case = 0; cfwb_count_case < 6; ++cfwb_count_case)
                    for (cfwb_pointer_case = 0; cfwb_pointer_case < 2; ++cfwb_pointer_case)
                        for (cfwb_answer_case = 0; cfwb_answer_case < 4; ++cfwb_answer_case)
                            for (cfwb_mutate = 0; cfwb_mutate < 2; ++cfwb_mutate)
                            {
                                cfwb_seed();
                                cfwb_tables[0].value.write = cfwb_write0;
                                cfwb_tables[1].value.write = cfwb_write1;
                                cfwb_count = cfwb_counts[cfwb_count_case];
                                cfwb_argument = cfwb_pointer_case ? cfwb_payload : 0;
                                cfwb_response = cfwb_answer_case == 0   ? cfwb_count
                                                : cfwb_answer_case == 1 ? cfwb_count + 1u
                                                : cfwb_answer_case == 2 ? cfwb_count - 1u
                                                                        : 0x80000000u;
                                enabled = cfwb_values[cfwb_handle_case] != 0 && cfwb_pointer_case != 0;
                                if (!enabled)
                                    op_conf_services = 0;
                                cfwb_snapshot();
                                expected = enabled && cfwb_response == cfwb_count;
                                CFWB_CHECK(op_conf_write_bytes((const void *)cfwb_argument, cfwb_count) == expected);
                                CFWB_CHECK(cfwb_stage == enabled);
                                cfwb_verify();
                            }
    printf("conf_write_bytes: %d checks, %d failures\n", cfwb_checks, cfwb_failures);
    return cfwb_failures != 0;
}

#undef CFWB_CHECK

#include "../src/conf_stream.h"
#include <stdio.h>
#include <limits.h>
static int cfrb_checks, cfrb_failures;
static void cfrb_check(int ok, int line)
{
    ++cfrb_checks;
    if (!ok)
    {
        ++cfrb_failures;
        printf("line%d failed\n", line);
    }
}
#define CFRB_CHECK(x) cfrb_check(!!(x), __LINE__)
typedef struct CFRB_OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} CFRB_OWNED_SERVICES;
static CFRB_OWNED_SERVICES cfrb_tables[2], cfrb_expected_tables[2];
static unsigned char cfrb_payload[40], cfrb_expected_payload[40];
static char cfrb_expected_name[136], cfrb_expected_mode[16], cfrb_expected_closed[140];
static OP_CONF_SERVICES *cfrb_expected_services;
static int cfrb_expected_active, cfrb_expected_input, cfrb_expected_output;
static const int cfrb_values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int cfrb_table_case, cfrb_handle_case, cfrb_active_case, cfrb_mutate, cfrb_stage;
static void cfrb_verify(void)
{
    CFRB_CHECK(memcmp(cfrb_tables, cfrb_expected_tables, sizeof(cfrb_tables)) == 0);
    CFRB_CHECK(op_conf_services == cfrb_expected_services);
    CFRB_CHECK(op_conf_read_active == cfrb_expected_active && op_conf_read_handle == cfrb_expected_input &&
               op_conf_write_handle == cfrb_expected_output);
    CFRB_CHECK(memcmp(op_conf_write_name, cfrb_expected_name, sizeof(cfrb_expected_name)) == 0);
    CFRB_CHECK(memcmp(op_conf_read_mode, cfrb_expected_mode, sizeof(cfrb_expected_mode)) == 0);
    CFRB_CHECK(memcmp(op_conf_not_open, cfrb_expected_closed, sizeof(cfrb_expected_closed)) == 0);
    CFRB_CHECK(memcmp(cfrb_payload, cfrb_expected_payload, sizeof(cfrb_payload)) == 0);
}
static void cfrb_snapshot(void)
{
    memcpy(cfrb_expected_tables, cfrb_tables, sizeof(cfrb_tables));
    cfrb_expected_services = op_conf_services;
    cfrb_expected_active = op_conf_read_active;
    cfrb_expected_input = op_conf_read_handle;
    cfrb_expected_output = op_conf_write_handle;
    memcpy(cfrb_expected_name, op_conf_write_name, sizeof(cfrb_expected_name));
    memcpy(cfrb_expected_mode, op_conf_read_mode, sizeof(cfrb_expected_mode));
    memcpy(cfrb_expected_closed, op_conf_not_open, sizeof(cfrb_expected_closed));
    memcpy(cfrb_expected_payload, cfrb_payload, sizeof(cfrb_payload));
}
static void cfrb_seed(void)
{
    memset(cfrb_tables, 0x57, sizeof(cfrb_tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(cfrb_payload, 0x71, sizeof(cfrb_payload));
    cfrb_payload[17] = 0;
    op_conf_services = &cfrb_tables[cfrb_table_case].value;
    op_conf_read_active = cfrb_values[cfrb_active_case];
    op_conf_read_handle = op_conf_write_handle = cfrb_values[cfrb_handle_case];
    cfrb_stage = 0;
}
static void cfrb_mutate_state(void)
{
    if (cfrb_mutate)
    {
        op_conf_services = cfrb_expected_services = &cfrb_tables[1 - cfrb_table_case].value;
        op_conf_read_active = cfrb_expected_active = -17;
        op_conf_read_handle = cfrb_expected_input = -19;
        op_conf_write_handle = cfrb_expected_output = -23;
        op_conf_write_name[1] = cfrb_expected_name[1] = 'W';
    }
}

static const unsigned int cfrb_counts[] = {0, 1, 8, 17, 0x7fffffffu, 0xffffffffu};

static int cfrb_count_case, cfrb_pointer_case, cfrb_answer_case;
static unsigned int cfrb_count, cfrb_response;
static void *cfrb_argument;
static unsigned int cfrb_io_common(int handle, const void *data, unsigned int n, int which)
{
    CFRB_CHECK(cfrb_stage == 0 && which == cfrb_table_case && handle == cfrb_values[cfrb_handle_case] &&
               data == cfrb_argument && n == cfrb_count);
    cfrb_verify();
    cfrb_mutate_state();
    if (cfrb_mutate)
    {
        cfrb_payload[0] = cfrb_expected_payload[0] = 0;
        cfrb_payload[3] = cfrb_expected_payload[3] = 'D';
    }
    ++cfrb_stage;
    return cfrb_response;
}
static unsigned int cfrb_read0(int h, void *p, unsigned int n)
{
    return cfrb_io_common(h, p, n, 0);
}
static unsigned int cfrb_read1(int h, void *p, unsigned int n)
{
    return cfrb_io_common(h, p, n, 1);
}
static int op_test_conf_read_bytes(void)
{
    int expected, enabled;
    for (cfrb_table_case = 0; cfrb_table_case < 2; ++cfrb_table_case)
        for (cfrb_handle_case = 0; cfrb_handle_case < 6; ++cfrb_handle_case)
            for (cfrb_active_case = 0; cfrb_active_case < 6; ++cfrb_active_case)
                for (cfrb_count_case = 0; cfrb_count_case < 6; ++cfrb_count_case)
                    for (cfrb_pointer_case = 0; cfrb_pointer_case < 2; ++cfrb_pointer_case)
                        for (cfrb_answer_case = 0; cfrb_answer_case < 4; ++cfrb_answer_case)
                            for (cfrb_mutate = 0; cfrb_mutate < 2; ++cfrb_mutate)
                            {
                                cfrb_seed();
                                cfrb_tables[0].value.read = cfrb_read0;
                                cfrb_tables[1].value.read = cfrb_read1;
                                cfrb_count = cfrb_counts[cfrb_count_case];
                                cfrb_argument = cfrb_pointer_case ? cfrb_payload : 0;
                                cfrb_response = cfrb_answer_case == 0   ? cfrb_count
                                                : cfrb_answer_case == 1 ? cfrb_count + 1u
                                                : cfrb_answer_case == 2 ? cfrb_count - 1u
                                                                        : 0x80000000u;
                                enabled = cfrb_values[cfrb_active_case] != 0 && cfrb_values[cfrb_handle_case] != 0;
                                if (!enabled)
                                    op_conf_services = 0;
                                cfrb_snapshot();
                                expected = enabled && cfrb_response == cfrb_count;
                                CFRB_CHECK(op_conf_read_bytes(cfrb_argument, cfrb_count) == expected);
                                CFRB_CHECK(cfrb_stage == enabled);
                                cfrb_verify();
                            }
    printf("conf_read_bytes: %d checks, %d failures\n", cfrb_checks, cfrb_failures);
    return cfrb_failures != 0;
}

#undef CFRB_CHECK

#include "../src/conf_stream.h"
#include <stdio.h>
#include <limits.h>
static int cfgf_checks, cfgf_failures;
static void cfgf_check(int ok, int line)
{
    ++cfgf_checks;
    if (!ok)
    {
        ++cfgf_failures;
        printf("line%d failed\n", line);
    }
}
#define CFGF_CHECK(x) cfgf_check(!!(x), __LINE__)
typedef struct CFGF_OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} CFGF_OWNED_SERVICES;
static CFGF_OWNED_SERVICES cfgf_tables[2], cfgf_expected_tables[2];
static unsigned char cfgf_payload[40], cfgf_expected_payload[40];
static char cfgf_expected_name[136], cfgf_expected_mode[16], cfgf_expected_closed[140];
static OP_CONF_SERVICES *cfgf_expected_services;
static int cfgf_expected_active, cfgf_expected_input, cfgf_expected_output;
static const int cfgf_values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int cfgf_table_case, cfgf_handle_case, cfgf_active_case, cfgf_mutate, cfgf_stage;
static void cfgf_verify(void)
{
    CFGF_CHECK(memcmp(cfgf_tables, cfgf_expected_tables, sizeof(cfgf_tables)) == 0);
    CFGF_CHECK(op_conf_services == cfgf_expected_services);
    CFGF_CHECK(op_conf_read_active == cfgf_expected_active && op_conf_read_handle == cfgf_expected_input &&
               op_conf_write_handle == cfgf_expected_output);
    CFGF_CHECK(memcmp(op_conf_write_name, cfgf_expected_name, sizeof(cfgf_expected_name)) == 0);
    CFGF_CHECK(memcmp(op_conf_read_mode, cfgf_expected_mode, sizeof(cfgf_expected_mode)) == 0);
    CFGF_CHECK(memcmp(op_conf_not_open, cfgf_expected_closed, sizeof(cfgf_expected_closed)) == 0);
    CFGF_CHECK(memcmp(cfgf_payload, cfgf_expected_payload, sizeof(cfgf_payload)) == 0);
}
static void cfgf_snapshot(void)
{
    memcpy(cfgf_expected_tables, cfgf_tables, sizeof(cfgf_tables));
    cfgf_expected_services = op_conf_services;
    cfgf_expected_active = op_conf_read_active;
    cfgf_expected_input = op_conf_read_handle;
    cfgf_expected_output = op_conf_write_handle;
    memcpy(cfgf_expected_name, op_conf_write_name, sizeof(cfgf_expected_name));
    memcpy(cfgf_expected_mode, op_conf_read_mode, sizeof(cfgf_expected_mode));
    memcpy(cfgf_expected_closed, op_conf_not_open, sizeof(cfgf_expected_closed));
    memcpy(cfgf_expected_payload, cfgf_payload, sizeof(cfgf_payload));
}
static void cfgf_seed(void)
{
    memset(cfgf_tables, 0x57, sizeof(cfgf_tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(cfgf_payload, 0x71, sizeof(cfgf_payload));
    cfgf_payload[17] = 0;
    op_conf_services = &cfgf_tables[cfgf_table_case].value;
    op_conf_read_active = cfgf_values[cfgf_active_case];
    op_conf_read_handle = op_conf_write_handle = cfgf_values[cfgf_handle_case];
    cfgf_stage = 0;
}

static int op_test_conf_get_file(void)
{
    int k;
    for (cfgf_table_case = 0; cfgf_table_case < 2; ++cfgf_table_case)
        for (cfgf_handle_case = 0; cfgf_handle_case < 6; ++cfgf_handle_case)
            for (cfgf_active_case = 0; cfgf_active_case < 6; ++cfgf_active_case)
                for (k = 0; k < 2; ++k)
                {
                    cfgf_seed();
                    if (k)
                        op_conf_services = 0;
                    cfgf_snapshot();
                    CFGF_CHECK(op_conf_get_file() == cfgf_values[cfgf_handle_case]);
                    CFGF_CHECK(cfgf_stage == 0);
                    cfgf_verify();
                }
    printf("conf_get_file: %d checks, %d failures\n", cfgf_checks, cfgf_failures);
    return cfgf_failures != 0;
}

#undef CFGF_CHECK

#include "../src/conf_stream.h"
#include <stdio.h>
#include <limits.h>
static int cfcy_checks, cfcy_failures;
static void cfcy_check(int ok, int line)
{
    ++cfcy_checks;
    if (!ok)
    {
        ++cfcy_failures;
        printf("line%d failed\n", line);
    }
}
#define CFCY_CHECK(x) cfcy_check(!!(x), __LINE__)
typedef struct CFCY_OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} CFCY_OWNED_SERVICES;
static CFCY_OWNED_SERVICES cfcy_tables[2], cfcy_expected_tables[2];
static unsigned char cfcy_payload[40], cfcy_expected_payload[40];
static char cfcy_expected_name[136], cfcy_expected_mode[16], cfcy_expected_closed[140];
static OP_CONF_SERVICES *cfcy_expected_services;
static int cfcy_expected_active, cfcy_expected_input, cfcy_expected_output;
static const int cfcy_values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int cfcy_table_case, cfcy_handle_case, cfcy_active_case, cfcy_mutate, cfcy_stage;
static void cfcy_verify(void)
{
    CFCY_CHECK(memcmp(cfcy_tables, cfcy_expected_tables, sizeof(cfcy_tables)) == 0);
    CFCY_CHECK(op_conf_services == cfcy_expected_services);
    CFCY_CHECK(op_conf_read_active == cfcy_expected_active && op_conf_read_handle == cfcy_expected_input &&
               op_conf_write_handle == cfcy_expected_output);
    CFCY_CHECK(memcmp(op_conf_write_name, cfcy_expected_name, sizeof(cfcy_expected_name)) == 0);
    CFCY_CHECK(memcmp(op_conf_read_mode, cfcy_expected_mode, sizeof(cfcy_expected_mode)) == 0);
    CFCY_CHECK(memcmp(op_conf_not_open, cfcy_expected_closed, sizeof(cfcy_expected_closed)) == 0);
    CFCY_CHECK(memcmp(cfcy_payload, cfcy_expected_payload, sizeof(cfcy_payload)) == 0);
}
static void cfcy_snapshot(void)
{
    memcpy(cfcy_expected_tables, cfcy_tables, sizeof(cfcy_tables));
    cfcy_expected_services = op_conf_services;
    cfcy_expected_active = op_conf_read_active;
    cfcy_expected_input = op_conf_read_handle;
    cfcy_expected_output = op_conf_write_handle;
    memcpy(cfcy_expected_name, op_conf_write_name, sizeof(cfcy_expected_name));
    memcpy(cfcy_expected_mode, op_conf_read_mode, sizeof(cfcy_expected_mode));
    memcpy(cfcy_expected_closed, op_conf_not_open, sizeof(cfcy_expected_closed));
    memcpy(cfcy_expected_payload, cfcy_payload, sizeof(cfcy_payload));
}
static void cfcy_seed(void)
{
    memset(cfcy_tables, 0x57, sizeof(cfcy_tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(cfcy_payload, 0x71, sizeof(cfcy_payload));
    cfcy_payload[17] = 0;
    op_conf_services = &cfcy_tables[cfcy_table_case].value;
    op_conf_read_active = cfcy_values[cfcy_active_case];
    op_conf_read_handle = op_conf_write_handle = cfcy_values[cfcy_handle_case];
    cfcy_stage = 0;
}
static void cfcy_mutate_state(void)
{
    if (cfcy_mutate)
    {
        op_conf_services = cfcy_expected_services = &cfcy_tables[1 - cfcy_table_case].value;
        op_conf_read_active = cfcy_expected_active = -17;
        op_conf_read_handle = cfcy_expected_input = -19;
        op_conf_write_handle = cfcy_expected_output = -23;
        op_conf_write_name[1] = cfcy_expected_name[1] = 'W';
    }
}

static int cfcy_active, cfcy_answer;
static unsigned int cfcy_read_common(int handle, void *data, unsigned int cfcy_count, int which)
{
    CFCY_CHECK(cfcy_active && cfcy_stage == 0 && cfcy_expected_services == &cfcy_tables[which].value &&
               handle == cfcy_expected_input && data == cfcy_payload && cfcy_count == 8);
    cfcy_verify();
    CFCY_CHECK(op_conf_get_file() == cfcy_expected_input);
    cfcy_mutate_state();
    if (cfcy_mutate)
        cfcy_payload[0] = cfcy_expected_payload[0] = 0;
    ++cfcy_stage;
    return cfcy_answer ? cfcy_count + 1u : cfcy_count;
}
static unsigned int cfcy_read0(int h, void *p, unsigned int n)
{
    return cfcy_read_common(h, p, n, 0);
}
static unsigned int cfcy_read1(int h, void *p, unsigned int n)
{
    return cfcy_read_common(h, p, n, 1);
}
static unsigned int cfcy_write_common(int handle, const void *data, unsigned int cfcy_count, int which)
{
    CFCY_CHECK(cfcy_active && (cfcy_stage == 1 || cfcy_stage == 2) &&
               cfcy_expected_services == &cfcy_tables[which].value && handle == cfcy_expected_output &&
               data == cfcy_payload &&
               cfcy_count == (cfcy_stage == 1 ? 3u
                              : cfcy_mutate   ? 0u
                                              : 4u));
    cfcy_verify();
    cfcy_mutate_state();
    if (cfcy_mutate)
        cfcy_payload[0] = cfcy_expected_payload[0] = 0;
    ++cfcy_stage;
    return cfcy_answer ? cfcy_count + 1u : cfcy_count;
}
static unsigned int cfcy_write0(int h, const void *p, unsigned int n)
{
    return cfcy_write_common(h, p, n, 0);
}
static unsigned int cfcy_write1(int h, const void *p, unsigned int n)
{
    return cfcy_write_common(h, p, n, 1);
}
static int cfcy_close_common(int handle, int which)
{
    CFCY_CHECK(cfcy_active && cfcy_stage == 3 && cfcy_expected_services == &cfcy_tables[which].value &&
               handle == cfcy_expected_output);
    cfcy_verify();
    cfcy_mutate_state();
    ++cfcy_stage;
    cfcy_expected_output = 0;
    return -1;
}
static int cfcy_close0(int h)
{
    return cfcy_close_common(h, 0);
}
static int cfcy_close1(int h)
{
    return cfcy_close_common(h, 1);
}
static char *cfcy_copy(char *out, const char *in, unsigned int cfcy_count)
{
    unsigned int i;
    CFCY_CHECK(cfcy_active && cfcy_stage == 4 && out == op_conf_write_name && in == op_conf_not_open &&
               cfcy_count == 127);
    if (!cfcy_active || cfcy_stage != 4 || out != op_conf_write_name || in != op_conf_not_open || cfcy_count != 127)
        return 0;
    cfcy_verify();
    for (i = 0; i < 127; ++i)
    {
        if (i < 8)
            cfcy_expected_name[i] = 'B';
        else
            cfcy_expected_name[i] = 0;
    }
    strncpy(op_conf_write_name, op_conf_not_open, 127);
    if (cfcy_mutate)
        op_conf_write_name[127] = cfcy_expected_name[127] = 'Z';
    cfcy_verify();
    cfcy_expected_name[127] = 0;
    ++cfcy_stage;
    return op_conf_not_open;
}
static int op_test_conf_connected(void)
{
    cfcy_active = 1;
    for (cfcy_table_case = 0; cfcy_table_case < 2; ++cfcy_table_case)
        for (cfcy_handle_case = 1; cfcy_handle_case < 6; ++cfcy_handle_case)
            for (cfcy_active_case = 1; cfcy_active_case < 6; ++cfcy_active_case)
                for (cfcy_answer = 0; cfcy_answer < 2; ++cfcy_answer)
                    for (cfcy_mutate = 0; cfcy_mutate < 2; ++cfcy_mutate)
                    {
                        cfcy_seed();
                        cfcy_tables[0].value.read = cfcy_read0;
                        cfcy_tables[1].value.read = cfcy_read1;
                        cfcy_tables[0].value.write = cfcy_write0;
                        cfcy_tables[1].value.write = cfcy_write1;
                        cfcy_tables[0].value.close = cfcy_close0;
                        cfcy_tables[1].value.close = cfcy_close1;
                        cfcy_payload[4] = 0;
                        cfcy_snapshot();
                        CFCY_CHECK(op_conf_get_file() == cfcy_expected_input);
                        cfcy_verify();
                        CFCY_CHECK(op_conf_read_bytes(cfcy_payload, 8) == !cfcy_answer);
                        CFCY_CHECK(cfcy_stage == 1);
                        cfcy_verify();
                        CFCY_CHECK(op_conf_write_bytes(cfcy_payload, 3) == !cfcy_answer);
                        CFCY_CHECK(cfcy_stage == 2);
                        cfcy_verify();
                        CFCY_CHECK(op_conf_write_string((const char *)cfcy_payload) == !cfcy_answer);
                        CFCY_CHECK(cfcy_stage == 3);
                        cfcy_verify();
                        op_conf_close_write();
                        CFCY_CHECK(cfcy_stage == 5);
                        cfcy_verify();
                        CFCY_CHECK(op_conf_write_bytes(cfcy_payload, 3) == 0);
                        CFCY_CHECK(op_conf_write_string((const char *)cfcy_payload) == 0);
                        CFCY_CHECK(cfcy_stage == 5);
                        CFCY_CHECK(op_conf_get_file() == cfcy_expected_input);
                        cfcy_verify();
                    }
    cfcy_active = 0;
    printf("configuration connected: %d checks, %d failures\n", cfcy_checks, cfcy_failures);
    return cfcy_failures != 0;
}

#undef CFCY_CHECK
