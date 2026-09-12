#include "baseline.h"

/* FUNCTION: WMAIN 0x00402021 */
op_face_packet *op_peek_face_packet(void)
{
    if (op_face_packet_count >= 4096)
        return 0;
    return &op_face_packets[op_face_packet_count];
}
