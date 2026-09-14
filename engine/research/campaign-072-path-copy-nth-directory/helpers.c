#include "api.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0046f82f
char op_path_drive(const char *path)
{
    int length;
    length = strlen(path);
    if (length > 1 && path[1] == ':')
        return (char)op_toupper(path[0]);
    return 0;
}

#include "api.h"
// FUNCTION: WMAIN 0x0046fa53
int op_path_first_directory_offset(const char *path)
{
    const char *cursor = path;
    while (*cursor) {
        if (*cursor == '\\') {
            ++cursor;
            continue;
        }
        if (cursor[1] == ':') {
            cursor += 2;
            continue;
        }
        if (op_path_separator_offset(cursor) == -1)
            break;
        return cursor - path;
    }
    return -1;
}

#include "api.h"
#pragma intrinsic(strlen)
// FUNCTION: WMAIN 0x0046fab6
int op_path_file_offset(const char *path)
{
    const char *cursor;
    int length;
    length = strlen(path);
    if (length) {
        cursor = path + length - 1;
        while (cursor >= path) {
            if (*cursor == '\\')
                break;
            if (*cursor == ':')
                break;
            --cursor;
        }
        ++cursor;
        if (*cursor)
            return cursor - path;
    }
    return -1;
}

#include "api.h"
// FUNCTION: WMAIN 0x0046fb32
int op_path_extension_offset(const char *path)
{
    const char *cursor;
    int offset;
    offset = op_path_file_offset(path);
    if (offset != -1) {
        cursor = path + offset;
        while (*cursor) {
            if (*cursor == '.') {
                ++cursor;
                if (*cursor)
                    return cursor - path;
            }
            ++cursor;
        }
    }
    return -1;
}

#include "api.h"
// FUNCTION: WMAIN 0x0046fb98
int op_path_separator_offset(const char *path)
{
    const char *found;
    found = op_find_character(path, '\\');
    if (!found)
        return -1;
    return found - path;
}

#include "api.h"
// FUNCTION: WMAIN 0x0046f9a8
int op_path_directory_offset(const char *source, int index)
{
    const char *cursor;
    int offset;
    offset = op_path_first_directory_offset(source);
    if(offset == -1) goto missing;
    cursor = source + offset;
    while(--index != 0) {
        for(;;) {
            if(!*cursor) goto missing;
            if(*cursor == 92) break;
            ++cursor;
        }
        ++cursor;
        while(*cursor == 92) ++cursor;
        if(!*cursor) goto missing;
    }
    if(op_path_separator_offset(cursor) == -1) goto missing;
    return cursor - source;
missing:
    return -1;
}
