/* Synthetic bucket storage; original bucket-table capacity remains unresolved. */
unsigned int op_special_surface_bucket;
op_surface_draw_entry op_surface_draw_entries[8192];
op_surface_draw_entry *op_surface_buckets[32];

static void surface_entry_tests(void)
{
    op_draw_surface surface, retained;
    unsigned char before[sizeof(op_draw_surface)];
    unsigned char *bytes = (unsigned char *)&surface;
    unsigned int packed, existing, index, expected_bucket, expected_opacity;
    unsigned int byte_index;
    CHECK(offsetof(op_draw_surface, opacity) == 0x0e);
    CHECK(offsetof(op_draw_surface, texture_index) == 0x24);
    CHECK(offsetof(op_draw_surface, packed_opacity) == 0x2b);
    CHECK(sizeof(op_surface_draw_entry) == 12);
    CHECK(offsetof(op_surface_draw_entry, next) == 4);
    for (packed = 0; packed < 256; ++packed) {
        for (existing = 0; existing < 3; ++existing) {
            for (byte_index = 0; byte_index < sizeof(surface); ++byte_index) bytes[byte_index] = 0xa5;
            surface.opacity = (unsigned char)(existing == 0 ? 0 : existing == 1 ? 17 : 255);
            surface.texture_index = 5;
            surface.packed_opacity = (unsigned char)packed;
            for (byte_index = 0; byte_index < sizeof(surface); ++byte_index) before[byte_index] = bytes[byte_index];
            for (index = 0; index < 32; ++index) op_surface_buckets[index] = &op_surface_draw_entries[index + 100];
            op_gathered_surface_count = packed == 255 ? 8191 : 3;
            index = op_gathered_surface_count;
            op_surface_draw_entries[index].surface = &retained;
            op_surface_draw_entries[index].opaque_08 = 0x12345678u;
            op_special_surface_bucket = 31;
            expected_bucket = (packed != 0 || existing != 0) ? 31 : 5;
            /* Five-bit replication, expressed independently with division/remainder. */
            expected_opacity = existing == 1 ? 17 : existing == 2 ? 255 :
                               (packed % 32) * 8 + (packed % 32) / 4;
            op_push_surface_draw_entry(&surface);
            CHECK(surface.opacity == expected_opacity);
            CHECK(op_gathered_surface_count == index);
            CHECK(op_surface_draw_entries[index].surface == &retained);
            CHECK(op_surface_draw_entries[index].opaque_08 == 0x12345678u);
            CHECK(op_surface_draw_entries[index].next == &op_surface_draw_entries[expected_bucket + 100]);
            CHECK(op_surface_buckets[expected_bucket] == &op_surface_draw_entries[index]);
            for (byte_index = 0; byte_index < sizeof(surface); ++byte_index)
                if (byte_index != 0x0e) CHECK(bytes[byte_index] == before[byte_index]);
            for (byte_index = 0; byte_index < 32; ++byte_index)
                if (byte_index != expected_bucket) CHECK(op_surface_buckets[byte_index] == &op_surface_draw_entries[byte_index + 100]);
        }
    }
    surface.packed_opacity = 0; surface.opacity = 0; surface.texture_index = 4;
    op_surface_buckets[4] = 0;
    op_gathered_surface_count = 0;
    op_push_surface_draw_entry(&surface);
    CHECK(op_surface_draw_entries[0].next == 0 && op_surface_buckets[4] == &op_surface_draw_entries[0]);
    op_gathered_surface_count = 1; /* The caller, not this helper, reserves the next slot. */
    op_push_surface_draw_entry(&surface);
    CHECK(op_surface_buckets[4] == &op_surface_draw_entries[1]);
    CHECK(op_surface_draw_entries[1].next == &op_surface_draw_entries[0]);
    CHECK(op_gathered_surface_count == 1);
    op_gathered_surface_count = 0;
}
