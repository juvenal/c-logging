# C/C++20 Logging Library

A lightweight, header-only logging library designed for easy integration and code instrumentation in both C and modern C++ projects.

## Features

- **Header-only**: Simple to drop into any project.
- **Multiple Log Levels**: `DEBUG`, `INFO`, `WARN`, `ERROR`.
- **Automatic Metadata**: Captures timestamps (with millisecond precision) and source locations (file and line number).
- **Flexible Output**: Log to `stdout`, `stderr`, or files.
- **Modern C++ Support**: The C++ version uses `std::format` and `std::source_location`.

---

## 1. C Integration (`logging.h`)

This is an **STB-style** header-only library.

### Integration
To include the implementation, define `LOGGING_IMPLEMENTATION` in **exactly one** C source file before including the header.

```c
#define LOGGING_IMPLEMENTATION
#include "src/include/logging.h"
```

In all other files, simply include the header:
```c
#include "src/include/logging.h"
```

### Usage & Instrumentation
Initialize the logger at the start of your application and use the macros for instrumentation:

```c
int main() {
    // Initialize: Log to stdout, starting at INFO level
    LOG_INIT(stdout, LOG_LEVEL_INFO);

    LOG_INFO("Application initialized.");

    if (error_condition) {
        LOG_ERROR("Failed to process data: %s", "Invalid input");
    }

    return 0;
}
```

---

## 2. C++20 Integration (`logging.hpp`)

Designed for C++20 projects leveraging modern type safety and formatting.

### Integration
Simply include the header. No implementation define is required as it uses `inline` definitions.

```cpp
#include "src/include/logging.hpp"
```

**Note**: Requires a compiler supporting C++20 (specifically `std::source_location` and `std::format`), such as Clang 13+, GCC 11+, or MSVC 19.29+.

### Usage & Instrumentation
Use `std::format` syntax for clean, type-safe logging:

```cpp
#include "src/include/logging.hpp"
#include <fstream>

int main() {
    set_log_level(LogLevel::DEBUG);

    int user_id = 101;
    log_debug("Fetching record for user_id: {}", user_id);

    try {
        // Your logic here
    } catch (const std::exception& e) {
        log_error("Exception caught: {}", e.what());
    }

    return 0;
}
```

---

## Code Instrumentation Best Practices

Using this library for instrumentation helps you understand your program's flow:

1.  **Flow Tracking**: Use `LOG_INFO` at the entry/exit points of major subsystems.
2.  **Debug Breadcrumbs**: Use `LOG_DEBUG` inside loops or conditional branches to trace logic without cluttering production logs.
3.  **Error Context**: Always include relevant variables (IDs, error codes) in your `LOG_ERROR` calls to speed up root-cause analysis.
4.  **Redirecting for Production**: In production, you can redirect logs to a file for later analysis:
    ```c
    FILE* log_file = fopen("service.log", "a");
    LOG_INIT(log_file, LOG_LEVEL_WARN);
    ```

## Building the Examples

**C Example:**
```bash
clang tests/main.c -o app
./app
```

**C++20 Example:**
```bash
clang++ -std=c++20 tests/main_cpp20.cpp -o app_cpp20
./app_cpp20
```

## Project Structure

The project follows a standard C/C++ library structure:

```
c-logging/
├── src/
│   └── include/
│       ├── logging.h    # C header file
│       └── logging.hpp  # C++ header file
├── tests/
│   ├── main.c          # C example/test
│   └── main_cpp20.cpp  # C++ example/test
├── README.md
└── ...
```
