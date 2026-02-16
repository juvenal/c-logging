#ifndef LOGGING_HPP
#define LOGGING_HPP

// C++ Standard Detection
#if __cplusplus >= 202002L  // C++20
    #define LOGGING_CPP20 1
    #include <source_location>
    #include <format>
    #include <string_view>
#elif __cplusplus >= 201703L  // C++17
    #define LOGGING_CPP17 1
    #include <string_view>
    #include <chrono>
    #include <ctime>
    #include <cstdio>
    #include <cstring>
    #include <memory>
#elif __cplusplus >= 201402L  // C++14
    #define LOGGING_CPP14 1
    #include <chrono>
    #include <ctime>
    #include <cstdio>
    #include <cstring>
    #include <memory>
#elif __cplusplus >= 201103L  // C++11
    #define LOGGING_CPP11 1
    #include <chrono>
    #include <ctime>
    #include <cstdio>
    #include <cstring>
    #include <memory>
#else
    #error "This library requires C++11 or later"
#endif

#include <iostream>
#include <iomanip>

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR,
    NONE
};

// Fallback implementation for std::source_location for pre-C++20
#if defined(LOGGING_CPP20)
    using source_location = std::source_location;
#else
    struct SourceLocation {
        const char* file_{__builtin_FILE()};
        const char* function_{__builtin_FUNCTION()};
        int line_{__builtin_LINE()};

        static constexpr SourceLocation current() noexcept {
            return {};
        }

        constexpr const char* file_name() const noexcept { return file_; }
        constexpr const char* function_name() const noexcept { return function_; }
        constexpr int line() const noexcept { return line_; }
    };

    using source_location = SourceLocation;
#endif

// Handle inline variables for different C++ standards
#if defined(LOGGING_CPP17)
    inline LogLevel current_log_level = LogLevel::INFO;
    inline std::ostream* log_output = &std::clog;
#else
    // For C++14 and earlier, use a different approach to avoid inline variables
    LogLevel& get_current_log_level() {
        static LogLevel current_level = LogLevel::INFO;
        return current_level;
    }
    
    std::ostream*& get_log_output() {
        static std::ostream* output_stream = &std::clog;
        return output_stream;
    }
    
    // Macro to access the variables
    #define current_log_level get_current_log_level()
    #define log_output (*get_log_output())
#endif

#if defined(LOGGING_CPP14)
constexpr std::string_view level_to_string(LogLevel level) {
    switch(level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKWN";
    }
}
#else
// For C++11, use a regular function instead of constexpr
inline std::string level_to_string(LogLevel level) {
    switch(level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO ";
        case LogLevel::WARN:  return "WARN ";
        case LogLevel::ERROR: return "ERROR";
        default: return "UNKWN";
    }
}
#endif

inline void set_log_level(LogLevel level) {
#if defined(LOGGING_CPP17)
    current_log_level = level;
#else
    get_current_log_level() = level;
#endif
}

inline void set_log_output(std::ostream& stream) {
#if defined(LOGGING_CPP17)
    log_output = &stream;
#else
    get_log_output() = &stream;
#endif
}

struct LogMessage {
    std::string_view fmt;
    source_location loc;

    template <typename S>
    constexpr LogMessage(const S& s, const source_location& l = source_location::current())
        : fmt(s), loc(l) {}
};

// Fallback formatting for pre-C++20 standards
#if defined(LOGGING_CPP20)
    #include <format>
    #define LOGGING_FORMAT(...) std::format(__VA_ARGS__)
    #define LOGGING_VFORMAT(fmt, args) std::vformat(fmt, args)
#else
    #include <sstream>
    #include <cstdarg>
    
    // Simple string formatting fallback for pre-C++20
    #if defined(LOGGING_CPP17)
        template<typename... Args>
        std::string format_string(const std::string_view fmt, Args&&... args) {
            // For C++17, we can use if constexpr and fold expressions
            std::ostringstream oss;
            
            // Convert format string to a C-style string for processing
            std::string fmt_str(fmt);
            
            if constexpr (sizeof...(args) == 0) {
                return std::string(fmt_str);
            } else {
                // This is a simplified implementation for demonstration
                // A full implementation would require proper format string parsing
                oss << fmt_str;
                ((oss << " " << args), ...); // Simple concatenation for demo purposes
                return oss.str();
            }
        }
    #else
        // For C++14 and C++11, we need a simpler approach without if constexpr and fold expressions
        template<typename T>
        std::string format_single_arg(const std::string_view fmt, T&& arg) {
            std::ostringstream oss;
            oss << fmt << " " << arg;
            return oss.str();
        }
        
        // Base case for no arguments
        inline std::string format_no_args(const std::string_view fmt) {
            return std::string(fmt);
        }
    #endif
    
    #define LOGGING_FORMAT(...) format_string(__VA_ARGS__)
    #define LOGGING_VFORMAT(fmt, args_tuple) format_string(fmt)
#endif

template <typename... Args>
void log(LogLevel level, LogMessage msg, Args&&... args) {
#if defined(LOGGING_CPP17)
    if (level < current_log_level || !log_output) return;
#else
    if (level < get_current_log_level() || !get_log_output()) return;
#endif

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
#if defined(LOGGING_CPP20)
        std::string message = std::vformat(msg.fmt, std::make_format_args(args...));
        #if defined(LOGGING_CPP17)
            *log_output << std::format("[{}.{:03}] [{}] {}:{}: {}\n",
        #else
            *get_log_output() << std::format("[{}.{:03}] [{}] {}:{}: {}\n",
        #endif
            time_str, ms.count(),
            level_to_string(level),
            msg.loc.file_name(), msg.loc.line(),
            message);
#else
        // For pre-C++20, use our fallback formatting
        std::string message;
        #if defined(LOGGING_CPP17)
            if constexpr (sizeof...(args) == 0) {
                message = std::string(msg.fmt);
            } else {
                // For C++17, we can use if constexpr and fold expressions
                std::ostringstream oss;
                oss << msg.fmt;
                ((oss << " " << args), ...);
                message = oss.str();
            }
        #elif defined(LOGGING_CPP14) || defined(LOGGING_CPP11)
            // For C++14 and C++11, we need to handle different argument counts
            if (sizeof...(args) == 0) {
                message = std::string(msg.fmt);
            } else {
                // For simplicity, we'll handle only one argument in C++11/14
                // A full implementation would need more complex template metaprogramming
                std::ostringstream oss;
                oss << msg.fmt;
                // Just handle the first argument for C++11/14
                using expander = int[];
                (void)expander{0, (oss << " " << args, 0)...};
                message = oss.str();
            }
        #endif
        
        #if defined(LOGGING_CPP17)
            *log_output << "[" << time_str << "." << std::setfill('0') << std::setw(3) << ms.count() << "] "
                        << "[" << level_to_string(level) << "] "
                        << msg.loc.file_name() << ":" << msg.loc.line() << ": "
                        << message << "\n";
        #else
            *get_log_output() << "[" << time_str << "." << std::setfill('0') << std::setw(3) << ms.count() << "] "
                              << "[" << level_to_string(level) << "] "
                              << msg.loc.file_name() << ":" << msg.loc.line() << ": "
                              << message << "\n";
        #endif
#endif

        #if defined(LOGGING_CPP17)
            log_output->flush();
        #else
            get_log_output()->flush();
        #endif
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