# Developer Documentation

Technical reference for contributors and maintainers of the C/C++20 logging library.

---

## Standards supported

| Header | Standard | Required features |
|---|---|---|
| `logging.h` | C17 | `timespec_get`, variadic macros |
| `logging.hpp` | **C++20 only** | `std::source_location`, `std::format`, `std::vformat`, inline variables |

`logging.hpp` enforces its requirement at the preprocessor level:

```cpp
#if __cplusplus < 202002L
    #error "logging.hpp requires C++20 or later"
#endif
```

Attempting to compile with `-std=c++17` or earlier produces an immediate `#error`. There are no preprocessor fallback branches, no compatibility shims, and no standard-detection macros (`LOGGING_CPP17`, etc.) — those were removed when the multi-standard ladder was replaced by the single guard above.

---

## Technical architecture

### C implementation (`logging.h`)

Follows the **STB single-header library** convention.

- **State**: Two global static variables, `_log_output` (`FILE*`) and `_log_level` (`LogLevel`), instantiated only when `LOGGING_IMPLEMENTATION` is defined.
- **Macros**: The `LOG` macro checks the level, formats a timestamp with `snprintf` into a stack buffer, then calls `fprintf`. `__VA_ARGS__` are forwarded directly, preserving standard `printf` semantics.
- **Timestamps**: `timespec_get` (C11/C17) for wall-clock time with nanosecond resolution, displayed at millisecond precision.
- **Thread safety**: Uses `localtime` — **not thread-safe**. See roadmap item 2.

### C++20 implementation (`logging.hpp`)

The entire header is ~78 lines with no conditional compilation.

#### Global state

```cpp
inline LogLevel      current_log_level = LogLevel::INFO;
inline std::ostream* log_output        = &std::clog;
```

`inline` variables (C++17+) allow the header to be included in multiple translation units without ODR violations. No function accessors or macros alias these variables.

#### Source location capture — `LogMessage`

```cpp
struct LogMessage {
    std::string_view     fmt;
    std::source_location loc;

    template <typename S>
    constexpr LogMessage(const S& s,
                         std::source_location l = std::source_location::current())
        : fmt(s), loc(l) {}
};
```

`std::source_location::current()` as a *default function-parameter value* is evaluated at the call site, not inside the header. This is the canonical C++20 pattern for zero-overhead call-site capture. The convenience macros (`log_info`, etc.) construct `LogMessage{fmt}` at the macro expansion point, which is in the user's file — so `loc` always records the caller's file and line.

#### Formatting — `log()`

```cpp
template <typename... Args>
void log(LogLevel level, LogMessage msg, Args&&... args) {
    ...
    const std::string message = std::vformat(msg.fmt, std::make_format_args(args...));
    *log_output << std::format("[{}.{:03d}] [{}] {}:{}: {}\n", ...);
    log_output->flush();
}
```

- `std::vformat` + `std::make_format_args` handle the user's format string and arguments. Format errors (bad specifiers, wrong argument count) are **not** caught — they propagate as `std::format_error`, making bugs visible immediately.
- `localtime_r` (POSIX) is used for thread-safe time conversion.
- There is no `try/catch` block suppressing exceptions.

#### Why macros for the log calls?

`log_debug`, `log_info`, `log_warn`, `log_error` are macros, not inline functions:

```cpp
#define log_info(fmt, ...) log(LogLevel::INFO, LogMessage{fmt}, ##__VA_ARGS__)
```

If they were plain inline functions, `LogMessage` would be constructed inside the function body and `source_location::current()` would capture the library file, not the user's file. The macro forces `LogMessage{fmt}` to be constructed at the call site.

---

## Output format

```
[HH:MM:SS.mmm] [LEVEL] file:line: message
```

`LEVEL` is always 5 characters wide (`DEBUG`, `INFO `, `WARN `, `ERROR`) to keep columns aligned.

---

## Building and testing

```bash
make clean && make    # full rebuild
make test             # run C and C++20 examples

# Syntax-only checks
g++ -std=c++17 -fsyntax-only src/include/logging.hpp   # must error
g++ -std=c++20 -Wall -Wextra -Wpedantic -fsyntax-only src/include/logging.hpp  # must pass

# Source-location regression test
g++ -std=c++20 -Wall -Isrc/include tests/test_fix_verification.cpp -o /tmp/test_fix && /tmp/test_fix
```

---

## Roadmap & potential expansions

### 1. Structured logging (JSON)

Output machine-parseable logs for ingestion by ELK Stack, Splunk, Datadog, etc.

```json
{
  "timestamp": "2026-02-19T14:23:07.412Z",
  "level": "ERROR",
  "location": { "file": "src/network.cpp", "line": 88 },
  "message": "Connection failed: code 503"
}
```

**C++**: A `LogFormatter` policy passed to `log()` (Text vs. JSON strategy pattern).
**C**: Parallel macro set (e.g., `LOG_JSON_ERROR`) that formats to JSON strings.

### 2. Thread safety enhancements

**C library**: Replace `localtime` with `localtime_r` (POSIX) / `localtime_s` (Windows) to fix the static-buffer race.
**C++ library**: Wrap stream writes in `std::osyncstream` (`<syncstream>`, C++20) to guarantee atomic output of a complete log line when multiple threads log simultaneously.

### 3. Asynchronous logging

Decouple the `log()` call from I/O to minimize latency on critical paths (real-time loops, hot paths).

- Lock-free SPSC or MPMC ring buffer.
- `log()` pushes the pre-formatted message and timestamp onto the queue.
- A dedicated background thread drains the queue to disk or network.

### 4. Multiple sinks

Allow a single `log()` call to write to several destinations simultaneously (console + file + network).

**C++**: Change `log_output` from `std::ostream*` to `std::vector<std::ostream*>`.
**C**: Accept a callback `void (*log_callback)(const char* msg)` alongside the `FILE*`.

### 5. ANSI color support

Improve readability during local development.

- Detect TTY via `isatty()`.
- Inject ANSI escape codes per level (e.g., `\033[31m` for ERROR).
- Strip codes automatically when output is not a TTY (file, pipe).

### 6. Log rotation

Prevent log files from growing without bound.

- Track bytes written to the current file.
- At threshold (e.g., 10 MB), close, rename to `app.log.1`, and open fresh `app.log`.
