#ifndef OP_CAMPAIGN050_API_H
#define OP_CAMPAIGN050_API_H
extern void *op_save_output_stream, *op_save_input_stream;
extern unsigned char op_save_error_context[];
extern const char op_save_failure_text[];
int op_save_write(const void *, unsigned int);
int op_save_write_chunk(int, const void *, unsigned int, unsigned short);
int op_save_read(void *, unsigned int);
int op_save_write_raw(const void *, unsigned int);
int op_save_read_raw(void *, unsigned int);
int op_save_seek_restore(long, int);
int op_save_header(int, int, unsigned short);
int op_stream_write(const void *, unsigned int, unsigned int, void *);
int op_stream_read(void *, unsigned int, unsigned int, void *);
int op_stream_seek(void *, long, int);
void op_save_error_begin(void *);
void op_save_error_end(void *);
int op_save_error_notify(int, int, int, const char *, int);
#endif
