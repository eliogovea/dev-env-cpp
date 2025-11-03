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
set_target_properties(egpoc_static PROPERTIES OUTPUT_NAME egpoc)
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

add_executable(egpoc_testing)

target_sources(egpoc_testing PRIVATE egpoc_testing.c)
target_sources(egpoc_testing PRIVATE egpoc_testing_main.c)
target_sources(egpoc_testing PRIVATE egpoc_memory_unittest.c)
target_link_libraries(egpoc_testing PRIVATE egpoc::egpoc_static)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set_target_properties(egpoc_testing PROPERTIES SUFFIX ".js")
    target_link_options(egpoc_testing PRIVATE $<$<STREQUAL:${CMAKE_SYSTEM_NAME},Emscripten>:-s WASM=1>)
    target_link_options(egpoc_testing PRIVATE "-s WASM=1")
    target_link_options(egpoc_testing PRIVATE "-s ENVIRONMENT=node")
    target_link_options(egpoc_testing PRIVATE "-s EXIT_RUNTIME=1")
    add_test(NAME egpoc_testing COMMAND node ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing.js)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    if (${CMAKE_CROSSCOMPILING})
        add_test(NAME egpoc_testing COMMAND wine ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing.exe)
    else ()
        add_test(NAME egpoc_testing COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing.exe)
    endif ()
else(MAKE_SYSTEM_NAME STREQUAL "Linux")
    add_test(NAME egpoc_testing COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing)
endif()
# ------------------------------------------------------------------------------
# Applications
# ------------------------------------------------------------------------------

# ------------------------------------------------------------------------------
# Applications: egpoc_app_example
# ------------------------------------------------------------------------------

add_executable(egpoc_app_example)
target_sources(egpoc_app_example PRIVATE  egpoc_app_example.c)
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

# ---------------------------------------
# Packaging: generators
# ---------------------------------------

set(CPACK_GENERATOR        "TGZ" CACHE STRING "Package generator (TGZ by default)")
set(CPACK_SOURCE_GENERATOR "TGZ" CACHE STRING "CPACK_SOURCE_GENERATOR: TGZ by default")

# ---------------------------------------
# Packaging: names
# ---------------------------------------

set(CPACK_PACKAGE_NAME                "${PROJECT_NAME}"                                                    CACHE STRING "Package name")
set(CPACK_PACKAGE_VENDOR              "${PROJECT_NAME}"                                                    CACHE STRING "Package vendor")
set(CPACK_PACKAGE_DESCRIPTION         "${PROJECT_NAME} TODO"                                               CACHE STRING "Package description")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${PROJECT_NAME} TODO"                                               CACHE STRING "Short description")
set(CPACK_PACKAGE_VERSION_MAJOR       "${PROJECT_VERSION_MAJOR}"                                           CACHE STRING "Package major version")
set(CPACK_PACKAGE_VERSION_MINOR       "${PROJECT_VERSION_MINOR}"                                           CACHE STRING "Package minor version")
set(CPACK_PACKAGE_VERSION_PATCH       "${PROJECT_VERSION_PATCH}"                                           CACHE STRING "Package patch version")
set(CPACK_PACKAGE_VERSION             "${PROJECT_VERSION}"                                                 CACHE STRING "Full package version")
set(CPACK_PACKAGE_HOMEPAGE_URL        "https://egpoc.com/egpoc"                                            CACHE STRING "Project homepage")
set(CPACK_PACKAGE_INSTALL_DIRECTORY   "egpoc-${CPACK_PACKAGE_VERSION}"                                     CACHE STRING "Installation directory")

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-wasm"                      CACHE STRING "Package file name")
else()
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION}-${CMAKE_SYSTEM_PROCESSOR}"       CACHE STRING "Package file name")
endif()

# ---------------------------------------
# Packaging: source
# ---------------------------------------

set(CPACK_SOURCE_IGNORE_FILES "")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/\\.git")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/build")

set(CPACK_SOURCE_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}" CACHE STRING "Source package file name")

# ---------------------------------------
# Packaging: common
# ---------------------------------------

set(CPACK_STRIP_FILES        ON CACHE BOOL   "Strip installed binaries")
set(CPACK_MONOLITHIC_INSTALL ON CACHE BOOL   "Use monolithic installation")
set(CPACK_THREADS            0  CACHE STRING "Use all available CPU cores for compression")
set(CPACK_VERBATIM_VARIABLES ON CACHE BOOL   "Escape variables")

# Resource files
# # TODO: setup CPACK_RESOURCE_FILE_LICENSE
# set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"     CACHE FILEPATH "License file")
# # TODO: setup CPACK_RESOURCE_FILE_README
# set(CPACK_RESOURCE_FILE_README  "${CMAKE_CURRENT_SOURCE_DIR}/README.md"   CACHE FILEPATH "ReadMe file")
# # TODO: setup CPACK_RESOURCE_FILE_WELCOME
# set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/Welcome.txt" CACHE FILEPATH "Welcome file")
# # TODO: setup CPACK_PACKAGE_DESCRIPTION_FILE
# set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/CPackDescription.txt" CACHE FILEPATH "Description file")
# # TODO: setup CPACK_PACKAGE_ICON
# set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/egpoc_icon.png" CACHE FILEPATH "Package icon")

# ----------------------------
# Packaging: debian
# ----------------------------
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Elio <elio@example.com>"   CACHE STRING "Debian package maintainer")
set(CPACK_DEBIAN_PACKAGE_DEPENDS    "libc6 (>= 2.29)"           CACHE STRING "Debian package dependencies")
set(CPACK_DEBIAN_PACKAGE_SECTION    "utils"                     CACHE STRING "Debian package section")
set(CPACK_DEBIAN_PACKAGE_PRIORITY   "optional"                  CACHE STRING "Debian package priority")
set(CPACK_DEBIAN_ARCHITECTURE       "${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "Debian package architecture")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS  ON                          CACHE BOOL   "Automatically detect shared library dependencies")


# ------------------------
# RPM specific options
# ------------------------
set(CPACK_RPM_PACKAGE_LICENSE "MIT" CACHE STRING "RPM license")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Utilities" CACHE STRING "RPM group")
set(CPACK_RPM_PACKAGE_REQUIRES "" CACHE STRING "RPM dependencies (comma separated)")
set(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}" CACHE STRING "RPM package description")
set(CPACK_RPM_PACKAGE_VENDOR "${CPACK_PACKAGE_VENDOR}" CACHE STRING "RPM package vendor")
set(CPACK_RPM_PACKAGE_URL "${CPACK_PACKAGE_HOMEPAGE_URL}" CACHE STRING "RPM project URL")
set(CPACK_RPM_PACKAGE_RELEASE "1" CACHE STRING "RPM package release")
set(CPACK_RPM_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})

# ------------------------
# NSIS specific options
# ------------------------
set(CPACK_NSIS_DISPLAY_NAME                    "${PROJECT_NAME}"                    CACHE STRING   "Project name")
set(CPACK_NSIS_INSTALL_ROOT                    "C:\\Program Files\\${PROJECT_NAME}" CACHE FILEPATH "Install directory")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON                                   CACHE BOOL     "Enable uninstall before install")
# # TODO: setup CPACK_PACKAGE_EXECUTABLES
# set(CPACK_PACKAGE_EXECUTABLES "egpoc;egpoc main executable" CACHE STRING "List of executables")
# TODO: setup CPACK_NSIS_CREATE_ICONS_EXTRA
# set(CPACK_NSIS_CREATE_ICONS_EXTRA "")
# TODO: setup CPACK_PACKAGE_ICON
# set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/installer_icon.ico")

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
