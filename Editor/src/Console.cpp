#include "Console.h"
#include "Console/ConsolePanel.h"

#include <iostream>
#include <ctime>

static std::string GetCurrentTimeString() {
    std::time_t now = std::time(nullptr);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", std::localtime(&now));
    return std::string(buffer);
}

static std::string LogTypeToString(LogType type) {
    switch (type) {
        case LogType::Debug:    return "[Debug]";
        case LogType::Info:     return "[Info]";
        case LogType::Warning:  return "[Warning]";
        case LogType::Error:    return "[Error]";
    }
    return "NULL";
}

void Console::Log(const std::string &msg, LogType logType /*= LogType::Info*/)
{
    const std::string log = LogTypeToString(logType) + ' ' + msg;
    std::cout << "[CONSOLE] " << log << std::endl;
    ConsolePanel::AddLog(log, logType);
}
