#ifndef OP_STREAM_HELPERS_H
#define OP_STREAM_HELPERS_H
#include <stddef.h>
#include <string.h>
typedef struct OP_STREAM
{
    void *file;
    int codec_enabled;
    unsigned char bit_mask, packing;
    unsigned char reserved0a[2];
    unsigned int bit_accumulator;
    unsigned char reserved10[12];
    int staged_count, staging_offset;
    unsigned char reserved24[8];
    int stage_mode, stage_remaining, stage_count;
    unsigned char staging[0x1000];
    int buffer_limit, buffer_position;
    unsigned char output[0x1000];
    unsigned char work[0x102c];
} OP_STREAM;
typedef struct OP_PACK_STREAM
{
    OP_STREAM base;
    unsigned int window_count, window_position, window_remaining;
    unsigned char reserved3078[0xbff0];
    int enabled;
} OP_PACK_STREAM;
typedef char
    op_stream_layout[(sizeof(OP_STREAM) == 0x306c && offsetof(OP_STREAM, bit_mask) == 8 &&
                      offsetof(OP_STREAM, staging_offset) == 0x20 && offsetof(OP_STREAM, buffer_position) == 0x103c &&
                      offsetof(OP_STREAM, output) == 0x1040 && offsetof(OP_STREAM, work) == 0x2040 &&
                      sizeof(OP_PACK_STREAM) == 0xf06c && offsetof(OP_PACK_STREAM, window_count) == 0x306c &&
                      offsetof(OP_PACK_STREAM, enabled) == 0xf068)
                         ? 1
                         : -1];
void *op_allocate(unsigned int);
void op_release(void *);
int op_close_diagnostic_stream(void *);
OP_STREAM *op_stream_open(void *);
OP_PACK_STREAM *op_pack_stream_open(void *);
void *op_stream_close(OP_STREAM *);
void op_stream_refill(OP_STREAM *);
int op_save_file_read(void *, unsigned int, unsigned int, void *);
int op_stream_read(void *, int, int, OP_STREAM *);
unsigned int op_stream_bits(OP_STREAM *, int);
#endif
