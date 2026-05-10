/*
 * query.c - MSI view/query functions wrapping libmsi's GObject API
 *
 * Implements the Windows Installer MsiDatabaseOpenView, MsiViewExecute,
 * MsiViewFetch, MsiViewModify, MsiViewGetColumnInfo, MsiViewClose,
 * and MsiViewGetError functions.
 */

#include "msi_interop.h"
#include "handle_table.h"
#include "string_utils.h"
#include "libmsi.h"
#include "msipriv.h"
#include <glib.h>
#include <string.h>

/* Convert a GError from libmsi into a Windows MSI error code, freeing the GError. */
static unsigned int gerror_to_msi(GError *error)
{
    if (!error) return ERROR_SUCCESS;
    unsigned int ret;
    switch (error->code) {
        case LIBMSI_RESULT_SUCCESS:          ret = ERROR_SUCCESS; break;
        case LIBMSI_RESULT_ACCESS_DENIED:    ret = ERROR_ACCESS_DENIED; break;
        case LIBMSI_RESULT_INVALID_HANDLE:   ret = ERROR_INVALID_HANDLE; break;
        case LIBMSI_RESULT_INVALID_PARAMETER:ret = ERROR_INVALID_PARAMETER; break;
        case LIBMSI_RESULT_INVALID_DATA:     ret = ERROR_INVALID_DATA; break;
        case LIBMSI_RESULT_MORE_DATA:        ret = ERROR_MORE_DATA; break;
        case LIBMSI_RESULT_BAD_QUERY_SYNTAX: ret = ERROR_BAD_QUERY_SYNTAX; break;
        case LIBMSI_RESULT_INVALID_FIELD:    ret = ERROR_INVALID_FIELD; break;
        case LIBMSI_RESULT_FUNCTION_FAILED:  ret = ERROR_FUNCTION_FAILED; break;
        case LIBMSI_RESULT_INVALID_TABLE:    ret = ERROR_INVALID_TABLE; break;
        case LIBMSI_RESULT_INVALID_DATATYPE: ret = ERROR_INVALID_DATATYPE; break;
        default:                             ret = ERROR_FUNCTION_FAILED; break;
    }
    g_error_free(error);
    return ret;
}

