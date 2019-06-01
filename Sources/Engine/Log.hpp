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
	enum class Style
	{
		Default,
		Bold,
		Dim,
		Underlined,
		Blink,
		Reverse,
		Hidden
	};

	enum class Colour
	{
		Default,
		Black,
		Red,
		Green,
		Yellow,
		Blue,
		Magenta,
		Cyan,
		LightGrey,
		DarkGrey,
		LightRed,
		LightGreen,
		LightYellow,
		LightBlue,
		LightMagenta,
		LightCyan,
		White
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
	
	static std::string FormatStyle(const Style &style);

	static std::string FormatColour(const Colour &colour);

	static void Print(const Style &style, const Colour &colour, const std::optional<std::string> &type, const std::string &string);
	
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
