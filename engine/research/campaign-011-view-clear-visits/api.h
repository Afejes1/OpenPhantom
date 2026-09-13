#ifndef OP_CAMPAIGN_011_VIEW_CLEAR_VISITS_API_H
#define OP_CAMPAIGN_011_VIEW_CLEAR_VISITS_API_H
typedef struct OP_VIEW_VERTEX {
    unsigned char opaque00[0x0d];
    unsigned char visited;
    unsigned char opaque0e[0x12];
} OP_VIEW_VERTEX;
typedef struct OP_VIEW_WORLD {
    unsigned char opaque00[0x9c];
    OP_VIEW_VERTEX *vertices;
} OP_VIEW_WORLD;
void op_view_clear_visits(OP_VIEW_WORLD *world, unsigned short first,
                          unsigned short last);
#endif
