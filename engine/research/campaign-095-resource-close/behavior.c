#include "api.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
static int checks, failures;
static int selected_operation = 0;
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
OP_RESOURCE_SLOT op_resource_slots[34];
OP_CONF_SERVICES *op_conf_services, *op_resource_backend;
int op_resource_log_handle;
const char op_resource_source[] = "authored-resource-source", op_resource_invalid_close[] = "authored-close-assert",
           op_resource_invalid_access[] = "authored-access-assert",
           op_resource_close_format[] = "authored-close-format";
static OP_RESOURCE_SLOT expected_slots[34];
static OP_CONF_SERVICES hosts[2], backends[2], expected_hosts[2], expected_backends[2];
static int selected, mutation, error_mutation, assert_calls, log_calls, backend_calls, wanted_host, wanted_backend,
    wanted_log, return_value;
static int should_assert, should_active, should_real, should_log;
static void verify(void)
{
    CHECK(!memcmp(op_resource_slots, expected_slots, sizeof(expected_slots)));
    CHECK(!memcmp(hosts, expected_hosts, sizeof(hosts)));
    CHECK(!memcmp(backends, expected_backends, sizeof(backends)));
    CHECK(op_conf_services == &hosts[wanted_host] && op_resource_backend == &backends[wanted_backend]);
    CHECK(op_resource_log_handle == wanted_log);
}
static void assertion(int table, const char *message, const char *source, int line)
{
    CHECK(table == wanted_host && should_assert && assert_calls == 0 && log_calls == 0 && backend_calls == 0);
    CHECK(message == op_resource_invalid_close && source == op_resource_source && line == 0x75e);
    verify();
    ++assert_calls;
    if (error_mutation)
    {
        op_resource_slots[selected].backend_handle = expected_slots[selected].backend_handle = 77;
        op_resource_slots[selected].real_file = expected_slots[selected].real_file = 1;
        op_resource_slots[selected].position = expected_slots[selected].position = -111;
        op_resource_slots[selected].eof = expected_slots[selected].eof = -222;
        op_conf_services = &hosts[wanted_host = 1];
        op_resource_backend = &backends[wanted_backend = 1];
        op_resource_log_handle = wanted_log = 1;
    }
}
static void assert0(const char *m, const char *s, int l)
{
    assertion(0, m, s, l);
}
static void assert1(const char *m, const char *s, int l)
{
    assertion(1, m, s, l);
}
static int debug(int table, int stream, const char *format, int handle)
{
    CHECK(selected_operation == 0 && should_log && table == wanted_host && stream == wanted_log && format == op_resource_close_format &&
          handle == selected);
    CHECK(assert_calls == should_assert && log_calls == 0 && backend_calls == 0);
    verify();
    ++log_calls;
    if (mutation)
    {
        op_resource_slots[selected].backend_handle = expected_slots[selected].backend_handle = 88;
        op_resource_slots[selected].real_file = expected_slots[selected].real_file = 0;
        op_resource_backend = &backends[wanted_backend = 1 - wanted_backend];
        op_conf_services = &hosts[wanted_host = 1 - wanted_host];
        op_resource_log_handle = wanted_log = -19;
    }
    return -123;
}
static int debug0(int stream, const char *format, ...)
{
    int handle;
    va_list args;
    va_start(args, format);
    handle = va_arg(args, int);
    va_end(args);
    return debug(0, stream, format, handle);
}
static int debug1(int stream, const char *format, ...)
{
    int handle;
    va_list args;
    va_start(args, format);
    handle = va_arg(args, int);
    va_end(args);
    return debug(1, stream, format, handle);
}
static int backend(int table, int operation, int handle)
{
    CHECK(operation == selected_operation && table == wanted_backend && should_active && should_real);
    CHECK(handle == expected_slots[selected].backend_handle);
    CHECK(assert_calls == should_assert && log_calls == should_log && backend_calls == 0);
    verify();
    ++backend_calls;
    if (mutation)
    {
        op_resource_slots[selected].backend_handle = expected_slots[selected].backend_handle = 0;
        op_resource_slots[selected].position = expected_slots[selected].position = 312;
        op_resource_slots[selected].eof = expected_slots[selected].eof = 411;
        op_resource_slots[selected].reserved20[1] = expected_slots[selected].reserved20[1] = 0x63;
        op_resource_slots[(selected + 1) % 34].reserved20[0] = expected_slots[(selected + 1) % 34].reserved20[0] = 0x71;
        op_resource_backend = &backends[wanted_backend = 1 - wanted_backend];
    }
    return return_value;
}
static int close0(int h)
{
    return backend(0, 0, h);
}
static int close1(int h)
{
    return backend(1, 0, h);
}
static int eof0(int h)
{
    return backend(0, 1, h);
}
static int eof1(int h)
{
    return backend(1, 1, h);
}
static int tell0(int h)
{
    return backend(0, 2, h);
}
static int tell1(int h)
{
    return backend(1, 2, h);
}
int main(void)
{
    static const int indices[7] = {0, 1, 2, 15, 31, 32, 33};
    static const int values[5] = {INT_MIN, -1, 0, 1, INT_MAX};
    static const int logs[3] = {0, 1, -1};
    int i, p, kind, v, g, result, expected_return;
    memset(hosts, 0x91, sizeof(hosts));
    memset(backends, 0x83, sizeof(backends));
    hosts[0].assert_failure = assert0;
    hosts[1].assert_failure = assert1;
    hosts[0].debug_print = debug0;
    hosts[1].debug_print = debug1;
    backends[0].close = close0;
    backends[1].close = close1;
    backends[0].eof = eof0;
    backends[1].eof = eof1;
    backends[0].tell = tell0;
    backends[1].tell = tell1;
    memcpy(expected_hosts, hosts, sizeof(hosts));
    memcpy(expected_backends, backends, sizeof(backends));
    for (i = 0; i < 7; ++i)
        for (p = 0; p < 2; ++p)
            for (kind = 0; kind < 3; ++kind)
                for (v = 0; v < 5; ++v)
                    for (g = 0; g < 3; ++g)
                        for (mutation = 0; mutation < 2; ++mutation)
                            for (error_mutation = 0; error_mutation < 2; ++error_mutation)
                            {
                                selected = indices[i];
                                assert_calls = log_calls = backend_calls = 0;
                                return_value = values[v];
                                memset(op_resource_slots, 0x35 + p, sizeof(op_resource_slots));
                                op_resource_slots[selected].backend_handle = kind ? values[(v + 1) % 5] : 0;
                                if (kind && !op_resource_slots[selected].backend_handle)
                                    op_resource_slots[selected].backend_handle = 73;
                                op_resource_slots[selected].real_file = kind == 2 ? (p ? -1 : 1) : 0;
                                op_resource_slots[selected].position = values[v];
                                op_resource_slots[selected].eof = values[4 - v];
                                memcpy(expected_slots, op_resource_slots, sizeof(expected_slots));
                                op_conf_services = &hosts[wanted_host = p];
                                op_resource_backend = &backends[wanted_backend = p];
                                op_resource_log_handle = wanted_log = logs[g];
                                should_assert = selected == 0 || selected == 33;
                                should_active = kind != 0 || (should_assert && error_mutation);
                                should_real = kind == 2 || (should_assert && error_mutation);
                                should_log = selected_operation == 0 && should_active && should_real &&
                                             (logs[g] != 0 || (should_assert && error_mutation));
                                expected_return = !should_active ? 1
                                                  : selected_operation == 0       ? 0
                                                  : should_real  ? return_value
                                                  : selected_operation == 1       ? values[4 - v]
                                                                 : values[v];
                                result = op_resource_close(selected);
                                if (selected_operation == 0 && should_active)
                                    memset(&expected_slots[selected], 0, sizeof(expected_slots[selected]));
                                CHECK(result == expected_return);
                                CHECK(assert_calls == should_assert);
                                CHECK(log_calls == should_log);
                                CHECK(backend_calls == (should_active && should_real));
                                verify();
                            }
    printf("resource_close: %d checks, %d failures\n", checks, failures);
    return failures != 0;
}
