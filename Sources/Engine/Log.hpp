#pragma once

#include <mutex>

#include "StdAfx.hpp"

namespace acid {
/**
 * @brief A logging class used in Acid, will write output to the standard stream and into a file.
 */
class ACID_EXPORT
Log
{
public:
	class Style {
	public:
		static constexpr std::string_view Default = "\033[0m";
		static constexpr std::string_view Bold = "\033[1m";
		static constexpr std::string_view Dim = "\033[2m";
		static constexpr std::string_view Underlined = "\033[4m";
		static constexpr std::string_view Blink = "\033[5m";
		static constexpr std::string_view Reverse = "\033[7m";
		static constexpr std::string_view Hidden = "\033[8m";
	};

	class Colour {
	public:
		static constexpr std::string_view Default = "\033[39m";
		static constexpr std::string_view Black = "\033[30m";
		static constexpr std::string_view Red = "\033[31m";
		static constexpr std::string_view Green = "\033[32m";
		static constexpr std::string_view Yellow = "\033[33m";
		static constexpr std::string_view Blue = "\033[34m";
		static constexpr std::string_view Magenta = "\033[35m";
		static constexpr std::string_view Cyan = "\033[36m";
		static constexpr std::string_view LightGrey = "\033[37m";
		static constexpr std::string_view DarkGrey = "\033[90m";
		static constexpr std::string_view LightRed = "\033[91m";
		static constexpr std::string_view LightGreen = "\033[92m";
		static constexpr std::string_view LightYellow = "\033[93m";
		static constexpr std::string_view LightBlue = "\033[94m";
		static constexpr std::string_view LightMagenta = "\033[95m";
		static constexpr std::string_view LightCyan = "\033[96m";
		static constexpr std::string_view White = "\033[97m";
	};
	
	static constexpr auto TimestampFormat = "%Y-%m-%d %H:%M:%S GMT%z";

	/**
	 * Outputs a message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Out(Args ... args) {
		std::unique_lock<std::mutex> lock(WriteMutex);
		Write(args...); // GetTime(), 
	}

	/**
	 * Outputs a message into the console.
	 * @tparam Args The value types to write.
	 * @param style The style to output as.
	 * @param colour The colour to output as.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Out(const std::string_view &style, const std::string_view &colour, Args ... args) {
		std::unique_lock<std::mutex> lock(WriteMutex);
		Write(style, colour, args..., Style::Default); // GetTime(), 
	}

	/**
	 * Outputs a error message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Error(Args ... args) {
		Out(Style::Default, Colour::Red, args...);
	}

	/**
	 * Outputs a warning message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Warning(Args ... args) {
		Out(Style::Default, Colour::Yellow, args...);
	}

	static void OpenLog(const std::filesystem::path &filepath);
	
private:
	// TODO: Only use mutex in synced writes (where output order must be the same).
	static std::mutex WriteMutex;
	static std::ostream &OutStream;
	static std::ofstream FileStream;

	/**
	 * A internal method used to write values to the out stream and to a file.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Write(Args ... args) {
		((OutStream << std::forward<Args>(args)), ...);
		((FileStream << std::forward<Args>(args)), ...);
		FileStream.flush(); // TODO: Flushing every write is not optimal.
	}

	/**
	 * A internal method that gets the current time in the log format.
	 */
	static auto GetTime() {
		auto time = std::time(nullptr);
		auto tm = *std::localtime(&time);
		return std::put_time(&tm, TimestampFormat);
	}
};
}
