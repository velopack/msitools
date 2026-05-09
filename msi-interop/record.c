/*
 * record.c - MSI record, handle management, and error functions
 *            wrapping libmsi's GObject API
 */

#include "msi_interop.h"
#include "handle_table.h"
#include "string_utils.h"
#include "libmsi.h"
#include "msipriv.h"
#include <glib.h>
#include <gio/gio.h>
#include <string.h>

/* Field type constants (duplicated from libmsi-record.c, which are file-local) */
#define LIBMSI_FIELD_TYPE_NULL   0
#define LIBMSI_FIELD_TYPE_INT    1
#define LIBMSI_FIELD_TYPE_STR    3
#define LIBMSI_FIELD_TYPE_STREAM 4

/* -------------------------------------------------------------------------- */
/* Helper: convert GError to MSI error code                                   */
/* -------------------------------------------------------------------------- */

static unsigned int
gerror_to_msi(GError *error)
{
    if (!error)
        return ERROR_SUCCESS;

    unsigned int ret;
    switch (error->code) {
    case LIBMSI_RESULT_SUCCESS:
        ret = ERROR_SUCCESS;
        break;
    case LIBMSI_RESULT_ACCESS_DENIED:
        ret = ERROR_ACCESS_DENIED;
        break;
    case LIBMSI_RESULT_INVALID_HANDLE:
        ret = ERROR_INVALID_HANDLE;
        break;
    case LIBMSI_RESULT_NOT_ENOUGH_MEMORY:
        ret = ERROR_GEN_FAILURE;
        break;
    case LIBMSI_RESULT_INVALID_DATA:
        ret = ERROR_INVALID_DATA;
        break;
    case LIBMSI_RESULT_INVALID_PARAMETER:
        ret = ERROR_INVALID_PARAMETER;
        break;
    case LIBMSI_RESULT_OPEN_FAILED:
        ret = ERROR_INSTALL_PACKAGE_OPEN_FAILED;
        break;
    case LIBMSI_RESULT_CALL_NOT_IMPLEMENTED:
        ret = ERROR_CALL_NOT_IMPLEMENTED;
        break;
    case LIBMSI_RESULT_MORE_DATA:
        ret = ERROR_MORE_DATA;
        break;
    case LIBMSI_RESULT_BAD_QUERY_SYNTAX:
        ret = ERROR_BAD_QUERY_SYNTAX;
        break;
    case LIBMSI_RESULT_INVALID_FIELD:
        ret = ERROR_INVALID_FIELD;
        break;
    case LIBMSI_RESULT_FUNCTION_FAILED:
        ret = ERROR_FUNCTION_FAILED;
        break;
    case LIBMSI_RESULT_INVALID_TABLE:
        ret = ERROR_INVALID_TABLE;
        break;
    case LIBMSI_RESULT_INVALID_DATATYPE:
        ret = ERROR_INVALID_DATATYPE;
        break;
    default:
        ret = ERROR_FUNCTION_FAILED;
        break;
    }

    g_error_free(error);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* Thread-local last error record                                             */
/* -------------------------------------------------------------------------- */

#ifdef _WIN32
static __declspec(thread) MSIHANDLE g_last_error_record = 0;
#else
static __thread MSIHANDLE g_last_error_record = 0;
#endif

static void set_last_error_record(MSIHANDLE h)
{
    if (g_last_error_record) {
        handle_table_close(g_last_error_record);
    }
    g_last_error_record = h;
}

/* -------------------------------------------------------------------------- */
/* MsiCloseHandle                                                             */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiCloseHandle(MSIHANDLE hAny)
{
    if (hAny == 0)
        return ERROR_SUCCESS;

    unsigned int r = handle_table_close(hAny);
    return (r == 0) ? ERROR_SUCCESS : ERROR_INVALID_HANDLE;
}

/* -------------------------------------------------------------------------- */
/* MsiCloseAllHandles                                                         */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiCloseAllHandles(void)
{
    return handle_table_close_all();
}

/* -------------------------------------------------------------------------- */
/* MsiCreateRecord                                                            */
/* -------------------------------------------------------------------------- */

MSIHANDLE WINAPI
MsiCreateRecord(UINT cParams)
{
    LibmsiRecord *rec = libmsi_record_new(cParams);
    if (!rec)
        return 0;

    MSIHANDLE handle = handle_table_alloc(G_OBJECT(rec), HANDLE_RECORD);
    g_object_unref(rec); /* handle_table_alloc added its own ref */

    return handle;
}

/* -------------------------------------------------------------------------- */
/* MsiRecordIsNull                                                            */
/* -------------------------------------------------------------------------- */

BOOL WINAPI
MsiRecordIsNull(MSIHANDLE hRecord, UINT iField)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return TRUE;

    return libmsi_record_is_null(rec, iField) ? TRUE : FALSE;
}

