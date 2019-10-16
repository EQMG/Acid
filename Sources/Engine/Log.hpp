#pragma once

#include "Maths/Time.hpp"

namespace acid {
/**
 * @brief A logging class used in Acid, will write output to the standard stream and into a file.
 */
class ACID_EXPORT Log {
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

	static constexpr auto TimestampFormat = "%H:%M:%S";

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
		Write(style, colour, args..., Style::Default);
	}

	/**
	 * Outputs a debug message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Debug(Args ... args) {
#ifdef ACID_DEBUG
		Out(Style::Default, Colour::LightBlue, args...);
#endif
	}

	/**
	 * Outputs a info message into the console.
	 * @tparam Args The value types to write.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Info(Args ... args) {
		Out(Style::Default, Colour::Green, args...);
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
	 * Outputs a assert message into the console.
	 * @tparam Args The value types to write.
	 * @param expr The expression to assertion check.
	 * @param args The values to write.
	 */
	template<typename ... Args>
	static void Assert(bool expr, Args ... args) {
		if (expr) {
			Out(Style::Default, Colour::Magenta, args...);
			assert(false);
		}
	}

	static void OpenLog(const std::filesystem::path &filepath);
	static void CloseLog();

private:
	// TODO: Only use mutex in synced writes (where output order must be the same).
	static std::mutex WriteMutex;
	static std::ofstream FileStream;

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

template<typename T>
class Loggable {
protected:
#define MESSAGE_PREFIX Time::GetDateTime(Log::TimestampFormat), " - [", typeid(T).name(), "] "
#define MESSAGE_PREFIX_THIS "(0x", std::hex, std::uppercase, reinterpret_cast<long>(this), ") ", std::dec

	template<typename ... Args>
	void WriteOut(Args ... args) const { Log::Out(MESSAGE_PREFIX, MESSAGE_PREFIX_THIS, args...); }

	template<typename ... Args>
	void WriteInfo(Args ... args) const { Log::Info("INFO: ", MESSAGE_PREFIX, MESSAGE_PREFIX_THIS, args...); }

	template<typename ... Args>
	void WriteWarning(Args ... args) const { Log::Warning("WARN: ", MESSAGE_PREFIX, MESSAGE_PREFIX_THIS, args...); }

	template<typename ... Args>
	void WriteError(Args ... args) const { Log::Error("ERROR: ", MESSAGE_PREFIX, MESSAGE_PREFIX_THIS, args...); }
};
}
