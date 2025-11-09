#include "egpoc_system.h"

#define _POSIX_C_SOURCE 200809L

#include <sys/mman.h>

#include <fcntl.h>
#include <string.h>
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

egpoc_system_error_t egpoc_system_memory_map(
    egpoc_system_t* system, egpoc_memory_map_t* map, char const* file, size_t size, uint32_t flags, uint64_t offset)
{
    (void)system;

    if (!map) {
        return egpoc_system_error_argument;
    }

    if (size == 0) {
        return egpoc_system_error_argument;
    }

    int prot = 0;

    if (flags & egpoc_system_memory_flag_read) {
        prot |= PROT_READ;
    }

    if (flags & egpoc_system_memory_flag_write) {
        prot |= PROT_WRITE;
    }

    if (flags & egpoc_system_memory_flag_execute) {
        prot |= PROT_EXEC;
    }

    int mapflags = 0;
    if (flags & egpoc_system_memory_flag_shared) {
        mapflags |= MAP_SHARED;
    }

    if (flags & egpoc_system_memory_flag_private) {
        mapflags |= MAP_PRIVATE;
    }

    if (flags & egpoc_system_memory_flag_anonymous) {
        mapflags |= MAP_ANONYMOUS;
    }

    int fd         = -1;
    int file_oflag = (flags & egpoc_system_memory_flag_write) ? O_RDWR : O_RDONLY;

    if (file && !(flags & egpoc_system_memory_flag_anonymous)) {
        fd = open(file, file_oflag, 0);
        if (fd < 0) {
            return egpoc_system_error_missing;
        }
    }

    void* addr = mmap(NULL, size, prot, mapflags, fd, (off_t)offset);

    if (fd >= 0) {
        close(fd);
    }

    if (addr == MAP_FAILED) {
        return egpoc_system_error_mmap;
    }

    map->address = addr;
    map->size    = size;
    map->offset  = offset;
    map->flags   = flags;
    map->source  = file ? file : NULL;

    return egpoc_system_error_none;
}

egpoc_system_error_t egpoc_system_memory_unmap(egpoc_system_t* system, egpoc_memory_map_t* map)
{
    (void)system;
    if (!map) {
        return egpoc_system_error_argument;
    }

    if (!map->address) {
        return egpoc_system_error_argument;
    }

    if (map->size == 0) {
        return egpoc_system_error_argument;
    }

    if (munmap(map->address, map->size) != 0) {
        return egpoc_system_error_mmap;
    }

    memset(map, 0, sizeof(*map));
    return egpoc_system_error_none;
}

egpoc_system_error_t egpoc_system_memory_sync(egpoc_system_t*           system,
                                              egpoc_memory_map_t const* map,
                                              uint64_t                  offset,
                                              size_t                    size)
{
    (void)system;
    (void)map;
    (void)offset;
    (void)size;
    return egpoc_system_error_missing;
}
