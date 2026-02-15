# C-Logging Constitution
<!-- Lightweight header-only logging library for C17 and C++20 -->
<!-- Sync Impact Report:
  - Version change: 1.0.0 → 1.1.0 (MINOR - new principle added)
  - Added: Principle VI. Multiplatform Support
  - Templates: No updates required (general constitution, not project-specific)
-->

## Core Principles

### I. Header-Only Distribution
The library MUST be distributed as header-only for seamless integration. C implementations MUST follow the STB convention where `LOGGING_IMPLEMENTATION` is defined in exactly one translation unit. C++ implementations MUST use `inline` definitions to avoid ODR violations. No build system or compilation steps beyond including the header.

Rationale: Header-only distribution maximizes portability and minimizes integration friction across diverse project types.

### II. Zero External Dependencies
The library MUST NOT depend on any external libraries or packages. All functionality MUST be implemented using only the C17 or C++20 standard library. No third-party logging frameworks, formatters, or utility libraries.

Rationale: Zero dependencies ensures the library can be embedded in any project without introducing version conflicts or build complexity.

### III. Standard Compliance
C code MUST conform to the ISO/IEC 9899:2018 (C17) standard. C++ code MUST conform to ISO/IEC 14882:2020 (C++20) or later. Features beyond the standard baseline are prohibited unless explicitly optional and detectable at compile time.

Rationale: Strict standard compliance ensures predictable behavior across compilers and platforms.

### IV. Clean Implementation
Code MUST be readable, minimal, and follow the principle of least surprise. Variable names MUST be descriptive. Functions MUST have single responsibilities. The implementation MUST avoid clever tricks, macro abuse, or obfuscation. Code SHOULD fit within a single file per language (logging.h, logging.hpp).

Rationale: Clean implementation ensures the library is maintainable, debuggable, and easy to understand for contributors.

### V. Instrumentation-First Design
The library MUST be designed specifically for code instrumentation. It MUST capture source location (file, line number) automatically. It MUST provide multiple log levels (DEBUG, INFO, WARN, ERROR). It MUST include timestamps with millisecond precision. Output MUST be human-readable by default.

Rationale: Instrumentation is the primary use case; the design must prioritize developer observability.

### VI. Multiplatform Support
The library MUST support macOS and all Linux flavors. Code MUST NOT use platform-specific extensions unless guarded with compile-time checks. POSIX functions MUST be used where needed and MUST have equivalent implementations for non-POSIX platforms if portability is required. The library MUST compile with Clang, GCC, and MSVC compilers.

Rationale: Cross-platform compatibility ensures the library serves the widest possible developer community.

## Technical Standards

### C17 Implementation Requirements
- Use `timespec_get` for timestamps
- Use variadic macros for log level macros
- Use `snprintf` for timestamp formatting (no dynamic allocation)
- Use `fprintf` for output (no custom I/O abstractions)

### C++20 Implementation Requirements
- Use `std::source_location` for automatic line/file capture
- Use `std::format` for type-safe formatting
- Use `std::string_view` for string parameters
- Use `inline` variables to avoid ODR violations

### Thread Safety
- C implementation SHOULD use `localtime_r` (POSIX) or equivalent for thread-safe time conversion
- C++ implementation MAY use `std::osyncstream` for atomic output when available
- Thread safety is a quality-of-implementation concern, not a hard requirement

## Integration Guidelines

### Drop-In Inclusion
Users MUST be able to use the library by simply including the header:
```c
#include "logging.h"  /* C */
#include "logging.hpp" /* C++ */
```

### No Configuration Required
The library MUST work without configuration. Default settings MUST be sensible for immediate use (INFO level, stdout output).

### Minimal Footprint
The compiled binary impact MUST be minimal. Users MUST be able to disable logging entirely via compile-time flags if needed.

## Governance

All contributions MUST verify compliance with these principles. Complexity MUST be justified against the core goals of simplicity and zero dependencies. New features MUST NOT introduce external dependencies or require build system changes.

**Version**: 1.1.0 | **Ratified**: 2026-02-14 | **Last Amended**: 2026-02-15
