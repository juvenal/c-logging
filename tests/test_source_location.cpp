#include "logging.hpp"
#include <iostream>

int main() {
    // Set output to cout to see the logs
    set_log_output(std::cout);
    
    log_info("This should show the file and line from main()");
    
    // Change to a function to test source location
    auto test_func = []() {
        log_info("This should show the file and line from the lambda");
    };
    
    test_func();
    
    return 0;
}