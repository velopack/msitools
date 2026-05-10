/*
 * summary.c - MSI summary information functions wrapping libmsi's GObject API
 */

#include "msi_interop.h"
#include "handle_table.h"
#include "string_utils.h"
#include "libmsi.h"
#include "msipriv.h"
#include <glib.h>
#include <string.h>

/* -------------------------------------------------------------------------- */
/* VT_ type constants (OLE/COM variant types used by MSI summary info)        */
/* -------------------------------------------------------------------------- */

#ifndef VT_EMPTY
#define VT_EMPTY    0
#endif
#ifndef VT_NULL
#define VT_NULL     1
#endif
#ifndef VT_I2
#define VT_I2       2
#endif
#ifndef VT_I4
#define VT_I4       3
#endif
#ifndef VT_LPSTR
#define VT_LPSTR   30
#endif
#ifndef VT_FILETIME
#define VT_FILETIME 64
#endif

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
    case LIBMSI_RESULT_UNKNOWN_PROPERTY:
        ret = ERROR_UNKNOWN_PROPERTY;
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
    case LIBMSI_RESULT_DATATYPE_MISMATCH:
        ret = ERROR_DATATYPE_MISMATCH;
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
/* Helper: map LibmsiPropertyType to VT_ constant                            */
/* -------------------------------------------------------------------------- */

static UINT
property_type_to_vt(LibmsiPropertyType pt)
{
    switch (pt) {
    case LIBMSI_PROPERTY_TYPE_INT:      return VT_I4;
    case LIBMSI_PROPERTY_TYPE_STRING:   return VT_LPSTR;
    case LIBMSI_PROPERTY_TYPE_FILETIME: return VT_FILETIME;
    case LIBMSI_PROPERTY_TYPE_EMPTY:
    default:                            return VT_EMPTY;
    }
}

/* -------------------------------------------------------------------------- */
/* MsiGetSummaryInformation                                                   */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiGetSummaryInformationA(MSIHANDLE hDatabase, LPCSTR szDatabasePath,
                          UINT uiUpdateCount, MSIHANDLE *phSummaryInfo)
{
    if (!phSummaryInfo)
        return ERROR_INVALID_PARAMETER;

    if (hDatabase == 0 && !szDatabasePath)
        return ERROR_INVALID_PARAMETER;

    LibmsiDatabase *db = NULL;
    gboolean opened_db = FALSE;

    if (hDatabase != 0) {
        db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
        if (!db)
            return ERROR_INVALID_HANDLE;
        opened_db = FALSE;
    } else {
        /* Open a temporary database from the given path */
        libmsi_global_lock();
        GError *error = NULL;
        guint flags = (uiUpdateCount > 0) ? LIBMSI_DB_FLAGS_TRANSACT
                                          : LIBMSI_DB_FLAGS_READONLY;
        db = libmsi_database_new(szDatabasePath, flags, NULL, &error);
        if (!db) {
            libmsi_global_unlock();
            return gerror_to_msi(error);
        }
        opened_db = TRUE;
        /* Lock is still held -- will be released below after summary info creation */
    }

    if (!opened_db)
        libmsi_global_lock();

    GError *error = NULL;
    LibmsiSummaryInfo *si = libmsi_summary_info_new(db, uiUpdateCount, &error);

    /* Always unref db: either we opened it (opened_db) or handle_table_get_typed added a ref */
    g_object_unref(db);

    if (!si) {
        libmsi_global_unlock();
        return gerror_to_msi(error);
    }

    MSIHANDLE handle = handle_table_alloc(G_OBJECT(si), HANDLE_SUMMARY_INFO);
    g_object_unref(si); /* handle_table_alloc added its own ref */

    libmsi_global_unlock();

    if (handle == 0)
        return ERROR_GEN_FAILURE;

    *phSummaryInfo = handle;
    return ERROR_SUCCESS;
}

