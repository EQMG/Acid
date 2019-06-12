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

void Log::Print(const std::string_view &style, const std::string_view &colour, const std::optional<std::string> &type, const std::string &string)
{
	std::lock_guard<std::mutex> lock(MUTEX);
	std::cout << style << colour << string << Style::Default;
	STREAM << string;
}

void Log::PopupMessage(const std::string &title, const std::string &message)
{
#if defined(ACID_BUILD_WINDOWS)
	MessageBox(nullptr, message.c_str(), title.c_str(), 0);
#endif
}

void Log::OpenLog(const std::string &filename)
{
	std::lock_guard<std::mutex> lock(MUTEX);
	STREAM.open(filename);
}
}
