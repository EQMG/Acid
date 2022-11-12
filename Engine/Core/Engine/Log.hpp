#pragma once

#include <cassert>
#include <sstream>
#include <mutex>
#include <filesystem>
#include <string_view>
#include <iostream>
#include <fstream>

#include "Maths/Time.hpp"

namespace acid {
/**
 * @brief A logging class used in Acid, will write output to the standard stream and into a file.
 */
class ACID_CORE_EXPORT Log {
public:
	class Styles {
	public:
		constexpr static std::string_view Default = "\033[0m";
		constexpr static std::string_view Bold = "\033[1m";
		constexpr static std::string_view Dim = "\033[2m";
		constexpr static std::string_view Underlined = "\033[4m";
		constexpr static std::string_view Blink = "\033[5m";
		constexpr static std::string_view Reverse = "\033[7m";
		constexpr static std::string_view Hidden = "\033[8m";
	};

	class Colours {
	public:
		constexpr static std::string_view Default = "\033[39m";
		constexpr static std::string_view Black = "\033[30m";
		constexpr static std::string_view Red = "\033[31m";
		constexpr static std::string_view Green = "\033[32m";
		constexpr static std::string_view Yellow = "\033[33m";
		constexpr static std::string_view Blue = "\033[34m";
		constexpr static std::string_view Magenta = "\033[35m";
		constexpr static std::string_view Cyan = "\033[36m";
		constexpr static std::string_view LightGrey = "\033[37m";
		constexpr static std::string_view DarkGrey = "\033[90m";
		constexpr static std::string_view LightRed = "\033[91m";
		constexpr static std::string_view LightGreen = "\033[92m";
		constexpr static std::string_view LightYellow = "\033[93m";
		constexpr static std::string_view LightBlue = "\033[94m";
		constexpr static std::string_view LightMagenta = "\033[95m";
		constexpr static std::string_view LightCyan = "\033[96m";
		constexpr static std::string_view White = "\033[97m";
	};

	constexpr static std::string_view TimestampFormat = "%H:%M:%S";

	/**
	 * Outputs a message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Out(Args ... args) {
		Write(args...); 
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
		Write(style, colour, args..., Styles::Default);
	}

	/**
	 * Outputs a debug message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Debug(Args ... args) {
#ifdef ACID_DEBUG
		Out(Styles::Default, Colours::LightBlue, args...);
#endif
	}

	/**
	 * Outputs a info message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Info(Args ... args) {
		Out(Styles::Default, Colours::Green, args...);
	}

	/**
	 * Outputs a warning message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Warning(Args ... args) {
		Out(Styles::Default, Colours::Yellow, args...);
	}

	/**
	 * Outputs a error message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Error(Args ... args) {
		Out(Styles::Default, Colours::Red, args...);
	}

	/**
	 * Outputs a assert message into the console.
	 * @tparam Args The value types to write.
	 * @param expr The expression to assertion check.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Assert(bool expr, Args ... args) {
		if (expr) {
			Out(Styles::Default, Colours::Magenta, args...);
			assert(false);
		}
	}

	static void OpenLog(const std::filesystem::path &filepath);
	static void CloseLog();

private:
	// TODO: Only use mutex in synced writes (where output order must be the same).
	inline static std::mutex WriteMutex;
	inline static std::ofstream FileStream;

	/**
	 * A internal method used to write values to the out stream and to a file.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Write(Args ... args) {
		std::unique_lock<std::mutex> lock(WriteMutex);
		
		((std::cout << std::forward<Args>(args)), ...);
		if (FileStream.is_open()) {
			((FileStream << std::forward<Args>(args)), ...);
		}
	}
};

template<typename T = std::nullptr_t>
class Loggable {
public:
	explicit Loggable(std::string &&className) :
		className(std::move(className)) {
	}
	template<typename = std::enable_if_t<!std::is_same_v<T, std::nullptr_t>>>
	Loggable() :
		Loggable(typeid(T).name()) {
	}

	virtual ~Loggable() = default;

protected:
	template<typename ... Args>
	void WriteOut(Args ... args) const {
		auto logPrefix = GetLogPrefix();
		Log::Out(logPrefix.rdbuf(), args...);
	}

	template<typename ... Args>
	void WriteInfo(Args ... args) const {
		auto logPrefix = GetLogPrefix();
		Log::Info("INFO: ", logPrefix.rdbuf(), args...);
	}

	template<typename ... Args>
	void WriteWarning(Args ... args) const {
		auto logPrefix = GetLogPrefix();
		Log::Warning("WARN: ", logPrefix.rdbuf(), args...);
	}

	template<typename ... Args>
	void WriteError(Args ... args) const {
		auto logPrefix = GetLogPrefix();
		Log::Error("ERROR: ", logPrefix.rdbuf(), args...);
	}

private:
	std::stringstream GetLogPrefix() const {
		std::stringstream logPrefix;
		logPrefix << Time::GetDateTime(Log::TimestampFormat) << " [" << className << "]" <<
			"(0x" << std::hex << std::uppercase << reinterpret_cast<long>(this) << ") " << std::dec;
		return logPrefix;
	}

	std::string className;
};
}
