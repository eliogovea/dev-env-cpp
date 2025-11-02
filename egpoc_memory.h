#pragma once

#ifndef EGPOC_MEMORY_H_
#define EGPOC_MEMORY_H_

#include <stddef.h>
#include <stdint.h>

typedef void egpoc_memory_owner_t;

typedef enum {
    egpoc_memory_error_none,
    egpoc_memory_error_unknown,
} egpoc_memory_error_t;

typedef void* (*egpoc_memory_acquire_t)(egpoc_memory_owner_t* owner, size_t size);

typedef void* (*egpoc_memory_release_t)(egpoc_memory_owner_t* owner, size_t size, void* data);

typedef struct {
    uint8_t* data;
    size_t   size;
    size_t   used;
} egpoc_memory_arena_t;

void* egpoc_memory_heap_acquire(egpoc_memory_owner_t* owner, size_t size);

void* egpoc_memory_heap_release(egpoc_memory_owner_t* owner, size_t size, void* data);

egpoc_memory_error_t egpoc_memory_arena_create(egpoc_memory_owner_t*  owner,
                                               egpoc_memory_acquire_t owner_aquire,
                                               egpoc_memory_arena_t*  arena,
                                               size_t                 arena_size);

egpoc_memory_error_t egpoc_memory_arena_destroy(egpoc_memory_owner_t*  owner,
                                                egpoc_memory_release_t owner_release,
                                                egpoc_memory_arena_t*  arena);

void* egpoc_memory_arena_acquire(egpoc_memory_owner_t* arena, size_t size);

void* egpoc_memory_arena_release(egpoc_memory_owner_t* arena, size_t size, void* data);

#endif  // EGPOC_MEMORY_H_
