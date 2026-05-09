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

/* ========================================================================= */
/* Internal context for cabinet creation                                     */
/* ========================================================================= */

typedef struct {
    GCabCabinet *cabinet;
    GCabFolder  *folder;
    char        *output_path;       /* UTF-8 full path of output .cab file */
    char        *cab_basename;      /* UTF-8 base name (e.g. "product.cab") */
    WCHAR       *cab_basename_w;    /* UTF-16 base name for CabCreatedInfo  */
    GPtrArray   *file_tokens;       /* array of char* (owned, UTF-8 tokens) */
    WCHAR       *first_token_w;     /* UTF-16 copy of first token added     */
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
    (void)max_size_mb;
    (void)max_threshold;

    if (cab_path == NULL || out_handle == NULL)
        return CAB_ERROR_INVALID_PARAM;

    *out_handle = 0;

    char *output_path = wchar_to_utf8(cab_path);
    if (output_path == NULL)
        return CAB_ERROR_INVALID_PARAM;

    GCabCabinet *cabinet = gcab_cabinet_new();
    if (cabinet == NULL) {
        g_free(output_path);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    GCabFolder *folder = gcab_folder_new(map_compression(compression_level));
    if (folder == NULL) {
        g_object_unref(cabinet);
        g_free(output_path);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    GError *error = NULL;
    if (!gcab_cabinet_add_folder(cabinet, folder, &error)) {
        g_clear_error(&error);
        g_object_unref(folder);
        g_object_unref(cabinet);
        g_free(output_path);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    CabContext *ctx = g_new0(CabContext, 1);
    ctx->cabinet       = cabinet;
    ctx->folder        = folder;
    ctx->output_path   = output_path;
    ctx->cab_basename  = path_basename(output_path);
    ctx->cab_basename_w = utf8_to_utf16(ctx->cab_basename, NULL);
    ctx->file_tokens   = g_ptr_array_new_with_free_func(g_free);
    ctx->first_token_w = NULL;

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

    GFile *gfile = g_file_new_for_path(file_path_utf8);
    GCabFile *cabfile = gcab_file_new_with_file(token_utf8, gfile);
    g_object_unref(gfile);

    if (cabfile == NULL) {
        g_free(file_path_utf8);
        g_free(token_utf8);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    GError *error = NULL;
    if (!gcab_folder_add_file(ctx->folder, cabfile, FALSE, NULL, &error)) {
        g_clear_error(&error);
        g_object_unref(cabfile);
        g_free(file_path_utf8);
        g_free(token_utf8);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    /* Keep track of the first token for CabCreatedInfo */
    if (ctx->file_tokens->len == 0) {
        ctx->first_token_w = utf8_to_utf16(token_utf8, NULL);
    }
    g_ptr_array_add(ctx->file_tokens, g_strdup(token_utf8));

    g_object_unref(cabfile);
    g_free(file_path_utf8);
    g_free(token_utf8);
    return CAB_SUCCESS;
}

/* ========================================================================= */
/* CabFlush                                                                  */
/* ========================================================================= */

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

    /* Create the output file and write the cabinet */
    GFile *ofile = g_file_new_for_path(ctx->output_path);
    GError *error = NULL;

    GOutputStream *out = G_OUTPUT_STREAM(
        g_file_replace(ofile, NULL, FALSE, G_FILE_CREATE_NONE, NULL, &error));
    g_object_unref(ofile);

    if (out == NULL) {
        g_clear_error(&error);
        return CAB_ERROR_OPEN_FAILED;
    }

    gboolean ok = gcab_cabinet_write_simple(
        ctx->cabinet, out, NULL, NULL, NULL, &error);

    /* Close the stream regardless of write success */
    g_output_stream_close(out, NULL, NULL);
    g_object_unref(out);

    if (!ok) {
        g_clear_error(&error);
        return CAB_ERROR_FUNCTION_FAILED;
    }

    /* Fill in the output info */
    if (out_cabs != NULL && max_cabs >= 1) {
        out_cabs[0].cabinet_name     = (const wchar_t *)ctx->cab_basename_w;
        out_cabs[0].first_file_token = (const wchar_t *)ctx->first_token_w;
        *out_count = 1;
    }

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

    if (ctx->folder != NULL)
        g_object_unref(ctx->folder);
    if (ctx->cabinet != NULL)
        g_object_unref(ctx->cabinet);

    g_free(ctx->output_path);
    g_free(ctx->cab_basename);
    g_free(ctx->cab_basename_w);
    g_free(ctx->first_token_w);

    if (ctx->file_tokens != NULL)
        g_ptr_array_unref(ctx->file_tokens);

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

    g_object_unref(out_gfile);
    g_object_unref(cab);

    if (!ok) {
        g_clear_error(&error);
        return CAB_ERROR_FUNCTION_FAILED;
    }

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
