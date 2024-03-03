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
