#include "api.h"

// FUNCTION: WMAIN 0x00429580
void op_texture_set_owned(void *resource, int owned)
{
    if (resource == 0) {
        return;
    }
    op_set_resource_owned(resource, owned);
}
