#include "Log.hpp"

#include <iostream>
#include <fstream>
#if defined(ACID_BUILD_WINDOWS)
#include <Windows.h>
#endif
#include "Files/FileSystem.hpp"
#include "Maths/Time.hpp"

namespace acid
{
std::mutex Log::MUTEX = std::mutex();
std::ofstream Log::STREAM = std::ofstream();

std::string Log::FormatStyle(const Style &style)
{
	switch (style)
	{
	case Style::Bold:
		return "\033[1m";
	case Style::Dim:
		return "\033[2m";
	case Style::Underlined:
		return "\033[4m";
	case Style::Blink:
		return "\033[5m";
	case Style::Reverse:
		return "\033[7m";
	case Style::Hidden:
		return "\033[8m";
	default:
		return "\033[0m";
	}
}
	
std::string Log::FormatColour(const Colour &colour)
{
	switch (colour)
	{
	case Colour::Black:
		return "\033[30m";
	case Colour::Red:
		return "\033[31m";
	case Colour::Green:
		return "\033[32m";
	case Colour::Yellow:
		return "\033[33m";
	case Colour::Blue:
		return "\033[34m";
	case Colour::Magenta:
		return "\033[35m";
	case Colour::Cyan:
		return "\033[36m";
	case Colour::LightGrey:
		return "\033[37m";
	case Colour::DarkGrey:
		return "\033[90m";
	case Colour::LightRed:
		return "\033[91m";
	case Colour::LightGreen:
		return "\033[92m";
	case Colour::LightYellow:
		return "\033[93m";
	case Colour::LightBlue:
		return "\033[94m";
	case Colour::LightMagenta:
		return "\033[95m";
	case Colour::LightCyan:
		return "\033[96m";
	case Colour::White:
		return "\033[97m";
	default:
		return "\033[39m";
	}
}

void Log::Print(const Style &style, const Colour &colour, const std::optional<std::string> &type, const std::string &string)
{
	auto typeString = ""; // Time::GetDateTime("[%H:%M:%S] ") + (type ? "[" + *type + "] " : "");
	std::lock_guard<std::mutex> lock(MUTEX);
	std::cout << FormatStyle(style) << FormatColour(colour) << typeString << string << FormatStyle(Style::Default);
	STREAM << typeString << string;
}

void Log::PopupMessage(const std::string &title, const std::string &message)
{
#if defined(ACID_BUILD_WINDOWS)
	MessageBox(nullptr, message.c_str(), title.c_str(), 0);
#endif
}

void Log::OpenLog(const std::string &filename)
{
	FileSystem::Create(filename);
	std::lock_guard<std::mutex> lock(MUTEX);
	STREAM.open(filename);
}
}
