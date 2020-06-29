#include <util/Logger.hpp>

namespace arterra {

    Logger::Logger() {
        Log("Logger Initialised", Logger::Severity::Debug);
    }

    Logger &Logger::Get() {
        static Logger logger;
        return logger;
    }

    void Logger::Log(const char *message, Logger::Severity severity) {
        auto sLevel = ResolveSeverityLevel(severity);
        if (sLevel > minSLevel) {
            switch (severity) {
            case Severity::Info:
                std::cout << "<INFO> \"" << message << "\"\n";
                break;
            case Severity::Debug:
                std::cout << "<DEBUG> \"" << message << "\"\n";
                break;
            case Severity::Warning:
                std::cout << "<WARN> \"" << message << "\"\n";
                break;
            case Severity::Error:
                std::cerr << "<ERROR> \"" << message << "\"\n";
                break;
            case Severity::Fatal:
                std::cerr << "<FATAL> \"" << message << "\"\n";
                exit(-2);
                break;
            }
        }
    }

    inline void Logger::SetMinimumLogLevel(Logger::Severity severity)
    {
        minSLevel = ResolveSeverityLevel(severity);
    }

    char Logger::ResolveSeverityLevel(Logger::Severity severity)
    {
        switch (severity) {
        case Severity::Info:
            return 1;
        case Severity::Debug:
            return 2;
        case Severity::Warning:
            return 3;
        case Severity::Error:
            return 4;
        case Severity::Fatal:
            return 5;
        };
    }


}