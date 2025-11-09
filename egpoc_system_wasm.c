#include "egpoc_system.h"

#include <stdint.h>

#include <emscripten/emscripten.h>

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
    emscripten_sleep(ms);
    return egpoc_system_error_none;
}

egpoc_system_error_t egpoc_system_memory_map(egpoc_system_t*     system,  //
                                             egpoc_memory_map_t* map,
                                             char const*         source,
                                             size_t              size,
                                             uint32_t            flags,
                                             uint64_t            offset)
{
    (void)system;
    (void)map;
    (void)source;
    (void)size;
    (void)flags;
    (void)offset;
    return egpoc_system_error_missing;
}

egpoc_system_error_t egpoc_system_memory_unmap(egpoc_system_t* system, egpoc_memory_map_t* map)
{
    (void)system;
    (void)map;
    return egpoc_system_error_missing;
}
