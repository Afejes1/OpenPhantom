#include "api.h"
// FUNCTION: WMAIN 0x0047bb8d
void op_material_invalidate_cache(OP_MATERIAL *material)
{
    OP_CACHE_ROW *current;
    unsigned int i;
    current = material->cache;
    for (i = 0; i < material->cache_count; ++i)
    {
        unsigned int j;
        for (j = 0; j < current->count; ++j)
        {
            current->entries[j].invalid00 = 0;
            current->entries[j].invalid08 = 0;
        }
        ++current;
    }
}
