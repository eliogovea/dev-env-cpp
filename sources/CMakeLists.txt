cmake_minimum_required(VERSION 3.11)

project(
    utilities
    VERSION 0.0.1
    LANGUAGES CXX
)

set(
    UTILITIES_PUBLIC_HEADERS
    index_coordinator.hpp
    linux_file_descriptor.hpp
    math_power.hpp
)

add_library(utilities_static STATIC)

target_sources(
    utilities_static
    PRIVATE index_coordinator.cpp
            linux_file_descriptor.cpp
            math_power.cpp
)

set_target_properties(
    utilities_static
    PROPERTIES OUTPUT_NAME               utilities
               PUBLIC_HEADER             "${UTILITIES_PUBLIC_HEADERS}"
               POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    utilities_static
    PRIVATE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_compile_features(
    utilities_static
    PUBLIC cxx_std_23
)

add_library(utilities::utilities_static ALIAS utilities_static)

add_library(utilities_shared SHARED)

target_sources(
    utilities_shared
    PRIVATE index_coordinator.cpp
            linux_file_descriptor.cpp
            math_power.cpp
)


set_target_properties(
    utilities_shared
    PROPERTIES OUTPUT_NAME               utilities
               PUBLIC_HEADER             "${UTILITIES_PUBLIC_HEADERS}"
               POSITION_INDEPENDENT_CODE ON
)

target_include_directories(
    utilities_shared
    PRIVATE $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>
            $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)

target_compile_features(
    utilities_shared
    PUBLIC cxx_std_23
)

add_library(utilities::utilities_shared ALIAS utilities_shared)

# test

include(CTest)
enable_testing()

find_package(GTest)

add_executable(utilities_test)

target_sources(
    utilities_test
    PRIVATE index_coordinator_test.cpp
            linux_file_descriptor_test.cpp
            math_power_test.cpp
)

target_link_libraries(
    utilities_test
    PRIVATE utilities::utilities_static
            GTest::gtest_main
)

add_test(
    NAME    utilities_test
    COMMAND utilities_test
)

# fuzzing

if (ENABLE_FUZZING)
    add_executable(index_coordinator_fuzzer)

    target_sources(
        index_coordinator_fuzzer
        PRIVATE index_coordinator_fuzzer.cpp
    )

    target_compile_options(
        index_coordinator_fuzzer
        PRIVATE -coverage
                -fsanitize=fuzzer,undefined,address
    )

    target_link_libraries(
        index_coordinator_fuzzer
        PRIVATE utilities_static
                -coverage
                -fsanitize=fuzzer,undefined,address
    )
endif(ENABLE_FUZZING)

# app

add_executable(math_power_app)

target_sources(
    math_power_app
    PRIVATE math_power_app.cpp
)

set_target_properties(
    math_power_app
    PROPERTIES OUTPUT_NAME math_power
)

target_link_libraries(
    math_power_app
    PRIVATE utilities::utilities_static
)

# install

include(GNUInstallDirs)

install(
    TARGETS                   utilities_static
                              utilities_shared
                              math_power_app
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT ${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