UINT WINAPI
MsiGetSummaryInformationW(MSIHANDLE hDatabase, LPCWSTR szDatabasePath,
                          UINT uiUpdateCount, MSIHANDLE *phSummaryInfo)
{
    if (!phSummaryInfo)
        return ERROR_INVALID_PARAMETER;

    char *path_utf8 = NULL;
    if (szDatabasePath) {
        path_utf8 = utf16_to_utf8((const WCHAR *)szDatabasePath);
        if (!path_utf8)
            return ERROR_INVALID_PARAMETER;
    }

    UINT ret = MsiGetSummaryInformationA(hDatabase, path_utf8,
                                         uiUpdateCount, phSummaryInfo);
    g_free(path_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiSummaryInfoGetPropertyCount                                             */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiSummaryInfoGetPropertyCount(MSIHANDLE hSummaryInfo, PUINT puiPropertyCount)
{
    if (!puiPropertyCount)
        return ERROR_INVALID_PARAMETER;

    LibmsiSummaryInfo *si =
        (LibmsiSummaryInfo *)handle_table_get_typed(hSummaryInfo,
                                                     HANDLE_SUMMARY_INFO);
    if (!si)
        return ERROR_INVALID_HANDLE;

    libmsi_global_lock();

    GArray *props = libmsi_summary_info_get_properties(si);
    g_object_unref(si);

    libmsi_global_unlock();

    if (props) {
        *puiPropertyCount = props->len;
        g_array_unref(props);
    } else {
        *puiPropertyCount = 0;
    }

    return ERROR_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/* MsiSummaryInfoSetProperty                                                  */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiSummaryInfoSetPropertyA(MSIHANDLE hSummaryInfo, UINT uiProperty,
                           UINT uiDataType, int iValue, FILETIME *pftValue,
                           LPCSTR szValue)
{
    LibmsiSummaryInfo *si =
        (LibmsiSummaryInfo *)handle_table_get_typed(hSummaryInfo,
                                                     HANDLE_SUMMARY_INFO);
    if (!si)
        return ERROR_INVALID_HANDLE;

    libmsi_global_lock();

    GError *error = NULL;
    gboolean ok = FALSE;
    LibmsiProperty prop = (LibmsiProperty)uiProperty;

    switch (uiDataType) {
    case VT_I4:
        ok = libmsi_summary_info_set_int(si, prop, iValue, &error);
        break;

    case VT_LPSTR:
        ok = libmsi_summary_info_set_string(si, prop,
                                            szValue ? szValue : "", &error);
        break;

    case VT_FILETIME:
        if (!pftValue) {
            g_object_unref(si);
            libmsi_global_unlock();
            return ERROR_INVALID_PARAMETER;
        }
        {
            guint64 ft = ((guint64)pftValue->dwHighDateTime << 32) |
                         (guint64)pftValue->dwLowDateTime;
            ok = libmsi_summary_info_set_filetime(si, prop, ft, &error);
        }
        break;

    case VT_EMPTY:
        {
            LibmsiPropertyType pt =
                libmsi_summary_info_get_property_type(si, prop, NULL);
            switch (pt) {
            case LIBMSI_PROPERTY_TYPE_STRING:
                ok = libmsi_summary_info_set_string(si, prop, "", &error);
                break;
            case LIBMSI_PROPERTY_TYPE_FILETIME:
                ok = libmsi_summary_info_set_filetime(si, prop, 0, &error);
                break;
            case LIBMSI_PROPERTY_TYPE_INT:
            default:
                ok = libmsi_summary_info_set_int(si, prop, 0, &error);
                break;
            }
        }
        break;

    default:
        g_object_unref(si);
        libmsi_global_unlock();
        return ERROR_INVALID_DATATYPE;
    }

    g_object_unref(si);

    libmsi_global_unlock();

    if (!ok)
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

UINT WINAPI
MsiSummaryInfoSetPropertyW(MSIHANDLE hSummaryInfo, UINT uiProperty,
                           UINT uiDataType, int iValue, FILETIME *pftValue,
                           LPCWSTR szValue)
{
    char *value_utf8 = NULL;

    if (szValue && uiDataType == VT_LPSTR) {
        value_utf8 = utf16_to_utf8((const WCHAR *)szValue);
        if (!value_utf8)
            return ERROR_INVALID_PARAMETER;
    }

    UINT ret = MsiSummaryInfoSetPropertyA(hSummaryInfo, uiProperty, uiDataType,
                                          iValue, pftValue, value_utf8);
    g_free(value_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiSummaryInfoGetProperty                                                  */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiSummaryInfoGetPropertyA(MSIHANDLE hSummaryInfo, UINT uiProperty,
                           PUINT puiDataType, LPINT piValue,
                           FILETIME *pftValue, LPSTR szValueBuf,
                           LPDWORD pcchValueBuf)
{
    LibmsiSummaryInfo *si =
        (LibmsiSummaryInfo *)handle_table_get_typed(hSummaryInfo,
                                                     HANDLE_SUMMARY_INFO);
    if (!si)
        return ERROR_INVALID_HANDLE;

    LibmsiProperty prop = (LibmsiProperty)uiProperty;

    /* Initialize output parameters to safe defaults */
    if (piValue)
        *piValue = 0;
    if (pftValue) {
        pftValue->dwLowDateTime = 0;
        pftValue->dwHighDateTime = 0;
    }

    libmsi_global_lock();

    GError *error = NULL;
    LibmsiPropertyType pt = libmsi_summary_info_get_property_type(si, prop,
                                                                   &error);
    if (error) {
        g_object_unref(si);
        libmsi_global_unlock();
        return gerror_to_msi(error);
    }

    UINT vt = property_type_to_vt(pt);
    if (puiDataType)
        *puiDataType = vt;

    UINT ret = ERROR_SUCCESS;

    switch (vt) {
    case VT_I4:
        if (piValue) {
            error = NULL;
            *piValue = libmsi_summary_info_get_int(si, prop, &error);
            if (error) {
                g_object_unref(si);
                libmsi_global_unlock();
                return gerror_to_msi(error);
            }
        }
        break;

    case VT_LPSTR:
        {
            error = NULL;
            const gchar *str = libmsi_summary_info_get_string(si, prop,
                                                               &error);
            if (error) {
                g_object_unref(si);
                libmsi_global_unlock();
                return gerror_to_msi(error);
            }

            ret = copy_string_to_bufA(str ? str : "", szValueBuf, pcchValueBuf);
        }
        break;

    case VT_FILETIME:
        if (pftValue) {
            error = NULL;
            guint64 ft = libmsi_summary_info_get_filetime(si, prop, &error);
            if (error) {
                g_object_unref(si);
                libmsi_global_unlock();
                return gerror_to_msi(error);
            }
            pftValue->dwHighDateTime = (DWORD)(ft >> 32);
            pftValue->dwLowDateTime = (DWORD)(ft & 0xFFFFFFFF);
        }
        break;

    case VT_EMPTY:
    default:
        break;
    }

    g_object_unref(si);
    libmsi_global_unlock();

    return ret;
}

UINT WINAPI
MsiSummaryInfoGetPropertyW(MSIHANDLE hSummaryInfo, UINT uiProperty,
                           PUINT puiDataType, LPINT piValue,
                           FILETIME *pftValue, LPWSTR szValueBuf,
                           LPDWORD pcchValueBuf)
{
    LibmsiSummaryInfo *si =
        (LibmsiSummaryInfo *)handle_table_get_typed(hSummaryInfo,
                                                     HANDLE_SUMMARY_INFO);
    if (!si)
        return ERROR_INVALID_HANDLE;

    LibmsiProperty prop = (LibmsiProperty)uiProperty;

    /* Initialize output parameters to safe defaults */
    if (piValue)
        *piValue = 0;
    if (pftValue) {
        pftValue->dwLowDateTime = 0;
        pftValue->dwHighDateTime = 0;
    }

    libmsi_global_lock();

    GError *error = NULL;
    LibmsiPropertyType pt = libmsi_summary_info_get_property_type(si, prop,
                                                                   &error);
    if (error) {
        g_object_unref(si);
        libmsi_global_unlock();
        return gerror_to_msi(error);
    }

    UINT vt = property_type_to_vt(pt);
    if (puiDataType)
        *puiDataType = vt;

    UINT ret = ERROR_SUCCESS;

    switch (vt) {
    case VT_I4:
        if (piValue) {
            error = NULL;
            *piValue = libmsi_summary_info_get_int(si, prop, &error);
            if (error) {
                g_object_unref(si);
                libmsi_global_unlock();
                return gerror_to_msi(error);
            }
        }
        break;

    case VT_LPSTR:
        {
            error = NULL;
            const gchar *str = libmsi_summary_info_get_string(si, prop,
                                                               &error);
            if (error) {
                g_object_unref(si);
                libmsi_global_unlock();
                return gerror_to_msi(error);
            }

            ret = copy_string_to_bufW(str ? str : "", (WCHAR *)szValueBuf,
                                      pcchValueBuf);
        }
        break;

    case VT_FILETIME:
        if (pftValue) {
            error = NULL;
            guint64 ft = libmsi_summary_info_get_filetime(si, prop, &error);
            if (error) {
                g_object_unref(si);
                libmsi_global_unlock();
                return gerror_to_msi(error);
            }
            pftValue->dwHighDateTime = (DWORD)(ft >> 32);
            pftValue->dwLowDateTime = (DWORD)(ft & 0xFFFFFFFF);
        }
        break;

    case VT_EMPTY:
    default:
        break;
    }

    g_object_unref(si);
    libmsi_global_unlock();

    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiSummaryInfoPersist                                                      */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiSummaryInfoPersist(MSIHANDLE hSummaryInfo)
{
    LibmsiSummaryInfo *si =
        (LibmsiSummaryInfo *)handle_table_get_typed(hSummaryInfo,
                                                     HANDLE_SUMMARY_INFO);
    if (!si)
        return ERROR_INVALID_HANDLE;

    libmsi_global_lock();

    GError *error = NULL;
    gboolean ok = libmsi_summary_info_persist(si, &error);
    g_object_unref(si);

    libmsi_global_unlock();

    if (!ok)
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}
