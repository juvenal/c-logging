# Test compilation commands for C++ standards

# C++20 (should work currently)
g++ -std=c++20 -Wall -Wextra -Wpedantic -fsyntax-only logging.hpp

# C++17 (currently fails)
g++ -std=c++17 -Wall -Wextra -Wpedantic -fsyntax-only logging.hpp

# C++14 (currently fails)
g++ -std=c++14 -Wall -Wextra -Wpedantic -fsyntax-only logging.hpp

# C++11 (currently fails)
g++ -std=c++11 -Wall -Wextra -Wpedantic -fsyntax-only logging.hpp

# Test with main_cpp20.cpp
g++ -std=c++20 -Wall -Wextra -Wpedantic main_cpp20.cpp -o app_cpp20_test

# Test with different standards for main_cpp20.cpp
g++ -std=c++17 -Wall -Wextra -Wpedantic main_cpp20.cpp -o app_cpp20_test_17
g++ -std=c++14 -Wall -Wextra -Wpedantic main_cpp20.cpp -o app_cpp20_test_14
g++ -std=c++11 -Wall -Wextra -Wpedantic main_cpp20.cpp -o app_cpp20_test_11