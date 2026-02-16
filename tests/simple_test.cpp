#include "logging.hpp"
#include <iostream>

int main() {
    set_log_output(std::cout);
    log_info("Simple test to verify source location fix");
    return 0;
}