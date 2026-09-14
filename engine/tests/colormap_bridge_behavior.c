static int cmb_checks, cmb_failures;
static void cmb_check(int ok, int line)
{
    ++cmb_checks;
    if (!ok)
    {
        ++cmb_failures;
        printf("colormap fallback bridge line%d failed\n", line);
    }
}
#define CMB_CHECK(x) cmb_check(!!(x), __LINE__)

/* The real reader's open-failure path invokes gray and returns success.
 * Preserve authored gray-return variation and callback mutations in older chains. */
static const char *cm_legacy_path;
static int cm_legacy_pending;
static OP_CMP_SERVICES cm_legacy_services;
static OP_FILE_HANDLE cm_legacy_open(const char *path, const char *mode)
{
    CMB_CHECK((cc_active && cc_mode == 1) || colormap_active);
    CMB_CHECK(op_colormap_services == &cm_legacy_services && mode == op_colormap_read_mode && path &&
              !cm_legacy_pending);
    cm_legacy_path = path;
    cm_legacy_pending = 1;
    return 0;
}
static int cm_legacy_close(OP_FILE_HANDLE handle)
{
    CMB_CHECK(0);
    (void)handle;
    return 0;
}
static unsigned int cm_legacy_read(OP_FILE_HANDLE handle, void *data, unsigned int bytes)
{
    CMB_CHECK(0);
    (void)handle;
    (void)data;
    (void)bytes;
    return 0;
}
static unsigned int cm_legacy_write(OP_FILE_HANDLE handle, const void *data, unsigned int bytes)
{
    CMB_CHECK(0);
    (void)handle;
    (void)data;
    (void)bytes;
    return 0;
}
static void cm_use_legacy_services(void)
{
    memset(&cm_legacy_services, 0, sizeof(cm_legacy_services));
    cm_legacy_services.open = cm_legacy_open;
    cm_legacy_services.close = cm_legacy_close;
    cm_legacy_services.read = cm_legacy_read;
    cm_legacy_services.write = cm_legacy_write;
    op_colormap_services = &cm_legacy_services;
    cm_legacy_path = 0;
    cm_legacy_pending = 0;
}
static int cm_legacy_gray(OP_COLORMAP *map)
{
    const char *path = cm_legacy_path;
    CMB_CHECK(cm_legacy_pending && path);
    if (!cm_legacy_pending || !path)
        return 0;
    cm_legacy_pending = 0;
    cm_legacy_path = 0;
    if (!path)
        return 0;
    if (cc_active && cc_mode == 1)
        return cc_gray(path, map);
    if (colormap_active)
        return colormap_gray(path, map);
    CMB_CHECK(0);
    return 0;
}

static int op_test_colormap_bridge(void)
{
    CMB_CHECK(!cm_legacy_pending && !cm_legacy_path);
    printf("colormap fallback bridge: %d checks, %d failures\n", cmb_checks, cmb_failures);
    return cmb_failures != 0;
}
#undef CMB_CHECK
