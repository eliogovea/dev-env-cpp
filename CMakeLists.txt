cmake_minimum_required(VERSION 3.25)

project(egpoc VERSION 0.1.0)

enable_language(C)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    enable_language(OBJC)
endif()

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
# Libraries: egpoc_warnings
# ------------------------------------------------------------------------------

add_library(egpoc_warnings INTERFACE)
add_library(egpoc::egpoc_warnings ALIAS egpoc_warnings)

if(MSVC)
    target_compile_options(egpoc_warnings INTERFACE /W4)
    # TODO: add warnings ...
elseif(CMAKE_C_COMPILER_ID MATCHES "Clang|AppleClang")
    # https://clang.llvm.org/docs/DiagnosticsReference.html
    target_compile_options(egpoc_warnings INTERFACE -Wall)
    target_compile_options(egpoc_warnings INTERFACE -Wextra)
    target_compile_options(egpoc_warnings INTERFACE -Wpedantic)
    target_compile_options(egpoc_warnings INTERFACE -Wshadow)
    target_compile_options(egpoc_warnings INTERFACE -Wconversion)
    target_compile_options(egpoc_warnings INTERFACE -Wsign-conversion)
    target_compile_options(egpoc_warnings INTERFACE -Wfloat-equal)
    target_compile_options(egpoc_warnings INTERFACE -Wcast-align)
    target_compile_options(egpoc_warnings INTERFACE -Wstrict-prototypes)
    target_compile_options(egpoc_warnings INTERFACE -Wmissing-prototypes)
    target_compile_options(egpoc_warnings INTERFACE -Wmissing-declarations)
    target_compile_options(egpoc_warnings INTERFACE -Wmissing-variable-declarations)
    target_compile_options(egpoc_warnings INTERFACE -Wunreachable-code)
    target_compile_options(egpoc_warnings INTERFACE -Wformat=2)
    target_compile_options(egpoc_warnings INTERFACE -Wpointer-arith)
    target_compile_options(egpoc_warnings INTERFACE -Wundef)
    target_compile_options(egpoc_warnings INTERFACE -Wbad-function-cast)
    target_compile_options(egpoc_warnings INTERFACE -Wnested-externs)
    target_compile_options(egpoc_warnings INTERFACE -Wwrite-strings)
    target_compile_options(egpoc_warnings INTERFACE -Waggregate-return)
    target_compile_options(egpoc_warnings INTERFACE -Wnull-dereference)
    target_compile_options(egpoc_warnings INTERFACE -Wimplicit-fallthrough)
    target_compile_options(egpoc_warnings INTERFACE -Wdouble-promotion)
