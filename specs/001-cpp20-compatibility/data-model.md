# Data Model: C++20 Compatible Logger

## Overview
Data structures and entities for the C++20 compatible logger library that maintains backward compatibility with C++11, C++14, and C++17.

## Core Entities

### LogLevel
Enumeration representing the severity level of log messages.

**Fields:**
- `DEBUG`: Detailed diagnostic information
- `INFO`: General information messages
- `WARN`: Warning messages about potential issues
- `ERROR`: Error conditions that may allow continued operation
- `NONE`: Disables all logging

**Validation:**
- Must be one of the defined enumeration values
- Used to filter log messages based on severity

### LogMessage
Container for log message format and source location information.

**Fields:**
- `fmt` (string_view): Format string with placeholders for arguments
- `loc` (source_location): Source code location where log was called (C++20) or fallback implementation (earlier standards)

**Relationships:**
- Associated with a LogLevel
- Contains variable arguments for formatting

### LogConfiguration
Runtime configuration for the logging system.

**Fields:**
- `current_log_level` (LogLevel): Minimum level to output (default: INFO)
- `log_output` (std::ostream*): Output stream for log messages (default: std::clog)

**State Transitions:**
- `set_log_level(LogLevel)`: Changes minimum output level
- `set_log_output(std::ostream&)`: Changes output destination

## Relationships

```
LogConfiguration (1) --> (Many) LogMessage
    LogConfiguration defines the current LogLevel that filters LogMessage output

LogLevel (1) --> (Many) LogMessage  
    Each LogMessage has one LogLevel that determines its severity
```

## Validation Rules

### From Functional Requirements:
- **FR-001**: All log levels must be properly defined and usable
- **FR-003**: Configuration must persist across different C++ standards
- **FR-004**: All existing logging functionality must continue to work
- **FR-007**: API interface must remain consistent across standards

### Type Safety:
- Format strings must be validated at compile time where possible
- Arguments must match format string placeholders
- Invalid log levels must be handled gracefully

## State Transitions

### Log Level Changes:
1. Initial state: LogLevel::INFO, output to std::clog
2. User calls set_log_level() → New minimum level threshold
3. User calls set_log_output() → New output destination

### Message Processing:
1. Log function called with level and message
2. Check if level >= current_log_level
3. Format message with provided arguments
4. Write formatted message to output stream with timestamp and source location
5. Flush output stream

## Platform Considerations

### C++20 Features:
- `std::source_location` for automatic file/line capture
- `std::format` for type-safe formatting
- `std::string_view` for efficient string handling

### Backward Compatibility:
- Fallback implementations for `std::source_location` using `__FILE__`, `__LINE__`
- Fallback formatting using `std::ostringstream` or `snprintf`
- Conditional compilation to enable features based on C++ standard