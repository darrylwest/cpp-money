# Claude Code Context

This document provides context for the c++23 money library.  The target environments are osx and linux.

## Project Overview

A library to support currency operations primarily targeted to north america.

## Build Instructions

The project is built using `CMake` and a C++23 compatible compiler. The `mk` script automates the build process.

- **Initialize the build:**
  ```bash
  ./mk init
  ```

- **Build the project:**
  ```bash
  ./mk build
  ```

- **Clean the build:**
  ```bash
  ./mk clean
  ```

- **Remove the build directory:**
  ```bash
  ./mk clobber
  ```

## Testing

The project has two test suites: `unit_tests` and `api_tests`. The `mk` script can be used to run them.

- **Run unit tests:**
  ```bash
  ./mk test
  ```

## Source Code


## Dependencies

These dependencies are managed through `CPM.cmake` and `find_package` in the `CMakeLists.txt` file.

## Other Commands


## c++23 Preferences

* functional implementations, pipelines, lambdas
* structs rather than classes



