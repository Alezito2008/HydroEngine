#pragma once
#include <string>

enum class LogType {
    Debug,
    Info,
    Warning,
    Error
};

namespace Console {
    void Log(const std::string& msg, LogType logType = LogType::Info);

    inline void Debug(const std::string& msg) { Log(msg, LogType::Debug); }
    inline void Info(const std::string& msg) { Log(msg, LogType::Info); }
    inline void Warning(const std::string& msg) { Log(msg, LogType::Warning); }
    inline void Error(const std::string& msg) { Log(msg, LogType::Error); }
}
