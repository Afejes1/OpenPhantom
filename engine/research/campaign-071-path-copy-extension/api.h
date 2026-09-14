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
void op_path_copy_extension(const char *, char *);
void op_path_copy_stem(const char *, char *);
void op_path_copy_directory(const char *, char *);
void op_path_copy_drive_directory(const char *, char *);
char *op_copy_keyframe_name(char *, const char *, unsigned int);
#endif
