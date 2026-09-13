#ifndef OP_CAMPAIGN_019_WORLD_READ_DIALOG_API_H
#define OP_CAMPAIGN_019_WORLD_READ_DIALOG_API_H
typedef struct OP_STREAM
{
    void *handle;
    int mode;
} OP_STREAM;
typedef struct OP_HEADER
{
    unsigned char before_dialog[0x2f0];
    unsigned int dialog_count;
    unsigned int dialog_payload_size;
    unsigned char tail[0x508];
    unsigned int payload_size;
} OP_HEADER;
typedef struct OP_DIALOG_RECORD
{
    unsigned char opaque[12];
    void *link;
} OP_DIALOG_RECORD;
typedef struct OP_WORLD
{
    unsigned char before_dialog[0xcd4];
    unsigned int dialog_count;
    OP_DIALOG_RECORD *dialog_records;
} OP_WORLD;
int op_stream_read(void *destination, int size, int count, OP_STREAM *stream);
int op_world_read_dialog(OP_STREAM *stream, OP_WORLD *world, OP_HEADER *header);
#endif
