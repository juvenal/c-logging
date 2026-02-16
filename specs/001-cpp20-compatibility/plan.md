# Implementation Plan: C++20 Compatibility Verification and Adjustment

**Branch**: `001-cpp20-compatibility` | **Date**: February 15, 2026 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/001-cpp20-compatibility/spec.md`

**Note**: This template is filled in by the `/speckit.plan` command. See `.specify/templates/commands/plan.md` for the execution workflow.

## Summary

Adjust the lightweight logger C++ header to clearly support C++20 standard features, primarily focused on macOS and then Linux. Modify the header file to achieve clean compilation of the test app with C++20 standard, while maintaining backward compatibility with C++11, C++14, and C++17.

## Technical Context

**Language/Version**: C++20, C++17, C++14, C++11 with primary focus on C++20 compatibility
**Primary Dependencies**: Zero external dependencies (following constitution principle of zero external dependencies)
**Storage**: N/A (header-only library)
**Testing**: Core functionality tests (basic logging operations, levels, and formatters) with common warning flags (-Wall, -Wextra, -Wpedantic)
**Target Platform**: macOS (primary), Linux (secondary) with Clang compiler
**Project Type**: Single header-only library (c/c++)
**Performance Goals**: Compilation process completes within 10% of original compilation time
**Constraints**: Must maintain same API interface, zero external dependencies, header-only distribution, clean compilation with no warnings using -Wall, -Wextra, -Wpedantic
**Scale/Scope**: Single header file (logging.hpp) supporting multiple C++ standards

## Constitution Check

*GATE: Must pass before Phase 0 research. Re-check after Phase 1 design.*

### Gate Analysis:
- **I. Header-Only Distribution**: ✅ PASSED - Implementation will remain header-only in logging.hpp
- **II. Zero External Dependencies**: ✅ PASSED - No external dependencies will be added
- **III. Standard Compliance**: ✅ PASSED - Will comply with C++20 standard while maintaining backward compatibility
- **IV. Clean Implementation**: ✅ PASSED - Changes will follow clean implementation practices
- **V. Instrumentation-First Design**: ✅ PASSED - Core logging functionality preserved
- **VI. Multiplatform Support**: ✅ PASSED - Supporting macOS and Linux as required

All constitutional principles are satisfied by this implementation approach.

## Project Structure

### Documentation (this feature)

```text
specs/001-cpp20-compatibility/
├── plan.md              # This file (/speckit.plan command output)
├── research.md          # Phase 0 output (/speckit.plan command)
├── data-model.md        # Phase 1 output (/speckit.plan command)
├── quickstart.md        # Phase 1 output (/speckit.plan command)
├── contracts/           # Phase 1 output (/speckit.plan command)
└── tasks.md             # Phase 2 output (/speckit.tasks command - NOT created by /speckit.plan)
```

### Source Code (repository root)
```text
logging.hpp              # Main header file to be updated for C++20 compatibility
logging.h                # C header file (may need minor updates for consistency)
main_cpp20.cpp           # C++20 test application
main.c                   # C test application
app_cpp20                # Compiled C++20 test executable
app                      # Compiled C test executable
```

**Structure Decision**: Single header-only library structure maintained. The primary change will be to logging.hpp to ensure C++20 compatibility while preserving backward compatibility with older standards. Minor updates may be made to logging.h for consistency, and test applications will be used to verify compatibility.

## Complexity Tracking

> **Fill ONLY if Constitution Check has violations that must be justified**

| Violation | Why Needed | Simpler Alternative Rejected Because |
|-----------|------------|-------------------------------------|
| [e.g., 4th project] | [current need] | [why 3 projects insufficient] |
| [e.g., Repository pattern] | [specific problem] | [why direct DB access insufficient] |
