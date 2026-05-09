/*
 * msicab.h - Cross-platform cabinet API for WiX toolset.
 *
 * These functions replace wixnative.exe (which uses Windows FCI/FDI cabinet
 * APIs). We will add [DllImport] calls in Cabinet.cs and DateTimeInterop.cs
 * to call these instead of spawning a child process.
 *
 * Backed by gcab (libgcab) internally.
 *
 * All strings are UTF-16 (wchar_t*) to match C# P/Invoke marshalling.
 */

#ifndef MSICAB_H
#define MSICAB_H

#include <stdint.h>
#include <wchar.h>

#ifdef _WIN32
#define MSICAB_API __declspec(dllexport)
#define MSICAB_CALL __stdcall
#else
#define MSICAB_API __attribute__((visibility("default")))
#define MSICAB_CALL
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Types
 * -------------------------------------------------------------------------- */

typedef uintptr_t CABHANDLE;

/* Compression levels matching WixToolset.Data.CompressionLevel enum:
 *   None=0, Low=1, Medium=2, High=3, Mszip=4 */
#define CAB_COMPRESS_NONE   0
#define CAB_COMPRESS_LOW    1
#define CAB_COMPRESS_MEDIUM 2
#define CAB_COMPRESS_HIGH   3
#define CAB_COMPRESS_MSZIP  4

/* MSIFILEHASHINFO -- matches the layout in MsiInterop / msi.h.
 * Used for duplicate file detection during cabinet creation. */
#ifndef MSI_INTEROP_FILEHASHINFO_DEFINED
#define MSI_INTEROP_FILEHASHINFO_DEFINED
typedef struct MSIFILEHASHINFO {
    uint32_t FileHashInfoSize;  /* must be 20 */
    int32_t  Data[4];
} MSIFILEHASHINFO;
#endif

/* Info about a cabinet created by CabFlush. */
typedef struct {
    const wchar_t *cabinet_name;     /* name of the created cabinet file */
    const wchar_t *first_file_token; /* token of the first file in this cabinet */
} CabCreatedInfo;

/* Info about a file inside an existing cabinet. */
typedef struct {
    const wchar_t *file_id;
    int32_t        size;       /* file size in bytes */
    uint16_t       date;       /* last modified date (MS-DOS format) */
    uint16_t       time;       /* last modified time (MS-DOS format) */
} CabFileInfo;

/* Error codes (matching Win32 subset used by WiX). */
#define CAB_SUCCESS             0
#define CAB_ERROR_INVALID_PARAM 87
#define CAB_ERROR_OPEN_FAILED   110
#define CAB_ERROR_FUNCTION_FAILED 1627

/* --------------------------------------------------------------------------
 * Cabinet creation
 * -------------------------------------------------------------------------- */

/* Begin building a cabinet.
 *   cab_path:          full path for the output .cab file
 *   compression_level: CAB_COMPRESS_xxx
 *   max_file_count:    expected number of files (hint, 0 = unknown)
 *   max_size_mb:       max cabinet size in MB (0 = unlimited)
 *   max_threshold:     max threshold (0 = default)
 *   out_handle:        receives the cab builder handle */
MSICAB_API uint32_t MSICAB_CALL CabCreate(
    const wchar_t *cab_path,
    int32_t        compression_level,
    int32_t        max_file_count,
    int32_t        max_size_mb,
    int32_t        max_threshold,
    CABHANDLE     *out_handle
);

/* Add a file to the cabinet being built.
 *   handle:    cab builder handle from CabCreate
 *   file_path: full path to the source file on disk
 *   token:     the name/token to store the file as inside the cabinet
 *   hash:      optional MSIFILEHASHINFO for duplicate detection (may be NULL) */
MSICAB_API uint32_t MSICAB_CALL CabAddFile(
    CABHANDLE              handle,
    const wchar_t         *file_path,
    const wchar_t         *token,
    const MSIFILEHASHINFO *hash
);

/* Finalize the cabinet and write it to disk.
 *   handle:     cab builder handle from CabCreate
 *   out_cabs:   caller-allocated array to receive created cabinet info
 *   max_cabs:   capacity of out_cabs array
 *   out_count:  receives actual number of cabinets created (may be >1 if split)
 *
 * Strings in out_cabs are valid until CabDestroy is called. */
MSICAB_API uint32_t MSICAB_CALL CabFlush(
    CABHANDLE       handle,
    CabCreatedInfo *out_cabs,
    int32_t         max_cabs,
    int32_t        *out_count
);

/* Free the cab builder handle and all associated resources.
 * Safe to call after CabFlush or on error (acts as cancel). */
MSICAB_API void MSICAB_CALL CabDestroy(
    CABHANDLE handle
);

/* --------------------------------------------------------------------------
 * Cabinet enumeration
 * -------------------------------------------------------------------------- */

/* Callback invoked once per file in the cabinet.
 * Return 0 to continue, non-zero to stop enumeration. */
typedef int32_t (MSICAB_CALL *CabEnumCallback)(
    const CabFileInfo *file_info,
    uintptr_t          context
);

/* Enumerate all files in an existing cabinet. */
MSICAB_API uint32_t MSICAB_CALL CabEnumerate(
    const wchar_t  *cab_path,
    CabEnumCallback callback,
    uintptr_t       context
);

/* --------------------------------------------------------------------------
 * Cabinet extraction
 * -------------------------------------------------------------------------- */

/* Callback invoked when extraction of a file begins.
 * Return 0 to continue, non-zero to skip/abort. */
typedef int32_t (MSICAB_CALL *CabExtractCallback)(
    const wchar_t *file_id,
    uintptr_t      context
);

/* Extract all files from a cabinet to a directory. */
MSICAB_API uint32_t MSICAB_CALL CabExtract(
    const wchar_t     *cab_path,
    const wchar_t     *output_folder,
    CabExtractCallback callback,
    uintptr_t          context
);

/* --------------------------------------------------------------------------
 * Date/time utility
 *
 * Replaces the kernel32.dll FileTimeToLocalFileTime + FileTimeToDosDateTime
 * calls in DateTimeInterop.cs.
 * -------------------------------------------------------------------------- */

/* Convert a Windows FILETIME (int64, 100-ns ticks since 1601-01-01) to
 * MS-DOS date and time values. Returns non-zero on success. */
MSICAB_API int32_t MSICAB_CALL FileTimeToDosDateTimeW(
    int64_t   filetime,
    uint16_t *out_fat_date,
    uint16_t *out_fat_time
);

#ifdef __cplusplus
}
#endif

#endif /* MSICAB_H */
