#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "egpoc_memory.h"
#include "egpoc_platform.h"

int main(int argc, char* argv[])
{
    (void)argc;

    egpoc_memory_arena_t arena       = {.data = NULL, .size = 0, .used = 0};
    egpoc_memory_error_t arena_error = egpoc_memory_error_none;

    arena_error = egpoc_memory_arena_create(NULL,  //
                                            egpoc_memory_heap_acquire,
                                            &arena,
                                            1 << 20);

    if (arena_error != egpoc_memory_error_none) {
        fprintf(stderr, "Error. egpoc_memory_arena_create failed\n");
        return EXIT_FAILURE;
    }

    egpoc_platform_t*      platform       = NULL;
    egpoc_platform_error_t platform_error = egpoc_platform_error_none;

    platform_error = egpoc_platform_create(&arena,  //
                                           &egpoc_memory_arena_acquire,
                                           &egpoc_memory_arena_release,
                                           argv[0],
                                           256,
                                           256,
                                           &platform);

    if (platform_error != egpoc_platform_error_none) {
        fprintf(stderr, "Error. egpoc_platform_create failed\n");
        return EXIT_FAILURE;
    }

    size_t events_capacity = 16;

    egpoc_platform_event_t* events
        = egpoc_memory_arena_acquire(&arena, events_capacity * sizeof(egpoc_platform_event_t));

    if (!events) {
        fprintf(stderr, "Error. egpoc_memory_arena_acquire failed\n");
        return EXIT_FAILURE;
    }

#ifndef EGPOC_PLATFORM_WASM
    while (1) {
        size_t                 events_count = 0;
        egpoc_platform_error_t events_error = egpoc_platform_events(platform, events, events_capacity, &events_count);

        if (events_error != egpoc_platform_error_none) {
            fprintf(stderr, "Error. egpoc_platform_events failed\n");
            continue;
        }

        if (events_count > 0) {
            fprintf(stderr, "events count: %zu\n", events_count);
        }

        // small sleep so we don't spin the CPU
        usleep(16000);  // ~60Hz
    }
#endif  // EGPOC_PLATFORM_WASM

    return EXIT_SUCCESS;
}
