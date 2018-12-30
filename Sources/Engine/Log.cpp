#include "Log.hpp"

#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::mutex Log::MUTEX = std::mutex();
	std::ofstream Log::STREAM = std::ofstream();

	void Log::Out(const std::string &string)
	{
		std::lock_guard<std::mutex> lock(MUTEX);
		fprintf(stdout, string.c_str());
		STREAM << string;
	}

	void Log::Error(const std::string &string)
	{
		std::lock_guard<std::mutex> lock(MUTEX);
		fprintf(stderr, string.c_str());
		STREAM << string;
	}

	void Log::OpenLog(const std::string &filename)
	{
		std::lock_guard<std::mutex> lock(MUTEX);
		FileSystem::Create(filename);
		STREAM.open(filename);
	}
}