cmake_minimum_required(VERSION 3.11)

project(
    power
    VERSION 0.0.1
    LANGUAGES CXX
)

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
    power_unit_test
)

target_sources(
    power_unit_test
    PRIVATE power_test.cpp
)

target_link_libraries(
    power_unit_test
    PRIVATE power::power
            GTest::gtest_main
)

add_test(
    NAME    power_unit_test
    COMMAND power_unit_test
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

# install

install(
    TARGETS                   power
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    TARGETS                   power_app
                              power_unit_test
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
