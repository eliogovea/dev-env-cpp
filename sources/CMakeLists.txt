cmake_minimum_required(VERSION 3.11)

project(
    utilities
    VERSION 0.0.1
    LANGUAGES CXX
)

include(GNUInstallDirs)
if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
	set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
	set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
endif()

enable_testing()
find_package(GTest REQUIRED)

# power

add_library(power INTERFACE)

set(
    power_PUBLIC_HEADERS
    power.hpp
)

set_target_properties(
    power
    PROPERTIES PUBLIC_HEADER "${power_PUBLIC_HEADERS}"
)

target_include_directories(
    power
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>

)

target_compile_features(
    power
    INTERFACE cxx_std_23
)

add_library(power::power ALIAS power)

add_executable(
    power_test
)

target_sources(
    power_test
    PRIVATE power_test.cpp
)

target_link_libraries(
    power_test
    PRIVATE power::power
            GTest::gtest_main
)

add_test(
    NAME power_test
    COMMAND power_test
)

# power app

add_executable(
    power_app
    power_app.cpp
)

set_target_properties(
    power_app
    PROPERTIES OUTPUT_NAME power
)

target_link_libraries(
    power_app
    power::power
)

# geometry

add_library(geometry INTERFACE)

set(
    geometry_PUBLIC_HEADERS
    geometry_line.hpp
    geometry_line_traits.hpp
    geometry_point.hpp
    geometry_point_operations.hpp
    geometry_point_traits.hpp
    geometry_polygon_convex.hpp
    geometry_segment.hpp
    geometry_segment_traits.hpp
    geometry_value_sign.hpp
    power.hpp
)

set_target_properties(
    geometry
    PROPERTIES PUBLIC_HEADER "${geometry_PUBLIC_HEADERS}"
)

target_include_directories(
    geometry
    INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
              $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(
    geometry
    INTERFACE cxx_std_23
)

add_library(geometry::geometry ALIAS geometry)

#

add_library(geometry_objects OBJECT)

target_sources(
    geometry_objects
    PRIVATE geometry_line.cpp
            geometry_line_traits.cpp
            geometry_point.cpp
            geometry_point_operations.cpp
            geometry_point_test.cpp
            geometry_point_traits.cpp
            geometry_polygon_convex.cpp
            geometry_segment.cpp
            geometry_segment_traits.cpp
            geometry_value_sign.cpp
)

target_link_libraries(
    geometry_objects
    PRIVATE geometry::geometry
)

add_executable(
    geometry_test
)

target_sources(
    geometry_test
    PRIVATE geometry_line_test.cpp
            geometry_line_traits_test.cpp
            geometry_point_operations_test.cpp
            geometry_point_test.cpp
            geometry_point_traits_test.cpp
            geometry_polygon_convex_test.cpp
            geometry_segment_test.cpp
            geometry_segment_traits_test.cpp
            geometry_value_sign_test.cpp
)

target_link_libraries(
    geometry_test
    PRIVATE geometry::geometry
            GTest::gtest_main
)

add_test(
    NAME geometry_test
    COMMAND geometry_test
)

# svg

add_library(svg_image_static STATIC)

target_sources(
    svg_image_static
    PRIVATE svg_image.cpp
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

target_link_libraries(
    svg_image_static
    PRIVATE geometry::geometry
)

# svg app

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
    TARGETS                   geometry
                              power
                              svg_image_static
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    TARGETS                   geometry_test
                              power_app
                              power_test
                              svg_image_testapp
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
