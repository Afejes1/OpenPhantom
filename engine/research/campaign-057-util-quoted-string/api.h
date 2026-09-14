#ifndef OP_CAMPAIGN057_API_H
#define OP_CAMPAIGN057_API_H
#include <string.h>
typedef int(__cdecl *OP_UTIL_COMPARE)(int, int, void *);
typedef void(__cdecl *OP_UTIL_SWAP)(int, int, void *);
unsigned int op_span_delimiters(const char *, const char *);
char *op_find_delimiter(const char *, const char *);
char *op_find_character(const char *, int);
char *op_copy_keyframe_name(char *, const char *, unsigned int);
char *op_util_tokenize(char *, char *, unsigned int, const char *);
char *op_util_quoted_string(const char *, char *, unsigned int);
void op_util_sort_range(int, int, OP_UTIL_COMPARE, OP_UTIL_SWAP, void *);
#endif
