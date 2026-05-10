#ifndef MSI_INTEROP_STRING_UTILS_H
#define MSI_INTEROP_STRING_UTILS_H

#include <stddef.h>
#include "msi_interop.h"

#ifndef _WIN32
typedef unsigned short WCHAR;
#endif

char *utf16_to_utf8(const WCHAR *utf16);

WCHAR *utf8_to_utf16(const char *utf8, size_t *out_len);

unsigned int copy_string_to_bufA(const char *str, char *buf, DWORD *pcch);

unsigned int copy_string_to_bufW(const char *str, WCHAR *buf, DWORD *pcch);

#endif
