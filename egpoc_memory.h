#pragma once

#ifndef EGPOC_MEMORY_H_
#define EGPOC_MEMORY_H_

#include <stddef.h>
#include <stdint.h>

/// @file egpoc_memory.h
/// @brief Memory abstraction for custom allocators and arenas.
///        All allocations go through user-provided acquire/release functions.

/// ---------------------------------------------------------------------------
/// Basic types
/// ---------------------------------------------------------------------------

/// @brief Opaque owner/context for allocator callbacks.
typedef void egpoc_memory_owner_t;

/// @brief Common error codes for memory operations.
typedef enum {
    EGPOC_MEMORY_ERROR_NONE = 0,
    EGPOC_MEMORY_ERROR_UNKNOWN,
    EGPOC_MEMORY_ERROR_OUT_OF_MEMORY,
    EGPOC_MEMORY_ERROR_INVALID_ARGUMENT
} egpoc_memory_error_t;

/// @brief Function signature for acquiring memory.
/// @param[in] owner User-provided owner/context.
/// @param[in] size Number of bytes to allocate.
/// @return Pointer to allocated memory, or NULL on failure.
typedef void* (*egpoc_memory_acquire_t)(egpoc_memory_owner_t* owner, size_t size);

/// @brief Function signature for releasing memory.
/// @param[in] owner User-provided owner/context.
/// @param[in] size Number of bytes to release (same as passed to acquire).
/// @param[in] data Pointer returned by acquire.
/// @return Pointer, typically NULL; used for consistency.
typedef void* (*egpoc_memory_release_t)(egpoc_memory_owner_t* owner, size_t size, void* data);

/// ---------------------------------------------------------------------------
/// Arena allocator
/// ---------------------------------------------------------------------------

/// @brief Simple linear memory arena.
typedef struct {
    uint8_t* data;  ///< Pointer to the arena memory.
    size_t   size;  ///< Total size of the arena.
    size_t   used;  ///< Bytes already allocated.
} egpoc_memory_arena_t;

/// @brief Creates a memory arena using user-provided allocator.
/// @param[in] owner Memory owner/context.
/// @param[in] acquire Allocator function.
/// @param[out] arena Pointer to arena struct to initialize.
/// @param[in] arena_size Size of the arena in bytes.
/// @return Error code.
egpoc_memory_error_t egpoc_memory_arena_create(egpoc_memory_owner_t*  owner,
                                               egpoc_memory_acquire_t acquire,
                                               egpoc_memory_arena_t*  arena,
                                               size_t                 arena_size);

/// @brief Destroys a memory arena using user-provided release function.
/// @param[in] owner Memory owner/context.
/// @param[in] release Release function.
/// @param[in,out] arena Pointer to arena to destroy.
/// @return Error code.
egpoc_memory_error_t egpoc_memory_arena_destroy(egpoc_memory_owner_t*  owner,
                                                egpoc_memory_release_t release,
                                                egpoc_memory_arena_t*  arena);

/// @brief Acquires memory from an arena.
/// @param[in,out] arena Pointer to arena.
/// @param[in] size Number of bytes to allocate.
/// @return Pointer to memory, or NULL if insufficient space.
void* egpoc_memory_arena_acquire(egpoc_memory_owner_t* arena, size_t size);

/// @brief Releases memory from an arena (linear arena typically does nothing).
/// @param[in,out] arena Pointer to arena.
/// @param[in] size Size of memory (for bookkeeping, optional).
/// @param[in] data Pointer returned by acquire.
/// @return Typically NULL.
void* egpoc_memory_arena_release(egpoc_memory_owner_t* arena, size_t size, void* data);

/// ---------------------------------------------------------------------------
/// Heap allocator
/// ---------------------------------------------------------------------------

/// @brief Standard heap acquire using system malloc aligned to pointer size.
void* egpoc_memory_heap_acquire(egpoc_memory_owner_t* owner, size_t size);

/// @brief Standard heap release using free.
void* egpoc_memory_heap_release(egpoc_memory_owner_t* owner, size_t size, void* data);

#endif  // EGPOC_MEMORY_H_
