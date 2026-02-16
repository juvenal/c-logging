# Data Model: Fixed Source Location Logger

## Overview
Data structures and entities for the logger library with corrected source location reporting that ensures log messages show the actual file and line number where the logging function was called from.

## Core Entities

### LogLevel
Enumeration representing the severity level of log messages.

**Fields:**
- `DEBUG`: Detailed diagnostic information
- `INFO`: General information messages
- `WARN`: Warning messages about potential issues
- `ERROR`: Error conditions that may allow continued operation

**Validation:**
- Must be one of the defined enumeration values
- Used to filter log messages based on severity

### SourceLocation
Container for source code location information (file, line, function) captured at the call site.

**Fields:**
- `file_name` (string): Path to the source file where logging function was called
- `line` (integer): Line number in the source file where logging function was called
- `function_name` (string): Name of the function where logging function was called
- `column` (integer): Column number (where supported)

**Relationships:**
- Associated with a LogMessage
- Captured at the call site of the logging function

### LogMessage
Container for log message content and metadata.

**Fields:**
- `level` (LogLevel): Severity level of the message
- `timestamp` (datetime): Time when the message was logged
- `source_location` (SourceLocation): Location where the logging function was called
- `content` (string): Formatted message content
- `output_stream` (stream): Destination for the log message

**Relationships:**
- Associated with a LogLevel
- Contains SourceLocation information captured at call site

## Relationships

```
LogMessage (1) --> (1) SourceLocation
    LogMessage contains the SourceLocation captured at the call site

LogMessage (1) --> (1) LogLevel
    Each LogMessage has one LogLevel that determines its severity
```

## Validation Rules

### From Functional Requirements:
- **FR-001**: SourceLocation must contain the actual file and line where the logging function was called from
- **FR-002**: API must remain unchanged except for source location reporting
- **FR-003**: All logging levels must work correctly with fixed source location
- **FR-004**: Existing functionality must be preserved
- **FR-005**: Must work across C++ standards with C++20 focus
- **FR-006**: Must handle different calling contexts correctly
- **FR-007**: Performance must remain consistent

### Type Safety:
- Format strings must be validated at compile time where possible
- Arguments must match format string placeholders
- Invalid source locations must be handled gracefully

## State Transitions

### Message Processing:
1. Logging function called with format string and arguments
2. Source location automatically captured at call site
3. Message formatted with provided arguments
4. Message written to output stream with timestamp and source location
5. Output stream flushed

## Platform Considerations

### C++20 Features:
- `std::source_location` for accurate call-site location capture
- Default parameters to maintain API compatibility
- Template parameter packs for flexible argument handling

### Backward Compatibility:
- Fallback implementations for `std::source_location` using `__FILE__`, `__LINE__`, `__FUNCTION__`
- Conditional compilation to enable features based on C++ standard
- Maintained API compatibility through default parameters