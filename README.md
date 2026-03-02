# C/C++20 Logging Library

A lightweight, header-only logging library for code instrumentation in C and C++20 projects.

## Features

- **Header-only**: Drop a single file into any project — no build steps required.
- **Multiple log levels**: `DEBUG`, `INFO`, `WARN`, `ERROR`.
- **Automatic metadata**: Timestamps with millisecond precision; source location (file and line) captured automatically at the call site.
- **Flexible output**: Log to `stdout`, `stderr`, any file, or any `std::ostream`.
- **Type-safe formatting**: The C++ header uses `std::format` syntax.

---

## 1. C Integration (`logging.h`)

This is an **STB-style** header-only library.

### Requirements

- C17 or later
- A C17-conforming compiler (GCC 8+, Clang 6+, MSVC 19.14+)

### Integration

Define `LOGGING_IMPLEMENTATION` in **exactly one** C source file before including the header:

```c
#define LOGGING_IMPLEMENTATION
#include "src/include/logging.h"
```

In all other files, include the header without the define:

```c
#include "src/include/logging.h"
```

### Usage

```c
int main() {
    LOG_INIT(stdout, LOG_LEVEL_INFO);

    LOG_INFO("Application started.");
    LOG_ERROR("Failed to process data: %s", "Invalid input");

    return 0;
}
```

---

## 2. C++20 Integration (`logging.hpp`)

### Requirements

- **C++20 or later** — enforced at compile time via `#error`.
- GCC 11+, Clang 13+, or MSVC 19.29+.

### Integration

Include the header. No implementation define or build step needed.

```cpp
#include "src/include/logging.hpp"
```

### Usage

```cpp
#include "src/include/logging.hpp"
#include <fstream>

int main() {
    // Default: INFO level, output to std::clog.
    log_info("Application started. value={}", 42);

    // Change level at runtime.
    set_log_level(LogLevel::DEBUG);
    log_debug("Debug enabled. pi={:.2f}", 3.14159);

    // Redirect to a file.
    std::ofstream log_file("app.log");
    set_log_output(log_file);
    log_warn("Low disk space: {}%", 7);
    log_error("Connection failed: code {}", 503);

    // Back to stderr.
    set_log_output(std::clog);
    log_info("Logging to stderr again.");

    return 0;
}
```

### Output format

```
[HH:MM:SS.mmm] [LEVEL] file.cpp:line: message
```

Example:
```
[14:23:07.412] [ERROR] src/network.cpp:88: Connection failed: code 503
```

### API reference

| Symbol | Description |
|---|---|
| `LogLevel::DEBUG / INFO / WARN / ERROR / NONE` | Log level enum |
| `log_debug(fmt, ...)` | Log at DEBUG level |
| `log_info(fmt, ...)` | Log at INFO level |
| `log_warn(fmt, ...)` | Log at WARN level |
| `log_error(fmt, ...)` | Log at ERROR level |
| `set_log_level(LogLevel)` | Change the minimum output level |
| `set_log_output(std::ostream&)` | Redirect output to any stream |

Format strings use `std::format` / `{}` syntax. See [cppreference — std::format](https://en.cppreference.com/w/cpp/utility/format/format).

---

## Building the examples

```bash
make        # builds both C and C++20 examples
make test   # builds and runs both
make clean  # removes binaries and log files
```

Manual compilation:

```bash
# C example
gcc -std=c17 -Isrc/include tests/main.c -o app

# C++20 example
g++ -std=c++20 -Isrc/include tests/main_cpp20.cpp -o app_cpp20
```

---

## Project structure

```
c-logging/
├── src/
│   └── include/
│       ├── logging.h    # C17 header (STB-style)
│       └── logging.hpp  # C++20 header (inline, ~78 lines)
├── tests/
│   ├── main.c                   # C example
│   ├── main_cpp20.cpp           # C++20 example
│   └── test_fix_verification.cpp # Source-location regression test
├── CMakeLists.txt
├── Makefile
└── README.md
```

---

## Code instrumentation tips

1. **Flow tracking**: log at `INFO` on entry/exit of major subsystems.
2. **Debug breadcrumbs**: log at `DEBUG` inside loops or branches — gated out at `INFO` in production with zero cost.
3. **Error context**: always include relevant variables (IDs, codes) in `log_error` to speed up root-cause analysis.
4. **File logging in production**:
   ```cpp
   std::ofstream log_file("service.log", std::ios::app);
   set_log_level(LogLevel::WARN);
   set_log_output(log_file);
   ```
