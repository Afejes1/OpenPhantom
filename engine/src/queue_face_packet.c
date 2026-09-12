#include "baseline.h"

/* FUNCTION: WMAIN 0x00401eb8 */
int op_queue_face_packet(op_face_packet *packet)
{
    op_material_bucket *current_bucket;
    int search_index = 0;
    void *material_key;
    op_packet_face *pFace;

    if (packet == 0)
        return 0;
    packet->render_tag = op_render_queue_tag;
    pFace = packet->face;
    if (pFace == 0)
        return 0;
    material_key = pFace->material;
    if (material_key == 0)
        return 0;

    current_bucket = 0;
    if (op_last_material_bucket != 0 &&
        op_last_material_bucket->material == material_key) {
        current_bucket = op_last_material_bucket;
    } else {
        for (search_index = 0; search_index < op_material_bucket_count; ++search_index) {
            current_bucket = &op_material_buckets[search_index];
            if (current_bucket->material == material_key)
                break;
        }
        if (search_index >= op_material_bucket_count)
            current_bucket = 0;
    }
    if (current_bucket == 0) {
        current_bucket = &op_material_buckets[op_material_bucket_count];
        ++op_material_bucket_count;
        if (op_material_bucket_count > 64) {
            --op_material_bucket_count;
            return 0;
        }
    }
    packet->next = 0;
    if (current_bucket->head == 0) {
        current_bucket->material = material_key;
        current_bucket->head = packet;
        current_bucket->tail = packet;
    } else {
        current_bucket->tail->next = packet;
        current_bucket->tail = packet;
    }
    op_last_material_bucket = current_bucket;
    ++op_face_packet_count;
    return 1;
}
