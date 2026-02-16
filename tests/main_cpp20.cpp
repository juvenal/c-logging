#include "../src/include/logging.hpp"
#include <fstream>

int main() {
    // 0. Set to stdout for visibility
    set_log_output(std::cout);

    // 1. Default usage
    log_info("Starting C++20 application with std::format support. int={}", 42);
    log_debug("Hidden debug message");

    // 2. Change level
    set_log_level(LogLevel::DEBUG);
    log_debug("Now seeing debug. float={:.2f}", 3.14159);

    // 3. Log to file
    std::ofstream log_file("app_cpp20.log");
    if (log_file) {
        set_log_output(log_file);
        log_warn("Warning in file: {}", "Disk space low");
        log_error("Error in file: code {}", 500);
    }

    // 4. Back to console
    set_log_output(std::clog);
    log_info("Back to console.");

    return 0;
}
