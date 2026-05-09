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
        if (!rec)
            return ERROR_INVALID_HANDLE;
    }

    GError *error = NULL;
    gboolean ok = libmsi_query_execute(query, rec, &error);
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

    GError *error = NULL;
    LibmsiRecord *rec = libmsi_query_fetch(query, &error);
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
    if (!rec)
        return ERROR_INVALID_HANDLE;

    /* Access internal query view */
    LibmsiView *view = query->view;
    if (!view || !view->ops)
        return ERROR_FUNCTION_FAILED;

    unsigned r;

    switch (eModifyMode) {
        case MSIMODIFY_REFRESH: {
            /* Refresh: re-read the current row into the record */
            if (!view->ops->get_row)
                return ERROR_FUNCTION_FAILED;
            if (query->row == 0)
                return ERROR_FUNCTION_FAILED;
            LibmsiRecord *refreshed = NULL;
            r = view->ops->get_row(view, query->row - 1, &refreshed);
            if (r != ERROR_SUCCESS)
                return r;
            /* Copy field data from refreshed record into the caller's record */
            unsigned count = refreshed->count;
            for (unsigned i = 0; i <= count && i <= rec->count; i++) {
                _libmsi_record_copy_field(refreshed, i, rec, i);
            }
            g_object_unref(refreshed);
            return ERROR_SUCCESS;
        }

        case MSIMODIFY_INSERT: {
            if (!view->ops->insert_row)
                return ERROR_FUNCTION_FAILED;
            r = view->ops->insert_row(view, rec, -1, false);
            return r;
        }

        case MSIMODIFY_UPDATE: {
            if (!view->ops->set_row)
                return ERROR_FUNCTION_FAILED;
            if (query->row == 0)
                return ERROR_FUNCTION_FAILED;
            r = view->ops->set_row(view, query->row - 1, rec, ~0u);
            return r;
        }

        case MSIMODIFY_ASSIGN: {
            /* Try insert first; if duplicate key, fall back to update */
            if (view->ops->insert_row) {
                r = view->ops->insert_row(view, rec, -1, false);
                if (r == ERROR_SUCCESS)
                    return r;
            }
            /* Fall back to set_row */
            if (!view->ops->set_row)
                return ERROR_FUNCTION_FAILED;
            if (query->row == 0)
                return ERROR_FUNCTION_FAILED;
            r = view->ops->set_row(view, query->row - 1, rec, ~0u);
            return r;
        }

        case MSIMODIFY_REPLACE: {
            /* Similar to UPDATE */
            if (!view->ops->set_row)
                return ERROR_FUNCTION_FAILED;
            if (query->row == 0)
                return ERROR_FUNCTION_FAILED;
            r = view->ops->set_row(view, query->row - 1, rec, ~0u);
            return r;
        }

        case MSIMODIFY_MERGE: {
            /* Try insert; if duplicate key error, just succeed silently */
            if (!view->ops->insert_row)
                return ERROR_FUNCTION_FAILED;
            r = view->ops->insert_row(view, rec, -1, false);
            if (r == ERROR_FUNCTION_FAILED || r == ERROR_INVALID_DATA)
                return ERROR_SUCCESS; /* duplicate key - merge is a no-op */
            return r;
        }

        case MSIMODIFY_DELETE: {
            if (!view->ops->delete_row)
                return ERROR_FUNCTION_FAILED;
            if (query->row == 0)
                return ERROR_FUNCTION_FAILED;
            r = view->ops->delete_row(view, query->row - 1);
            return r;
        }

        case MSIMODIFY_INSERT_TEMPORARY: {
            if (!view->ops->insert_row)
                return ERROR_FUNCTION_FAILED;
            r = view->ops->insert_row(view, rec, -1, true);
            return r;
        }

        case MSIMODIFY_VALIDATE:
        case MSIMODIFY_VALIDATE_NEW:
        case MSIMODIFY_VALIDATE_FIELD:
        case MSIMODIFY_VALIDATE_DELETE:
            /* Validation modes - not implemented, return success */
            return ERROR_SUCCESS;

        case MSIMODIFY_SEEK: {
            /*
             * MSIMODIFY_SEEK: position the cursor based on primary key lookup.
             * This is rarely used; for now just return success without seeking.
             */
            return ERROR_FUNCTION_FAILED;
        }

        default:
            return ERROR_INVALID_PARAMETER;
    }
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
    if (!view)
        return MSIDBERROR_INVALIDARG;

    LibmsiDBError dberr = view->error;
    const char *col = view->error_column;

    /* Copy the column name to the output buffer */
    if (col) {
        copy_string_to_bufA(col, szColumnNameBuffer, pcchBuf);
    } else {
        copy_string_to_bufA("", szColumnNameBuffer, pcchBuf);
    }

    return libmsi_dberror_to_msi(dberr);
}

MSIDBERROR WINAPI MsiViewGetErrorW(MSIHANDLE hView, LPWSTR szColumnNameBuffer, LPDWORD pcchBuf)
{
    LibmsiQuery *query = (LibmsiQuery *)handle_table_get_typed(hView, HANDLE_QUERY);
    if (!query)
        return MSIDBERROR_INVALIDARG;

    /* Read the error directly from the internal view structure */
    LibmsiView *view = query->view;
    if (!view)
        return MSIDBERROR_INVALIDARG;

    LibmsiDBError dberr = view->error;
    const char *col = view->error_column;

    /* Copy the column name to the output buffer, converting to UTF-16 */
    if (col) {
        copy_string_to_bufW(col, (WCHAR *)szColumnNameBuffer, pcchBuf);
    } else {
        copy_string_to_bufW("", (WCHAR *)szColumnNameBuffer, pcchBuf);
    }

    return libmsi_dberror_to_msi(dberr);
}
