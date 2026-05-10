#include "handle_table.h"
#include <glib.h>
#include <gsf/gsf-utils.h>
#include <stdlib.h>
#include "libmsi.h"

typedef struct {
    GObject *obj;
    HandleType type;
    gboolean in_use;
} HandleSlot;

static HandleSlot *slots = NULL;
static guint slots_capacity = 0;
static guint slots_count = 0;

// Free-list: singly linked via an index stored in the slot.
// When a slot is free, we store the next free index in a separate array.
static guint *free_list = NULL;
static guint free_list_count = 0;

static GMutex table_mutex;
static gboolean initialized = FALSE;

/*
 * Global recursive mutex that serializes ALL calls into libmsi and libgsf.
 *
 * Neither libmsi nor libgsf is thread-safe:
 *  - Every public libmsi_* function does g_return_val_if_fail(LIBMSI_IS_*)
 *    which exercises the GObject type system concurrently.
 *  - libmsi_database_new calls g_object_new(LIBMSI_TYPE_DATABASE, ...) and
 *    then opens OLE compound files via libgsf, which has no internal locking.
 *  - Summary info, query, and record operations all touch shared database
 *    state (string pools, table lists, streams) without synchronization.
 *
 * A single GRecMutex protects all libmsi/libgsf calls.  It must be recursive
 * because our W-suffix functions call their A-suffix counterparts (e.g.
 * MsiOpenDatabaseW -> MsiOpenDatabaseA), and MsiGetSummaryInformationA
 * may internally call MsiOpenDatabaseA when hDatabase == 0.
 */
static GRecMutex libmsi_mutex;

void
libmsi_global_lock(void)
{
    g_rec_mutex_lock(&libmsi_mutex);
}

void
libmsi_global_unlock(void)
{
    g_rec_mutex_unlock(&libmsi_mutex);
}

#define INITIAL_CAPACITY 64

void
handle_table_init(void)
{
    g_mutex_lock(&table_mutex);
    if (initialized) {
        g_mutex_unlock(&table_mutex);
        return;
    }

    slots_capacity = INITIAL_CAPACITY;
    slots_count = 0;
    slots = g_new0(HandleSlot, slots_capacity);
    free_list = g_new0(guint, slots_capacity);
    free_list_count = 0;
    initialized = TRUE;

    g_mutex_unlock(&table_mutex);
}

static void
ensure_capacity(void)
{
    if (free_list_count > 0)
        return;

    guint new_capacity = slots_capacity * 2;
    slots = g_renew(HandleSlot, slots, new_capacity);
    free_list = g_renew(guint, free_list, new_capacity);

    // Zero-initialize the new slots
    memset(&slots[slots_capacity], 0, sizeof(HandleSlot) * (new_capacity - slots_capacity));

    slots_capacity = new_capacity;
}

// Convert a 0-based index to a 1-based MSIHANDLE
static inline MSIHANDLE
index_to_handle(guint index)
{
    return (MSIHANDLE)(index + 1);
}

// Convert a 1-based MSIHANDLE to a 0-based index
static inline guint
handle_to_index(MSIHANDLE handle)
{
    return (guint)(handle - 1);
}

MSIHANDLE
handle_table_alloc(GObject *obj, HandleType type)
{
    if (obj == NULL)
        return 0;

    g_mutex_lock(&table_mutex);

    if (!initialized) {
        g_mutex_unlock(&table_mutex);
        return 0;
    }

    guint index;

    if (free_list_count > 0) {
        // Reuse a freed slot
        free_list_count--;
        index = free_list[free_list_count];
    } else {
        // Grow if needed
        ensure_capacity();
        index = slots_count;
        slots_count++;
    }

    g_object_ref(obj);
    slots[index].obj = obj;
    slots[index].type = type;
    slots[index].in_use = TRUE;

    MSIHANDLE handle = index_to_handle(index);

    g_mutex_unlock(&table_mutex);

    return handle;
}

