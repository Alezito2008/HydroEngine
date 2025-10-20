#pragma once
#include "Console.h"

#include <string>


namespace ConsolePanel {
    void Render();
    void AddLog(const std::string& msg, LogType logType);
}