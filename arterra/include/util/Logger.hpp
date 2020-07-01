#pragma once

#include <iostream>

namespace arterra {

	class Logger {
	public:
		// Severity levels for organising the output.
		// (Enum classes are scoped-ish)
		enum Severity { Info, Debug, Warning, Error, Fatal };

		// Ctor
		Logger() { Log(Debug, "Logger initialised"); }

		// Singleton getter
		static Logger& Get()
		{
			// Return a local static logger.
			static Logger logger;
			return logger;
		}

		// End the message write.
		void Log()
		{
			std::cout << std::endl;
			firstArgument = true;
		}

		// Recursively write the message objects.
		template <typename T, typename... Args> void Log(T first, Args... args)
		{
			std::cout << first;
			Log(args...);
		}

		// First write the severity level.
		template <typename... Args> void Log(Severity first, Args... args)
		{
			std::cout << "<" << ResolveSeverityLevel(first) << ">\t: ";
			Log(args...);
		}

		// Set a minimum level for a log to be printed.
		void SetMinimumLogLevel(Severity severity) { _minSLevel = severity; }

	private:
		// Convert enum level to char.
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
			return "?????";
		}

		// Minimum level for something to be logged (DEBUG).
		char _minSLevel = 1;

		bool firstArgument;
	};
} // namespace arterra