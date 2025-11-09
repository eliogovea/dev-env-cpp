#pragma once

#ifndef EGPOC_SYSTEM_H_
#define EGPOC_SYSTEM_H_

#include "egpoc_memory.h"

typedef struct egpoc_system_t egpoc_system_t;

typedef enum {
    egpoc_system_error_none,
    egpoc_system_error_unknown,
    egpoc_system_error_argument,
    egpoc_system_error_acquire,
    egpoc_system_error_release,
    egpoc_system_error_timeout,
    egpoc_system_error_invalid,
    egpoc_system_error_unimplemented,
} egpoc_system_error_t;

char const* egpoc_system_error_to_cstring(egpoc_system_error_t error);

egpoc_system_error_t egpoc_system_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         egpoc_system_t**       system);

egpoc_system_error_t egpoc_system_destroy(egpoc_memory_owner_t*  memory_owner,
                                          egpoc_memory_acquire_t memory_acquire,
                                          egpoc_memory_release_t memory_release,
                                          egpoc_system_t**       system);

egpoc_system_error_t egpoc_system_sleep_ms(egpoc_system_t* system, unsigned int ms);

#endif  // EGPOC_SYSTEM_H_
