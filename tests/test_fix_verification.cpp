#include "logging.hpp"
#include <iostream>
#include <fstream>

// Test function to verify source location
void test_function() {
    log_info("Inside test_function() - should show this file and line 7");
}

// Test class with methods
class TestClass {
public:
    static void static_method() {
        log_info("Inside static_method() - should show this file and line 14");
    }
    
    void instance_method() {
        log_info("Inside instance_method() - should show this file and line 18");
    }
};

int main() {
    // Set output to cout to see the logs immediately
    set_log_output(std::cout);
    
    log_info("In main() function - should show this file and line 25");
    
    // Call test function
    test_function();
    
    // Test class methods
    TestClass::static_method();
    TestClass tc;
    tc.instance_method();
    
    // Test lambda
    auto lambda = []() {
        log_info("Inside lambda - should show this file and line 37");
    };
    lambda();
    
    // Test with parameters
    log_info("Testing with parameter: {}", 42);
    
    return 0;
}