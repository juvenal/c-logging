cmake_minimum_required(VERSION 3.10)

# Project name and version
project(c-logging VERSION 1.0.0 LANGUAGES C CXX)

# Set C and C++ standards
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/src/include)

# Define header-only library targets
add_library(logging_c INTERFACE)
target_include_directories(logging_c INTERFACE ${PROJECT_SOURCE_DIR}/src/include)

add_library(logging_cpp INTERFACE)
target_include_directories(logging_cpp INTERFACE ${PROJECT_SOURCE_DIR}/src/include)

# Example/test executables
add_executable(app tests/main.c)
target_include_directories(app PRIVATE ${PROJECT_SOURCE_DIR}/src/include)

add_executable(app_cpp20 tests/main_cpp20.cpp)
target_include_directories(app_cpp20 PRIVATE ${PROJECT_SOURCE_DIR}/src/include)

# Installation
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/include/
        DESTINATION include
        FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")