#pragma once

#ifndef VNLC_LOGGER_HPP
#define VNLC_LOGGER_HPP

#include <string_view>

class VnlcLogger {
private:
    inline static bool verbose = false;

public:
    static inline void setVerbose(bool isVerbose) { verbose = isVerbose; }

    static void debug(std::string_view message);
    static void info(std::string_view message);
    static void warn(std::string_view message);
    static void error(std::string_view message);
    static void fatal(std::string_view message);
};

#ifndef NDEBUG
#define VNLC_LOG_DEBUG(message) VnlcLogger::debug(message)
#else
#define VNLC_LOG_DEBUG(message)                                                                                                                                                                                                                                \
    do {                                                                                                                                                                                                                                                       \
    } while (0)
#endif

#define VNLC_LOG_INFO(message) VnlcLogger::info(message)
#define VNLC_LOG_WARN(message) VnlcLogger::warn(message)
#define VNLC_LOG_ERROR(message) VnlcLogger::error(message)
#define VNLC_LOG_FATAL(message) VnlcLogger::fatal(message)

#endif // VNLC_LOGGER_HPP