cmake_minimum_required(VERSION 3.11)

project(
    cpp-project
    VERSION      0.0.1
    DESCRIPTION  "C++ Project"
    HOMEPAGE_URL "https://github.com/eliogovea/dev-env-cpp"
    LANGUAGES    CXX
)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_LIST_DIR}/cmake/modules)

include(CTest)
enable_testing()

add_subdirectory(sources)

include(Package)
