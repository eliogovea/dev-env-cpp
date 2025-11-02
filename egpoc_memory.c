#include "egpoc_memory.h"

#include <stddef.h>
#include <stdlib.h>

void* egpoc_memory_heap_acquire(egpoc_memory_owner_t* owner, size_t size)
{
    (void)owner;
    return malloc(size);
}

void* egpoc_memory_heap_release(egpoc_memory_owner_t* owner, size_t size, void* data)
{
    (void)owner;
    (void)size;
    free(data);
    return NULL;
}

egpoc_memory_error_t egpoc_memory_arena_create(egpoc_memory_owner_t*  owner,
                                               egpoc_memory_acquire_t owner_aquire,
                                               egpoc_memory_arena_t*  arena,
                                               size_t                 arena_size)
{
    arena->data = owner_aquire(owner, arena_size);

    if (arena->data == NULL) {
        arena->size = 0;
        arena->used = 0;
        return egpoc_memory_error_unknown;
    } else {
        arena->size = arena_size;
        arena->used = 0;
        return egpoc_memory_error_none;
    }
}

egpoc_memory_error_t egpoc_memory_arena_destroy(egpoc_memory_owner_t*  owner,
                                                egpoc_memory_release_t owner_release,
                                                egpoc_memory_arena_t*  arena)
{
    arena->data = owner_release(owner, arena->size, arena->data);

    if (arena->data == NULL) {
        arena->size = 0;
        arena->used = 0;
        return egpoc_memory_error_none;
    } else {
        return egpoc_memory_error_unknown;
    }
}

void* egpoc_memory_arena_acquire(egpoc_memory_owner_t* arena, size_t size)
{
    egpoc_memory_arena_t* stack = (egpoc_memory_arena_t*)arena;

    void*  data;
    size_t used;

    if (size <= stack->size - stack->used) {
        data = stack->data + stack->size;
        used = size;
    } else {
        data = NULL;
        used = 0;
    }

    stack->used += used;

    return data;
}

void* egpoc_memory_arena_release(egpoc_memory_owner_t* arena, size_t size, void* data)
{
    (void)arena;
    (void)size;
    (void)data;

    return NULL;
}
