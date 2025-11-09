#include "egpoc_system.h"

#include <windows.h>

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

    *system = memory_acquire(memory_owner, sizeof(egpoc_system_t));

    if (!*system) {
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
    (void)memory_owner;
    (void)memory_acquire;
    (void)memory_release;
    (void)system;

    return egpoc_system_error_unimplemented;
}

egpoc_system_error_t egpoc_system_sleep_ms(egpoc_system_t* system, unsigned int ms)
{
    (void)system;
    (void)ms;

    if (!system) {
        return egpoc_system_error_argument;
    }

    Sleep(ms);
    return egpoc_system_error_none;

    return egpoc_system_error_unimplemented;
}
