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
- `make-clang-debug-xray`
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

#### Static analysis

`cmake --workflow --preset static-analysis` runs:

- `clang-tidy`

#### Coverage report

`cmake --workflow --preset make-gcc-debug-coverage` generates:

- `LCOV` coverage file in `build/make-gcc-debug-coverage/lcov.info`
- `HTML` coverage report in `build/make-gcc-debug-coverage/lcov/index.html`

changes to `master` branch triggers an action to deploy the coverage report to [github pages](https://eliogovea.github.io/dev-env-cpp/master/coverage/index.html)

changes to `dev` branch triggers an action to deploy the coverage report to [github pages](https://eliogovea.github.io/dev-env-cpp/dev/coverage/index.html)

#### Documentation

```
mkdir -p build
doxygen Doxyfile
```

changes to `master` branch triggers an action to deploy the documentation to [github pages](https://eliogovea.github.io/dev-env-cpp/master/documentation/index.html)

changes to `dev` branch triggers an action to deploy the documentation to [github pages](https://eliogovea.github.io/dev-env-cpp/dev/documentation/index.html)
