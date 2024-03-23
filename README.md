# C++ Development Environment Template

## Overview

C++ development environment for seting up a consistent and reproducible experience across different machines. It uses Docker to encapsulate standard development tools, including C++ compilers and debugging tools, within a container

## Getting Started

1. **Prerequisites:**

- Install [Docker](https://docs.docker.com/get-docker/)
- Install [Visual Studio Code](https://code.visualstudio.com/).
- Install [Remote - Containers extension](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers).

2. **Clone the repository:**

- ```bash
  git clone https://github.com/eliogovea/dev-env-cpp.git
  ```

3. **Open in a DevContainer:**

- Open VSCode,
- Open the cloned project directory.
- Accept VSCode's prompt to reopen the project in a container.

## Tools

### Docker

The Docker container comes equipped with essential tools:

- GCC
- Clang
- Python3
- Git
- Make
- CMake
- Ccache
- Lcov
- Valgrind

These tools support a wide range of development tasks from compiling and building to code analysis and version control.

### VSCode Settings and Extensions

Includes a pre-defined settings and extensions for C++ development with features like auto-formatting, linting, and intelligent code completion.

## Sample Project Structure with CMake Integration

A sample project structure is provided, demonstrating best practices and facilitating immediate productivity. It uses CMake for project configuration, supporting various build types and targets, and includes workflows for tasks like library creation and unit testing.

### CMake presets

This project defines a set of presets, in most cases using the following name pattern: `generator-compiler-type[-details]`

#### Configure

```bash
cmake --preset make-gcc-debug
```

To list all presets use:

```bash
cmake --list-presets
```

#### Build

```bash
cmake --build --preset make-gcc-debug
```

To list all presets use:

```bash
cmake --build --list-presets
```

#### Test

```bash
ctest --preset make-gcc-debug
```

To list all presets use:

```bash
ctest --list-presets
```

#### Package

```bash
cpack --preset make-gcc-debug
```

To list all presets use:

```bash
cpack --list-presets
```

#### Workflow

CMake workflows presets allow running multiple steps (configure, build, test, package) with a single command using: `cmake --workflow PRESET_NAME`

- `make-gcc-debug`
- `make-clang-debug`
- `make-clang-debug-sanitize-address`
- `make-clang-debug-sanitize-leak`
- `make-clang-debug-sanitize-memory`
- `make-clang-debug-sanitize-thread`
- `make-clang-debug-sanitize-undefined`
- `make-gcc-release`
- `make-clang-release`
- `make-gcc-debug-coverage`
- `static-analysis`

```bash
cmake --workflow --preset make-clang-debug
```

To list all presets use:

```bash
cmake --workflow --list-presets
```

## GitHub Workflows

This template includes pre-configured GitHub Actions workflows to automate various development and maintenance tasks.

### Unit Test Code Coverage Report and Deployment to GitHub Pages

The workflow for generating and deploying code coverage reports is defined in `.github/workflows/coverage.yml`. It automates the following tasks:

1. **Setup Environment:** Initializes the environment with necessary dependencies for building the project and generating the coverage report.
2. **Build and Test:** Compiles the project, runs unit tests, and generates a code coverage report.
3. **Deploy to GitHub Pages:** Deploys the generated code coverage report to GitHub Pages, making it accessible to reviewers and contributors.

### Documentation Generation and Deployment with Doxygen

This template facilitates the automatic generation and deployment of your project's documentation using Doxygen and GitHub Actions.

