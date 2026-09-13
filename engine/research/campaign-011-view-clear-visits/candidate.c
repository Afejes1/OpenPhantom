#include "api.h"

// FUNCTION: WMAIN 0x00419440
void op_view_clear_visits(OP_VIEW_WORLD *world, unsigned short first,
                          unsigned short last)
{
    OP_VIEW_VERTEX *vertex;
    unsigned int remaining;

    if (world == 0 || world->vertices == 0)
        return;
    vertex = &world->vertices[first];
    if (first > last)
        return;
    remaining = (unsigned int)last - (unsigned int)first + 1;
    do {
        vertex->visited = 0;
        ++vertex;
        --remaining;
    } while (remaining != 0);
}
