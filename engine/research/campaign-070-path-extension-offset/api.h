#ifndef OP_PATH_HELPERS_H
#define OP_PATH_HELPERS_H
#include <string.h>
char op_path_drive(const char *);
int op_path_first_directory_offset(const char *);
int op_path_file_offset(const char *);
int op_path_extension_offset(const char *);
int op_path_separator_offset(const char *);
int op_toupper(int);
char *op_find_character(const char *, int);
#endif