/* Convert a LibmsiDBError enum to MSIDBERROR enum. */
static MSIDBERROR libmsi_dberror_to_msi(LibmsiDBError dberr)
{
    switch (dberr) {
        case LIBMSI_DB_ERROR_SUCCESS:          return MSIDBERROR_NOERROR;
        case LIBMSI_DB_ERROR_INVALIDARG:       return MSIDBERROR_INVALIDARG;
        case LIBMSI_DB_ERROR_MOREDATA:         return MSIDBERROR_MOREDATA;
        case LIBMSI_DB_ERROR_FUNCTIONERROR:    return MSIDBERROR_FUNCTIONERROR;
        case LIBMSI_DB_ERROR_DUPLICATEKEY:     return MSIDBERROR_DUPLICATEKEY;
        case LIBMSI_DB_ERROR_REQUIRED:         return MSIDBERROR_REQUIRED;
        case LIBMSI_DB_ERROR_BADLINK:          return MSIDBERROR_BADLINK;
        case LIBMSI_DB_ERROR_OVERFLOW:         return MSIDBERROR_OVERFLOW;
        case LIBMSI_DB_ERROR_UNDERFLOW:        return MSIDBERROR_UNDERFLOW;
        case LIBMSI_DB_ERROR_NOTINSET:         return MSIDBERROR_NOTINSET;
        case LIBMSI_DB_ERROR_BADVERSION:       return MSIDBERROR_BADVERSION;
        case LIBMSI_DB_ERROR_BADCASE:          return MSIDBERROR_BADCASE;
        case LIBMSI_DB_ERROR_BADGUID:          return MSIDBERROR_BADGUID;
        case LIBMSI_DB_ERROR_BADWILDCARD:      return MSIDBERROR_BADWILDCARD;
        case LIBMSI_DB_ERROR_BADIDENTIFIER:    return MSIDBERROR_BADIDENTIFIER;
        case LIBMSI_DB_ERROR_BADLANGUAGE:      return MSIDBERROR_BADLANGUAGE;
        case LIBMSI_DB_ERROR_BADFILENAME:      return MSIDBERROR_BADFILENAME;
        case LIBMSI_DB_ERROR_BADPATH:          return MSIDBERROR_BADPATH;
        case LIBMSI_DB_ERROR_BADCONDITION:     return MSIDBERROR_BADCONDITION;
        case LIBMSI_DB_ERROR_BADFORMATTED:     return MSIDBERROR_BADFORMATTED;
        case LIBMSI_DB_ERROR_BADTEMPLATE:       return MSIDBERROR_BADTEMPLATE;
        case LIBMSI_DB_ERROR_BADDEFAULTDIR:    return MSIDBERROR_BADDEFAULTDIR;
        case LIBMSI_DB_ERROR_BADREGPATH:       return MSIDBERROR_BADREGPATH;
        case LIBMSI_DB_ERROR_BADCUSTOMSOURCE:  return MSIDBERROR_BADCUSTOMSOURCE;
        case LIBMSI_DB_ERROR_BADPROPERTY:      return MSIDBERROR_BADPROPERTY;
        case LIBMSI_DB_ERROR_MISSINGDATA:      return MSIDBERROR_MISSINGDATA;
        case LIBMSI_DB_ERROR_BADCATEGORY:      return MSIDBERROR_BADCATEGORY;
        case LIBMSI_DB_ERROR_BADKEYTABLE:      return MSIDBERROR_BADKEYTABLE;
        case LIBMSI_DB_ERROR_BADMAXMINVALUES:  return MSIDBERROR_BADMAXMINVALUES;
        case LIBMSI_DB_ERROR_BADCABINET:       return MSIDBERROR_BADCABINET;
        case LIBMSI_DB_ERROR_BADSHORTCUT:      return MSIDBERROR_BADSHORTCUT;
        case LIBMSI_DB_ERROR_STRINGOVERFLOW:   return MSIDBERROR_STRINGOVERFLOW;
        case LIBMSI_DB_ERROR_BADLOCALIZEATTRIB:return MSIDBERROR_BADLOCALIZEATTRIB;
        default:                               return MSIDBERROR_FUNCTIONERROR;
    }
}

/* ========================================================================== */
/* MsiDatabaseOpenView                                                        */
/* ========================================================================== */

UINT WINAPI MsiDatabaseOpenViewA(MSIHANDLE hDatabase, LPCSTR szQuery, MSIHANDLE *phView)
{
    if (!szQuery || !phView)
        return ERROR_INVALID_PARAMETER;

    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return ERROR_INVALID_HANDLE;

    GError *error = NULL;
    LibmsiQuery *query = libmsi_query_new(db, szQuery, &error);
    g_object_unref(db);
    if (!query)
        return gerror_to_msi(error);

    MSIHANDLE h = handle_table_alloc(G_OBJECT(query), HANDLE_QUERY);
    g_object_unref(query); /* handle table took a ref */
    if (!h)
        return ERROR_FUNCTION_FAILED;

    *phView = h;
    return ERROR_SUCCESS;
}

UINT WINAPI MsiDatabaseOpenViewW(MSIHANDLE hDatabase, LPCWSTR szQuery, MSIHANDLE *phView)
{
    if (!szQuery || !phView)
        return ERROR_INVALID_PARAMETER;

    char *utf8 = utf16_to_utf8((const WCHAR *)szQuery);
    if (!utf8)
        return ERROR_FUNCTION_FAILED;

    UINT ret = MsiDatabaseOpenViewA(hDatabase, utf8, phView);
    g_free(utf8);
    return ret;
}

/* ========================================================================== */
/* MsiViewExecute                                                             */
/* ========================================================================== */

