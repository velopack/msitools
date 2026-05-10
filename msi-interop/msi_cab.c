/*
 * msi_cab.c - Cabinet file operations backed by gcab (libgcab).
 *
 * Implements the functions declared in msi_cab.h, providing cabinet
 * creation, enumeration and extraction for the msi-interop shared
 * library.  All wchar_t* parameters are treated as UTF-16 data (16-bit
 * code units) regardless of platform, matching C# P/Invoke semantics.
 */

#include "msi_cab.h"
#include "string_utils.h"

#include <gcab-cabinet.h>
#include <gcab-folder.h>
#include <gcab-file.h>

#include <gio/gio.h>
#include <glib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

/* ========================================================================= */
/* Internal context for cabinet creation                                     */
/* ========================================================================= */

/* Information about a single file added via CabAddFile. */
typedef struct {
    char  *file_path;   /* UTF-8 path on disk     */
    char  *token;       /* UTF-8 token (name in cab) */
    int64_t file_size;  /* source file size in bytes  */
} CabFileEntry;

typedef struct {
    char        *output_path;       /* UTF-8 full path of output .cab file */
    char        *cab_basename;      /* UTF-8 base name (e.g. "product.cab") */
    char        *output_dir;        /* UTF-8 directory containing output .cab */
    GPtrArray   *files;             /* array of CabFileEntry* (owned) */
    int32_t      compression_level; /* CAB_COMPRESS_xxx value */
    int64_t      max_size_bytes;    /* max cabinet size in bytes (0 = no limit) */

    /* Populated by CabFlush -- arrays of WCHAR* (UTF-16) strings kept alive
     * until CabDestroy so callers can read CabCreatedInfo pointers. */
    GPtrArray   *result_cab_names;  /* WCHAR* cabinet basenames */
    GPtrArray   *result_tokens;     /* WCHAR* first-file tokens */
} CabContext;

/* ========================================================================= */
/* Helpers                                                                   */
/* ========================================================================= */

/*
 * Convert a const wchar_t* (which carries UTF-16 data from C# P/Invoke)
 * to a newly-allocated UTF-8 string.  On Windows wchar_t is 16-bit so a
 * simple cast suffices.  On other platforms wchar_t may be 32-bit but the
 * data from P/Invoke is still 16-bit code units; we cast to WCHAR* (unsigned
 * short*) which is what utf16_to_utf8() expects.
 */
static char *
wchar_to_utf8(const wchar_t *ws)
{
    if (ws == NULL)
        return NULL;
    return utf16_to_utf8((const WCHAR *)ws);
}

/*
 * Extract the base name (file name without directory) from a UTF-8 path.
 * Returns a newly-allocated string.
 */
static char *
path_basename(const char *path)
{
    if (path == NULL)
        return g_strdup("");

    const char *sep = strrchr(path, '/');
#ifdef _WIN32
    const char *bsep = strrchr(path, '\\');
    if (bsep != NULL && (sep == NULL || bsep > sep))
        sep = bsep;
#endif
    if (sep != NULL)
        return g_strdup(sep + 1);
    return g_strdup(path);
}

/*
 * Map CAB_COMPRESS_xxx to GCabCompression.
 */
static GCabCompression
map_compression(int32_t level)
{
    switch (level) {
    case CAB_COMPRESS_NONE:
        return GCAB_COMPRESSION_NONE;
    case CAB_COMPRESS_MSZIP:
        return GCAB_COMPRESSION_MSZIP;
    case CAB_COMPRESS_LOW:
    case CAB_COMPRESS_MEDIUM:
    case CAB_COMPRESS_HIGH:
        /* gcab supports NONE and MSZIP for writing; use MSZIP as a
         * reasonable default for the LZX/Quantum levels WiX may request. */
        return GCAB_COMPRESSION_MSZIP;
    default:
        return GCAB_COMPRESSION_MSZIP;
    }
}

static void
cab_file_entry_free(gpointer data)
{
    CabFileEntry *entry = (CabFileEntry *)data;
    if (entry == NULL) return;
    g_free(entry->file_path);
    g_free(entry->token);
    g_free(entry);
}

