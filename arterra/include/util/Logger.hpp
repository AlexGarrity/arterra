#pragma once

#include <iostream>

namespace arterra {

class Logger {
    enum class Severity { Info,
        Debug,
        Warning,
        Error,
        Fatal };

public:
    Logger();

    Logger& Get();

    void Log(const char* message, Severity severity);
    void SetMinimumLogLevel(Severity severity);

private:
    char ResolveSeverityLevel(Severity severity);

        // Minimum level for something to be logged
    char minSLevel = 1;
};
}