#include "Log.hpp"

#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::ostringstream Log::STREAM = std::ostringstream();

	void Log::Out(const std::string &string)
	{
		fprintf(stdout, string.c_str());
		STREAM << string;
	}

	void Log::Error(const std::string &string)
	{
		fprintf(stderr, string.c_str());
		STREAM << string;
	}

	void Log::CreateLog(const std::string &filename)
	{
		FileSystem::Create(filename);
		FileSystem::WriteTextFile(filename, STREAM.str());
	}
}