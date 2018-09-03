#include "Log.hpp"

#include <stdarg.h>
#include <memory>
#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::ostringstream Log::STREAM = std::ostringstream();

	void Log::CreateLog(const std::string &filename)
	{
		FileSystem::CreateFile(filename);
		FileSystem::WriteTextFile(filename, STREAM.str());
	}

	std::string Log::StringFormat(const std::string &format, ...)
	{
		// Reserve two times as much as the length of the format.
		int final_n, n = ((int)format.size()) * 2;
		std::unique_ptr<char[]> formatted;
		va_list ap;

		while (true)
		{
			formatted.reset(new char[n]); // Wrap the plain char array into the unique_ptr
			strcpy(&formatted[0], format.c_str());
			va_start(ap, format);
			final_n = vsnprintf(&formatted[0], n, format.c_str(), ap);
			va_end(ap);

			if (final_n < 0 || final_n >= n)
			{
				n += abs(final_n - n + 1);
			}
			else
			{
				break;
			}
		}

		return std::string(formatted.get());
	}
}