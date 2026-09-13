#include "api.h"
// FUNCTION: WMAIN 0x0043cd22
int op_zap_shutdown(void)
{
    op_release_sprite(&op_zap_sprite);
    return 1;
}
