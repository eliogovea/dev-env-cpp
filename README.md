# C++ development environment

Template for a `CMake` based `C++` projects using `vscode`'s `devcontainer` to have a reproducible development environment

## Devcontainer

`.devcontainer/Dockerfile` includes a set tools to compile, debug and test the project, for example:

- gcc
- g++
- gdb
- clang
- clang++
- clang-format
- clang-tidy
- cmake
- ccache
- git
- python3
- valgrind

`devcontainer.json` configures `vscode` settings and plugins

## CMake Project

### Toolchains

`cmake/toolchain` directory includes a set of [`cmake` `toolchain` files](https://cmake.org/cmake/help/latest/manual/cmake-toolchains.7.html) to define all parameters you want to use while building a `CMake` project.

- `cmake/toolchain/gcc.cmake`
  - Use the system `gcc` to compile
  - Enable warnings for all build types
    - `-Wall`
    - `-Wextra`
    - `-Wpedantic`
  - Enable warnings as errors for all build types
  - Set flags for differetn build types ``
    - `CMAKE_BUILD_TYPE=Debug`
      - -O0
      - -g
    - `CMAKE_BUILD_TYPE=Release`
      - -O2
    - `CMAKE_BUILD_TYPE=RelWithDebInfo`
      - -O2
      - -g
    - `CMAKE_BUILD_TYPE=RelMinSize`
      - Os
- `cmake/toolchain/llvm.cmake`
  - Use the system `clang++` to compile
  - Enable warnings for all build types
    - `-Wall`
    - `-Wextra`
    - `-Wpedantic`
  - Enable warnings as errors for all build types
  - Set flags for differetn build types ``
    - `CMAKE_BUILD_TYPE=Debug`
      - -O0
      - -g
    - `CMAKE_BUILD_TYPE=Release`
      - -O2
    - `CMAKE_BUILD_TYPE=RelWithDebInfo`
      - -O2
      - -g
    - `CMAKE_BUILD_TYPE=RelMinSize`
      - Os
- `cmake/toolchain/gcc-coverage.cmake`
  - generage code coverage

### Presets

`CMakePresets.json` includes definitions for several [`CMake presets`](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html). Each preset is named after the toolchain file used in the config step

- `gcc`
- `llvm`
- `gcc-coverage`

Each `preset` creates a directory in `build`, example: `build/gcc` has the build using `gcc` toolchain

#### Example

- Configure
  - `cmake --preset gcc`
- Build
  - `cmake --build --preset gcc`
- Test
  - `ctest --preset gcc`
- Workflow
  - `cmake --workflow --preset gcc`

#### Coverage report

```bash
cmake --workflow --preset gcc-coverage
gcovr --verbose \
      --root . \
      --object-directory ./build/gcc-coverage/ \
      --filter sources/ \
      --exclude sources/.*_app.cpp \
      --exclude sources/.*_test.cpp \
      --html \
      --html-details \
      --output ./build/gcc-coverage/coverage.html
```

#### Package

Example using `gcc` preset

- `cpack --preset gcc-package-TGZ`
- `cpack --preset gcc-package-DEB`
- `cpack --preset gcc-package-ALL`