/*
 * Extract the directory portion of a UTF-8 path.
 * Returns a newly-allocated string (empty string if no directory).
 */
static char *
path_dirname(const char *path)
{
    if (path == NULL)
        return g_strdup("");

    const char *sep = strrchr(path, '/');
#ifdef _WIN32
    const char *bsep = strrchr(path, '\\');
    if (bsep != NULL && (sep == NULL || bsep > sep))
        sep = bsep;
#endif
    if (sep != NULL)
        return g_strndup(path, (gsize)(sep - path));
    return g_strdup("");
}

/* ========================================================================= */
/* CabCreate                                                                 */
/* ========================================================================= */

MSICAB_API uint32_t MSICAB_CALL
CabCreate(const wchar_t *cab_path,
          int32_t        compression_level,
          int32_t        max_file_count,
          int32_t        max_size_mb,
          int32_t        max_threshold,
          CABHANDLE     *out_handle)
{
    (void)max_file_count;
    (void)max_threshold;

    if (cab_path == NULL || out_handle == NULL)
        return CAB_ERROR_INVALID_PARAM;

    *out_handle = 0;

    char *output_path = wchar_to_utf8(cab_path);
    if (output_path == NULL)
        return CAB_ERROR_INVALID_PARAM;

    CabContext *ctx = g_new0(CabContext, 1);
    ctx->output_path       = output_path;
    ctx->cab_basename      = path_basename(output_path);
    ctx->output_dir        = path_dirname(output_path);
    ctx->files             = g_ptr_array_new_with_free_func(cab_file_entry_free);
    ctx->compression_level = compression_level;
    ctx->max_size_bytes    = (max_size_mb > 0)
                             ? (int64_t)max_size_mb * 1024 * 1024
                             : 0;
    ctx->result_cab_names  = g_ptr_array_new_with_free_func(g_free);
    ctx->result_tokens     = g_ptr_array_new_with_free_func(g_free);

    *out_handle = (CABHANDLE)(uintptr_t)ctx;
    return CAB_SUCCESS;
}

/* ========================================================================= */
/* CabAddFile                                                                */
/* ========================================================================= */

MSICAB_API uint32_t MSICAB_CALL
CabAddFile(CABHANDLE              handle,
           const wchar_t         *file_path,
           const wchar_t         *token,
           const MSIFILEHASHINFO *hash)
{
    (void)hash;

    if (handle == 0 || file_path == NULL || token == NULL)
        return CAB_ERROR_INVALID_PARAM;

    CabContext *ctx = (CabContext *)(uintptr_t)handle;

    char *file_path_utf8 = wchar_to_utf8(file_path);
    char *token_utf8     = wchar_to_utf8(token);
    if (file_path_utf8 == NULL || token_utf8 == NULL) {
        g_free(file_path_utf8);
        g_free(token_utf8);
        return CAB_ERROR_INVALID_PARAM;
    }

    /* Query file size on disk */
    int64_t file_size = 0;
    GFile *gfile = g_file_new_for_path(file_path_utf8);
    GFileInfo *finfo = g_file_query_info(gfile, G_FILE_ATTRIBUTE_STANDARD_SIZE,
                                         G_FILE_QUERY_INFO_NONE, NULL, NULL);
    if (finfo != NULL) {
        file_size = g_file_info_get_size(finfo);
        g_object_unref(finfo);
    }
    g_object_unref(gfile);

    CabFileEntry *entry = g_new0(CabFileEntry, 1);
    entry->file_path = file_path_utf8;
    entry->token     = token_utf8;
    entry->file_size = file_size;

    g_ptr_array_add(ctx->files, entry);

    return CAB_SUCCESS;
}

/* ========================================================================= */
/* CabFlush                                                                  */
/* ========================================================================= */

/*
 * Generate the cabinet file name for split cabinet number `index`.
 *   index 0  -> basename unchanged  (e.g. "test.cab")
 *   index 1  -> append 'a'          (e.g. "testa.cab")
 *   index 2  -> append 'b'          (e.g. "testb.cab")
 *   ...
 *   index 26 -> append 'z'
 *   index 27+ -> append decimal     (e.g. "test27.cab")
 *
 * Returns a newly-allocated UTF-8 base name.
 */
