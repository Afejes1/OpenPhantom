#ifndef OP_CAMPAIGN_017_WORLD_SKIP_CHUNK_API_H
#define OP_CAMPAIGN_017_WORLD_SKIP_CHUNK_API_H

typedef struct OP_STREAM {
    void *handle;
    int mode;
} OP_STREAM;

int op_stream_seek(void *handle, long offset, int origin);
void *op_allocate(unsigned int bytes);
int op_stream_read(void *destination, unsigned int element_size,
                   unsigned int count, OP_STREAM *stream);
void op_release(void *memory);
int op_world_skip_chunk(OP_STREAM *stream, unsigned int bytes);

#endif
