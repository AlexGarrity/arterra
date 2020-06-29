#include <util/Logger.hpp>

namespace arterra {

	// Initialise logger
	Logger::Logger() {
		Log("Logger Initialised", Logger::Severity::Debug);
	}

	Logger &Logger::Get() {
		// Create a local static logger
		static Logger logger;
		// Return the logger as a reference (can't be null)
		return logger;
	}

	void Logger::Log(const char *message, Logger::Severity severity) {
		// Convert the provided severity to a char
		auto sLevel = ResolveSeverityLevel(severity);
		// If the user has set the level low enough...
		if (sLevel > _minSLevel) {
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
				// Exit the program - something wen't badly wrong
				exit(-1);
				break;
			}
		}
	}

	inline void Logger::SetMinimumLogLevel(Logger::Severity severity)
	{
		// Set minSLevel to the provided level
		_minSLevel = ResolveSeverityLevel(severity);
	}

	inline char Logger::ResolveSeverityLevel(Logger::Severity severity)
	{
		// Return a char based on the enum
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
		return 0;
	}


}