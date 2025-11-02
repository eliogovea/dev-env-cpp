#pragma once

#ifndef EGPOC_PLATFORM_H_
#define EGPOC_PLATFORM_H_

#include "egpoc_memory.h"

typedef void egpoc_platform_t;

typedef struct {
    int quit;
    int mouseX;
    int mouseY;
    int mouseL;
    int mouseR;
} egpoc_platform_event_t;

typedef enum {
    egpoc_platform_error_none,
    egpoc_platform_error_unknown,
} egpoc_platform_error_t;

egpoc_platform_error_t egpoc_platform_create(egpoc_memory_owner_t*  memory_owner,
                                             egpoc_memory_acquire_t memory_acquire,
                                             egpoc_memory_release_t memory_release,
                                             char const*            window_title,
                                             unsigned int           window_width,
                                             unsigned int           window_height,
                                             egpoc_platform_t**     platform);

egpoc_platform_error_t egpoc_platform_events(egpoc_platform_t*       platform,
                                             egpoc_platform_event_t* events,
                                             size_t                  events_count_limit,
                                             size_t*                 events_count);
#endif  // EGPOC_PLATFORM_H_
