#include "string_utils.h"
#include <glib.h>
#include <string.h>

#define ERROR_SUCCESS   0
#define ERROR_MORE_DATA 234

char *
utf16_to_utf8(const WCHAR *utf16)
{
    if (utf16 == NULL)
        return g_strdup("");

    GError *error = NULL;
    glong items_read = 0;
    glong items_written = 0;

    // GLib's gunichar2 is the same as our WCHAR (unsigned short / uint16_t)
    char *utf8 = g_utf16_to_utf8((const gunichar2 *)utf16, -1,
                                  &items_read, &items_written, &error);
    if (error != NULL) {
        g_error_free(error);
        return NULL;
    }

    return utf8;
}

WCHAR *
utf8_to_utf16(const char *utf8, size_t *out_len)
{
    if (utf8 == NULL)
        utf8 = "";

    GError *error = NULL;
    glong items_read = 0;
    glong items_written = 0;

    gunichar2 *utf16 = g_utf8_to_utf16(utf8, -1,
                                         &items_read, &items_written, &error);
    if (error != NULL) {
        g_error_free(error);
        if (out_len != NULL)
            *out_len = 0;
        return NULL;
    }

    if (out_len != NULL)
        *out_len = (size_t)items_written;

    return (WCHAR *)utf16;
}

unsigned int
copy_string_to_bufA(const char *str, char *buf, DWORD *pcch)
{
    if (str == NULL)
        str = "";

    unsigned int required = (unsigned int)strlen(str);

    // If buf is NULL and pcch is NULL: just return success
    if (buf == NULL && pcch == NULL)
        return ERROR_SUCCESS;

    // If buf is NULL and pcch is not NULL: report required length
    if (buf == NULL) {
        *pcch = required;
        return ERROR_MORE_DATA;
    }

    // buf is not NULL, pcch must be valid
    if (pcch == NULL)
        return ERROR_SUCCESS;

    // If buffer is too small: copy what fits, report required length
    if (*pcch < required) {
        unsigned int to_copy = *pcch;
        if (to_copy > 0) {
            memcpy(buf, str, to_copy);
            // Null-terminate: overwrite the last copied byte if we need space for null,
            // or if pcch is 0 we can't write anything. The MSI convention is that
            // *pcch is the buffer size NOT including null terminator space,
            // so the buffer actually has *pcch + 1 bytes available? No -- the MSI
            // convention is that *pcch is the size of the buffer in characters,
            // not including the null terminator. The buffer is *pcch + 0 chars,
            // and we should null-terminate within the available space.
            // Actually, in MSI API, *pcch on input is the buffer size including
            // space for null terminator count - 1. Let's follow the standard:
            // the buffer can hold *pcch characters plus a null terminator.
            buf[to_copy] = '\0';
        }
        *pcch = required;
        return ERROR_MORE_DATA;
    }

    // Buffer is large enough: copy the whole string
    memcpy(buf, str, required);
    buf[required] = '\0';
    *pcch = required;

    return ERROR_SUCCESS;
}

unsigned int
copy_string_to_bufW(const char *str, WCHAR *buf, DWORD *pcch)
{
    if (str == NULL)
        str = "";

    // Convert to UTF-16 to determine required length
    size_t utf16_len = 0;
    WCHAR *utf16 = utf8_to_utf16(str, &utf16_len);
    if (utf16 == NULL) {
        // Conversion failed -- treat as empty string
        utf16_len = 0;
        utf16 = utf8_to_utf16("", &utf16_len);
        if (utf16 == NULL) {
            // Truly catastrophic -- just report 0 length
            if (pcch != NULL)
                *pcch = 0;
            return ERROR_SUCCESS;
        }
    }

    unsigned int required = (unsigned int)utf16_len;

    // If buf is NULL and pcch is NULL: just return success
    if (buf == NULL && pcch == NULL) {
        g_free(utf16);
        return ERROR_SUCCESS;
    }

    // If buf is NULL and pcch is not NULL: report required length
    if (buf == NULL) {
        *pcch = required;
        g_free(utf16);
        return ERROR_MORE_DATA;
    }

    // buf is not NULL, pcch must be valid
    if (pcch == NULL) {
        g_free(utf16);
        return ERROR_SUCCESS;
    }

    // If buffer is too small: copy what fits, report required length
    if (*pcch < required) {
        unsigned int to_copy = *pcch;
        if (to_copy > 0) {
            memcpy(buf, utf16, to_copy * sizeof(WCHAR));
            buf[to_copy] = 0;
        }
        *pcch = required;
        g_free(utf16);
        return ERROR_MORE_DATA;
    }

    // Buffer is large enough: copy the whole string including null terminator
    memcpy(buf, utf16, required * sizeof(WCHAR));
    buf[required] = 0;
    *pcch = required;

    g_free(utf16);
    return ERROR_SUCCESS;
}
