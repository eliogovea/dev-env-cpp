#include "egpoc_window.h"

#include "egpoc_memory.h"
#include "egpoc_system.h"

typedef struct egpoc_window_t {
    egpoc_system_error_t error;
} egpoc_window_t;

egpoc_system_error_t egpoc_window_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         char const*            window_title,
                                         unsigned int           window_width,
                                         unsigned int           window_height,
                                         egpoc_window_t**       window)
{
    (void)memory_owner;
    (void)memory_acquire;
    (void)memory_release;
    (void)window_title;
    (void)window_width;
    (void)window_height;
    (void)window;

    return egpoc_system_error_missing;
}

egpoc_system_error_t egpoc_window_events(egpoc_window_t*       window,
                                         egpoc_window_event_t* events,
                                         size_t                events_count_limit,
                                         size_t*               events_count)
{
    (void)window;
    (void)events;
    (void)events_count_limit;
    (void)events_count;

    return egpoc_system_error_missing;
}
