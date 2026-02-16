# API Contract: Fixed Source Location Logger

## Overview
API specification for the logger library with corrected source location reporting that ensures log messages show the actual file and line number where the logging function was called from.

## Public Interface

### Enumerations

#### LogLevel
Defines the severity levels for log messages.

```cpp
enum class LogLevel {
    DEBUG,  // Detailed diagnostic information
    INFO,   // General information messages
    WARN,   // Warning messages about potential issues
    ERROR,  // Error conditions that may allow continued operation
};
```

### Global Variables

#### current_log_level
Current minimum log level for output.

```cpp
inline LogLevel current_log_level = LogLevel::INFO;
```

#### log_output
Current output stream for log messages.

```cpp
inline std::ostream* log_output = &std::clog;
```

### Functions

#### set_log_level
Sets the minimum log level for output.

```cpp
void set_log_level(LogLevel level);
```

**Parameters:**
- `level`: The minimum LogLevel to output

**Behavior:**
- Sets the global current_log_level
- Messages with lower priority will be filtered out

#### set_log_output
Sets the output stream for log messages.

```cpp
void set_log_output(std::ostream& stream);
```

**Parameters:**
- `stream`: Reference to the output stream

**Behavior:**
- Sets the global log_output pointer
- All subsequent log messages will be written to this stream

### Logging Functions

#### log
Generic logging function with format string and source location.

```cpp
template <typename... Args>
void log(LogLevel level, 
         const std::string_view fmt,
         const std::source_location& loc = std::source_location::current(),
         Args&&... args);
```

**Parameters:**
- `level`: LogLevel for this message
- `fmt`: Format string with placeholders for arguments
- `loc`: Source location (automatically captured at call site)
- `args`: Variable arguments for format string

**Behavior:**
- Checks if level >= current_log_level
- Formats the message using std::format (C++20) or fallback
- Outputs timestamp, level, source location, and formatted message
- Flushes the output stream
- Source location correctly captured from call site

#### Convenience Logging Functions
Functions for specific log levels with the same signature pattern.

```cpp
template <typename... Args>
void log_debug(const std::string_view fmt,
               const std::source_location& loc = std::source_location::current(),
               Args&&... args);

template <typename... Args>
void log_info(const std::string_view fmt,
              const std::source_location& loc = std::source_location::current(),
              Args&&... args);

template <typename... Args>
void log_warn(const std::string_view fmt,
              const std::source_location& loc = std::source_location::current(),
              Args&&... args);

template <typename... Args>
void log_error(const std::string_view fmt,
               const std::source_location& loc = std::source_location::current(),
               Args&&... args);
```

## Expected Behavior

### Message Format
Log messages follow the format:
```
[HH:MM:SS.mmm] [LEVEL] filename:line_number: formatted_message
```

### Source Location Accuracy
- The filename and line_number should reflect the location where the logging function was called from in the user's code
- NOT the location within the logging library implementation

### Filtering
Messages with LogLevel below current_log_level are silently dropped.

### Threading
The logger is not thread-safe by design. Concurrent calls may interleave output.

### Error Handling
If formatting fails, the logger should gracefully degrade or use a fallback mechanism.

## Compatibility Guarantees

### Across C++ Standards
- Same function signatures (with default parameters for source location)
- Same enum values
- Same behavior for core functionality
- Feature-appropriate implementations per standard

### Backward Compatibility
- Existing code using the logger should continue to work
- No breaking changes to public API (default parameters maintain compatibility)
- Same default behavior
- Correct source location reporting