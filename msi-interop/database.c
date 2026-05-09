/*
 * database.c - MSI database functions wrapping libmsi's GObject API
 */

#include "msi_interop.h"
#include "handle_table.h"
#include "string_utils.h"
#include "libmsi.h"
#include <glib.h>
#include <string.h>
#include <stdint.h>
#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#else
#include <unistd.h>
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
/* Helper: map MSIDBOPEN persist value to LibmsiDbFlags                       */
/* -------------------------------------------------------------------------- */

static guint
persist_to_flags(uintptr_t persist)
{
    switch (persist) {
    case 0: /* MSIDBOPEN_READONLY */
        return LIBMSI_DB_FLAGS_READONLY;
    case 1: /* MSIDBOPEN_TRANSACT */
        return LIBMSI_DB_FLAGS_TRANSACT;
    case 2: /* MSIDBOPEN_DIRECT */
        return LIBMSI_DB_FLAGS_TRANSACT;
    case 3: /* MSIDBOPEN_CREATE */
        return LIBMSI_DB_FLAGS_CREATE;
    case 4: /* MSIDBOPEN_CREATEDIRECT */
        return LIBMSI_DB_FLAGS_CREATE;
    default:
        return LIBMSI_DB_FLAGS_READONLY;
    }
}

/* -------------------------------------------------------------------------- */
/* MsiOpenDatabase                                                            */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiOpenDatabaseA(LPCSTR szDatabasePath, LPCSTR szPersist, MSIHANDLE *phDatabase)
{
    if (!szDatabasePath || !phDatabase)
        return ERROR_INVALID_PARAMETER;

    guint flags;
    const char *persist_path = NULL;

    if ((uintptr_t)szPersist <= 4) {
        flags = persist_to_flags((uintptr_t)szPersist);
    } else {
        /* szPersist is a real path string -- use it as the output/persist path */
        flags = LIBMSI_DB_FLAGS_TRANSACT;
        persist_path = szPersist;
    }

    GError *error = NULL;
    LibmsiDatabase *db = libmsi_database_new(szDatabasePath, flags,
                                              persist_path, &error);
    if (!db)
        return gerror_to_msi(error);

    MSIHANDLE handle = handle_table_alloc(G_OBJECT(db), HANDLE_DATABASE);
    g_object_unref(db); /* handle_table_alloc added its own ref */

    if (handle == 0)
        return ERROR_GEN_FAILURE;

    *phDatabase = handle;
    return ERROR_SUCCESS;
}

