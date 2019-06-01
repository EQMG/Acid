#pragma once

#include <mutex>
#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief A logging class used in Acid, will write output to a file one the application has closed.
 */
class ACID_EXPORT Log
{
public:
	class Style
	{
	public:
		static constexpr char *Default = "\033[0m";
		static constexpr char *Bold = "\033[1m";
		static constexpr char *Dim = "\033[2m";
		static constexpr char *Underlined = "\033[4m";
		static constexpr char *Blink = "\033[5m";
		static constexpr char *Reverse = "\033[7m";
		static constexpr char *Hidden = "\033[8m";
	};

	class Colour
	{
	public:
		static constexpr char *Default = "\033[39m";
		static constexpr char *Black = "\033[30m";
		static constexpr char *Red = "\033[31m";
		static constexpr char *Green = "\033[32m";
		static constexpr char *Yellow = "\033[33m";
		static constexpr char *Blue = "\033[34m";
		static constexpr char *Magenta = "\033[35m";
		static constexpr char *Cyan = "\033[36m";
		static constexpr char *LightGrey = "\033[37m";
		static constexpr char *DarkGrey = "\033[90m";
		static constexpr char *LightRed = "\033[91m";
		static constexpr char *LightGreen = "\033[92m";
		static constexpr char *LightYellow = "\033[93m";
		static constexpr char *LightBlue = "\033[94m";
		static constexpr char *LightMagenta = "\033[95m";
		static constexpr char *LightCyan = "\033[96m";
		static constexpr char *White = "\033[97m";
	};

	/**
	 * Outputs a message into the console.
	 * @tparam Args The args types.
	 * @param style The style to output as.
	 * @param colour The colour to output as.
	 * @param format The format to output into.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Out(const Style &style, const Colour &colour, const std::string &format, Args &&... args)
	{
		Print(style, colour, std::nullopt, StringFormat(format, std::forward<Args>(args)...));
	}
	
	/**
	 * Outputs a message into the console.
	 * @tparam Args The args types.
	 * @param style The style to output as.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Out(const Style &style, const std::string &format, Args &&... args)
	{
		Print(style, Colour::Default, std::nullopt, StringFormat(format, std::forward<Args>(args)...));
	}
	
	/**
	 * Outputs a message into the console.
	 * @tparam Args The args types.
	 * @param colour The colour to output as.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Out(const Colour &colour, const std::string &format, Args &&... args)
	{
		Print(Style::Default, colour, std::nullopt, StringFormat(format, std::forward<Args>(args)...));
	}
	
	/**
	 * Outputs a message into the console.
	 * @tparam Args The args types.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Out(const std::string &format, Args &&... args)
	{
		Print(Style::Default, Colour::Default, std::nullopt, StringFormat(format, std::forward<Args>(args)...));
	}
	
	/**
	 * Outputs a debug into the console.
	 * @tparam Args The args types.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Debug(const std::string &format, Args &&... args)
	{
		Print(Style::Default, Colour::LightCyan, "Debug", StringFormat(format, std::forward<Args>(args)...));
	}
	
	/**
	 * Outputs a info into the console.
	 * @tparam Args The args types.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Info(const std::string &format, Args &&... args)
	{
		Print(Style::Default, Colour::Default, "Info", StringFormat(format, std::forward<Args>(args)...));
	}
	
	/**
	 * Outputs a warning into the console.
	 * @tparam Args The args types.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Warning(const std::string &format, Args &&... args)
	{
		Print(Style::Default, Colour::LightYellow, "Warning", StringFormat(format, std::forward<Args>(args)...));
	}

	/**
	 * Outputs a error into the console.
	 * @tparam Args The args types.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Error(const std::string &format, Args &&... args)
	{
		Print(Style::Default, Colour::LightRed, "Error", StringFormat(format, std::forward<Args>(args)...));
	}
	
	/**
	 * Displays a popup menu.
	 * @param title The title.
	 * @param format The format to output into.
	 * @param args The args to be added into the format.
	 */
	template<typename... Args>
	static void Popup(const std::string &title, const std::string &format, Args &&... args)
	{
		PopupMessage(title, StringFormat(format, std::forward<Args>(args)...));
	}

	/**
	 * Sets a file to output all log messages into.
	 * @param filename The filename to output into.
	 */
	static void OpenLog(const std::string &filename);

private:
	ACID_STATE static std::mutex MUTEX;
	ACID_STATE static std::ofstream STREAM;
	
	static void Print(const std::string &style, const std::string &colour, const std::optional<std::string> &type, const std::string &string);
	
	static void PopupMessage(const std::string &title, const std::string &message);

	template<typename... Args>
	static std::string StringFormat(const std::string &format, Args &&... args)
	{
		size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
		std::unique_ptr<char[]> buf(new char[size]);
		snprintf(buf.get(), size, format.c_str(), args ...);
		return std::string(buf.get(), buf.get() + size - 1); // Excludes the '\0'
	}
};
}
