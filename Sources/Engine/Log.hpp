#pragma once

#include <mutex>
#include "StdAfx.hpp"

namespace acid
{
/**
 * @brief A logging class used in Acid, will write output to the standard stream and into a file.
 */
class ACID_EXPORT Log
{
public:
	class Style
	{
	public:
		static constexpr std::string_view Default{"\033[0m"};
		static constexpr std::string_view Bold{"\033[1m"};
		static constexpr std::string_view Dim{"\033[2m"};
		static constexpr std::string_view Underlined{"\033[4m"};
		static constexpr std::string_view Blink{"\033[5m"};
		static constexpr std::string_view Reverse{"\033[7m"};
		static constexpr std::string_view Hidden{"\033[8m"};
	};

	class Colour
	{
	public:
		static constexpr std::string_view Default{"\033[39m"};
		static constexpr std::string_view Black{"\033[30m"};
		static constexpr std::string_view Red{"\033[31m"};
		static constexpr std::string_view Green{"\033[32m"};
		static constexpr std::string_view Yellow{"\033[33m"};
		static constexpr std::string_view Blue{"\033[34m"};
		static constexpr std::string_view Magenta{"\033[35m"};
		static constexpr std::string_view Cyan{"\033[36m"};
		static constexpr std::string_view LightGrey{"\033[37m"};
		static constexpr std::string_view DarkGrey{"\033[90m"};
		static constexpr std::string_view LightRed{"\033[91m"};
		static constexpr std::string_view LightGreen{"\033[92m"};
		static constexpr std::string_view LightYellow{"\033[93m"};
		static constexpr std::string_view LightBlue{"\033[94m"};
		static constexpr std::string_view LightMagenta{"\033[95m"};
		static constexpr std::string_view LightCyan{"\033[96m"};
		static constexpr std::string_view White{"\033[97m"};
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
	static void Out(const std::string_view &style, const std::string_view &colour, const std::string &format, Args &&... args)
	{
		Print(style, colour, std::nullopt, StringFormat(format, std::forward<Args>(args)...));
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
		Print(Style::Default, Colour::Default, std::nullopt, StringFormat(format, StringClean(std::forward<Args>(args))...));
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
		Print(Style::Default, Colour::LightCyan, "Debug", StringFormat(format, StringClean(std::forward<Args>(args))...));
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
		Print(Style::Default, Colour::Default, "Info", StringFormat(format, StringClean(std::forward<Args>(args))...));
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
		Print(Style::Default, Colour::LightYellow, "Warning", StringFormat(format, StringClean(std::forward<Args>(args))...));
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
		Print(Style::Default, Colour::LightRed, "Error", StringFormat(format, StringClean(std::forward<Args>(args))...));
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
		PopupMessage(title, StringFormat(format, StringClean(std::forward<Args>(args))...));
	}

	/**
	 * Sets a file to output all log messages into.
	 * @param filename The filename to output into.
	 */
	static void OpenLog(const std::string &filename);

private:
	ACID_STATE static std::mutex MUTEX;
	ACID_STATE static std::ofstream STREAM;
	
	static void Print(const std::string_view &style, const std::string_view &colour, const std::optional<std::string> &type, const std::string &string);
	
	static void PopupMessage(const std::string &title, const std::string &message);

	template<typename T>
	static auto StringClean(const T &t)
	{
		if constexpr (std::is_same_v<std::string, T>)
		{
			return t.c_str();
		}
		else if constexpr (std::is_same_v<std::filesystem::path, T>)
		{
			return t.c_str();
		}
		else
		{
			return t;
		}
	}

	template<typename... Args>
	static std::string StringFormat(const std::string &format, Args &&... args)
	{
		auto size = std::snprintf(nullptr, 0, format.c_str(), args ...);
		std::string output(size, '\0');
		std::sprintf(&output[0], format.c_str(), std::forward<Args>(args)...);
		return output;
	}
};
}
