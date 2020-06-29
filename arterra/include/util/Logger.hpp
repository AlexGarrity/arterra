#pragma once

#include <iostream>

namespace arterra {

class Logger {
public:
	// Severity levels as an enum classes
	// (Enum classes are scoped-ish)
	enum class Severity { Info,
		Debug,
		Warning,
		Error,
		Fatal };

	// Ctor
	Logger();

	// Singleton getter
	Logger& Get();

	// Log function
	void Log(const char* message, Severity severity);
	// Set minimum level for log to show
	void SetMinimumLogLevel(Severity severity);

private:
	// Convert enum level to char
	char ResolveSeverityLevel(Severity severity);

	// Minimum level for something to be logged
	char _minSLevel = 1;
};
}