#pragma once

#include <iostream>

namespace arterra {

class Logger {
public:
    // Severity levels as an enum classes
    // (Enum classes are scoped-ish)
    enum Severity { Info,
        Debug,
        Warning,
        Error,
        Fatal };

    // Ctor
    Logger() { Log(Debug, "Logger initialised"); }

    // Singleton getter
    static Logger& Get()
    {
        // Return a local static logger
        static Logger logger;
        return logger;
    }

    // Last log arg
    void Log()
    {
        std::cout << std::endl;
        firstArgument = true;
    }

    // Recursive log function
    template <typename T, typename... Args>
    void Log(T first, Args... args)
    {
        std::cout << first;
        Log(args...);
    }

    // Log function
    template <typename... Args>
    void Log(Severity first, Args... args)
    {
        std::cout << "<" << ResolveSeverityLevel(first) << ">\t: ";
        Log(args...);
    }

    // Set minimum level for log to show
    void SetMinimumLogLevel(Severity severity) { _minSLevel = severity; }

private:
    // Convert enum level to char
    std::string ResolveSeverityLevel(Severity severity)
    {
        switch (severity) {
        case Info:
            return "INFO";
        case Debug:
            return "DEBUG";
        case Warning:
            return "WARN";
        case Error:
            return "ERROR";
        case Fatal:
            return "FATAL";
        }
    }

    // Minimum level for something to be logged
    char _minSLevel = 1;

    bool firstArgument;
};
} // namespace arterra