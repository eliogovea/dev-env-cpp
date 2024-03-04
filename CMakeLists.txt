cmake_minimum_required(VERSION 3.11)

project(
    cpp-project
    VERSION      0.0.1
    DESCRIPTION  "C++ Project"
    HOMEPAGE_URL "https://github.com/eliogovea/dev-env-cpp"
    LANGUAGES    CXX
)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

include(CTest)
enable_testing()

add_subdirectory(sources)

include(Package)

if (ENABLE_COVERAGE)
    if ("${COVERAGE_LCOV}" STREQUAL "")
        find_program(COVERAGE_LCOV lcov)
    endif()

    if ("${COVERAGE_GCOV}" STREQUAL "")
        find_program(COVERAGE_GCOV gcov)
    endif()

    if ("${COVERAGE_GENHTML}" STREQUAL "")
        find_program(COVERAGE_GENHTML genhtml)
    endif()

    if ("${COVERAGE_LCOV}" STREQUAL "COVERAGE_LCOV-NOTFOUND")
        message(FATAL_ERROR "COVERAGE_LCOV not found")
    endif()

    if ("${COVERAGE_GCOV}" STREQUAL "COVERAGE_GCOV-NOTFOUND")
        message(FATAL_ERROR "COVERAGE_GCOV not found")
    endif()

    if ("${COVERAGE_GENHTML}" STREQUAL "COVERAGE_GENHTML-NOTFOUND")
        message(FATAL_ERROR "COVERAGE_GENHTML not found")
    endif()

    add_custom_target(
        coverage_lcov
        COMMAND ${COVERAGE_LCOV} --gcov-tool ${COVERAGE_GCOV}
                                 --capture
                                 --no-external
                                 --exclude *_test.cpp
                                 --directory ${CMAKE_CURRENT_LIST_DIR}
                                 --output-file ${CMAKE_CURRENT_BINARY_DIR}/lcov.info
    )
    add_custom_target(
        coverage_html
        COMMAND ${COVERAGE_GENHTML} --output-directory ${CMAKE_CURRENT_BINARY_DIR}/lcov
                                    --keep-descriptions
                                    --function-coverage
                                    --branch-coverage
                                    --highlight
                                    --legend
                                    --missed
                                    ${CMAKE_CURRENT_BINARY_DIR}/lcov.info
        DEPENDS coverage_lcov
    )
endif(ENABLE_COVERAGE)
