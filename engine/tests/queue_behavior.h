/* Synthetic queue data only; no retail data or material contents. */
static void queue_reset(void)
{
    int i;
    op_projected_vertex_count = 0;
    op_face_packet_count = 0;
    op_material_bucket_count = 0;
    op_last_material_bucket = 0;
    op_render_queue_tag = 0;
    for (i = 0; i < 64; ++i) {
        op_material_buckets[i].material = 0;
        op_material_buckets[i].head = 0;
        op_material_buckets[i].tail = 0;
    }
}

static void queue_tests(void)
{
    int i;
    int materials[65];
    op_packet_face faces[65];
    op_face_packet *packet;
    CHECK(sizeof(op_projected_vertex) == 16);
    CHECK(sizeof(op_face_packet) == 0xb8);
    CHECK(sizeof(op_material_bucket) == 12);
    CHECK(offsetof(op_packet_face, material) == 0x20);
    CHECK(offsetof(op_face_packet, face) == 4);
    CHECK(offsetof(op_face_packet, render_tag) == 0x17);
    queue_reset();

    CHECK(op_peek_projected_vertices(0) == op_projected_vertices);
    CHECK(op_peek_projected_vertices(8192) == op_projected_vertices);
    CHECK(op_peek_projected_vertices(8193) == 0);
    CHECK(op_projected_vertex_count == 0);
    op_projected_vertex_count = 8191;
    CHECK(op_peek_projected_vertices(1) == op_projected_vertices + 8191);
    CHECK(op_peek_projected_vertices(2) == 0);
    op_projected_vertex_count = 8192;
    CHECK(op_peek_projected_vertices(0) == op_projected_vertices + 8192);
    CHECK(op_peek_projected_vertices(1) == 0);
    op_projected_vertex_count = 1;
    /* The original unsigned sum wraps before the capacity comparison. */
    CHECK(op_peek_projected_vertices(0xffffffffu) == op_projected_vertices + 1);
    CHECK(op_projected_vertex_count == 1);
    CHECK(op_peek_face_packet() == op_face_packets);
    CHECK(op_peek_face_packet() == op_face_packets);
    CHECK(op_face_packet_count == 0);
    op_face_packet_count = 4095;
    CHECK(op_peek_face_packet() == op_face_packets + 4095);
    op_face_packet_count = 4096;
    CHECK(op_peek_face_packet() == 0);
    op_face_packet_count = 2147483647;
    CHECK(op_peek_face_packet() == 0);
    queue_reset();

    CHECK(op_queue_face_packet(0) == 0);
    CHECK(op_face_packet_count == 0 && op_material_bucket_count == 0);
    packet = op_peek_face_packet();
    packet->next = op_face_packets + 1;
    packet->face = 0;
    packet->render_tag = 3;
    op_render_queue_tag = 0xa5;
    CHECK(op_queue_face_packet(packet) == 0);
    CHECK(packet->render_tag == 0xa5);
    CHECK(packet->next == op_face_packets + 1);
    faces[0].material = 0;
    packet->face = &faces[0];
    op_render_queue_tag = 0x81;
    CHECK(op_queue_face_packet(packet) == 0);
    CHECK(packet->render_tag == 0x81);
    CHECK(packet->next == op_face_packets + 1);
    CHECK(op_face_packet_count == 0 && op_last_material_bucket == 0);

    for (i = 0; i < 65; ++i) {
        materials[i] = i;
        faces[i].material = &materials[i];
    }
    packet->face = &faces[0];
    CHECK(op_queue_face_packet(packet) == 1);
    CHECK(packet->next == 0 && op_face_packet_count == 1);
    CHECK(op_material_bucket_count == 1);
    CHECK(op_last_material_bucket == op_material_buckets);
    CHECK(op_material_buckets[0].material == &materials[0]);
    CHECK(op_material_buckets[0].head == packet && op_material_buckets[0].tail == packet);
    packet = op_peek_face_packet();
    packet->face = &faces[0];
    CHECK(op_queue_face_packet(packet) == 1); /* Cached material. */
    CHECK(op_material_bucket_count == 1 && op_face_packet_count == 2);
    CHECK(op_face_packets[0].next == packet && packet->next == 0);
    CHECK(op_material_buckets[0].tail == packet);
    packet = op_peek_face_packet();
    packet->face = &faces[1];
    CHECK(op_queue_face_packet(packet) == 1);
    CHECK(op_material_bucket_count == 2 && op_last_material_bucket == op_material_buckets + 1);
    packet = op_peek_face_packet();
    packet->face = &faces[0];
    CHECK(op_queue_face_packet(packet) == 1); /* Search finds an earlier bucket. */
    CHECK(op_material_bucket_count == 2 && op_face_packet_count == 4);
    CHECK(op_face_packets[1].next == packet);
    CHECK(op_material_buckets[0].tail == packet && packet->next == 0);

    queue_reset();
    for (i = 0; i < 64; ++i) {
        packet = op_peek_face_packet();
        packet->face = &faces[i];
        CHECK(op_queue_face_packet(packet) == 1);
        CHECK(op_material_bucket_count == i + 1);
        CHECK(op_material_buckets[i].head == packet && op_material_buckets[i].tail == packet);
    }
    packet = op_peek_face_packet();
    packet->face = &faces[64];
    packet->next = op_face_packets;
    op_render_queue_tag = 0xfe;
    CHECK(op_queue_face_packet(packet) == 0); /* Sixty-fifth material rejected. */
    CHECK(packet->render_tag == 0xfe && packet->next == op_face_packets);
    CHECK(op_face_packet_count == 64 && op_material_bucket_count == 64);
    CHECK(op_last_material_bucket == op_material_buckets + 63);
    for (i = 0; i < 64; ++i) {
        CHECK(op_material_buckets[i].material == &materials[i]);
        CHECK(op_material_buckets[i].head == op_face_packets + i);
        CHECK(op_material_buckets[i].tail == op_face_packets + i);
        CHECK(op_face_packets[i].next == 0);
    }
    packet->face = &faces[63]; /* Cache hit still succeeds with all buckets used. */
    CHECK(op_queue_face_packet(packet) == 1);
    CHECK(op_material_bucket_count == 64 && op_face_packet_count == 65);
    CHECK(op_face_packets[63].next == packet && packet->next == 0);
    packet = op_peek_face_packet();
    packet->face = &faces[0]; /* Search hit also succeeds at capacity. */
    CHECK(op_queue_face_packet(packet) == 1);
    CHECK(op_material_bucket_count == 64 && op_face_packet_count == 66);
    CHECK(op_face_packets[0].next == packet && packet->next == 0);
    /* The final available packet commits, then the availability helper rejects. */
    op_face_packet_count = 4095;
    packet = op_peek_face_packet();
    packet->face = &faces[0];
    CHECK(op_queue_face_packet(packet) == 1);
    CHECK(op_face_packet_count == 4096 && op_peek_face_packet() == 0);
    CHECK(op_material_buckets[0].tail == packet && packet->next == 0);
    queue_reset();
}
