#ifndef MSI_INTEROP_STRING_UTILS_H
#define MSI_INTEROP_STRING_UTILS_H

#include <stddef.h>

// WCHAR is always 16-bit UTF-16LE for MSI interop
typedef unsigned short WCHAR;

// Convert UTF-16LE string to UTF-8. Returns newly allocated UTF-8 string (must be g_free'd).
// Returns NULL on error.
char *utf16_to_utf8(const WCHAR *utf16);

// Convert UTF-8 string to UTF-16LE. Returns newly allocated UTF-16LE string (must be g_free'd).
// If out_len is not NULL, receives the length in WCHAR units (not including null terminator).
// Returns NULL on error.
WCHAR *utf8_to_utf16(const char *utf8, size_t *out_len);

// Copy a UTF-8 string into a caller-provided narrow (char*) buffer following MSI API conventions:
// - If buf is NULL and pcch is NULL: just return ERROR_SUCCESS
// - If buf is NULL and pcch is not NULL: set *pcch to required length (not including null), return ERROR_MORE_DATA
// - If buf is not NULL and *pcch is less than required: copy what fits, set *pcch to required length, return ERROR_MORE_DATA
// - Otherwise: copy string, set *pcch to actual length (not including null), return ERROR_SUCCESS
unsigned int copy_string_to_bufA(const char *str, char *buf, unsigned int *pcch);

// Same but for wide (WCHAR*) buffers.
unsigned int copy_string_to_bufW(const char *str, WCHAR *buf, unsigned int *pcch);

#endif
