#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "egpoc_memory.h"

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    uint8_t data[16];

    egpoc_memory_arena_t arena = {
        .data = data,
        .size = sizeof(data),
        .used = 0,
    };

    void* x = egpoc_memory_arena_acquire(&arena, 16);
    assert(x != NULL);
    assert(arena.data == data);
    assert(arena.size == 16);
    assert(arena.used == 16);

    void* y = egpoc_memory_arena_acquire(&arena, 16);
    assert(y == NULL);
    assert(arena.data == data);
    assert(arena.size == 16);
    assert(arena.used == 16);

    return EXIT_SUCCESS;
}