elseif(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
    target_compile_options(egpoc_warnings INTERFACE -Wall)
    target_compile_options(egpoc_warnings INTERFACE -Wextra)
    target_compile_options(egpoc_warnings INTERFACE -Wpedantic)
    target_compile_options(egpoc_warnings INTERFACE -Wshadow)
    target_compile_options(egpoc_warnings INTERFACE -Wconversion)
    target_compile_options(egpoc_warnings INTERFACE -Wsign-conversion)
    target_compile_options(egpoc_warnings INTERFACE -Wfloat-equal)
    target_compile_options(egpoc_warnings INTERFACE -Wcast-align)
    target_compile_options(egpoc_warnings INTERFACE -Wstrict-prototypes)
    target_compile_options(egpoc_warnings INTERFACE -Wmissing-prototypes)
    target_compile_options(egpoc_warnings INTERFACE -Wmissing-declarations)
    target_compile_options(egpoc_warnings INTERFACE -Wunreachable-code)
    target_compile_options(egpoc_warnings INTERFACE -Wformat=2)
    target_compile_options(egpoc_warnings INTERFACE -Wpointer-arith)
    target_compile_options(egpoc_warnings INTERFACE -Wundef)
    target_compile_options(egpoc_warnings INTERFACE -Wbad-function-cast)
    target_compile_options(egpoc_warnings INTERFACE -Wnested-externs)
    target_compile_options(egpoc_warnings INTERFACE -Wwrite-strings)
    target_compile_options(egpoc_warnings INTERFACE -Waggregate-return)
    target_compile_options(egpoc_warnings INTERFACE -Wlogical-op)
    target_compile_options(egpoc_warnings INTERFACE -Wduplicated-cond)
    target_compile_options(egpoc_warnings INTERFACE -Wnull-dereference)
    target_compile_options(egpoc_warnings INTERFACE -Wimplicit-fallthrough=5)
    target_compile_options(egpoc_warnings INTERFACE -Wdouble-promotion)
else()
    message(WARNING "Unknown C compiler: ${CMAKE_C_COMPILER_ID}")
endif()

# ------------------------------------------------------------------------------
# Libraries: egpoc_static
# ------------------------------------------------------------------------------

set(
    EGPOC_HEADERS
    egpoc_bytes.h
    egpoc_chars.h
    egpoc_debug.h
    egpoc_memory.h
    egpoc_network.h
    egpoc_window.h
    egpoc_system.h
)

set(
    EGPOC_SOURCES
    egpoc_bytes.c
    egpoc_chars.c
    egpoc_debug.c
    egpoc_memory.c
    egpoc_network.c
    egpoc_window.c
    egpoc_system.c
)

add_library(egpoc_static STATIC)
add_library(egpoc::egpoc_static ALIAS egpoc_static)
target_sources(egpoc_static PRIVATE "${EGPOC_SOURCES}")
set_target_properties(egpoc_static PROPERTIES OUTPUT_NAME egpoc)
set_target_properties(egpoc_static PROPERTIES PUBLIC_HEADER "${EGPOC_HEADERS}")
target_compile_features(egpoc_static PRIVATE c_std_11)
target_include_directories(egpoc_static PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
target_include_directories(egpoc_static PUBLIC $<INSTALL_INTERFACE:include>)
target_link_libraries(egpoc_static PRIVATE egpoc::egpoc_warnings)

# ------------------------------------------------------------------------------
# Libraries: egpoc_static (platform-specific configurations)
# ------------------------------------------------------------------------------

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    find_package(X11 REQUIRED)
    target_sources(egpoc_static PRIVATE egpoc_system_posix.c)
    target_sources(egpoc_static PRIVATE egpoc_window_x11.c)
    target_link_libraries(egpoc_static PUBLIC X11)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_sources(egpoc_static PRIVATE egpoc_system_win32.c)
    target_sources(egpoc_static PRIVATE egpoc_window_win32.c)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    target_sources(egpoc_static PRIVATE egpoc_system_wasm.c)
    target_sources(egpoc_static PRIVATE egpoc_window_wasm.c)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_sources(egpoc_static PRIVATE egpoc_system_posix.c)
    target_sources(egpoc_static PRIVATE egpoc_window_macos.m)
    target_compile_definitions(egpoc_static PRIVATE _DARWIN_C_SOURCE)
    target_link_libraries(egpoc_static PRIVATE "-framework Cocoa")
else()
    target_sources(egpoc_static PRIVATE egpoc_system_none.c)
    target_sources(egpoc_static PRIVATE egpoc_window_none.c)
endif()

# ------------------------------------------------------------------------------
# Unit Tests
# ------------------------------------------------------------------------------

add_executable(egpoc_testing)

target_sources(egpoc_testing PRIVATE egpoc_testing.c)
target_sources(egpoc_testing PRIVATE egpoc_testing_main.c)
target_sources(egpoc_testing PRIVATE egpoc_memory_unittest.c)
target_link_libraries(egpoc_testing PRIVATE egpoc::egpoc_static)
target_link_libraries(egpoc_testing PRIVATE egpoc::egpoc_warnings)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set_target_properties(egpoc_testing PROPERTIES SUFFIX ".js")
    target_link_options(egpoc_testing PRIVATE "-s WASM=1")
    target_link_options(egpoc_testing PRIVATE "-s ENVIRONMENT=node")
    target_link_options(egpoc_testing PRIVATE "-s EXIT_RUNTIME=1")
    add_test(NAME egpoc_testing COMMAND node ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing.js)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    add_test(NAME egpoc_testing COMMAND wine ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing.exe)
elseif(MAKE_SYSTEM_NAME STREQUAL "Linux")
    add_test(NAME egpoc_testing COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    add_test(NAME egpoc_testing COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/egpoc_testing)
else()
    # unknownw ?
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
target_link_libraries(egpoc_app_example PRIVATE egpoc::egpoc_warnings)

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
                              egpoc_warnings
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
set(CPACK_PACKAGE_HOMEPAGE_URL        "TODO"                                                               CACHE STRING "Project homepage")
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
# Packaging: DEB
# ----------------------------
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "TODO"                      CACHE STRING "Debian package maintainer")
set(CPACK_DEBIAN_PACKAGE_DEPENDS    "libc6 (>= 2.29)"           CACHE STRING "Debian package dependencies")
set(CPACK_DEBIAN_PACKAGE_SECTION    "utils"                     CACHE STRING "Debian package section")
set(CPACK_DEBIAN_PACKAGE_PRIORITY   "optional"                  CACHE STRING "Debian package priority")
set(CPACK_DEBIAN_ARCHITECTURE       "${CMAKE_SYSTEM_PROCESSOR}" CACHE STRING "Debian package architecture")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS  ON                          CACHE BOOL   "Automatically detect shared library dependencies")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "")
list(APPEND CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_LIST_DIR}/dist/deb/preinst")
list(APPEND CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_LIST_DIR}/dist/deb/postinst")
list(APPEND CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_LIST_DIR}/dist/deb/prerm")
list(APPEND CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_LIST_DIR}/dist/deb/postrm")

