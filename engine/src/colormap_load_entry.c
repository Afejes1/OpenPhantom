#include "colormap_io.h"
// FUNCTION: WMAIN 0x0047878c
int op_colormap_load_entry(const char *path, OP_COLORMAP *map)
{
    OP_FILE_HANDLE handle;
    OP_CMP_HEADER file_header;
    handle = op_colormap_services->open(path, op_colormap_read_mode);
    if (!handle)
        goto open_failed;
    op_copy_keyframe_name(map->name, op_resource_basename(path), 31);
    map->name[31] = 0;
    op_colormap_services->read(handle, &file_header, 64);
    map->flags = file_header.flags;
    map->field24 = file_header.field24;
    map->field28 = file_header.field28;
    map->field2c = file_header.field2c;
    if (op_compare_marker(file_header.marker, op_colormap_marker, 4))
        goto invalid_header;
    op_colormap_services->read(handle, map->palette, 768);
    map->handle334 = op_allocate(0x4100);
    if (!map->handle334)
        goto no_memory;
    map->light_table = (unsigned char *)map->handle334;
    if ((unsigned int)map->light_table & 255)
        map->light_table += 256 - ((unsigned int)map->light_table & 255);
    op_colormap_services->read(handle, map->light_table, 0x4000);
    if (map->flags & 1)
    {
        map->handle33c = op_allocate(0x10100);
        if (!map->handle33c)
            goto no_memory;
        map->alpha_table = (unsigned char *)map->handle33c;
        if ((unsigned int)map->alpha_table & 255)
            map->alpha_table += 256 - ((unsigned int)map->alpha_table & 255);
        op_colormap_services->read(handle, map->alpha_table, 0x10000);
    }
    op_colormap_services->close(handle);
    return 1;
open_failed:
    goto fallback;
invalid_header:
    op_colormap_services->close(handle);
    goto fallback;
no_memory:
    op_colormap_services->close(handle);
fallback:
    op_colormap_build_gray(map);
    return 1;
}