UINT WINAPI
MsiOpenDatabaseW(LPCWSTR szDatabasePath, LPCWSTR szPersist, MSIHANDLE *phDatabase)
{
    if (!szDatabasePath || !phDatabase)
        return ERROR_INVALID_PARAMETER;

    char *path_utf8 = utf16_to_utf8((const WCHAR *)szDatabasePath);
    if (!path_utf8)
        return ERROR_INVALID_PARAMETER;

    UINT ret;

    if ((uintptr_t)szPersist <= 4) {
        /* Small integer -- pass through as-is (cast to LPCSTR) */
        ret = MsiOpenDatabaseA(path_utf8, (LPCSTR)szPersist, phDatabase);
    } else {
        char *persist_utf8 = utf16_to_utf8((const WCHAR *)szPersist);
        if (!persist_utf8) {
            g_free(path_utf8);
            return ERROR_INVALID_PARAMETER;
        }
        ret = MsiOpenDatabaseA(path_utf8, persist_utf8, phDatabase);
        g_free(persist_utf8);
    }

    g_free(path_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiDatabaseCommit                                                          */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiDatabaseCommit(MSIHANDLE hDatabase)
{
    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return ERROR_INVALID_HANDLE;

    GError *error = NULL;
    if (!libmsi_database_commit(db, &error))
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

/* -------------------------------------------------------------------------- */
/* MsiGetDatabaseState                                                        */
/* -------------------------------------------------------------------------- */

MSIDBSTATE WINAPI
MsiGetDatabaseState(MSIHANDLE hDatabase)
{
    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return MSIDBSTATE_ERROR;

    if (libmsi_database_is_readonly(db))
        return MSIDBSTATE_READ;

    return MSIDBSTATE_WRITE;
}

/* -------------------------------------------------------------------------- */
/* MsiDatabaseGetPrimaryKeys                                                  */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiDatabaseGetPrimaryKeysA(MSIHANDLE hDatabase, LPCSTR szTableName, MSIHANDLE *phRecord)
{
    if (!szTableName || !phRecord)
        return ERROR_INVALID_PARAMETER;

    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return ERROR_INVALID_HANDLE;

    GError *error = NULL;
    LibmsiRecord *rec = libmsi_database_get_primary_keys(db, szTableName, &error);
    if (!rec)
        return gerror_to_msi(error);

    MSIHANDLE handle = handle_table_alloc(G_OBJECT(rec), HANDLE_RECORD);
    g_object_unref(rec);

    if (handle == 0)
        return ERROR_GEN_FAILURE;

    *phRecord = handle;
    return ERROR_SUCCESS;
}

UINT WINAPI
MsiDatabaseGetPrimaryKeysW(MSIHANDLE hDatabase, LPCWSTR szTableName, MSIHANDLE *phRecord)
{
    if (!szTableName || !phRecord)
        return ERROR_INVALID_PARAMETER;

    char *table_utf8 = utf16_to_utf8((const WCHAR *)szTableName);
    if (!table_utf8)
        return ERROR_INVALID_PARAMETER;

    UINT ret = MsiDatabaseGetPrimaryKeysA(hDatabase, table_utf8, phRecord);
    g_free(table_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiDatabaseIsTablePersistent                                               */
/* -------------------------------------------------------------------------- */

MSICONDITION WINAPI
MsiDatabaseIsTablePersistentA(MSIHANDLE hDatabase, LPCSTR szTableName)
{
    if (!szTableName)
        return MSICONDITION_ERROR;

    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return MSICONDITION_ERROR;

    GError *error = NULL;
    gboolean persistent = libmsi_database_is_table_persistent(db, szTableName, &error);
    if (error) {
        g_error_free(error);
        return MSICONDITION_ERROR;
    }

    return persistent ? MSICONDITION_TRUE : MSICONDITION_FALSE;
}

MSICONDITION WINAPI
MsiDatabaseIsTablePersistentW(MSIHANDLE hDatabase, LPCWSTR szTableName)
{
    if (!szTableName)
        return MSICONDITION_ERROR;

    char *table_utf8 = utf16_to_utf8((const WCHAR *)szTableName);
    if (!table_utf8)
        return MSICONDITION_ERROR;

    MSICONDITION ret = MsiDatabaseIsTablePersistentA(hDatabase, table_utf8);
    g_free(table_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiDatabaseImport                                                          */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiDatabaseImportA(MSIHANDLE hDatabase, LPCSTR szFolderPath, LPCSTR szFileName)
{
    if (!szFolderPath || !szFileName)
        return ERROR_INVALID_PARAMETER;

    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return ERROR_INVALID_HANDLE;

    /* Build full path: folder + separator + file */
    char *full_path = g_build_filename(szFolderPath, szFileName, NULL);

    GError *error = NULL;
    gboolean ok = libmsi_database_import(db, full_path, &error);
    g_free(full_path);

    if (!ok)
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

UINT WINAPI
MsiDatabaseImportW(MSIHANDLE hDatabase, LPCWSTR szFolderPath, LPCWSTR szFileName)
{
    if (!szFolderPath || !szFileName)
        return ERROR_INVALID_PARAMETER;

    char *folder_utf8 = utf16_to_utf8((const WCHAR *)szFolderPath);
    char *file_utf8 = utf16_to_utf8((const WCHAR *)szFileName);

    if (!folder_utf8 || !file_utf8) {
        g_free(folder_utf8);
        g_free(file_utf8);
        return ERROR_INVALID_PARAMETER;
    }

    UINT ret = MsiDatabaseImportA(hDatabase, folder_utf8, file_utf8);
    g_free(folder_utf8);
    g_free(file_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiDatabaseExport                                                          */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiDatabaseExportA(MSIHANDLE hDatabase, LPCSTR szTableName, LPCSTR szFolderPath,
                   LPCSTR szFileName)
{
    if (!szTableName || !szFolderPath || !szFileName)
        return ERROR_INVALID_PARAMETER;

    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return ERROR_INVALID_HANDLE;

    /* Build the full path and open it as a file descriptor */
    char *full_path = g_build_filename(szFolderPath, szFileName, NULL);

#ifdef _WIN32
    int fd = _open(full_path, _O_WRONLY | _O_CREAT | _O_TRUNC, 0644);
#else
    int fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
#endif
    g_free(full_path);

    if (fd < 0)
        return ERROR_FUNCTION_FAILED;

    GError *error = NULL;
    gboolean ok = libmsi_database_export(db, szTableName, fd, &error);

#ifdef _WIN32
    _close(fd);
#else
    close(fd);
#endif

    if (!ok)
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

UINT WINAPI
MsiDatabaseExportW(MSIHANDLE hDatabase, LPCWSTR szTableName, LPCWSTR szFolderPath,
                   LPCWSTR szFileName)
{
    if (!szTableName || !szFolderPath || !szFileName)
        return ERROR_INVALID_PARAMETER;

    char *table_utf8 = utf16_to_utf8((const WCHAR *)szTableName);
    char *folder_utf8 = utf16_to_utf8((const WCHAR *)szFolderPath);
    char *file_utf8 = utf16_to_utf8((const WCHAR *)szFileName);

    if (!table_utf8 || !folder_utf8 || !file_utf8) {
        g_free(table_utf8);
        g_free(folder_utf8);
        g_free(file_utf8);
        return ERROR_INVALID_PARAMETER;
    }

    UINT ret = MsiDatabaseExportA(hDatabase, table_utf8, folder_utf8, file_utf8);
    g_free(table_utf8);
    g_free(folder_utf8);
    g_free(file_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiDatabaseMerge                                                           */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiDatabaseMergeA(MSIHANDLE hDatabase, MSIHANDLE hDatabaseMerge, LPCSTR szTableName)
{
    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return ERROR_INVALID_HANDLE;

    LibmsiDatabase *db_merge = (LibmsiDatabase *)handle_table_get_typed(hDatabaseMerge, HANDLE_DATABASE);
    if (!db_merge)
        return ERROR_INVALID_HANDLE;

    GError *error = NULL;
    if (!libmsi_database_merge(db, db_merge, szTableName, &error))
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

UINT WINAPI
MsiDatabaseMergeW(MSIHANDLE hDatabase, MSIHANDLE hDatabaseMerge, LPCWSTR szTableName)
{
    char *table_utf8 = NULL;

    if (szTableName) {
        table_utf8 = utf16_to_utf8((const WCHAR *)szTableName);
        if (!table_utf8)
            return ERROR_INVALID_PARAMETER;
    }

    UINT ret = MsiDatabaseMergeA(hDatabase, hDatabaseMerge, table_utf8);
    g_free(table_utf8);
    return ret;
}

/* -------------------------------------------------------------------------- */
/* MsiDatabaseApplyTransform                                                  */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiDatabaseApplyTransformA(MSIHANDLE hDatabase, LPCSTR szTransformFile,
                           int iErrorConditions)
{
    if (!szTransformFile)
        return ERROR_INVALID_PARAMETER;

    LibmsiDatabase *db = (LibmsiDatabase *)handle_table_get_typed(hDatabase, HANDLE_DATABASE);
    if (!db)
        return ERROR_INVALID_HANDLE;

    GError *error = NULL;
    if (!libmsi_database_apply_transform(db, szTransformFile, &error))
        return gerror_to_msi(error);

    return ERROR_SUCCESS;
}

UINT WINAPI
MsiDatabaseApplyTransformW(MSIHANDLE hDatabase, LPCWSTR szTransformFile,
                           int iErrorConditions)
{
    if (!szTransformFile)
        return ERROR_INVALID_PARAMETER;

    char *file_utf8 = utf16_to_utf8((const WCHAR *)szTransformFile);
    if (!file_utf8)
        return ERROR_INVALID_PARAMETER;

    UINT ret = MsiDatabaseApplyTransformA(hDatabase, file_utf8, iErrorConditions);
    g_free(file_utf8);
    return ret;
}


/* -------------------------------------------------------------------------- */
/* MsiVerifyPackage                                                           */
/* -------------------------------------------------------------------------- */

UINT WINAPI
MsiVerifyPackageA(LPCSTR szPackagePath)
{
    if (!szPackagePath)
        return ERROR_INVALID_PARAMETER;

    MSIHANDLE hDatabase = 0;
    UINT ret = MsiOpenDatabaseA(szPackagePath, (LPCSTR)(uintptr_t)0, &hDatabase);
    if (ret != ERROR_SUCCESS)
        return ERROR_INSTALL_PACKAGE_INVALID;

    handle_table_close(hDatabase);
    return ERROR_SUCCESS;
}

UINT WINAPI
MsiVerifyPackageW(LPCWSTR szPackagePath)
{
    if (!szPackagePath)
        return ERROR_INVALID_PARAMETER;

    char *path_utf8 = utf16_to_utf8((const WCHAR *)szPackagePath);
    if (!path_utf8)
        return ERROR_INVALID_PARAMETER;

    UINT ret = MsiVerifyPackageA(path_utf8);
    g_free(path_utf8);
    return ret;
}
