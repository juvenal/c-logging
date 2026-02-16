# Quickstart Guide: C++20 Compatible Logger

## Overview
This guide explains how to use the C++20 compatible logger that maintains backward compatibility with C++11, C++14, and C++17.

## Getting Started

### Basic Usage
```cpp
#include "logging.hpp"

int main() {
    // Default usage - logs INFO and above to std::clog
    log_info("Application started");
    log_debug("This won't appear by default");
    
    // Enable debug logging
    set_log_level(LogLevel::DEBUG);
    log_debug("Debug message now visible");
    
    // Change output destination
    set_log_output(std::cout);
    log_info("Now logging to stdout");
    
    return 0;
}
```

### Advanced Usage
```cpp
#include "logging.hpp"

int main() {
    // Format with arguments (C++20 std::format style)
    log_info("Processing {} items", 42);
    log_warn("Temperature is {:.2f} degrees", 23.456);
    
    // File logging
    std::ofstream log_file("app.log");
    set_log_output(log_file);
    log_error("Error occurred: code {}", 500);
    
    return 0;
}
```

## Compilation Instructions

### For C++20:
```bash
g++ -std=c++20 -Wall -Wextra -Wpedantic -o app main.cpp
# or
clang++ -std=c++20 -Wall -Wextra -Wpedantic -o app main.cpp
```

### For C++17, C++14, C++11:
```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -o app main.cpp
g++ -std=c++14 -Wall -Wextra -Wpedantic -o app main.cpp
g++ -std=c++11 -Wall -Wextra -Wpedantic -o app main.cpp
```

## Key Features

### 1. Automatic Source Location
The logger automatically captures file name and line number where the log was called.

### 2. Type-Safe Formatting
Uses `std::format` in C++20, with fallback implementations for earlier standards.

### 3. Multiple Log Levels
- `log_debug()` - Detailed diagnostic information
- `log_info()` - General information messages
- `log_warn()` - Warning messages
- `log_error()` - Error conditions

### 4. Runtime Configuration
- Change log level with `set_log_level()`
- Change output stream with `set_log_output()`

## Backward Compatibility

The updated logger maintains full API compatibility across C++ standards:
- Same function signatures
- Same enum values
- Same behavior for core functionality
- Feature-appropriate implementations per standard

## Troubleshooting

### Common Issues:
1. **Missing format header**: Ensure you're using C++20 or later for std::format
2. **Compiler warnings**: Use `-Wall -Wextra -Wpedantic` flags as specified
3. **Linker errors**: This is a header-only library, no linking required

### Verification:
To verify clean compilation:
```bash
# Test with C++20
g++ -std=c++20 -Wall -Wextra -Wpedantic -fsyntax-only logging.hpp

# Test with C++17
g++ -std=c++17 -Wall -Wextra -Wpedantic -fsyntax-only logging.hpp
```