/* -------------------------------------------------------------------------- */
/* MsiRecordDataSize                                                          */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordDataSize(MSIHANDLE hRecord, UINT iField)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return 0;

    /* Access the internal structure to determine the field type */
    if (iField > rec->count)
        return 0;

    LibmsiField *field = &rec->fields[iField];

    switch (field->type) {
    case LIBMSI_FIELD_TYPE_NULL:
        return 0;
    case LIBMSI_FIELD_TYPE_INT:
        return sizeof(int);
    case LIBMSI_FIELD_TYPE_STR:
        return field->u.szVal ? (UINT)strlen(field->u.szVal) : 0;
    case LIBMSI_FIELD_TYPE_STREAM:
        if (field->u.stream)
            return (UINT)gsf_input_size(field->u.stream);
        return 0;
    default:
        return 0;
    }
}

/* -------------------------------------------------------------------------- */
/* MsiRecordSetInteger                                                        */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordSetInteger(MSIHANDLE hRecord, UINT iField, int iValue)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gboolean ok = libmsi_record_set_int(rec, iField, iValue);
    return ok ? ERROR_SUCCESS : ERROR_INVALID_FIELD;
}

/* -------------------------------------------------------------------------- */
/* MsiRecordSetStringA / W                                                    */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordSetStringA(MSIHANDLE hRecord, UINT iField, LPCSTR szValue)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gboolean ok = libmsi_record_set_string(rec, iField, szValue ? szValue : "");
    return ok ? ERROR_SUCCESS : ERROR_INVALID_FIELD;
}

UINT WINAPI
MsiRecordSetStringW(MSIHANDLE hRecord, UINT iField, LPCWSTR szValue)
{
    if (!szValue)
        return MsiRecordSetStringA(hRecord, iField, NULL);

    char *utf8 = utf16_to_utf8((const WCHAR *)szValue);
    if (!utf8)
        return ERROR_INVALID_PARAMETER;

    UINT ret = MsiRecordSetStringA(hRecord, iField, utf8);
    g_free(utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiRecordGetInteger                                                        */
/* -------------------------------------------------------------------------- */

int WINAPI
MsiRecordGetInteger(MSIHANDLE hRecord, UINT iField)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return MSI_NULL_INTEGER;

    return libmsi_record_get_int(rec, iField);
}

/* -------------------------------------------------------------------------- */
/* MsiRecordGetStringA / W                                                    */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordGetStringA(MSIHANDLE hRecord, UINT iField, LPSTR szValueBuf, LPDWORD pcchValueBuf)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gchar *str = libmsi_record_get_string(rec, iField);
    if (!str)
        str = g_strdup("");

    UINT ret = copy_string_to_bufA(str, szValueBuf, pcchValueBuf);
    g_free(str);
    return ret;
}

UINT WINAPI
MsiRecordGetStringW(MSIHANDLE hRecord, UINT iField, LPWSTR szValueBuf, LPDWORD pcchValueBuf)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gchar *str = libmsi_record_get_string(rec, iField);
    if (!str)
        str = g_strdup("");

    UINT ret = copy_string_to_bufW(str, szValueBuf, pcchValueBuf);
    g_free(str);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiRecordGetFieldCount                                                     */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordGetFieldCount(MSIHANDLE hRecord)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return (UINT)-1;

    return libmsi_record_get_field_count(rec);
}

/* -------------------------------------------------------------------------- */
/* MsiRecordSetStreamA / W                                                    */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordSetStreamA(MSIHANDLE hRecord, UINT iField, LPCSTR szFilePath)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gboolean ok = libmsi_record_load_stream(rec, iField, szFilePath);
    return ok ? ERROR_SUCCESS : ERROR_FUNCTION_FAILED;
}

