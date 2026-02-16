# Quickstart Guide: Fixed Source Location Logger

## Overview
This guide explains how to use the logger with corrected source location reporting that ensures log messages show the actual file and line number where the logging function was called from.

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

## Key Features

### 1. Accurate Source Location
The logger now correctly captures the file name and line number where the log was called from in your code, not from within the logging library itself.

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

The updated logger maintains full API compatibility:
- Same function signatures (with default parameters for source location)
- Same enum values
- Same behavior for core functionality
- Feature-appropriate implementations per standard

## Verification

### Testing the Fix
To verify the source location fix is working:

1. Create a test file with logging calls:
```cpp
// test_logging.cpp
#include "logging.hpp"

void some_function() {
    log_info("This should show test_logging.cpp:5 as source");  // Line 5
}

int main() {
    log_info("This should show test_logging.cpp:9 as source");  // Line 9
    some_function();
    return 0;
}
```

2. Compile and run:
```bash
g++ -std=c++20 -o test_logging test_logging.cpp
./test_logging
```

3. Verify that the output shows the correct file and line numbers from your code, not from within the logging library.

## Troubleshooting

### Common Issues:
1. **Incorrect source location**: Ensure you're using the updated logging.hpp file
2. **Compiler compatibility**: The fix works with C++11 and later, with optimal support in C++20
3. **Performance**: The fix maintains the same performance characteristics as the original implementation