GObject *
handle_table_get(MSIHANDLE handle)
{
    if (handle == 0)
        return NULL;

    g_mutex_lock(&table_mutex);

    if (!initialized) {
        g_mutex_unlock(&table_mutex);
        return NULL;
    }

    guint index = handle_to_index(handle);

    if (index >= slots_count || !slots[index].in_use) {
        g_mutex_unlock(&table_mutex);
        return NULL;
    }

    GObject *obj = slots[index].obj;
    g_object_ref(obj);

    g_mutex_unlock(&table_mutex);

    return obj;
}

GObject *
handle_table_get_typed(MSIHANDLE handle, HandleType type)
{
    if (handle == 0)
        return NULL;

    g_mutex_lock(&table_mutex);

    if (!initialized) {
        g_mutex_unlock(&table_mutex);
        return NULL;
    }

    guint index = handle_to_index(handle);

    if (index >= slots_count || !slots[index].in_use || slots[index].type != type) {
        g_mutex_unlock(&table_mutex);
        return NULL;
    }

    GObject *obj = slots[index].obj;
    g_object_ref(obj);

    g_mutex_unlock(&table_mutex);

    return obj;
}

unsigned int
handle_table_close(MSIHANDLE handle)
{
    if (handle == 0)
        return 1;

    g_mutex_lock(&table_mutex);

    if (!initialized) {
        g_mutex_unlock(&table_mutex);
        return 1;
    }

    guint index = handle_to_index(handle);

    if (index >= slots_count || !slots[index].in_use) {
        g_mutex_unlock(&table_mutex);
        return 1;
    }

    GObject *obj = slots[index].obj;
    slots[index].obj = NULL;
    slots[index].in_use = FALSE;

    // Add to free list
    free_list[free_list_count] = index;
    free_list_count++;

    g_mutex_unlock(&table_mutex);

    // Unref outside the lock to avoid potential deadlocks from destroy callbacks
    g_object_unref(obj);

    return 0;
}

unsigned int
handle_table_close_all(void)
{
    g_mutex_lock(&table_mutex);

    if (!initialized) {
        g_mutex_unlock(&table_mutex);
        return 0;
    }

    // Collect all in-use objects so we can unref them outside the lock
    guint count = 0;
    GObject **to_unref = g_new0(GObject *, slots_count);

    for (guint i = 0; i < slots_count; i++) {
        if (slots[i].in_use) {
            to_unref[count] = slots[i].obj;
            count++;
            slots[i].obj = NULL;
            slots[i].in_use = FALSE;
        }
    }

    // Reset the free list: all slots up to slots_count are now free
    free_list_count = 0;
    for (guint i = 0; i < slots_count; i++) {
        free_list[free_list_count] = i;
        free_list_count++;
    }

    g_mutex_unlock(&table_mutex);

    // Unref outside the lock
    for (guint i = 0; i < count; i++) {
        g_object_unref(to_unref[i]);
    }
    g_free(to_unref);

    return count;
}

static void
silent_log_handler(const gchar *log_domain, GLogLevelFlags log_level,
                   const gchar *message, gpointer user_data)
{
    (void)log_domain;
    (void)log_level;
    (void)message;
    (void)user_data;
}

__attribute__((constructor))
static void
handle_table_auto_init(void)
{
    gsf_init();
    g_type_ensure(libmsi_database_get_type());
    g_type_ensure(libmsi_query_get_type());
    g_type_ensure(libmsi_record_get_type());
    g_type_ensure(libmsi_summary_info_get_type());
    handle_table_init();

    // Suppress GLib critical warnings on stderr during process shutdown.
    // .NET finalizers may call MsiCloseHandle after GLib's internal state
    // is partially torn down, producing g_object_unref assertion messages
    // that cause test runners to detect a "crash".
    g_log_set_handler("GLib-GObject", G_LOG_LEVEL_CRITICAL | G_LOG_FLAG_FATAL, silent_log_handler, NULL);
    g_log_set_handler("GLib", G_LOG_LEVEL_CRITICAL | G_LOG_FLAG_FATAL, silent_log_handler, NULL);
}