# ------------------------
# Packaging: RPM
# ------------------------
set(CPACK_RPM_PACKAGE_LICENSE "MIT" CACHE STRING "RPM license")
set(CPACK_RPM_PACKAGE_GROUP "Applications/Utilities" CACHE STRING "RPM group")
set(CPACK_RPM_PACKAGE_REQUIRES "" CACHE STRING "RPM dependencies (comma separated)")
set(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}" CACHE STRING "RPM package description")
set(CPACK_RPM_PACKAGE_VENDOR "${CPACK_PACKAGE_VENDOR}" CACHE STRING "RPM package vendor")
set(CPACK_RPM_PACKAGE_URL "${CPACK_PACKAGE_HOMEPAGE_URL}" CACHE STRING "RPM project URL")
set(CPACK_RPM_PACKAGE_RELEASE "1" CACHE STRING "RPM package release")
set(CPACK_RPM_ARCHITECTURE ${CMAKE_SYSTEM_PROCESSOR})
set(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE    "${CMAKE_SOURCE_DIR}/dist/rpm/preinst")
set(CPACK_RPM_POST_INSTALL_SCRIPT_FILE   "${CMAKE_SOURCE_DIR}/dist/rpm/postinst")
set(CPACK_RPM_PRE_UNINSTALL_SCRIPT_FILE  "${CMAKE_SOURCE_DIR}/dist/rpm/prerm")
set(CPACK_RPM_POST_UNINSTALL_SCRIPT_FILE "${CMAKE_SOURCE_DIR}/dist/rpm/postrm")

# ------------------------
# Packaging: NSIS
# ------------------------

set(CPACK_NSIS_INSTALL_ROOT                    "$PROGRAMFILES")
set(CPACK_NSIS_DISPLAY_NAME                    "egpoc")
set(CPACK_NSIS_PACKAGE_NAME                    "egpoc installer")
# # TODO: setup CPACK_NSIS_INSTALLED_ICON_NAME
# set(CPACK_NSIS_INSTALLED_ICON_NAME             "$INSTDIR\\egpoc_app_example.exe")
# # TODO: setup CPACK_NSIS_MUI_ICON
# set(CPACK_NSIS_MUI_ICON                        "${CMAKE_SOURCE_DIR}/dist/egpoc.ico")
# # TODO: setup CPACK_NSIS_MUI_UNIICON
# set(CPACK_NSIS_MUI_UNIICON                     "${CMAKE_SOURCE_DIR}/dist/egpoc.ico")
set(CPACK_NSIS_HELP_LINK                       "TODO")
set(CPACK_NSIS_URL_INFO_ABOUT                  "TODO")
set(CPACK_NSIS_CONTACT                         "TODO")
set(CPACK_NSIS_EXECUTABLES_DIRECTORY           "bin")
set(CPACK_NSIS_CREATE_ICONS_EXTRA              "CreateShortCut '$SMPROGRAMS\\egpoc\\egpoc_app_example.lnk' '$INSTDIR\\egpoc_app_example.exe'")
set(CPACK_NSIS_DELETE_ICONS_EXTRA              "Delete '$SMPROGRAMS\\egpoc\\egpoc_app_example.lnk'")
set(CPACK_NSIS_MUI_FINISHPAGE_RUN              "$INSTDIR\\egpoc_app_example.exe")
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
set(CPACK_NSIS_MODIFY_PATH                     OFF)
set(CPACK_NSIS_COMPRESSOR                      "lzma")
set(CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS       "")
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS          "")
set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS        "")
set(CPACK_NSIS_MENU_LINKS                      "")

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
