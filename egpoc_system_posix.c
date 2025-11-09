#include "egpoc_system.h"

#define _POSIX_C_SOURCE 200809L
#include <unistd.h>

typedef struct egpoc_system_t {
    egpoc_system_error_t error;
} egpoc_system_t;

egpoc_system_error_t egpoc_system_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         egpoc_system_t**       system)
{
    (void)memory_release;

    if (!system) {
        return egpoc_system_error_argument;
    }

    *system = (egpoc_system_t*)memory_acquire(memory_owner, sizeof(egpoc_system_t));

    if (!(*system)) {
        return egpoc_system_error_acquire;
    }

    (*system)->error = egpoc_system_error_none;

    return egpoc_system_error_none;
}

egpoc_system_error_t egpoc_system_destroy(egpoc_memory_owner_t*  memory_owner,
                                          egpoc_memory_acquire_t memory_acquire,
                                          egpoc_memory_release_t memory_release,
                                          egpoc_system_t**       system)
{
    (void)memory_acquire;

    if (!system) {
        return egpoc_system_error_argument;
    }

    if (!(*system)) {
        return egpoc_system_error_argument;
    }

    *system = memory_release(memory_owner, sizeof(egpoc_system_t), *system);

    if (*system) {
        return egpoc_system_error_release;
    }

    return egpoc_system_error_none;
}

egpoc_system_error_t egpoc_system_sleep_ms(egpoc_system_t* system, unsigned int ms)
{
    (void)system;
    usleep(ms * 1000);
    return egpoc_system_error_none;
}
