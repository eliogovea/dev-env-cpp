#pragma once

#ifndef EGPOC_SYSTEM_H_
#define EGPOC_SYSTEM_H_

#include <stddef.h>
#include <stdint.h>

#include "egpoc_memory.h"

/// Forward declaration of the system context.
typedef struct egpoc_system_t egpoc_system_t;

/// System-level error codes.
typedef enum {
    egpoc_system_error_none,      ///< No error occurred.
    egpoc_system_error_unknown,   ///< Unknown or unspecified error.
    egpoc_system_error_argument,  ///< Invalid argument or parameter.
    egpoc_system_error_acquire,   ///< Resource acquisition failed.
    egpoc_system_error_release,   ///< Resource release failed.
    egpoc_system_error_timeout,   ///< Operation timed out.
    egpoc_system_error_invalid,   ///< Invalid state, pointer, or handle.
    egpoc_system_error_missing,   ///< Required feature or resource missing/unimplemented.
    egpoc_system_error_oom,       ///< Out of memory.
    egpoc_system_error_mmap       ///< Memory mapping or unmapping failure.
} egpoc_system_error_t;

/// Converts an error code to a human-readable C string.
char const* egpoc_system_error_to_cstring(egpoc_system_error_t error);

/// Creates a new system context.
///
/// @param memory_owner  The memory owner structure.
/// @param memory_acquire  Function used to acquire memory.
/// @param memory_release  Function used to release memory.
/// @param system  Output pointer to the created system context.
egpoc_system_error_t egpoc_system_create(egpoc_memory_owner_t*  memory_owner,
                                         egpoc_memory_acquire_t memory_acquire,
                                         egpoc_memory_release_t memory_release,
                                         egpoc_system_t**       system);

/// Destroys an existing system context.
///
/// @param memory_owner  The memory owner structure.
/// @param memory_acquire  Function used to acquire memory.
/// @param memory_release  Function used to release memory.
/// @param system  Pointer to the system context to destroy.
egpoc_system_error_t egpoc_system_destroy(egpoc_memory_owner_t*  memory_owner,
                                          egpoc_memory_acquire_t memory_acquire,
                                          egpoc_memory_release_t memory_release,
                                          egpoc_system_t**       system);

/// Suspends the current thread for the specified number of milliseconds.
///
/// @param system  The system context.
/// @param ms  Number of milliseconds to sleep.
egpoc_system_error_t egpoc_system_sleep_ms(egpoc_system_t* system, unsigned int ms);

/// --------------------------------------------------------------------------
/// Memory Mapping API
/// --------------------------------------------------------------------------

/// Memory mapping flags.
typedef enum {
    egpoc_system_memory_flag_read      = 1u << 0,  ///< Map region with read access.
    egpoc_system_memory_flag_write     = 1u << 1,  ///< Map region with write access.
    egpoc_system_memory_flag_execute   = 1u << 2,  ///< Map region with execute access.
    egpoc_system_memory_flag_shared    = 1u << 3,  ///< Shared mapping (changes visible to others).
    egpoc_system_memory_flag_private   = 1u << 4,  ///< Private mapping (copy-on-write).
    egpoc_system_memory_flag_anonymous = 1u << 5   ///< Anonymous mapping (no backing file).
} egpoc_system_memory_flags_t;

/// Describes a memory-mapped region.
typedef struct egpoc_memory_map_t {
    void*       address;  ///< Base address of the mapped region.
    size_t      size;     ///< Size of the mapped region in bytes.
    uint64_t    offset;   ///< Offset into the source (e.g., file) if applicable.
    uint32_t    flags;    ///< Combination of egpoc_system_memory_flag_* values.
    char const* source;   ///< Optional file/device path or identifier.
} egpoc_memory_map_t;

/// Maps a memory region into the process address space.
///
/// @param system  The system context.
/// @param map  Output structure describing the mapped memory.
/// @param source  Optional file or device path (NULL for anonymous mapping).
/// @param size  Size of the region to map in bytes.
/// @param flags  Access and behavior flags (see egpoc_system_memory_flags_t).
/// @param offset  Offset into the source (usually 0 for anonymous).
egpoc_system_error_t egpoc_system_memory_map(
    egpoc_system_t* system, egpoc_memory_map_t* map, char const* source, size_t size, uint32_t flags, uint64_t offset);

/// Unmaps a previously mapped memory region.
///
/// @param system  The system context.
/// @param map  Pointer to the mapping structure to unmap.
egpoc_system_error_t egpoc_system_memory_unmap(egpoc_system_t* system, egpoc_memory_map_t* map);

/// Synchronizes a mapped region with its backing storage (if any).
///
/// @param system  The system context.
/// @param map  The mapping to synchronize.
/// @param offset  Byte offset within the mapped region.
/// @param size  Number of bytes to synchronize (0 = entire region).
egpoc_system_error_t egpoc_system_memory_sync(egpoc_system_t*           system,
                                              egpoc_memory_map_t const* map,
                                              uint64_t                  offset,
                                              size_t                    size);

#endif  // EGPOC_SYSTEM_H_