UINT WINAPI
MsiRecordSetStreamW(MSIHANDLE hRecord, UINT iField, LPCWSTR szFilePath)
{
    if (!szFilePath)
        return MsiRecordSetStreamA(hRecord, iField, NULL);

    char *utf8 = utf16_to_utf8((const WCHAR *)szFilePath);
    if (!utf8)
        return ERROR_INVALID_PARAMETER;

    UINT ret = MsiRecordSetStreamA(hRecord, iField, utf8);
    g_free(utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiRecordReadStream                                                        */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordReadStream(MSIHANDLE hRecord, UINT iField, char *szDataBuf, LPDWORD pcbDataBuf)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    if (!pcbDataBuf)
        return ERROR_INVALID_PARAMETER;

    GInputStream *stream = libmsi_record_get_stream(rec, iField);
    if (!stream)
        return ERROR_INVALID_FIELD;

    if (!szDataBuf) {
        /*
         * Caller is querying the remaining size.
         * The GInputStream wraps a GsfInput -- we can't easily get its total
         * remaining size from GInputStream alone, so we use the internal
         * GsfInput directly for sizing.
         */
        if (iField <= rec->count &&
            rec->fields[iField].type == LIBMSI_FIELD_TYPE_STREAM &&
            rec->fields[iField].u.stream) {
            GsfInput *gsf = rec->fields[iField].u.stream;
            gsf_off_t remaining = gsf_input_remaining(gsf);
            *pcbDataBuf = (DWORD)remaining;
        } else {
            *pcbDataBuf = 0;
        }
        g_object_unref(stream);
        return ERROR_SUCCESS;
    }

    GError *error = NULL;
    gssize bytes_read = g_input_stream_read(stream, szDataBuf, *pcbDataBuf,
                                            NULL, &error);
    g_object_unref(stream);

    if (bytes_read < 0) {
        if (error)
            g_error_free(error);
        *pcbDataBuf = 0;
        return ERROR_FUNCTION_FAILED;
    }

    *pcbDataBuf = (DWORD)bytes_read;
    return ERROR_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/* MsiRecordClearData                                                         */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiRecordClearData(MSIHANDLE hRecord)
{
    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gboolean ok = libmsi_record_clear(rec);
    return ok ? ERROR_SUCCESS : ERROR_FUNCTION_FAILED;
}

/* -------------------------------------------------------------------------- */
/* MsiGetLastErrorRecord                                                      */
/* -------------------------------------------------------------------------- */

MSIHANDLE WINAPI
MsiGetLastErrorRecord(void)
{
    MSIHANDLE h = g_last_error_record;
    g_last_error_record = 0;  /* caller takes ownership */
    return h;
}

/* -------------------------------------------------------------------------- */
/* MsiFormatRecord - basic implementation                                     */
/* -------------------------------------------------------------------------- */

/*
 * Simple [N] placeholder substitution.  Handles the common case where
 * hInstall == 0 (no active install session).
 *
 * - If the format string (field 0) is empty/NULL, concatenate all field
 *   values separated by nothing (matching Windows behaviour).
 * - Otherwise, replace [1], [2], ... [99] with the corresponding field values.
 */
static gchar *
format_record_impl(LibmsiRecord *rec)
{
    guint count = libmsi_record_get_field_count(rec);
    gchar *fmt = libmsi_record_get_string(rec, 0);

    /* No format string -- concatenate all fields */
    if (!fmt || *fmt == '\0') {
        g_free(fmt);
        GString *result = g_string_new(NULL);
        for (guint i = 1; i <= count; i++) {
            gchar *val = libmsi_record_get_string(rec, i);
            if (val) {
                g_string_append(result, val);
                g_free(val);
            }
        }
        return g_string_free(result, FALSE);
    }

    /*
     * Walk the format string and replace [N] tokens.
     * We support field indices 0..99.
     */
    GString *result = g_string_new(NULL);
    const char *p = fmt;

    while (*p) {
        if (*p == '[') {
            /* Try to parse a field index */
            const char *start = p + 1;
            const char *end = start;
            while (*end >= '0' && *end <= '9')
                end++;

            if (end > start && *end == ']') {
                /* Valid [N] token */
                char idx_buf[8];
                size_t idx_len = (size_t)(end - start);
                if (idx_len < sizeof(idx_buf)) {
                    memcpy(idx_buf, start, idx_len);
                    idx_buf[idx_len] = '\0';
                    unsigned int idx = (unsigned int)atoi(idx_buf);

                    if (idx <= count) {
                        gchar *val = libmsi_record_get_string(rec, idx);
                        if (val) {
                            g_string_append(result, val);
                            g_free(val);
                        }
                    }
                    /* else: index out of range, replace with empty */

                    p = end + 1;  /* skip past ']' */
                    continue;
                }
            }
        }

        g_string_append_c(result, *p);
        p++;
    }

    g_free(fmt);
    return g_string_free(result, FALSE);
}

UINT WINAPI
MsiFormatRecordA(MSIHANDLE hInstall, MSIHANDLE hRecord, LPSTR szResultBuf, LPDWORD pcchResultBuf)
{
    (void)hInstall;  /* install session not supported in this interop layer */

    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gchar *formatted = format_record_impl(rec);
    if (!formatted)
        formatted = g_strdup("");

    UINT ret = copy_string_to_bufA(formatted, szResultBuf, pcchResultBuf);
    g_free(formatted);
    return ret;
}

UINT WINAPI
MsiFormatRecordW(MSIHANDLE hInstall, MSIHANDLE hRecord, LPWSTR szResultBuf, LPDWORD pcchResultBuf)
{
    (void)hInstall;

    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec)
        return ERROR_INVALID_HANDLE;

    gchar *formatted = format_record_impl(rec);
    if (!formatted)
        formatted = g_strdup("");

    UINT ret = copy_string_to_bufW(formatted, szResultBuf, pcchResultBuf);
    g_free(formatted);
    return ret;
}
