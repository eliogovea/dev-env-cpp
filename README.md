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

### Presets

`CMakePresets.json` includes definitions for several [`CMake presets`](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html)

- `make-gcc-debug`
- `make-gcc-release`
- `make-gcc-debug-coverage`
- `make-clang-debug`
- `make-clang-debug-sanitize-address`
- `make-clang-debug-sanitize-leak`
- `make-clang-debug-sanitize-memory`
- `make-clang-debug-sanitize-thread`
- `make-clang-debug-sanitize-undefined`
- `make-clang-release`

Each `preset` creates a directory inside `build`, example: `build/make-gcc-debug`

#### Example

- Configure
  - `cmake --preset make-gcc-debug`
- Build
  - `cmake --build --preset make-gcc-debug`
- Test
  - `ctest --preset make-gcc-debug`
- Workflow
  - `cmake --workflow --preset make-gcc-debug`

#### Coverage report

```bash
cmake --workflow --preset make-gcc-debug-coverage
gcovr --verbose \
      --root . \
      --object-directory ./build/make-gcc-debug-coverage/ \
      --filter sources/ \
      --exclude sources/.*_app.cpp \
      --exclude sources/.*_test.cpp \
      --html \
      --html-details \
      --output ./build/make-gcc-debug-coverage/coverage.html
```
