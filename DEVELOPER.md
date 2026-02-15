# Developer Documentation

This document provides technical details on the internal workings of the C and C++20 logging libraries, along with a roadmap for future improvements and architectural considerations.

## Technical Architecture

### 1. C Implementation (`logging.h`)

The C library follows the **STB single-header library** convention.

*   **State Management**:
    *   Uses global static variables `_log_output` (`FILE*`) and `_log_level` (`LogLevel`) to maintain state.
    *   These are instantiated only when `LOGGING_IMPLEMENTATION` is defined.
*   **Macro Magic**:
    *   The `LOG` macro handles the logic: checking the log level, generating the timestamp, and calling `fprintf`.
    *   It uses `snprintf` to format the timestamp into a stack-allocated buffer (`_time_buf`) to avoid dynamic allocation.
    *   **Variadic Macros**: `__VA_ARGS__` are passed directly to `fprintf`, preserving standard `printf` behavior.
*   **Timestamp Precision**:
    *   Uses `timespec_get` (C11) to obtain wall-clock time with nanosecond precision, truncated to milliseconds for display.
    *   *Note*: The current implementation uses `localtime`, which returns a pointer to a static buffer. This makes the standard implementation **not thread-safe**.

### 2. C++20 Implementation (`logging.hpp`)

The C++ library leverages modern language features for type safety and performance.

*   **Modern Features**:
    *   `std::source_location`: Automatically captures file name and line number at the call site without macros.
    *   `std::format`: Provides type-safe, Python-like string formatting, superior to `printf` in safety and extensibility.
    *   `std::string_view`: Reduces string copying when passing format strings.
*   **State Management**:
    *   Uses `inline` global variables (`current_log_level`, `log_output`) to allow the header to be included in multiple translation units without ODR (One Definition Rule) violations.
*   **Thread Safety**:
    *   Uses `localtime_r` for thread-safe time structure generation.
    *   *Constraint*: While the time generation is safe, writing to `std::ostream` without a mutex or `std::osyncstream` (C++20) may result in interleaved output if multiple threads log simultaneously.

---

## Roadmap & Potential Expansions

The following features are identified as high-value improvements for future iterations.

### 1. Structured Logging (JSON)

**Goal**: Output logs in a machine-parseable format for ingestion by tools like ELK Stack, Splunk, or Datadog.

**Proposed Format**:
```json
{
  "timestamp": "2023-10-27T10:00:00.123Z",
  "level": "ERROR",
  "location": {
    "file": "main.cpp",
    "line": 42
  },
  "message": "Connection timeout",
  "context": {
    "user_id": 101,
    "retry_count": 3
  }
}
```

**Implementation Strategy**:
*   **C**: Create a parallel set of macros (e.g., `LOG_JSON_INFO`) that formatting the output as a JSON string.
*   **C++**: Create a `LogFormatter` strategy pattern. The `log` function could accept a formatting policy (Text vs. JSON).

### 2. Thread Safety Enhancements

**C Library**:
*   Replace `localtime` with `localtime_r` (POSIX) or `localtime_s` (Windows) to fix race conditions on the time buffer.
*   Introduce an optional mutex callback for the `LOG` macro to lock the file stream during the `fprintf` call.

**C++ Library**:
*   Wrap the output stream insertion in `std::osyncstream` (available in C++20 headers `<syncstream>`) to guarantee atomic output of the entire log line.

### 3. Asynchronous Logging

**Goal**: Decouple the logging call from the I/O operation to minimize latency in the critical path (e.g., real-time loops).

**Implementation Strategy**:
*   Implement a lock-free ring buffer (SPSC or MPMC).
*   The `log` function pushes the message and timestamp to the queue.
*   A background thread wakes up to drain the queue and write to disk/network.

### 4. Multiple Sinks (Outputs)

**Goal**: Allow logging to multiple destinations simultaneously (e.g., Console for development + File for persistence).

**Implementation Strategy**:
*   **C++**: Change `log_output` from a single pointer to a `std::vector<std::ostream*>`. Iterate through all registered streams when logging.
*   **C**: Allow registering a custom callback function `void (*log_callback)(const char* msg)` instead of just a raw `FILE*`.

### 5. Color Support (ANSI)

**Goal**: Improve readability during local development.

**Implementation Strategy**:
*   Detect if the output stream is a TTY.
*   Inject ANSI escape codes (e.g., `\033[31m` for Red/Error) into the format string based on the `LogLevel`.
*   Ensure these codes are stripped when logging to files.

### 6. Log Rotation

**Goal**: Prevent log files from consuming infinite disk space.

**Implementation Strategy**:
*   Track the number of bytes written to the file.
*   When a threshold (e.g., 10MB) is reached, close the current file, rename it (e.g., `app.log.1`), and open a fresh `app.log`.
