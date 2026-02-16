# Research: Fix Source Location Reporting in Logger

## Overview
Analysis of the current source location reporting bug in the logger library and identification of the most effective approach to fix it, ensuring that log messages show the actual file and line number where the logging function was called from, rather than showing the location within the logging library itself.

## Current State Analysis

### Problem Identification
The current logging functions are reporting file and line information from within the logger header file itself, rather than capturing the actual location where the logging function was called from in the user's code. This happens because the source location is being captured inside the logging implementation rather than at the call site.

### Root Cause
In the current implementation, when logging functions use `std::source_location::current()` (C++20) or similar mechanisms, they capture the location within the logging function itself, not the location where the logging function was called from. This is because the source location is captured inside the implementation of the logging function rather than being passed from the call site.

## Required Changes

### 1. Parameter-Based Source Location
The most effective approach is to modify the logging functions to accept source location as a parameter with a default value of the current location. This way, the location is captured at the call site rather than inside the function.

```cpp
// Current problematic approach:
void log_info(const std::string_view fmt, Args&&... args) {
    auto loc = std::source_location::current();  // Captures location inside this function
    // ...
}

// Correct approach:
void log_info(const std::string_view fmt, 
             const std::source_location& loc = std::source_location::current(),
             Args&&... args) {
    // loc now captures the call site location
    // ...
}
```

### 2. Template Function Approach
For more complex scenarios, we can use a template wrapper that automatically captures the source location at the call site:

```cpp
template<typename... Args>
void log_impl(LogLevel level, 
             const std::source_location& loc,
             const std::string_view fmt, 
             Args&&... args) {
    // Implementation that uses the passed location
}

template<typename... Args>
void log_info(const std::string_view fmt, 
             const std::source_location& loc = std::source_location::current(),
             Args&&... args) {
    log_impl(LogLevel::INFO, loc, fmt, std::forward<Args>(args)...);
}
```

### 3. Fallback Implementation for Older Standards
For C++ standards prior to C++20, we need to implement a fallback mechanism:

```cpp
#if __cplusplus >= 202002L
    // Use std::source_location
    using source_location = std::source_location;
#else
    // Custom implementation using __FILE__, __LINE__, and __FUNCTION__
    struct source_location {
        const char* file_ = __builtin_FILE();
        const char* function_ = __builtin_FUNCTION();
        int line_ = __builtin_LINE();
        
        static source_location current() noexcept {
            return {};
        }
        
        const char* file_name() const { return file_; }
        const char* function_name() const { return function_; }
        int line() const { return line_; }
    };
#endif
```

## Implementation Approaches

### Approach 1: Parameter Modification (Recommended)
Modify all logging functions to accept source location as a parameter with a default value. This is the cleanest approach that maintains API compatibility while fixing the core issue.

**Pros:**
- Maintains backward compatibility
- Fixes the source location reporting issue completely
- Clean and straightforward implementation
- Works well with C++20 std::source_location

**Cons:**
- Requires changes to function signatures (though default parameters maintain compatibility)
- May affect inlining in some cases

### Approach 2: Macro-Based Solution
Use macros to inject source location information at the call site.

**Pros:**
- Guaranteed call-site location capture
- No changes to function signatures needed

**Cons:**
- Changes the API from functions to macros
- Loses the benefits of type safety that functions provide
- More complex maintenance

### Approach 3: Wrapper Pattern
Create wrapper functions that capture location and pass it to implementation functions.

**Pros:**
- Maintains clean separation of concerns
- Preserves function-based API
- Flexible implementation

**Cons:**
- Adds an extra layer of indirection
- Slightly more complex code structure

## Decision: Parameter Modification Approach

### Rationale:
The parameter modification approach is recommended because it:
- Maintains API compatibility through default parameters
- Directly addresses the root cause of the issue
- Works seamlessly with C++20's std::source_location
- Provides fallback mechanisms for older standards
- Keeps the function-based API that offers better type safety than macros

### Alternatives considered:
- Macro-based solution: Would require changing the API fundamentally
- Wrapper pattern: Would add unnecessary complexity for this specific issue

## Testing Strategy

### Test Scenarios:
1. Regular function calls
2. Method calls (instance and static)
3. Lambda expressions
4. Template instantiations
5. Macro expansions
6. Inline function calls

### Verification:
- Log output should show the correct file and line number from the call site
- All existing functionality should continue to work
- Performance should remain consistent with the original implementation