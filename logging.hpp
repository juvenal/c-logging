#ifndef LOGGING_HPP
#define LOGGING_HPP

#include <iostream>
#include <source_location>
#include <chrono>
#include <string_view>
#include <format>
#include <ctime>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    NONE
};

inline LogLevel current_log_level = LogLevel::INFO;
inline std::ostream* log_output = &std::clog;

constexpr std::string_view level_to_string(LogLevel level) {
    switch(level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKWN";
    }
}

inline void set_log_level(LogLevel level) {
    current_log_level = level;
}

inline void set_log_output(std::ostream& stream) {
    log_output = &stream;
}

struct LogMessage {
    std::string_view fmt;
    std::source_location loc;

    template <typename S>
    constexpr LogMessage(const S& s, const std::source_location& l = std::source_location::current())
        : fmt(s), loc(l) {}
};

template <typename... Args>
void log(LogLevel level, LogMessage msg, Args&&... args) {
    if (level < current_log_level || !log_output) return;

    auto now = std::chrono::system_clock::now();
    auto now_time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
    
    struct tm tm_buf;
    // Use localtime_r for POSIX thread safety, or localtime_s for Windows.
    // For standard C++, std::chrono::current_zone() (C++20) is preferred but support varies.
    // We fall back to standard C function.
    localtime_r(&now_time, &tm_buf);

    char time_str[32];
    std::strftime(time_str, sizeof(time_str), "%H:%M:%S", &tm_buf);

    try {
        std::string message = std::vformat(msg.fmt, std::make_format_args(args...));
        
        *log_output << std::format("[{}.{:03}] [{}] {}:{}: {}\n",
            time_str, ms.count(),
            level_to_string(level),
            msg.loc.file_name(), msg.loc.line(),
            message);
        
        log_output->flush();
    } catch (const std::exception& e) {
        // Fallback or ignore
    }
}

// Convenience wrappers
template <typename... Args>
void log_debug(LogMessage msg, Args&&... args) {
    log(LogLevel::DEBUG, msg, std::forward<Args>(args)...);
}

template <typename... Args>
void log_info(LogMessage msg, Args&&... args) {
    log(LogLevel::INFO, msg, std::forward<Args>(args)...);
}

template <typename... Args>
void log_warn(LogMessage msg, Args&&... args) {
    log(LogLevel::WARN, msg, std::forward<Args>(args)...);
}

template <typename... Args>
void log_error(LogMessage msg, Args&&... args) {
    log(LogLevel::ERROR, msg, std::forward<Args>(args)...);
}

#endif // LOGGING_HPP