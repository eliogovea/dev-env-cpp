#include <stdio.h>
#include <stdlib.h>

#include "egpoc_memory.h"
#include "egpoc_platform.h"
#include "egpoc_system.h"

int main(int argc, char* argv[])
{
    (void)argc;

    egpoc_memory_arena_t arena       = {.data = NULL, .size = 0, .used = 0};
    egpoc_memory_error_t arena_error = EGPOC_MEMORY_ERROR_NONE;

    arena_error = egpoc_memory_arena_create(NULL,  //
                                            egpoc_memory_heap_acquire,
                                            &arena,
                                            1 << 20);

    if (arena_error != EGPOC_MEMORY_ERROR_NONE) {
        fprintf(stderr, "Error. egpoc_memory_arena_create failed\n");
        return EXIT_FAILURE;
    }

    egpoc_system_t*      system       = NULL;
    egpoc_system_error_t system_error = egpoc_system_error_none;

    system_error = egpoc_system_create(&arena,  //
                                       &egpoc_memory_arena_acquire,
                                       &egpoc_memory_arena_release,
                                       &system);

    if (system_error != egpoc_system_error_none) {
        fprintf(stderr,                                     //
                "Error. egpoc_window_create failed: %s\n",  //
                egpoc_system_error_to_cstring(system_error));
        return EXIT_FAILURE;
    }

    egpoc_window_t*      platform       = NULL;
    egpoc_window_error_t platform_error = egpoc_window_error_none;

    platform_error = egpoc_window_create(&arena,  //
                                         &egpoc_memory_arena_acquire,
                                         &egpoc_memory_arena_release,
                                         argv[0],
                                         256,
                                         256,
                                         &platform);

    if (platform_error != egpoc_window_error_none) {
        fprintf(stderr, "Error. egpoc_window_create failed\n");
        return EXIT_FAILURE;
    }

    size_t events_capacity = 16;

    egpoc_window_event_t* events = egpoc_memory_arena_acquire(&arena, events_capacity * sizeof(egpoc_window_event_t));

    if (!events) {
        fprintf(stderr, "Error. egpoc_memory_arena_acquire failed\n");
        return EXIT_FAILURE;
    }

#ifndef EGPOC_PLATFORM_WASM
    while (1) {
        size_t               events_count = 0;
        egpoc_window_error_t events_error = egpoc_window_events(platform, events, events_capacity, &events_count);

        if (events_error != egpoc_window_error_none) {
            fprintf(stderr, "Error. egpoc_window_events failed\n");
            continue;
        }

        if (events_count > 0) {
            fprintf(stderr, "events count: %zu\n", events_count);
        }

        // small sleep so we don't spin the CPU
        egpoc_system_sleep_ms(system, 16);  // ~60Hz
    }
#endif  // EGPOC_PLATFORM_WASM

    return EXIT_SUCCESS;
}
