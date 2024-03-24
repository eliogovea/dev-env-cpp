cmake_minimum_required(VERSION 3.11)

project(
    svg
    VERSION 0.0.1
    LANGUAGES CXX
)

enable_testing()
find_package(GTest REQUIRED)

### static library

add_library(svg_image_static STATIC)

target_sources(
    svg_image_static
    PRIVATE svg_image.cpp
)

set_target_properties(
    svg_image_static
    PROPERTIES OUTPUT_NAME svg_image
)

target_compile_features(
    svg_image_static
    PUBLIC cxx_std_23
)

target_include_directories(
    svg_image_static
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

### static shared

add_library(svg_image_shared SHARED)

target_sources(
    svg_image_shared
    PRIVATE svg_image.cpp
)

set_target_properties(
    svg_image_shared
    PROPERTIES OUTPUT_NAME               svg_image
               POSITION_INDEPENDENT_CODE ON
)

target_compile_features(
    svg_image_shared
    PUBLIC cxx_std_23
)

target_include_directories(
    svg_image_shared
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

### testapp

add_executable(
    svg_image_testapp
)

target_sources(
    svg_image_testapp
    PRIVATE svg_image_testapp.cpp
)

target_link_libraries(
    svg_image_testapp
    PRIVATE svg_image_static
)

# install

install(
    TARGETS                   svg_image_static
                              svg_image_shared
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    TARGETS                   svg_image_testapp
    COMPONENT                 ${PROJECT_NAME}
    CONFIGURATIONS            Debug
    EXPORT                    ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT ${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
)
