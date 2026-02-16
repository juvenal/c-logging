# Makefile for C/C++20 Logging Library

CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c17 -Isrc/include
CXXFLAGS = -Wall -Wextra -std=c++20 -Isrc/include
LDFLAGS =

# Directories
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
TESTS_DIR = tests

# Targets
.PHONY: all clean test c_example cpp_example

all: c_example cpp_example

c_example: app
app: $(TESTS_DIR)/main.c
	$(CC) $(CFLAGS) -o $@ $<

cpp_example: app_cpp20
app_cpp20: $(TESTS_DIR)/main_cpp20.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

test: all
	@echo "Running C example..."
	./app
	@echo "\nRunning C++20 example..."
	./app_cpp20

clean:
	rm -f app app_cpp20 *.log

install:
	@echo "To use this library, simply include the headers from $(INCLUDE_DIR)/ in your project."

help:
	@echo "Available targets:"
	@echo "  all       - Build both C and C++ examples (default)"
	@echo "  c_example - Build the C example"
	@echo "  cpp_example - Build the C++20 example"
	@echo "  test      - Run both examples"
	@echo "  clean     - Remove built binaries and log files"
	@echo "  install   - Show installation instructions"
	@echo "  help      - Show this help message"