cmake_minimum_required(VERSION 3.25)

project(egpoc VERSION 0.1.0)

enable_language(C)

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

add_library(egpoc_static STATIC)

target_sources(
    egpoc_static
    PRIVATE egpoc_bytes.c
            egpoc_chars.c
            egpoc_memory.c
            egpoc_network.c
            egpoc_platform.c
            egpoc_platform_wasm.c
            egpoc_platform_win32.c
            egpoc_platform_x11.c
)

target_compile_features(
    egpoc_static
    PRIVATE c_std_11
)

target_include_directories(
    egpoc_static
    PUBLIC ${CMAKE_CURRENT_LIST_DIR}
)

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(
        egpoc_static
        PRIVATE EGPOC_PLATFORM_X11
    )
    target_link_libraries(
        egpoc_static
        PUBLIC X11
    )
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(
        egpoc_static
        PRIVATE EGPOC_PLATFORM_WIN32
    )
endif ()

if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    target_compile_definitions(
        egpoc_static
        PRIVATE EGPOC_PLATFORM_WASM
    )
endif ()

add_executable(egpoc_app_example)

target_sources(
    egpoc_app_example
    PRIVATE egpoc_app_example.c
)

target_link_libraries(
    egpoc_app_example
    PRIVATE egpoc_static
)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set_target_properties(
        egpoc_app_example
        PROPERTIES SUFFIX ".html"
    )

    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_LIST_DIR}/egpoc_app_example.html
                ${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/egpoc_app_example.html
        COMMENT "Copying HTML template for egpoc app"
    )

    add_custom_target(egpoc_app_example_html ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html
    )

    add_dependencies(egpoc_app_example egpoc_app_example_html)

    target_link_options(
        egpoc_app_example
        PRIVATE "-s WASM=1"
                "-s EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap','ccall','getValue','setValue']"
                "-s USE_SDL=2"
                "-s ALLOW_MEMORY_GROWTH=1"
                "--shell-file=${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html"
    )
endif()

# Create a symlink from the preset-specific compile_commands.json for clangd and other LSP tools

get_filename_component(COMPILE_COMMAND_JSON_DIR ${PROJECT_BINARY_DIR} NAME)

add_custom_target(
    clangd_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink
            "${COMPILE_COMMAND_JSON_DIR}/compile_commands.json"
            "compile_commands.json"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/build"
    VERBATIM
    COMMENT "Linking compile_commands.json for clangd"
)
