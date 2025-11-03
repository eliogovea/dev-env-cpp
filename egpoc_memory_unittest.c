#include <stdint.h>
#include <stdlib.h>

#include "egpoc_memory.h"
#include "egpoc_testing.h"

EGPOC_TEST(memory, arena)
{
    uint8_t data[16];

    egpoc_memory_arena_t arena = {
        .data = data,
        .size = sizeof(data),
        .used = 0,
    };

    void* x = egpoc_memory_arena_acquire(&arena, 16);
    EGPOC_EXPECT(x != NULL);
    EGPOC_EXPECT(arena.data == data);
    EGPOC_EXPECT(arena.size == 16);
    EGPOC_EXPECT(arena.used == 16);

    void* y = egpoc_memory_arena_acquire(&arena, 16);
    EGPOC_EXPECT(y == NULL);
    EGPOC_EXPECT(arena.data == data);
    EGPOC_EXPECT(arena.size == 16);
    EGPOC_EXPECT(arena.used == 16);

    return EXIT_SUCCESS;
}
