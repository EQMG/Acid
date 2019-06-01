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

void Log::Print(const std::string &style, const std::string &colour, const std::optional<std::string> &type, const std::string &string)
{
	auto typeString = ""; // Time::GetDateTime("[%H:%M:%S] ") + (type ? "[" + *type + "] " : "");
	std::lock_guard<std::mutex> lock(MUTEX);
	std::cout << style << colour << typeString << string << Style::Default;
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
