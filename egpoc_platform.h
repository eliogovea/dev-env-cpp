#pragma once

#ifndef EGPOC_PLATFORM_H_
#define EGPOC_PLATFORM_H_

#include "egpoc_memory.h"

typedef struct egpoc_window_t egpoc_window_t;

typedef struct {
    int quit;
    int mouseX;
    int mouseY;
    int mouseL;
    int mouseR;
} egpoc_window_event_t;

typedef enum {
    egpoc_window_error_none,
    egpoc_window_error_unknown,
} egpoc_window_error_t;

egpoc_window_error_t egpoc_window_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         char const*            window_title,
                                         unsigned int           window_width,
                                         unsigned int           window_height,
                                         egpoc_window_t**       platform);

egpoc_window_error_t egpoc_window_events(egpoc_window_t*       platform,
                                         egpoc_window_event_t* events,
                                         size_t                events_count_limit,
                                         size_t*               events_count);

#endif  // EGPOC_PLATFORM_H_
