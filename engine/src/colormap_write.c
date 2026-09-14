#include "colormap_io.h"
#include <string.h>
#pragma intrinsic(memset)
// FUNCTION: WMAIN 0x00478a56
int op_colormap_write(const char *path, OP_COLORMAP *map)
{
    OP_FILE_HANDLE handle;
    OP_CMP_HEADER file_header;
    op_copy_keyframe_name(file_header.marker, op_colormap_marker, 4);
    file_header.version = 30;
    file_header.flags = map->flags;
    file_header.field24 = map->field24;
    file_header.field28 = map->field28;
    file_header.field2c = map->field2c;
    memset(file_header.reserved, 0, 40);
    handle = op_colormap_services->open(path, op_colormap_write_mode);
    if (!handle)
        goto failed;
    op_colormap_services->write(handle, &file_header, 64);
    op_colormap_services->write(handle, map->palette, 768);
    op_colormap_services->write(handle, map->light_table, 0x4000);
    if (map->flags & 1)
        op_colormap_services->write(handle, map->alpha_table, 0x10000);
    op_colormap_services->close(handle);
    return 1;
failed:
    return 0;
}
