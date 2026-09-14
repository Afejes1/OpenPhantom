#ifndef OP_UTILITIES_H
#define OP_UTILITIES_H
#include <stddef.h>
#include <stdarg.h>
#include "conf_stream.h"
typedef struct OP_INT_RECT
{
    int x, y, width, height;
} OP_INT_RECT;
typedef char rect_layout[(sizeof(OP_INT_RECT) == 16 && offsetof(OP_INT_RECT, y) == 4 &&
                          offsetof(OP_INT_RECT, width) == 8 && offsetof(OP_INT_RECT, height) == 12)
                             ? 1
                             : -1];
int op_rect_overlap_inclusive(const OP_INT_RECT *, const OP_INT_RECT *);
int op_rect_overlap_strict(const OP_INT_RECT *, const OP_INT_RECT *);
void op_rect_union(OP_INT_RECT *, const OP_INT_RECT *);
void *op_allocate(unsigned int);
int op_vsnprintf(char *, unsigned int, const char *, va_list);
int op_tolower(int);
char *op_util_strdup(const char *);
void op_util_snprintf(char *, unsigned int, const char *, ...);
void op_util_strlwr(char *);
int op_util_file_exists(const char *);
typedef int(__cdecl *OP_UTIL_COMPARE)(int, int, void *);
typedef void(__cdecl *OP_UTIL_SWAP)(int, int, void *);
unsigned int op_span_delimiters(const char *, const char *);
char *op_find_delimiter(const char *, const char *);
char *op_find_character(const char *, int);
char *op_util_tokenize(char *, char *, unsigned int, const char *);
char *op_util_quoted_string(const char *, char *, unsigned int);
void op_util_sort_range(int, int, OP_UTIL_COMPARE, OP_UTIL_SWAP, void *);
#endif
