#include "logging.hpp"
#include <iostream>
int main() { set_log_output(std::cout); log_info("Test message"); return 0; }