static char *
make_split_cab_name(const char *basename, int index)
{
    if (index == 0)
        return g_strdup(basename);

    /* Find the extension (last '.') */
    const char *dot = strrchr(basename, '.');
    if (dot == NULL) {
        /* No extension -- just append the suffix */
        if (index >= 1 && index <= 26)
            return g_strdup_printf("%s%c", basename, 'a' + (index - 1));
        else
            return g_strdup_printf("%s%d", basename, index);
    }

    /* Split into stem and extension */
    char *stem = g_strndup(basename, (gsize)(dot - basename));
    char *result;

    if (index >= 1 && index <= 26)
        result = g_strdup_printf("%s%c%s", stem, 'a' + (index - 1), dot);
    else
        result = g_strdup_printf("%s%d%s", stem, index, dot);

    g_free(stem);
    return result;
}

/*
 * Write a single cabinet containing files[start_idx..end_idx-1].
 * `cab_path` is the full UTF-8 output path.
 *
 * Returns TRUE on success.
 */
static gboolean
write_single_cabinet(CabContext *ctx,
                     guint       start_idx,
                     guint       end_idx,
                     const char *cab_path,
                     GError    **error)
{
    GCabCabinet *cabinet = gcab_cabinet_new();
    GCabFolder  *folder  = gcab_folder_new(map_compression(ctx->compression_level));

    if (!gcab_cabinet_add_folder(cabinet, folder, error)) {
        g_object_unref(folder);
        g_object_unref(cabinet);
        return FALSE;
    }

    for (guint i = start_idx; i < end_idx; i++) {
        CabFileEntry *entry = g_ptr_array_index(ctx->files, i);
        GFile *gfile = g_file_new_for_path(entry->file_path);
        GCabFile *cabfile = gcab_file_new_with_file(entry->token, gfile);
        g_object_unref(gfile);

        if (cabfile == NULL) {
            g_object_unref(folder);
            g_object_unref(cabinet);
            return FALSE;
        }

        if (!gcab_folder_add_file(folder, cabfile, FALSE, NULL, error)) {
            g_object_unref(cabfile);
            g_object_unref(folder);
            g_object_unref(cabinet);
            return FALSE;
        }
        g_object_unref(cabfile);
    }

    GFile *ofile = g_file_new_for_path(cab_path);
    GOutputStream *out = G_OUTPUT_STREAM(
        g_file_replace(ofile, NULL, FALSE, G_FILE_CREATE_NONE, NULL, error));
    g_object_unref(ofile);

    if (out == NULL) {
        g_object_unref(folder);
        g_object_unref(cabinet);
        return FALSE;
    }

    gboolean ok = gcab_cabinet_write_simple(cabinet, out, NULL, NULL, NULL, error);
    g_output_stream_close(out, NULL, NULL);
    g_object_unref(out);
    g_object_unref(folder);
    g_object_unref(cabinet);

    return ok;
}

