# Implementation Plan: Fix Source Location Reporting in Logger

**Branch**: `002-fix-source-location` | **Date**: February 15, 2026 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/002-fix-source-location/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/commands/plan.md` for the execution workflow.

## Summary

Fix the source location reporting bug in the logger library so that log messages show the actual file and line number where the logging function was called from, rather than showing the location within the logging library itself. The implementation will focus on C++20 features like std::source_location with fallbacks for older standards.

## Technical Context

**Language/Version**: C++20, C++17, C++14, C++11 with primary focus on C++20 implementation and fallbacks for older standards
**Primary Dependencies**: Zero external dependencies (following constitution principle of zero external dependencies)
**Storage**: N/A (header-only library)
**Testing**: Comprehensive testing across all calling contexts (regular functions, methods, static methods, lambdas) and C++ standards
**Target Platform**: macOS and Linux (following constitution multiplatform support requirement)
**Project Type**: Single header-only library (C++)
**Performance Goals**: Maintain same performance characteristics as original implementation (no significant impact on application performance)
**Constraints**: Must maintain same API and behavior, only correcting source location reporting; work across C++ standards with C++20 focus; handle edge cases like macros, inline functions, lambdas, and templates
**Scale/Scope**: Single header file (logging.hpp) modification to fix source location reporting

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

### Gate Analysis:
- **I. Header-Only Distribution**: ✅ PASSED - Implementation will remain header-only in logging.hpp
- **II. Zero External Dependencies**: ✅ PASSED - No external dependencies will be added
- **III. Standard Compliance**: ✅ PASSED - Will comply with C++ standards (C++20 with fallbacks for older standards)
- **IV. Clean Implementation**: ✅ PASSED - Changes will follow clean implementation practices
- **V. Instrumentation-First Design**: ✅ PASSED - Core logging functionality preserved while improving source location capture
- **VI. Multiplatform Support**: ✅ PASSED - Supporting macOS and Linux as required

All constitutional principles are satisfied by this implementation approach.

## Project Structure

### Documentation (this feature)

```text
specs/002-fix-source-location/
├── plan.md              # This file (/speckit.plan command output)
├── research.md          # Phase 0 output (/speckit.plan command)
├── data-model.md        # Phase 1 output (/speckit.plan command)
├── quickstart.md        # Phase 1 output (/speckit.plan command)
├── contracts/           # Phase 1 output (/speckit.plan command)
└── tasks.md             # Phase 2 output (/speckit.tasks command - NOT created by /speckit.plan)
```

### Source Code (repository root)
```text
logging.hpp              # Main header file to be updated for correct source location reporting
logging.h                # C header file (may need minor updates for consistency)
main_cpp20.cpp           # C++20 test application
main.c                   # C test application
app_cpp20                # Compiled C++20 test executable
app                      # Compiled C test executable
```

**Structure Decision**: Single header-only library structure maintained. The primary change will be to logging.hpp to ensure correct source location reporting using C++20 std::source_location with fallbacks for older standards. Test applications will be used to verify the fix works correctly.

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

| Violation | Why Needed | Simpler Alternative Rejected Because |
|-----------|------------|-------------------------------------|
| [e.g., 4th project] | [current need] | [why 3 projects insufficient] |
| [e.g., Repository pattern] | [specific problem] | [why direct DB access insufficient] |
