#include "utilities.h"
// FUNCTION: WMAIN 0x004972a4
void op_util_snprintf(char *destination, unsigned int size, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    op_vsnprintf(destination, size - 1, format, args);
    destination[size - 1] = 0;
    va_end(args);
}
