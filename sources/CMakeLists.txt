cmake_minimum_required(VERSION 3.11)

project(
    power
    VERSION 0.0.1
    LANGUAGES CXX
)

add_library(${PROJECT_NAME} INTERFACE)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES OUTPUT_NAME ${PROJECT_NAME}
               PUBLIC_HEADER power.hpp
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

# test

include(CTest)
enable_testing()

find_package(GTest)

add_executable(
    ${PROJECT_NAME}_test
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

# app

add_executable(
    ${PROJECT_NAME}_app
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

# install

include(GNUInstallDirs)

install(
    TARGETS                   ${PROJECT_NAME}
    EXPORT                    ${PROJECT_NAME}
    COMPONENT                 ${PROJECT_NAME}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}
    LIBRARY       DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME       DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(
    EXPORT ${PROJECT_NAME}
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME}
)
