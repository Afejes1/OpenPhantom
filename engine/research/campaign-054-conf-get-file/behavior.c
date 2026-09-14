#include "api.h"
#include <stdio.h>
#include <limits.h>
static int checks, failures;
static void check(int ok, int line)
{
    ++checks;
    if (!ok)
    {
        ++failures;
        printf("line%d failed\n", line);
    }
}
#define CHECK(x) check(!!(x), __LINE__)
typedef struct OWNED_SERVICES
{
    unsigned int before;
    OP_CONF_SERVICES value;
    unsigned int after;
} OWNED_SERVICES;
static OWNED_SERVICES tables[2], expected_tables[2];
OP_CONF_SERVICES *op_conf_services;
int op_conf_read_active, op_conf_read_handle, op_conf_write_handle;
char op_conf_write_name[136], op_conf_read_mode[16], op_conf_not_open[140];
static unsigned char payload[40], expected_payload[40];
static char expected_name[136], expected_mode[16], expected_closed[140];
static OP_CONF_SERVICES *expected_services;
static int expected_active, expected_input, expected_output;
static const int values[] = {0, 1, -1, INT_MIN, INT_MAX, 73};
static int table_case, handle_case, active_case, mutate, stage;
static void verify(void)
{
    CHECK(memcmp(tables, expected_tables, sizeof(tables)) == 0);
    CHECK(op_conf_services == expected_services);
    CHECK(op_conf_read_active == expected_active && op_conf_read_handle == expected_input &&
          op_conf_write_handle == expected_output);
    CHECK(memcmp(op_conf_write_name, expected_name, sizeof(expected_name)) == 0);
    CHECK(memcmp(op_conf_read_mode, expected_mode, sizeof(expected_mode)) == 0);
    CHECK(memcmp(op_conf_not_open, expected_closed, sizeof(expected_closed)) == 0);
    CHECK(memcmp(payload, expected_payload, sizeof(payload)) == 0);
}
static void snapshot(void)
{
    memcpy(expected_tables, tables, sizeof(tables));
    expected_services = op_conf_services;
    expected_active = op_conf_read_active;
    expected_input = op_conf_read_handle;
    expected_output = op_conf_write_handle;
    memcpy(expected_name, op_conf_write_name, sizeof(expected_name));
    memcpy(expected_mode, op_conf_read_mode, sizeof(expected_mode));
    memcpy(expected_closed, op_conf_not_open, sizeof(expected_closed));
    memcpy(expected_payload, payload, sizeof(payload));
}
static void seed(void)
{
    memset(tables, 0x57, sizeof(tables));
    memset(op_conf_write_name, 0x63, sizeof(op_conf_write_name));
    memset(op_conf_read_mode, 0x41, sizeof(op_conf_read_mode));
    op_conf_read_mode[7] = 0;
    memset(op_conf_not_open, 0x42, sizeof(op_conf_not_open));
    op_conf_not_open[8] = 0;
    memset(payload, 0x71, sizeof(payload));
    payload[17] = 0;
    op_conf_services = &tables[table_case].value;
    op_conf_read_active = values[active_case];
    op_conf_read_handle = op_conf_write_handle = values[handle_case];
    stage = 0;
}

int main(void)
{
    int k;
    for (table_case = 0; table_case < 2; ++table_case)
        for (handle_case = 0; handle_case < 6; ++handle_case)
            for (active_case = 0; active_case < 6; ++active_case)
                for (k = 0; k < 2; ++k)
                {
                    seed();
                    if (k)
                        op_conf_services = 0;
                    snapshot();
                    CHECK(op_conf_get_file() == values[handle_case]);
                    CHECK(stage == 0);
                    verify();
                }
    printf("conf_get_file: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
