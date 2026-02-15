#define LOGGING_IMPLEMENTATION
#include "logging.h"

int main() {
    // 1. Log to stdout, default level INFO
    LOG_INIT(stdout, LOG_LEVEL_INFO);
    
    LOG_INFO("Starting application...");
    LOG_DEBUG("This debug message should NOT appear.");
    
    // 2. Change level to DEBUG (keeping stdout)
    LOG_SET_LEVEL(LOG_LEVEL_DEBUG);
    LOG_DEBUG("Now debug logging is enabled.");
    
    // 3. Log to a file
    FILE* f = fopen("app.log", "w");
    if (f) {
        LOG_INIT(f, LOG_LEVEL_WARN);
        LOG_INFO("This info message goes to file, but won't appear due to level WARN.");
        LOG_WARN("This warning goes to app.log");
        LOG_ERROR("This error goes to app.log");
        fclose(f);
    }

    // 4. Restore to stderr
    LOG_INIT(stderr, LOG_LEVEL_INFO);
    LOG_INFO("Back to stderr.");

    return 0;
}
