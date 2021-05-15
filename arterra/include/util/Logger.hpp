#pragma once

#include <iostream>

namespace arterra {

	// Static methods for logging information/data to the console.
	namespace Logger {

		static void Log() { std::cout << "\033[0m" << std::endl; }

		template <typename T, typename... Args> static void Log(T first, Args... args)
		{
			std::cout << first << " ";
			Log(args...);
		}

		/// @brief Print an `Informative` statement. For status updates/general feedback.
		template <typename T, typename... Args> static void Info(T first, Args... args)
		{
			std::cout << "\033[92m<INFO>\t: ";
			Log(first, args...);
		}

		/// @brief Print a `Debugging` statement. For debugging purposes.
		template <typename T, typename... Args> static void Debug(T first, Args... args)
		{
			std::cout << "\033[94m<DEBUG>\t: ";
			Log(first, args...);
		}

		/// @brief Print a `Warning` statement. For issues which don't cause any runtime errors.
		template <typename T, typename... Args> static void Warning(T first, Args... args)
		{
			std::cout << "\033[93m<WARN>\t: ";
			Log(first, args...);
		}

		/// @brief Print an `Error` statement. For issues which do cause runtime errors.
		template <typename T, typename... Args> static void Error(T first, Args... args)
		{
			std::cout << "\033[33m<ERROR>\t: ";
			Log(first, args...);
		}

		/// @brief Print a `Fatal` statement. For issues which cause the program to crash immediately.
		template <typename T, typename... Args> static void Fatal(T first, Args... args)
		{
			std::cout << "\033[31m<FATAL>\t: ";
			Log(first, args...);
		}

	}

};