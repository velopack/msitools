/*
 * file_utils.c - MsiGetFileHash and MsiGetFileVersion implementations
 *
 * MsiGetFileHash computes an MD5 hash of a file's contents and returns
 * the 16-byte digest as four 32-bit values in MSIFILEHASHINFO.
 *
 * MsiGetFileVersion reads PE version info (VS_FIXEDFILEINFO) from
 * executable files and returns the file version and language strings.
 */

#include "msi_interop.h"
#include "string_utils.h"
#include <glib.h>
#include <glib/gstdio.h>
#include <string.h>
#include <stdio.h>

#ifndef ERROR_FILE_NOT_FOUND
#define ERROR_FILE_NOT_FOUND 2L
#endif

#ifndef ERROR_FILE_INVALID
#define ERROR_FILE_INVALID 1006L
#endif

#ifndef ERROR_OPEN_FAILED
#define ERROR_OPEN_FAILED 110L
#endif

/* VS_FIXEDFILEINFO signature */
#define VS_FFI_SIGNATURE 0xFEEF04BD

/* ========================================================================== */
/* MsiGetFileHash                                                             */
/* ========================================================================== */

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileHashA(LPCSTR szFilePath, DWORD dwOptions, PMSIFILEHASHINFO pHash)
{
    (void)dwOptions;

    if (szFilePath == NULL || pHash == NULL)
        return ERROR_INVALID_PARAMETER;

    if (pHash->dwFileHashInfoSize != 20)
        return ERROR_INVALID_PARAMETER;

    /* Read the entire file */
    gchar *contents = NULL;
    gsize length = 0;
    GError *error = NULL;

    if (!g_file_get_contents(szFilePath, &contents, &length, &error)) {
        if (error != NULL)
            g_error_free(error);
        return ERROR_FILE_NOT_FOUND;
    }

    /* Compute MD5 hash */
    GChecksum *checksum = g_checksum_new(G_CHECKSUM_MD5);
    if (checksum == NULL) {
        g_free(contents);
        return ERROR_GEN_FAILURE;
    }

    g_checksum_update(checksum, (const guchar *)contents, (gssize)length);
    g_free(contents);

    guint8 digest[16];
    gsize digest_len = sizeof(digest);
    g_checksum_get_digest(checksum, digest, &digest_len);
    g_checksum_free(checksum);

    if (digest_len != 16)
        return ERROR_GEN_FAILURE;

    /* Copy the 16 bytes into the four DWORD values.
     * Each DWORD is 4 bytes of the MD5 digest in native byte order.
     * We use memcpy to avoid alignment/endianness issues -- the Windows
     * MSI API stores the raw MD5 bytes directly into the DWORD array. */
    memcpy(&pHash->dwData[0], &digest[0],  4);
    memcpy(&pHash->dwData[1], &digest[4],  4);
    memcpy(&pHash->dwData[2], &digest[8],  4);
    memcpy(&pHash->dwData[3], &digest[12], 4);

    return ERROR_SUCCESS;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileHashW(LPCWSTR szFilePath, DWORD dwOptions, PMSIFILEHASHINFO pHash)
{
    if (szFilePath == NULL)
        return ERROR_INVALID_PARAMETER;

    char *path_utf8 = utf16_to_utf8(szFilePath);
    if (path_utf8 == NULL)
        return ERROR_INVALID_PARAMETER;

    UINT ret = MsiGetFileHashA(path_utf8, dwOptions, pHash);
    g_free(path_utf8);
    return ret;
}

/* ========================================================================== */
/* MsiGetFileVersion - PE version info reader                                 */
/* ========================================================================== */

/*
 * Minimal PE version reader.
 *
 * Strategy: read the whole file into memory (up to 100 MB), then scan
 * for the VS_FIXEDFILEINFO magic signature (0xFEEF04BD in little-endian:
 * bytes 0xBD 0x04 0xEF 0xFE). If found, extract dwFileVersionMS and
 * dwFileVersionLS to produce a "major.minor.build.revision" string.
 *
 * For the language, we attempt to find a translation table by searching
 * for the "Translation" key near a VarFileInfo block. If not found,
 * we return "0" as a sensible default.
 */

/* Maximum file size we are willing to read (100 MB) */
#define MAX_PE_FILE_SIZE (100 * 1024 * 1024)

/* Helper: check MZ signature at start of data */
static int
is_pe_file(const guint8 *data, gsize len)
{
    if (len < 2)
        return 0;
    return (data[0] == 'M' && data[1] == 'Z');
}

/* Helper: read a little-endian uint16 from a byte pointer */
static unsigned int
read_le16(const guint8 *p)
{
    return (unsigned int)p[0] | ((unsigned int)p[1] << 8);
}

/* Helper: read a little-endian uint32 from a byte pointer */
static unsigned int
read_le32(const guint8 *p)
{
    return (unsigned int)p[0]
         | ((unsigned int)p[1] << 8)
         | ((unsigned int)p[2] << 16)
         | ((unsigned int)p[3] << 24);
}

/*
 * Search for VS_FIXEDFILEINFO signature in the file data and extract
 * the version. Returns 1 on success, 0 if not found.
 */
static int
find_version_info(const guint8 *data, gsize len,
                  unsigned int *major, unsigned int *minor,
                  unsigned int *build, unsigned int *revision)
{
    /* The signature 0xFEEF04BD is stored as bytes: BD 04 EF FE */
    const guint8 sig[4] = { 0xBD, 0x04, 0xEF, 0xFE };

    if (len < 4)
        return 0;

    /* Scan for the signature. VS_FIXEDFILEINFO is typically DWORD-aligned. */
    gsize search_limit = len - 4;
    for (gsize i = 0; i <= search_limit; i += 4) {
        if (data[i] == sig[0] && data[i+1] == sig[1] &&
            data[i+2] == sig[2] && data[i+3] == sig[3]) {

            /* VS_FIXEDFILEINFO layout after dwSignature:
             *   offset  0: dwSignature        (4 bytes) -- already found at data[i]
             *   offset  4: dwStrucVersion      (4 bytes)
             *   offset  8: dwFileVersionMS     (4 bytes)
             *   offset 12: dwFileVersionLS     (4 bytes)
             *
             * We need at least 16 bytes from position i. */
            if (i + 16 > len)
                continue;

            unsigned int struc_version = read_le32(data + i + 4);
            /* Sanity check: dwStrucVersion should be 0x00010000 or similar */
            if ((struc_version & 0xFFFF0000) == 0 && struc_version != 0)
                continue; /* doesn't look right, keep searching */

            unsigned int file_version_ms = read_le32(data + i + 8);
            unsigned int file_version_ls = read_le32(data + i + 12);

            *major    = (file_version_ms >> 16) & 0xFFFF;
            *minor    = file_version_ms & 0xFFFF;
            *build    = (file_version_ls >> 16) & 0xFFFF;
            *revision = file_version_ls & 0xFFFF;

            /* Sanity: check that we also have a valid product version after
             * (at offset 16-23). The product version fields follow and should
             * be reasonable. We skip this check since some files may have
             * zeroed product versions. */

            return 1;
        }
    }

    return 0;
}

/*
 * Search for the translation table in the VERSION_INFO resource.
 * Look for a VarFileInfo\Translation entry which contains pairs of
 * (language, codepage) as 16-bit values.
 *
 * Returns 1 if found, with lang_id set to the language ID.
 */
static int
find_translation_info(const guint8 *data, gsize len, unsigned int *lang_id)
{
    /* Search for the UTF-16LE string "Translation" which appears in
     * the VarFileInfo structure. In UTF-16LE:
     * T=0x0054 r=0x0072 a=0x0061 n=0x006E s=0x0073 l=0x006C
     * a=0x0061 t=0x0074 i=0x0069 o=0x006F n=0x006E
     * That's 11 UTF-16 chars = 22 bytes */
    static const guint8 translation_utf16[] = {
        0x54, 0x00, 0x72, 0x00, 0x61, 0x00, 0x6E, 0x00,
        0x73, 0x00, 0x6C, 0x00, 0x61, 0x00, 0x74, 0x00,
        0x69, 0x00, 0x6F, 0x00, 0x6E, 0x00
    };
    gsize sig_len = sizeof(translation_utf16);

    if (len < sig_len + 4)
        return 0;

    gsize search_limit = len - sig_len - 4;
    for (gsize i = 0; i <= search_limit; i += 2) {
        if (memcmp(data + i, translation_utf16, sig_len) == 0) {
            /* After "Translation\0" (null terminator = 2 more bytes),
             * there may be padding to align to a 4-byte boundary,
             * followed by the translation data (array of WORD pairs).
             * The first WORD is the language ID. */
            gsize pos = i + sig_len + 2; /* skip null terminator */

            /* Align to 4-byte boundary */
            pos = (pos + 3) & ~((gsize)3);

            if (pos + 4 <= len) {
                *lang_id = read_le16(data + pos);
                return 1;
            }
        }
    }

    return 0;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileVersionA(LPCSTR szFilePath, LPSTR lpVersionBuf, LPDWORD pcchVersionBuf, LPSTR lpLangBuf, LPDWORD pcchLangBuf)
{
    if (szFilePath == NULL)
        return ERROR_INVALID_PARAMETER;

    /* Read the file */
    gchar *contents = NULL;
    gsize length = 0;
    GError *error = NULL;

    if (!g_file_get_contents(szFilePath, &contents, &length, &error)) {
        if (error != NULL)
            g_error_free(error);
        return ERROR_FILE_NOT_FOUND;
    }

    /* Reject files that are too large */
    if (length > MAX_PE_FILE_SIZE) {
        g_free(contents);
        return ERROR_FILE_NOT_FOUND;
    }

    const guint8 *data = (const guint8 *)contents;

    /* Check for MZ signature */
    if (!is_pe_file(data, length)) {
        g_free(contents);
        return ERROR_FILE_INVALID;
    }

    /* Try to find VS_FIXEDFILEINFO */
    unsigned int major = 0, minor = 0, build = 0, revision = 0;
    int found_version = find_version_info(data, length, &major, &minor, &build, &revision);

    if (!found_version) {
        g_free(contents);
        return ERROR_FILE_INVALID;
    }

    /* Format the version string */
    char version_str[64];
    snprintf(version_str, sizeof(version_str), "%u.%u.%u.%u",
             major, minor, build, revision);

    /* Try to find the language ID */
    unsigned int lang_id = 0;
    int found_lang = find_translation_info(data, length, &lang_id);

    char lang_str[32];
    if (found_lang)
        snprintf(lang_str, sizeof(lang_str), "%u", lang_id);
    else
        snprintf(lang_str, sizeof(lang_str), "0");

    g_free(contents);

    /* Copy version string to output buffer */
    UINT ret_version = copy_string_to_bufA(version_str, lpVersionBuf, pcchVersionBuf);

    /* Copy language string to output buffer */
    UINT ret_lang = copy_string_to_bufA(lang_str, lpLangBuf, pcchLangBuf);

    /* If either buffer was too small, return ERROR_MORE_DATA */
    if (ret_version == ERROR_MORE_DATA || ret_lang == ERROR_MORE_DATA)
        return ERROR_MORE_DATA;

    return ERROR_SUCCESS;
}

MSI_INTEROP_EXPORT UINT WINAPI MsiGetFileVersionW(LPCWSTR szFilePath, LPWSTR lpVersionBuf, LPDWORD pcchVersionBuf, LPWSTR lpLangBuf, LPDWORD pcchLangBuf)
{
    if (szFilePath == NULL)
        return ERROR_INVALID_PARAMETER;

    char *path_utf8 = utf16_to_utf8(szFilePath);
    if (path_utf8 == NULL)
        return ERROR_INVALID_PARAMETER;

    /* Read the file */
    gchar *contents = NULL;
    gsize length = 0;
    GError *error = NULL;

    if (!g_file_get_contents(path_utf8, &contents, &length, &error)) {
        if (error != NULL)
            g_error_free(error);
        g_free(path_utf8);
        return ERROR_FILE_NOT_FOUND;
    }
    g_free(path_utf8);

    /* Reject files that are too large */
    if (length > MAX_PE_FILE_SIZE) {
        g_free(contents);
        return ERROR_FILE_NOT_FOUND;
    }

    const guint8 *data = (const guint8 *)contents;

    /* Check for MZ signature */
    if (!is_pe_file(data, length)) {
        g_free(contents);
        return ERROR_FILE_INVALID;
    }

    /* Try to find VS_FIXEDFILEINFO */
    unsigned int major = 0, minor = 0, build = 0, revision = 0;
    int found_version = find_version_info(data, length, &major, &minor, &build, &revision);

    if (!found_version) {
        g_free(contents);
        return ERROR_FILE_INVALID;
    }

    /* Format the version string */
    char version_str[64];
    snprintf(version_str, sizeof(version_str), "%u.%u.%u.%u",
             major, minor, build, revision);

    /* Try to find the language ID */
    unsigned int lang_id = 0;
    int found_lang = find_translation_info(data, length, &lang_id);

    char lang_str[32];
    if (found_lang)
        snprintf(lang_str, sizeof(lang_str), "%u", lang_id);
    else
        snprintf(lang_str, sizeof(lang_str), "0");

    g_free(contents);

    /* Copy version string to wide output buffer */
    UINT ret_version = copy_string_to_bufW(version_str, lpVersionBuf, pcchVersionBuf);

    /* Copy language string to wide output buffer */
    UINT ret_lang = copy_string_to_bufW(lang_str, lpLangBuf, pcchLangBuf);

    /* If either buffer was too small, return ERROR_MORE_DATA */
    if (ret_version == ERROR_MORE_DATA || ret_lang == ERROR_MORE_DATA)
        return ERROR_MORE_DATA;

    return ERROR_SUCCESS;
}
