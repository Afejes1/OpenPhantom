#include "api.h"
// FUNCTION: WMAIN 0x0046b780
unsigned int op_font_query(void)
{
    if (!op_current_font)
        return 0;
    return op_query_font_resource(op_current_font->font);
}
