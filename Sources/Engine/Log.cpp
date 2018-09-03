#include "Log.hpp"

#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::ostringstream Log::STREAM = std::ostringstream();

	void Log::CreateLog(const std::string &filename)
	{
		FileSystem::CreateFile(filename);
		FileSystem::WriteTextFile(filename, STREAM.str());
	}
}