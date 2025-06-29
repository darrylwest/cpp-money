# Claude Code Context

This document provides context for the c++23 money library.  The target environments are osx and linux.

## Project Overview

A C++23 library to support currency operations primarily targeted to North America. The library provides a `Money` struct with precise decimal arithmetic using int64_t internally to avoid floating-point precision issues. Current version: 0.5.4-105.

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

- **Format code:**
  ```bash
  ./mk format
  ```

- **Watch for changes and auto-rebuild:**
  ```bash
  ./mk watch
  ```

- **Complete workflow (pull, rebuild, test):**
  ```bash
  ./mk all
  ```

## Testing

The project uses Catch2 v3 testing framework with comprehensive unit tests covering constructors, arithmetic operations, string parsing, formatting, and edge cases.

- **Run tests:**
  ```bash
  ./mk test
  ```

## Source Code

- **Header:** `include/money/money.hpp` - Main `Money` struct with comprehensive operator overloading
- **Implementation:** `src/money.cpp` - String formatting and std::formatter specialization  
- **Tests:** `tests/money_tests.cpp` - Comprehensive unit tests using Catch2

The `Money` struct provides:
- Static factory methods: `from_dollars()`, `from_string()`, `zero()`
- Arithmetic operators: `+, -, *, /`
- Comparison operators: `==, !=, <, <=, >, >=`
- State checking: `is_zero()`, `is_positive()`, `is_negative()`
- String conversion: `to_string()` with US locale formatting

## Dependencies

- **CMake 3.30+** - Build system
- **C++23 compatible compiler** - GCC 14+, Clang 18+, or equivalent
- **Catch2 v3** - Testing framework (managed via find_package)
- **clang-format** - Code formatting (optional)

## Installation

After building, install the library and headers:

```bash
cd build
make install
```

This installs:
- Static library: `libmoney.a`
- Headers: `money/money.hpp`

## Code Style

The project uses clang-format with Google style (120 column limit, 4-space indentation). Format code with:

```bash
./mk format
```

## c++23 Preferences

* functional implementations, pipelines, lambdas
* structs rather than classes