MSICAB_API uint32_t MSICAB_CALL
CabFlush(CABHANDLE       handle,
         CabCreatedInfo *out_cabs,
         int32_t         max_cabs,
         int32_t        *out_count)
{
    if (handle == 0)
        return CAB_ERROR_INVALID_PARAM;

    CabContext *ctx = (CabContext *)(uintptr_t)handle;

    if (out_count == NULL)
        return CAB_ERROR_INVALID_PARAM;

    *out_count = 0;

    guint total_files = ctx->files->len;
    if (total_files == 0) {
        /* Create an empty cabinet file (valid cabinet with no files). */
        GError *error = NULL;
        gboolean ok = write_single_cabinet(ctx, 0, 0, ctx->output_path, &error);
        g_clear_error(&error);

        if (!ok)
            return CAB_ERROR_FUNCTION_FAILED;

        /* Report the single empty cabinet */
        WCHAR *cab_name_w = utf8_to_utf16(ctx->cab_basename, NULL);
        WCHAR *empty_token_w = utf8_to_utf16("", NULL);
        g_ptr_array_add(ctx->result_cab_names, cab_name_w);
        g_ptr_array_add(ctx->result_tokens, empty_token_w);

        if (out_cabs != NULL && max_cabs > 0) {
            out_cabs[0].cabinet_name     = (const wchar_t *)g_ptr_array_index(ctx->result_cab_names, 0);
            out_cabs[0].first_file_token = (const wchar_t *)g_ptr_array_index(ctx->result_tokens, 0);
        }
        *out_count = 1;
        return CAB_SUCCESS;
    }

    /* ------------------------------------------------------------------ */
    /* Partition files into cabinets based on max_size_bytes.              */
    /* Each partition is a range [start, end) of file indices.             */
    /* ------------------------------------------------------------------ */
    GArray *partitions = g_array_new(FALSE, FALSE, sizeof(guint));
    guint part_start = 0;
    g_array_append_val(partitions, part_start); /* first partition starts at 0 */

    if (ctx->max_size_bytes > 0) {
        int64_t running_size = 0;
        for (guint i = 0; i < total_files; i++) {
            CabFileEntry *entry = g_ptr_array_index(ctx->files, i);
            int64_t fsize = entry->file_size;

            /* If adding this file would exceed the limit AND the current
             * partition already contains at least one file, start a new
             * partition.  (A single file larger than the limit gets its
             * own cabinet -- we cannot split within a file.) */
            if (running_size > 0 && running_size + fsize > ctx->max_size_bytes) {
                g_array_append_val(partitions, i);
                running_size = 0;
            }
            running_size += fsize;
        }
    }
    /* Sentinel: end of last partition */
    g_array_append_val(partitions, total_files);

    int32_t num_cabs = (int32_t)(partitions->len - 1);

    /* ------------------------------------------------------------------ */
    /* Write each partition as a separate cabinet file.                    */
    /* ------------------------------------------------------------------ */
    for (int32_t ci = 0; ci < num_cabs; ci++) {
        guint pstart = g_array_index(partitions, guint, ci);
        guint pend   = g_array_index(partitions, guint, ci + 1);

        /* Generate the cabinet base name and full path */
        char *cab_name = make_split_cab_name(ctx->cab_basename, ci);
        char *cab_path;
        if (ctx->output_dir[0] != '\0')
            cab_path = g_strdup_printf("%s%c%s", ctx->output_dir,
                                       G_DIR_SEPARATOR, cab_name);
        else
            cab_path = g_strdup(cab_name);

        GError *error = NULL;
        gboolean ok = write_single_cabinet(ctx, pstart, pend, cab_path, &error);
        g_free(cab_path);

        if (!ok) {
            g_free(cab_name);
            g_clear_error(&error);
            g_array_free(partitions, TRUE);
            return CAB_ERROR_FUNCTION_FAILED;
        }

        /* Store result info (kept alive until CabDestroy) */
        WCHAR *cab_name_w = utf8_to_utf16(cab_name, NULL);
        g_free(cab_name);

        CabFileEntry *first_entry = g_ptr_array_index(ctx->files, pstart);
        WCHAR *first_token_w = utf8_to_utf16(first_entry->token, NULL);

        g_ptr_array_add(ctx->result_cab_names, cab_name_w);
        g_ptr_array_add(ctx->result_tokens, first_token_w);
    }

    g_array_free(partitions, TRUE);

    /* ------------------------------------------------------------------ */
    /* Fill in caller's output array.                                      */
    /* ------------------------------------------------------------------ */
    int32_t to_report = num_cabs;
    if (out_cabs == NULL || max_cabs <= 0)
        to_report = 0;
    else if (to_report > max_cabs)
        to_report = max_cabs;

    for (int32_t i = 0; i < to_report; i++) {
        out_cabs[i].cabinet_name     = (const wchar_t *)g_ptr_array_index(
                                           ctx->result_cab_names, i);
        out_cabs[i].first_file_token = (const wchar_t *)g_ptr_array_index(
                                           ctx->result_tokens, i);
    }

    *out_count = num_cabs;

    return CAB_SUCCESS;
}

/* ========================================================================= */
/* CabDestroy                                                                */
/* ========================================================================= */

