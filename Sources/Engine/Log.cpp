#include "Log.hpp"

#include <fstream>
#if defined(ACID_BUILD_WINDOWS)
#include <Windows.h>
#endif
#include "Files/FileSystem.hpp"

namespace acid
{
std::mutex Log::MUTEX = std::mutex();
std::ofstream Log::STREAM = std::ofstream();

void Log::Out(const std::string& string)
{
	std::lock_guard<std::mutex> lock(MUTEX);
	fprintf(stdout, "%s", string.c_str());
	STREAM << string;
}

void Log::Error(const std::string& string)
{
	std::lock_guard<std::mutex> lock(MUTEX);
	fprintf(stderr, "%s", string.c_str());
	STREAM << string;
}

void Log::Popup(const std::string& title, const std::string& message)
{
#if defined(ACID_BUILD_WINDOWS)
	MessageBox(nullptr, message.c_str(), title.c_str(), 0);
#endif
}

void Log::OpenLog(const std::string& filename)
{
	std::lock_guard<std::mutex> lock(MUTEX);
	FileSystem::Create(filename);
	STREAM.open(filename);
}
}
