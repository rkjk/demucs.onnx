#ifndef DEMUCS_ONNX_LOGGING_HPP
#define DEMUCS_ONNX_LOGGING_HPP

#include <iostream>
#include <sstream>
#include <string>

#ifdef __ANDROID__
#include <android/log.h>
#define DEMUCS_LOG_TAG "DemucsONNX"
#endif

namespace demucsonnx {

enum class LogLevel {
    DEBUG,
    INFO,
    WARN,
    ERROR
};

inline void log_impl(LogLevel level, const std::string& message) {
#ifdef __ANDROID__
    int priority = ANDROID_LOG_INFO;
    switch (level) {
        case LogLevel::DEBUG: priority = ANDROID_LOG_DEBUG; break;
        case LogLevel::INFO:  priority = ANDROID_LOG_INFO;  break;
        case LogLevel::WARN:  priority = ANDROID_LOG_WARN;  break;
        case LogLevel::ERROR: priority = ANDROID_LOG_ERROR; break;
    }
    __android_log_print(priority, DEMUCS_LOG_TAG, "%s", message.c_str());
#else
    std::ostream& os = (level == LogLevel::ERROR) ? std::cerr : std::cout;
    const char* level_str = "";
    switch (level) {
        case LogLevel::DEBUG: level_str = "[DEMUCS DEBUG] "; break;
        case LogLevel::INFO:  level_str = "[DEMUCS INFO]  "; break;
        case LogLevel::WARN:  level_str = "[DEMUCS WARN]  "; break;
        case LogLevel::ERROR: level_str = "[DEMUCS ERROR] "; break;
    }
    os << level_str << message << std::endl;
#endif
}

} // namespace demucsonnx

#define DEMUCS_LOG_STREAM(level, msg) \
    do { \
        std::stringstream ss; \
        ss << msg; \
        demucsonnx::log_impl(demucsonnx::LogLevel::level, ss.str()); \
    } while (0)

#ifdef NDEBUG
#define DEMUCS_LOG_DEBUG(msg) do {} while (0)
#else
#define DEMUCS_LOG_DEBUG(msg) DEMUCS_LOG_STREAM(DEBUG, msg)
#endif

#define DEMUCS_LOG_INFO(msg)  DEMUCS_LOG_STREAM(INFO,  msg)
#define DEMUCS_LOG_WARN(msg)  DEMUCS_LOG_STREAM(WARN,  msg)
#define DEMUCS_LOG_ERROR(msg) DEMUCS_LOG_STREAM(ERROR, msg)

#endif // DEMUCS_ONNX_LOGGING_HPP
