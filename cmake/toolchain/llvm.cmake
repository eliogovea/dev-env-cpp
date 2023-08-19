set(CMAKE_CXX_COMPILER clang++-17)

set(CMAKE_CXX_FLAGS_INIT "" CACHE INTERNAL "")

# https://clang.llvm.org/docs/DiagnosticsReference.html#wall
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wall")

# https://clang.llvm.org/docs/DiagnosticsReference.html#wextra
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wextra")

# https://clang.llvm.org/docs/DiagnosticsReference.html#wpedantic
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Wpedantic")

# https://clang.llvm.org/docs/UsersManual.html#cmdoption-werror
set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT} -Werror")

# CMAKE_BUILD_TYPE=Debug

set(CMAKE_CXX_FLAGS_DEBUG "" CACHE INTERNAL "")

# https://clang.llvm.org/docs/CommandGuide/clang.html#cmdoption-o0
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

# https://clang.llvm.org/docs/UsersManual.html#cmdoption-g
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

# CMAKE_BUILD_TYPE=Release

set(CMAKE_CXX_FLAGS_RELEASE "" CACHE INTERNAL "")

# https://clang.llvm.org/docs/CommandGuide/clang.html#cmdoption-o0
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# CMAKE_BUILD_TYPE=RelWithDebInfo

set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "" CACHE INTERNAL "")

# https://clang.llvm.org/docs/CommandGuide/clang.html#cmdoption-o0
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2")

# https://clang.llvm.org/docs/UsersManual.html#cmdoption-g
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g")

# CMAKE_BUILD_TYPE=MinSizeRel

set(CMAKE_CXX_FLAGS_MINSIZEREL "" CACHE INTERNAL "")

# https://clang.llvm.org/docs/CommandGuide/clang.html#cmdoption-o0
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os")
