#ifndef OP_CAMPAIGN052_API_H
#define OP_CAMPAIGN052_API_H
#include <stddef.h>
typedef struct OP_DIRECTORY
{
    unsigned int unused0, position;
    char pattern[512];
    int find_handle;
} OP_DIRECTORY;
typedef struct OP_FIND_DATA
{
    unsigned int attributes, time_created, time_accessed, time_written, size;
    char name[260];
} OP_FIND_DATA;
typedef struct OP_DIRECTORY_ENTRY
{
    char name[260];
    unsigned int is_directory, time_written;
} OP_DIRECTORY_ENTRY;
typedef char op_directory_layout[sizeof(OP_DIRECTORY) == 524 && offsetof(OP_DIRECTORY, pattern) == 8 &&
                                         offsetof(OP_DIRECTORY, find_handle) == 520
                                     ? 1
                                     : -1];
typedef char op_find_layout
    [sizeof(OP_FIND_DATA) == 280 && offsetof(OP_FIND_DATA, name) == 20 && sizeof(OP_DIRECTORY_ENTRY) == 268 ? 1 : -1];
extern char op_directory_all_format[], op_directory_extension_format[];
void *op_allocate(unsigned int);
void op_release(void *);
int op_format_sprite_name(char *, char *, ...);
int op_file_find_close(int);
int op_file_find_first(const char *, OP_FIND_DATA *);
int op_file_find_next(int, OP_FIND_DATA *);
OP_DIRECTORY *op_directory_open(char *, int, char *);
void op_directory_close(OP_DIRECTORY *);
void op_directory_rewind(OP_DIRECTORY *);
int op_directory_read(OP_DIRECTORY *, OP_DIRECTORY_ENTRY *);
int op_directory_find_one(char *, int, char *, OP_DIRECTORY_ENTRY *);
#endif