MSICAB_API void MSICAB_CALL
CabDestroy(CABHANDLE handle)
{
    if (handle == 0)
        return;

    CabContext *ctx = (CabContext *)(uintptr_t)handle;

    g_free(ctx->output_path);
    g_free(ctx->cab_basename);
    g_free(ctx->output_dir);

    if (ctx->files != NULL)
        g_ptr_array_unref(ctx->files);
    if (ctx->result_cab_names != NULL)
        g_ptr_array_unref(ctx->result_cab_names);
    if (ctx->result_tokens != NULL)
        g_ptr_array_unref(ctx->result_tokens);

    g_free(ctx);
}

/* ========================================================================= */
/* CabEnumerate                                                              */
/* ========================================================================= */

MSICAB_API uint32_t MSICAB_CALL
CabEnumerate(const wchar_t  *cab_path,
             CabEnumCallback callback,
             uintptr_t       context)
{
    if (cab_path == NULL || callback == NULL)
        return CAB_ERROR_INVALID_PARAM;

    char *path_utf8 = wchar_to_utf8(cab_path);
    if (path_utf8 == NULL)
        return CAB_ERROR_INVALID_PARAM;

    GFile *gfile = g_file_new_for_path(path_utf8);
    GError *error = NULL;

    GFileInputStream *fis = g_file_read(gfile, NULL, &error);
    g_object_unref(gfile);
    g_free(path_utf8);

    if (fis == NULL) {
        g_clear_error(&error);
        return CAB_ERROR_OPEN_FAILED;
    }

    GCabCabinet *cab = gcab_cabinet_new();
    if (!gcab_cabinet_load(cab, G_INPUT_STREAM(fis), NULL, &error)) {
        g_clear_error(&error);
        g_object_unref(fis);
        g_object_unref(cab);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    g_object_unref(fis);

    GPtrArray *folders = gcab_cabinet_get_folders(cab);
    uint32_t result = CAB_SUCCESS;

    for (guint fi = 0; fi < folders->len; fi++) {
        GCabFolder *folder = g_ptr_array_index(folders, fi);
        GSList *files = gcab_folder_get_files(folder);

        for (GSList *l = files; l != NULL; l = l->next) {
            GCabFile *cabfile = GCAB_FILE(l->data);

            const gchar *name = gcab_file_get_name(cabfile);
            guint32 size = gcab_file_get_size(cabfile);

            /* Get DOS date/time from the GDateTime */
            uint16_t dos_date = 0;
            uint16_t dos_time = 0;
            GDateTime *dt = gcab_file_get_date_time(cabfile);
            if (dt != NULL) {
                int year  = g_date_time_get_year(dt);
                int month = g_date_time_get_month(dt);
                int day   = g_date_time_get_day_of_month(dt);
                int hour  = g_date_time_get_hour(dt);
                int min   = g_date_time_get_minute(dt);
                int sec   = g_date_time_get_second(dt);

                dos_date = (uint16_t)(((year - 1980) << 9) | (month << 5) | day);
                dos_time = (uint16_t)((hour << 11) | (min << 5) | (sec / 2));
                g_date_time_unref(dt);
            }

            /* Convert name to UTF-16 for the callback */
            WCHAR *name_w = utf8_to_utf16(name, NULL);

            CabFileInfo info;
            info.file_id = (const wchar_t *)name_w;
            info.size    = (int32_t)size;
            info.date    = dos_date;
            info.time    = dos_time;

            int32_t cb_result = callback(&info, context);
            g_free(name_w);

            if (cb_result != 0) {
                result = CAB_SUCCESS;
                goto done;
            }
        }
    }

done:
    g_object_unref(cab);
    return result;
}

/* ========================================================================= */
/* CabExtract                                                                */
/* ========================================================================= */

/* Callback context for extraction progress notifications */
typedef struct {
    CabExtractCallback user_callback;
    uintptr_t          user_context;
} ExtractCallbackCtx;

/*
 * gcab file callback invoked for each file during extraction.
 * We forward the notification to the user's CabExtractCallback.
 */
static gboolean
extract_file_cb(GCabFile *cabfile, gpointer user_data)
{
    ExtractCallbackCtx *ecb = (ExtractCallbackCtx *)user_data;
    if (ecb == NULL || ecb->user_callback == NULL)
        return TRUE;

    const gchar *name = gcab_file_get_name(cabfile);
    WCHAR *name_w = utf8_to_utf16(name, NULL);

    int32_t ret = ecb->user_callback((const wchar_t *)name_w, ecb->user_context);
    g_free(name_w);

    /* Return TRUE to continue extraction, FALSE to skip/abort */
    return (ret == 0) ? TRUE : FALSE;
}

MSICAB_API uint32_t MSICAB_CALL
CabExtract(const wchar_t     *cab_path,
           const wchar_t     *output_folder,
           CabExtractCallback callback,
           uintptr_t          context)
{
    if (cab_path == NULL || output_folder == NULL)
        return CAB_ERROR_INVALID_PARAM;

    char *path_utf8   = wchar_to_utf8(cab_path);
    char *outdir_utf8 = wchar_to_utf8(output_folder);
    if (path_utf8 == NULL || outdir_utf8 == NULL) {
        g_free(path_utf8);
        g_free(outdir_utf8);
        return CAB_ERROR_INVALID_PARAM;
    }

    /* Open and load the cabinet */
    GFile *gfile = g_file_new_for_path(path_utf8);
    GError *error = NULL;

    GFileInputStream *fis = g_file_read(gfile, NULL, &error);
    g_object_unref(gfile);
    g_free(path_utf8);

    if (fis == NULL) {
        g_clear_error(&error);
        g_free(outdir_utf8);
        return CAB_ERROR_OPEN_FAILED;
    }

    GCabCabinet *cab = gcab_cabinet_new();
    if (!gcab_cabinet_load(cab, G_INPUT_STREAM(fis), NULL, &error)) {
        g_clear_error(&error);
        g_object_unref(fis);
        g_object_unref(cab);
        g_free(outdir_utf8);
        return CAB_ERROR_FUNCTION_FAILED;
    }
    g_object_unref(fis);

    /* Prepare the output directory */
    GFile *out_gfile = g_file_new_for_path(outdir_utf8);
    g_free(outdir_utf8);

    /* Set up the extract callback bridge */
    ExtractCallbackCtx ecb;
    ecb.user_callback = callback;
    ecb.user_context  = context;

    gboolean ok = gcab_cabinet_extract_simple(
        cab,
        out_gfile,
        callback != NULL ? extract_file_cb : NULL,
        callback != NULL ? &ecb : NULL,
        NULL,
        &error);

    if (!ok) {
        g_object_unref(out_gfile);
        g_object_unref(cab);
        g_clear_error(&error);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    /* --------------------------------------------------------------------- */
    /* Restore file timestamps from the cabinet metadata.                    */
    /* gcab_cabinet_extract_simple does not preserve the stored date/time,   */
    /* so we iterate over every file in the cabinet and set the modification */
    /* time on the corresponding extracted file on disk.                     */
    /* --------------------------------------------------------------------- */
    GPtrArray *folders = gcab_cabinet_get_folders(cab);
    for (guint fi = 0; fi < folders->len; fi++) {
        GCabFolder *folder = g_ptr_array_index(folders, fi);
        GSList *files = gcab_folder_get_files(folder);

        for (GSList *l = files; l != NULL; l = l->next) {
            GCabFile *cabfile = GCAB_FILE(l->data);

            GDateTime *dt = gcab_file_get_date_time(cabfile);
            if (dt == NULL)
                continue;

            /* Use the extract name if set, otherwise the original name */
            const gchar *name = gcab_file_get_extract_name(cabfile);
            if (name == NULL)
                name = gcab_file_get_name(cabfile);

            GFile *extracted = g_file_get_child(out_gfile, name);

            /* Convert GDateTime to seconds since Unix epoch */
            gint64 unix_time = g_date_time_to_unix(dt);

            GFileInfo *finfo = g_file_info_new();
            g_file_info_set_attribute_uint64(
                finfo,
                G_FILE_ATTRIBUTE_TIME_MODIFIED,
                (guint64)unix_time);
            g_file_info_set_attribute_uint32(
                finfo,
                G_FILE_ATTRIBUTE_TIME_MODIFIED_USEC,
                0);
            g_file_info_set_attribute_uint64(
                finfo,
                G_FILE_ATTRIBUTE_TIME_ACCESS,
                (guint64)unix_time);
            g_file_info_set_attribute_uint32(
                finfo,
                G_FILE_ATTRIBUTE_TIME_ACCESS_USEC,
                0);
#ifdef _WIN32
            g_file_info_set_attribute_uint64(
                finfo,
                "time::created",
                (guint64)unix_time);
#endif

            g_file_set_attributes_from_info(
                extracted,
                finfo,
                G_FILE_QUERY_INFO_NONE,
                NULL,
                NULL);

#ifdef _WIN32
            /* GLib may not support writing time::created via attributes.
             * Use the native Windows API to set creation time directly. */
            {
                char *file_path = g_file_get_path(extracted);
                if (file_path) {
                    /* Convert Unix time to Windows FILETIME */
                    static const int64_t EPOCH_DIFF = 116444736000000000LL;
                    int64_t ft = (int64_t)unix_time * 10000000LL + EPOCH_DIFF;
                    FILETIME filetime;
                    filetime.dwLowDateTime = (DWORD)(ft & 0xFFFFFFFF);
                    filetime.dwHighDateTime = (DWORD)(ft >> 32);

                    wchar_t *wpath = (wchar_t *)g_utf8_to_utf16(file_path, -1, NULL, NULL, NULL);
                    if (wpath) {
                        HANDLE hFile = CreateFileW(wpath, FILE_WRITE_ATTRIBUTES,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
                        if (hFile != INVALID_HANDLE_VALUE) {
                            SetFileTime(hFile, &filetime, &filetime, &filetime);
                            CloseHandle(hFile);
                        }
                        g_free(wpath);
                    }
                    g_free(file_path);
                }
            }
#endif

            g_object_unref(finfo);
            g_object_unref(extracted);
            g_date_time_unref(dt);
        }
    }

    g_object_unref(out_gfile);
    g_object_unref(cab);

    return CAB_SUCCESS;
}

/* ========================================================================= */
/* FileTimeToDosDateTimeW                                                    */
/* ========================================================================= */

MSICAB_API int32_t MSICAB_CALL
FileTimeToDosDateTimeW(int64_t   filetime,
                       uint16_t *out_fat_date,
                       uint16_t *out_fat_time)
{
    if (out_fat_date == NULL || out_fat_time == NULL)
        return 0;

    /*
     * Windows FILETIME: 100-nanosecond intervals since 1601-01-01 00:00:00 UTC.
     * Unix epoch:       seconds since 1970-01-01 00:00:00 UTC.
     * Difference:       11644473600 seconds = 116444736000000000 ticks.
     */
    static const int64_t EPOCH_DIFF = 116444736000000000LL;

    if (filetime < EPOCH_DIFF) {
        /* Date before Unix epoch -- clamp to DOS epoch */
        *out_fat_date = (uint16_t)((0 << 9) | (1 << 5) | 1); /* 1980-01-01 */
        *out_fat_time = 0;
        return 1;
    }

    int64_t unix_seconds = (filetime - EPOCH_DIFF) / 10000000LL;

    /* Use GDateTime for thread-safe conversion */
    GDateTime *dt = g_date_time_new_from_unix_utc(unix_seconds);
    if (dt == NULL) {
        *out_fat_date = (uint16_t)((0 << 9) | (1 << 5) | 1);
        *out_fat_time = 0;
        return 1;
    }

    int year  = g_date_time_get_year(dt);
    int month = g_date_time_get_month(dt);
    int day   = g_date_time_get_day_of_month(dt);
    int hour  = g_date_time_get_hour(dt);
    int min   = g_date_time_get_minute(dt);
    int sec   = g_date_time_get_second(dt);
    g_date_time_unref(dt);

    /* DOS dates start at 1980 */
    if (year < 1980) {
        year = 1980;
        month = 1;
        day = 1;
        hour = 0;
        min = 0;
        sec = 0;
    }

    *out_fat_date = (uint16_t)(((year - 1980) << 9) | (month << 5) | day);
    *out_fat_time = (uint16_t)((hour << 11) | (min << 5) | (sec / 2));

    return 1;
}
