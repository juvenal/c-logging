# Tasks: Fix Source Location Reporting in Logger

**Feature**: Fix Source Location Reporting in Logger  
**Branch**: `002-fix-source-location`  
**Generated**: February 15, 2026  
**Input**: Implementation plan from `/specs/002-fix-source-location/plan.md`

## Overview

This document outlines the implementation tasks for fixing the source location reporting bug in the logger library so that log messages show the actual file and line number where the logging function was called from, rather than showing the location within the logging library itself. The implementation will focus on C++20 features like std::source_location with fallbacks for older standards.

## Dependencies

- **User Story 2 depends on User Story 1**: Consistent debugging experience requires accurate source location reporting to be established first
- **User Story 3 depends on User Story 1**: Backward compatibility requires the core fix to be implemented first

## Parallel Execution Opportunities

- **Within User Story 1**: Different logging functions (log_debug, log_info, log_warn, log_error) can be updated in parallel
- **Within User Story 2**: Testing of different logging levels can be done in parallel
- **Within User Story 3**: Verification of different calling contexts can be done in parallel

## Implementation Strategy

1. **MVP Scope**: Complete User Story 1 (Accurate Source Location Reporting) for basic functionality
2. **Incremental Delivery**: Each user story builds upon the previous one with increasing verification
3. **Verification First**: Test basic functionality before expanding to edge cases

---

## Phase 1: Setup

- [X] T001 Set up development environment with C++20 compiler support
- [X] T002 Create backup of original logging.hpp file
- [X] T003 Prepare test files to verify source location fix
- [X] T004 Verify current behavior showing incorrect source location

## Phase 2: Foundational Tasks

- [X] T005 [P] Implement C++ standard detection macros in logging.hpp
- [X] T006 [P] Create fallback implementation for std::source_location for pre-C++20 standards
- [X] T007 [P] Update LogMessage struct to accept source location parameter
- [X] T008 [P] Modify core log function to accept source location parameter

## Phase 3: User Story 1 - Accurate Source Location Reporting (Priority: P1)

**Goal**: Ensure log messages show the actual file and line number where the logging function was called from, rather than showing the location within the logging library itself.

**Independent Test**: When a logging function is called from a specific file and line, the resulting log message displays that exact file and line number, not the internal location within the logging library.

- [X] T009 [US1] Update log_info function to accept source location parameter with default value
- [X] T010 [US1] Update log_debug function to accept source location parameter with default value
- [X] T011 [US1] Update log_warn function to accept source location parameter with default value
- [X] T012 [US1] Update log_error function to accept source location parameter with default value
- [X] T013 [US1] Test with C++20 standard to verify correct source location reporting
- [X] T014 [US1] Verify log output shows correct file and line from call site in test application

## Phase 4: User Story 2 - Consistent Debugging Experience (Priority: P2)

**Goal**: Ensure all log messages consistently show accurate source locations regardless of the logging level (debug, info, warn, error).

**Independent Test**: All logging functions (log_debug, log_info, log_warn, log_error) correctly report the caller's source location rather than the internal logging implementation location.

- [X] T015 [US2] Test all logging levels from same location to verify consistent source location reporting
- [X] T016 [US2] Test logging functions from different files and lines to verify correct source location reporting
- [X] T017 [US2] Verify all logging levels work correctly with fixed source location
- [X] T018 [US2] Test with different C++ standards (C++11, C++14, C++17) to ensure consistent behavior
- [X] T018.1 [US2] Test with C++20 standard to ensure optimal behavior
- [X] T018.2 [US2] Verify fallback implementation works correctly for C++11 standard
- [X] T018.3 [US2] Verify fallback implementation works correctly for C++14 standard
- [X] T018.4 [US2] Verify fallback implementation works correctly for C++17 standard

## Phase 5: User Story 3 - Backward Compatibility (Priority: P3)

**Goal**: Ensure the fix for source location reporting does not break existing functionality or change the API.

**Independent Test**: After applying the fix, all existing logging calls continue to work exactly as before, except that they now report correct source locations.

- [X] T019 [US3] Verify existing code continues to work without modifications
- [X] T020 [US3] Test logging calls from regular functions to verify correct source location
- [X] T021 [US3] Test logging calls from methods (instance and static) to verify correct source location
- [X] T022 [US3] Test logging calls from lambda expressions to verify correct source location
- [X] T023 [US3] Test logging calls from template instantiations to verify correct source location
- [X] T024 [US3] Test logging calls from macro expansions to verify correct source location
- [X] T025 [US3] Verify performance remains consistent with original implementation
- [X] T025.1 [US3] Test logging calls from inline functions to verify correct source location
- [X] T025.2 [US3] Create and test specific macro that calls logging functions to verify correct source location
- [X] T025.3 [US3] Test logging calls from complex template scenarios to verify correct source location

## Phase 6: Polish & Cross-Cutting Concerns

- [X] T026 Verify 100% of log messages display correct source file and line number
- [X] T027 Test all functional requirements (FR-001 through FR-007) are met
- [X] T028 Validate success criteria (SC-001 through SC-004) are achieved
- [X] T029 Update documentation to reflect source location reporting changes
- [X] T030 Run complete test suite to ensure no regressions
- [X] T031 Clean up temporary files and finalize implementation
- [X] T032 Verify edge cases from specification are properly handled (macros, inline functions, lambdas, templates)