UINT WINAPI MsiViewExecute(MSIHANDLE hView, MSIHANDLE hRecord)
{
    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return ERROR_INVALID_HANDLE;

    LibmsiRecord *rec = NULL;
    if (hRecord != 0) {
        rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
        if (!rec) {
            g_object_unref(query);
            return ERROR_INVALID_HANDLE;
        }
    }

    GError *error = NULL;
    gboolean ok = libmsi_query_execute(query, rec, &error);

    g_object_unref(query);
    if (rec) g_object_unref(rec);

    if (!ok)
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

/* ========================================================================== */
/* MsiViewFetch                                                               */
/* ========================================================================== */

UINT WINAPI MsiViewFetch(MSIHANDLE hView, MSIHANDLE *phRecord)
{
    if (!phRecord)
        return ERROR_INVALID_PARAMETER;

    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return ERROR_INVALID_HANDLE;

    if (!query->view || !query->view->ops || !query->database) {
        g_object_unref(query);
        return ERROR_FUNCTION_FAILED;
    }

    GError *error = NULL;
    LibmsiRecord *rec = libmsi_query_fetch(query, &error);
    g_object_unref(query);

    if (!rec) {
        if (error) {
            return gerror_to_msi(error);
        }
        return ERROR_NO_MORE_ITEMS;
    }

    MSIHANDLE h = handle_table_alloc(G_OBJECT(rec), HANDLE_RECORD);
    g_object_unref(rec); /* handle table took a ref */
    if (!h)
        return ERROR_FUNCTION_FAILED;

    *phRecord = h;
    return ERROR_SUCCESS;
}

/* ========================================================================== */
/* MsiViewModify                                                              */
/* ========================================================================== */

UINT WINAPI MsiViewModify(MSIHANDLE hView, MSIMODIFY eModifyMode, MSIHANDLE hRecord)
{
    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return ERROR_INVALID_HANDLE;

    LibmsiRecord *rec = (LibmsiRecord *)handle_table_get_typed(hRecord, HANDLE_RECORD);
    if (!rec) {
        g_object_unref(query);
        return ERROR_INVALID_HANDLE;
    }

    /* Access internal query view */
    LibmsiView *view = query->view;
    if (!view || !view->ops) {
        g_object_unref(query);
        g_object_unref(rec);
        return ERROR_FUNCTION_FAILED;
    }

    unsigned r;
    UINT ret;

    switch (eModifyMode) {
        case MSIMODIFY_REFRESH: {
            if (!view->ops->get_row || query->row == 0) {
                ret = ERROR_FUNCTION_FAILED;
                break;
            }
            LibmsiRecord *refreshed = NULL;
            r = view->ops->get_row(view, query->row - 1, &refreshed);
            if (r != ERROR_SUCCESS) {
                ret = r;
                break;
            }
            unsigned count = refreshed->count;
            for (unsigned i = 0; i <= count && i <= rec->count; i++) {
                _libmsi_record_copy_field(refreshed, i, rec, i);
            }
            g_object_unref(refreshed);
            ret = ERROR_SUCCESS;
            break;
        }

        case MSIMODIFY_INSERT:
            if (!view->ops->insert_row) { ret = ERROR_FUNCTION_FAILED; break; }
            ret = view->ops->insert_row(view, rec, -1, false);
            break;

        case MSIMODIFY_UPDATE:
            if (!view->ops->set_row || query->row == 0) { ret = ERROR_FUNCTION_FAILED; break; }
            ret = view->ops->set_row(view, query->row - 1, rec, ~0u);
            break;

        case MSIMODIFY_ASSIGN:
            if (view->ops->insert_row) {
                r = view->ops->insert_row(view, rec, -1, false);
                if (r == ERROR_SUCCESS) { ret = r; break; }
            }
            if (!view->ops->set_row || query->row == 0) { ret = ERROR_FUNCTION_FAILED; break; }
            ret = view->ops->set_row(view, query->row - 1, rec, ~0u);
            break;

        case MSIMODIFY_REPLACE:
            if (!view->ops->set_row || query->row == 0) { ret = ERROR_FUNCTION_FAILED; break; }
            ret = view->ops->set_row(view, query->row - 1, rec, ~0u);
            break;

        case MSIMODIFY_MERGE:
            if (!view->ops->insert_row) { ret = ERROR_FUNCTION_FAILED; break; }
            r = view->ops->insert_row(view, rec, -1, false);
            ret = (r == ERROR_FUNCTION_FAILED || r == ERROR_INVALID_DATA) ? ERROR_SUCCESS : r;
            break;

        case MSIMODIFY_DELETE:
            if (!view->ops->delete_row || query->row == 0) { ret = ERROR_FUNCTION_FAILED; break; }
            ret = view->ops->delete_row(view, query->row - 1);
            break;

        case MSIMODIFY_INSERT_TEMPORARY:
            if (!view->ops->insert_row) { ret = ERROR_FUNCTION_FAILED; break; }
            ret = view->ops->insert_row(view, rec, -1, true);
            break;

        case MSIMODIFY_VALIDATE:
        case MSIMODIFY_VALIDATE_NEW:
        case MSIMODIFY_VALIDATE_FIELD:
        case MSIMODIFY_VALIDATE_DELETE:
            ret = ERROR_SUCCESS;
            break;

        case MSIMODIFY_SEEK:
            ret = ERROR_FUNCTION_FAILED;
            break;

        default:
            ret = ERROR_INVALID_PARAMETER;
            break;
    }

    g_object_unref(query);
    g_object_unref(rec);
    return ret;
}

/* ========================================================================== */
/* MsiViewGetColumnInfo                                                       */
/* ========================================================================== */

UINT WINAPI MsiViewGetColumnInfo(MSIHANDLE hView, MSICOLINFO eColumnInfo, MSIHANDLE *phRecord)
{
    if (!phRecord)
        return ERROR_INVALID_PARAMETER;

    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return ERROR_INVALID_HANDLE;

    GError *error = NULL;
    LibmsiRecord *rec = libmsi_query_get_column_info(query, (LibmsiColInfo)eColumnInfo, &error);
    g_object_unref(query);

    if (!rec)
        return gerror_to_msi(error);

    MSIHANDLE h = handle_table_alloc(G_OBJECT(rec), HANDLE_RECORD);
    g_object_unref(rec); /* handle table took a ref */
    if (!h)
        return ERROR_FUNCTION_FAILED;

    *phRecord = h;
    return ERROR_SUCCESS;
}

/* ========================================================================== */
/* MsiViewClose                                                               */
/* ========================================================================== */

UINT WINAPI MsiViewClose(MSIHANDLE hView)
{
    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return ERROR_INVALID_HANDLE;

    GError *error = NULL;
    gboolean ok = libmsi_query_close(query, &error);
    g_object_unref(query);

    if (!ok)
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

/* ========================================================================== */
/* MsiViewGetError                                                            */
/* ========================================================================== */

MSIDBERROR WINAPI MsiViewGetErrorA(MSIHANDLE hView, LPSTR szColumnNameBuffer, LPDWORD pcchBuf)
{
    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return MSIDBERROR_INVALIDARG;

    /* Read the error directly from the internal view structure */
    LibmsiView *view = query->view;
    if (!view) {
        g_object_unref(query);
        return MSIDBERROR_INVALIDARG;
    }

    LibmsiDBError dberr = view->error;
    const char *col = view->error_column;

    /* Copy the column name to the output buffer */
    if (col) {
        copy_string_to_bufA(col, szColumnNameBuffer, pcchBuf);
    } else {
        copy_string_to_bufA("", szColumnNameBuffer, pcchBuf);
    }

    g_object_unref(query);
    return libmsi_dberror_to_msi(dberr);
}

MSIDBERROR WINAPI MsiViewGetErrorW(MSIHANDLE hView, LPWSTR szColumnNameBuffer, LPDWORD pcchBuf)
{
    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return MSIDBERROR_INVALIDARG;

    /* Read the error directly from the internal view structure */
    LibmsiView *view = query->view;
    if (!view) {
        g_object_unref(query);
        return MSIDBERROR_INVALIDARG;
    }

    LibmsiDBError dberr = view->error;
    const char *col = view->error_column;

    /* Copy the column name to the output buffer, converting to UTF-16 */
    if (col) {
        copy_string_to_bufW(col, (WCHAR *)szColumnNameBuffer, pcchBuf);
    } else {
        copy_string_to_bufW("", (WCHAR *)szColumnNameBuffer, pcchBuf);
    }

    g_object_unref(query);
    return libmsi_dberror_to_msi(dberr);
}
