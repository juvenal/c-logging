# Feature Specification: Fix Source Location Reporting in Logger

**Feature Branch**: `002-fix-source-location`
**Created**: February 15, 2026
**Status**: Draft
**Input**: User description: "The current logging functions are reporting file and line of the original message from the logger help header file itself, not the effective file and line code that are calling the inline logger function. It should place the effective file and line code from where the call of the inline logger output is, not the inline function. It's a bug that needs to be fixed."

## Clarifications

### Session 2026-02-15

- Q: What is the priority approach for implementing the source location fix across different C++ standards? → A: Focus primarily on C++20 implementation with fallbacks for older standards
- Q: What implementation approach should be used for the source location fix? → A: Use modern C++20 features like `std::source_location` when available with fallbacks to `__FILE__` and `__LINE__` for older standards
- Q: What scope of testing should be performed for the source location fix? → A: Test all documented calling contexts (regular functions, methods, static methods, lambdas) to ensure comprehensive coverage of the fix
- Q: What performance impact should be maintained after the fix? → A: Maintain the same performance characteristics as the original implementation, as logging should not significantly impact application performance
- Q: How should edge cases be handled in the implementation? → A: The implementation should correctly handle all documented edge cases (macros, inline functions, lambdas, templates) to ensure comprehensive fix coverage

## User Scenarios & Testing *(mandatory)*

### User Story 1 - Accurate Source Location Reporting (Priority: P1)

As a C++ developer using the logging library, I want the log messages to show the actual file and line number where the logging function was called from, rather than showing the location within the logging library itself, so that I can quickly identify where issues occur in my own code.

**Why this priority**: This is critical for debugging effectiveness. When log messages show incorrect source locations, developers waste time trying to find the wrong location in the codebase.

**Independent Test**: When I call a logging function from my code at a specific file and line, the resulting log message should display that exact file and line number, not the internal location within the logging library.

**Acceptance Scenarios**:

1. **Given** I have a C++ file "my_app.cpp" with a log call on line 42, **When** I execute the log call, **Then** the log output shows "my_app.cpp:42" as the source location
2. **Given** I have multiple files calling the same logging function, **When** each file executes a log call, **Then** each log message shows the correct source file and line from where it was called

---

### User Story 2 - Consistent Debugging Experience (Priority: P2)

As a developer debugging an application, I want all log messages to consistently show accurate source locations regardless of the logging level (debug, info, warn, error), so that I can rely on the log output for troubleshooting.

**Why this priority**: Consistency in logging behavior is important for building trust in the debugging process and reducing cognitive load when interpreting logs.

**Independent Test**: All logging functions (log_debug, log_info, log_warn, log_error) correctly report the caller's source location rather than the internal logging implementation location.

**Acceptance Scenarios**:

1. **Given** I call different logging functions from the same location, **When** each function executes, **Then** all log messages show the same correct source location
2. **Given** I call logging functions from different files and lines, **When** each function executes, **Then** each log message shows its respective correct source location

---

### User Story 3 - Backward Compatibility (Priority: P3)

As a user of the logging library, I want the fix for source location reporting to not break existing functionality or change the API, so that my current code continues to work without modifications.

**Why this priority**: Maintaining backward compatibility ensures that existing applications can receive the fix without requiring code changes or risking regressions.

**Independent Test**: After applying the fix, all existing logging calls continue to work exactly as before, except that they now report correct source locations.

**Acceptance Scenarios**:

1. **Given** I have existing code using the logging library, **When** I update to the fixed version, **Then** all logging functionality works as before with correct source locations
2. **Given** I have automated tests that verify log output format, **When** I run them with the fixed library, **Then** they pass with the exception of source location accuracy improvements

---

### Edge Cases

- The system MUST correctly handle logging calls from macros defined in different files
- The system MUST correctly handle logging calls from inline functions or lambdas
- The system MUST correctly handle logging calls from template instantiations

## Requirements *(mandatory)*

### Functional Requirements

- **FR-001**: System MUST report the actual file and line number where the logging function is called from, not from within the logging implementation
- **FR-002**: System MUST maintain the same logging API and behavior, only correcting the source location reporting
- **FR-003**: System MUST work correctly with all logging levels (DEBUG, INFO, WARN, ERROR)
- **FR-004**: System MUST preserve all existing functionality and not introduce breaking changes
- **FR-005**: System MUST work correctly across different C++ standards (C++11, C++14, C++17, C++20), with primary implementation focus on C++20 and fallbacks for older standards
- **FR-006**: System MUST correctly handle logging calls from different contexts (regular functions, methods, static methods, lambdas) with comprehensive testing across all these contexts
- **FR-007**: System MUST maintain the same performance characteristics as the original implementation, with no significant impact on application performance

### Key Entities

- **Log Message**: Container for log content, level, timestamp, and source location information
- **Source Location**: File name and line number where the logging function was called from
- **Logging Function**: API entry point for creating log messages (log_debug, log_info, log_warn, log_error)

## Success Criteria *(mandatory)*

### Measurable Outcomes

- **SC-001**: 100% of log messages display the correct source file and line number where the logging call originated
- **SC-002**: All existing logging functionality continues to work without any regressions
- **SC-003**: The fix works consistently across all supported C++ standards (C++11, C++14, C++17, C++20), with optimal implementation for C++20 and functional fallbacks for older standards
- **SC-004**: No changes required to existing client code to benefit from the corrected source location reporting