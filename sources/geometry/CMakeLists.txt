cmake_minimum_required(VERSION 3.11)

project(
    geometry
    VERSION 0.0.1
    LANGUAGES CXX
)

enable_testing()
find_package(GTest REQUIRED)

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
    geometry_unit_test
)

target_sources(
    geometry_unit_test
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
    geometry_unit_test
    PRIVATE geometry::geometry
            GTest::gtest_main
)

add_test(
    NAME    geometry_unit_test
    COMMAND geometry_unit_test
)

if (ENABLE_FUZZ_TESTING)
    function(add_fuzz_test fuzzer_name fuzzer_source)
        add_executable(${fuzzer_name})

        target_sources(
            ${fuzzer_name}
            PRIVATE ${fuzzer_source}
        )

        target_link_libraries(
            ${fuzzer_name}
            PRIVATE ${PROJECT_NAME}
                    -coverage
                    -fsanitize=fuzzer,undefined,address
        )

        target_compile_features(
            ${fuzzer_name}
            PRIVATE cxx_std_20
        )

        target_compile_options(
            ${fuzzer_name}
            PRIVATE -fsanitize=fuzzer,undefined,address
        )
    endfunction()

    add_fuzz_test(geometry_fuzzer_convex geometry_fuzzer_convex.cpp)
endif()

install(
    TARGETS                   geometry
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    TARGETS                   geometry_unit_test
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
