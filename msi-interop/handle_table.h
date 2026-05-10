#ifndef MSI_INTEROP_HANDLE_TABLE_H
#define MSI_INTEROP_HANDLE_TABLE_H

#include <glib-object.h>

// Handle type tags to distinguish what kind of object an MSIHANDLE refers to
typedef enum {
    HANDLE_DATABASE,
    HANDLE_QUERY,
    HANDLE_RECORD,
    HANDLE_SUMMARY_INFO,
} HandleType;

typedef unsigned int MSIHANDLE;

// Initialize handle table (call once at library load)
void handle_table_init(void);

// Allocate a handle for a GObject, adds a ref. Returns 0 on failure.
MSIHANDLE handle_table_alloc(GObject *obj, HandleType type);

// Get the GObject for a handle. Adds a ref; caller must g_object_unref(). Returns NULL if invalid.
GObject *handle_table_get(MSIHANDLE handle);

// Get the GObject for a handle with type check. Adds a ref; caller must g_object_unref(). Returns NULL if invalid or wrong type.
GObject *handle_table_get_typed(MSIHANDLE handle, HandleType type);

// Close a handle, unrefs the GObject. Returns 0 on success, non-zero on error.
unsigned int handle_table_close(MSIHANDLE handle);

// Close all handles, unrefs all GObjects. Returns count of handles that were open.
unsigned int handle_table_close_all(void);

// Global recursive mutex protecting ALL calls into libmsi/libgsf.
// Must be held around every libmsi API call to prevent concurrent access
// to the non-thread-safe GObject type system and libgsf storage layer.
void libmsi_global_lock(void);
void libmsi_global_unlock(void);

#endif
