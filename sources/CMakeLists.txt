cmake_minimum_required(VERSION 3.11)

project(
    math
    VERSION 0.0.1
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} INTERFACE)

set(
    PROJECT_PUBLIC_HEADERS
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
    ${PROJECT_NAME}
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
               PUBLIC_HEADER "${PROJECT_PUBLIC_HEADERS}"
)

target_include_directories(
    ${PROJECT_NAME}
    INTERFACE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
              $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_compile_features(
    ${PROJECT_NAME}
    INTERFACE cxx_std_20
)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# Target to check all headers
add_library(${PROJECT_NAME}_objects STATIC)

target_sources(
    ${PROJECT_NAME}_objects
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
    ${PROJECT_NAME}_objects
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
)

# tests

include(CTest)
enable_testing()

find_package(GTest)

add_executable(
    ${PROJECT_NAME}_test
)

target_sources(
    ${PROJECT_NAME}_test
    PRIVATE geometry_line_test.cpp
            geometry_line_traits_test.cpp
            geometry_point_operations_test.cpp
            geometry_point_test.cpp
            geometry_point_traits_test.cpp
            geometry_polygon_convex_test.cpp
            geometry_segment_test.cpp
            geometry_segment_traits_test.cpp
            geometry_value_sign_test.cpp
            power_test.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_test
    PRIVATE ${PROJECT_NAME}::${PROJECT_NAME}
            GTest::gtest_main
)

add_test(
    NAME ${PROJECT_NAME}_test
    COMMAND ${PROJECT_NAME}_test
)

# apps

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
    ${PROJECT_NAME}::${PROJECT_NAME}
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

# install

include(GNUInstallDirs)

install(
    TARGETS                   ${PROJECT_NAME}
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    TARGETS                   ${PROJECT_NAME}_test
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
