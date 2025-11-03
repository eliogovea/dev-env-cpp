cmake_minimum_required(VERSION 3.25)

project(egpoc VERSION 0.1.0)

enable_language(C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)

include(CTest)
enable_testing()

include(GNUInstallDirs)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

# ------------------------------------------------------------------------------
# Libraries
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# Libraries: egpoc_static
# ------------------------------------------------------------------------------

set(
    EGPOC_HEADERS
    egpoc_bytes.h
    egpoc_chars.h
    egpoc_memory.h
    egpoc_network.h
    egpoc_platform.h
)

set(
    EGPOC_SOURCES
    egpoc_bytes.c
    egpoc_chars.c
    egpoc_memory.c
    egpoc_network.c
    egpoc_platform.c
    egpoc_platform_wasm.c
    egpoc_platform_win32.c
    egpoc_platform_x11.c
)

add_library(egpoc_static STATIC)
add_library(egpoc::egpoc_static ALIAS egpoc_static)
target_sources(egpoc_static PRIVATE "${EGPOC_SOURCES}")
set_target_properties(egpoc_static PROPERTIES OUTPUT_NAME   egpoc)
set_target_properties(egpoc_static PROPERTIES PUBLIC_HEADER "${EGPOC_HEADERS}")
target_compile_features(egpoc_static PRIVATE c_std_11)
target_include_directories(egpoc_static PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_include_directories(egpoc_static PUBLIC $<INSTALL_INTERFACE:include>)

# ------------------------------------------------------------------------------
# Libraries: egpoc_static (platform-specific configurations)
# ------------------------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    target_compile_definitions(egpoc_static PRIVATE EGPOC_PLATFORM_X11)
    target_link_libraries(egpoc_static PUBLIC X11)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_definitions(egpoc_static PRIVATE EGPOC_PLATFORM_WIN32)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    target_compile_definitions(egpoc_static PRIVATE EGPOC_PLATFORM_WASM)
endif()

# ------------------------------------------------------------------------------
# Unit Tests
# ------------------------------------------------------------------------------

add_executable(egpoc_memory_unittest egpoc_memory_unittest.c)
target_link_libraries(egpoc_memory_unittest PRIVATE egpoc::egpoc_static)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set_target_properties(egpoc_memory_unittest PROPERTIES SUFFIX ".js")
    target_link_options(egpoc_memory_unittest PRIVATE $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Emscripten>:-s WASM=1>)
    target_link_options(egpoc_memory_unittest PRIVATE "-s WASM=1")
    target_link_options(egpoc_memory_unittest PRIVATE "-s ENVIRONMENT=node")
    target_link_options(egpoc_memory_unittest PRIVATE "-s EXIT_RUNTIME=1")
    add_test(NAME egpoc_memory_unittest COMMAND node ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_memory_unittest.js)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (${CMAKE_CROSSCOMPILING})
        add_test(NAME egpoc_memory_unittest COMMAND wine ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_memory_unittest.exe)
    else ()
        add_test(NAME egpoc_memory_unittest COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_memory_unittest.exe)
    endif ()
else(MAKE_SYSTEM_NAME STREQUAL "Linux")
    add_test(NAME egpoc_memory_unittest COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_memory_unittest)
endif()
# ------------------------------------------------------------------------------
# Applications
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# Applications: egpoc_app_example
# ------------------------------------------------------------------------------

add_executable(egpoc_app_example egpoc_app_example.c)

target_link_libraries(egpoc_app_example PRIVATE egpoc::egpoc_static)

# ------------------------------------------------------------------------------
# Applications: egpoc_app_example (platform-specific configurations)
# ------------------------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_LIST_DIR}/egpoc_app_example.html
                ${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html
        DEPENDS ${CMAKE_CURRENT_LIST_DIR}/egpoc_app_example.html
        COMMENT "Copying HTML template for egpoc app"
    )

    add_custom_target(egpoc_app_example_html ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html)

    set_target_properties(egpoc_app_example PROPERTIES SUFFIX ".html")
    target_link_options(egpoc_app_example PRIVATE "-s WASM=1")
    target_link_options(egpoc_app_example PRIVATE "-s EXTRA_EXPORTED_RUNTIME_METHODS=['cwrap','ccall','getValue','setValue']")
    target_link_options(egpoc_app_example PRIVATE "-s USE_SDL=2")
    target_link_options(egpoc_app_example PRIVATE "-s ALLOW_MEMORY_GROWTH=1")
    target_link_options(egpoc_app_example PRIVATE "--shell-file=${CMAKE_CURRENT_BINARY_DIR}/egpoc_app_example.html")
    add_dependencies(egpoc_app_example egpoc_app_example_html)
endif()

# ------------------------------------------------------------------------------
# Install
# ------------------------------------------------------------------------------

install(
    TARGETS                   egpoc_static
                              egpoc_app_example
    EXPORT                    egpocTargets
    COMPONENT                 ${PROJECT_NAME}
    ARCHIVE       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT      egpocTargets
    FILE        egpocTargets.cmake
    NAMESPACE   egpoc::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/egpoc
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/egpocConfigVersion.cmake"
    VERSION       ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_LIST_DIR}/egpocConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/egpocConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/egpoc
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/egpocConfig.cmake"
          "${CMAKE_CURRENT_BINARY_DIR}/egpocConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/egpoc
)

# ------------------------------------------------------------------------------
# Packaging
# ------------------------------------------------------------------------------

include(CPack)

# ------------------------------------------------------------------------------
# clangd compile_commands.json symlink
# ------------------------------------------------------------------------------

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
