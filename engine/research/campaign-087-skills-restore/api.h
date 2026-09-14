#ifndef OP_SKILLS_STATE_H
#define OP_SKILLS_STATE_H
#include <stddef.h>
#include <string.h>
typedef struct OP_SKILLS_STATE
{
    int words[48];
    int difficulty;
    unsigned char reservedc4[20];
} OP_SKILLS_STATE;
typedef char
    op_skills_layout[(sizeof(OP_SKILLS_STATE) == 0xd8 && offsetof(OP_SKILLS_STATE, difficulty) == 0xc0) ? 1 : -1];
extern OP_SKILLS_STATE op_skills_state;
extern int op_skills_active_index, op_skills_previous_index;
void op_skills_new_game(void);
void op_skills_easier(void);
void op_skills_harder(void);
#endif
#ifndef OP_SAVE_STREAM_H
#define OP_SAVE_STREAM_H
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
int op_save_file_write(const void *, unsigned int, unsigned int, void *);
int op_save_file_read(void *, unsigned int, unsigned int, void *);
int op_stream_seek(void *, long, int);
void op_save_error_begin(void *);
void op_save_error_end(void *);
int op_save_error_notify(int, int, int, const char *, int);
#endif

#ifndef OP_SKILLS_CONTROL_H
#define OP_SKILLS_CONTROL_H
int op_skills_save(int);
int op_skills_restore(int);
int op_skills_effective(void);
#endif
