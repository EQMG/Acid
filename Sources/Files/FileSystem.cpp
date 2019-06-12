#include "FileSystem.hpp"

#include "Helpers/String.hpp"

namespace acid
{
#if defined(ACID_BUILD_WINDOWS)
const char FileSystem::Separator{'\\'};
const char FileSystem::AltSeparator{'/'};
#else
const char FileSystem::Separator{'/'};
const char FileSystem::AltSeparator{'\\'};
#endif

std::string FileSystem::ParentDirectory(const std::string &path)
{
	if (path.empty())
	{
		return path;
	}

#if defined(ACID_BUILD_WINDOWS)
	auto end{path.find_last_of(std::string(1, Separator) + "/")};
#else
	auto end{path.find_last_of(Separator)};
#endif

	if (end == path.length() - 1)
	{
#if defined(ACID_BUILD_WINDOWS)
		end = path.find_last_of(std::string(1, Separator) + "/", end);
#else
		end = path.find_last_of(Separator, end);
#endif
	}

	if (end == std::string::npos)
	{
		return "";
	}

	return path.substr(0, end);
}

std::string FileSystem::FileName(const std::string &path)
{
	auto start{path.find_last_of(Separator)};

#if defined(ACID_BUILD_WINDOWS)
	// WIN32 also understands '/' as the separator.
	if (start == std::string::npos)
	{
		start = path.find_last_of('/');
	}
#endif

	if (start == std::string::npos)
	{
		start = 0;
	}
	else
	{
		start++; // We do not want the separator.
	}

	return path.substr(start);
}

std::string FileSystem::FileSuffix(const std::string &path)
{
	auto start{path.find_last_of(Separator)};

#if defined(ACID_BUILD_WINDOWS)
	// WIN32 also understands '/' as the separator.
	if (start == std::string::npos)
	{
		start = path.find_last_of('/');
	}
#endif

	if (start == std::string::npos)
	{
		start = 0;
	}

	else
	{
		start++; // We do not want the separator.
	}

	auto end{path.find_last_of('.')};

	if (end == std::string::npos || end < start)
	{
		return "";
	}

	return path.substr(end);
}

std::string FileSystem::JoinPath(const std::vector<std::string> &parts)
{
	std::string joined;
	std::size_t i{};

	for (const auto &part : parts)
	{
		joined.append(part);

		if (i++ != parts.size() - 1)
		{
			joined.append(1, '/');
		}
	}

	return joined;
}

std::vector<std::string> FileSystem::SplitPath(const std::string &path, const char &delim)
{
	std::vector<std::string> split;
	std::string::size_type previousIndex{};
	auto separatorIndex{path.find(delim)};

	while (separatorIndex != std::string::npos)
	{
		auto part{path.substr(previousIndex, separatorIndex - previousIndex)};

		if (part != "..")
		{
			split.push_back(part);
		}
		else
		{
			split.pop_back();
		}

		previousIndex = separatorIndex + 1;
		separatorIndex = path.find(delim, previousIndex);
	}

	split.push_back(path.substr(previousIndex));

	if (split.size() == 1 && delim == Separator)
	{
		auto alternative{SplitPath(path, AltSeparator)};

		if (alternative.size() > 1)
		{
			return alternative;
		}
	}

	return split;
}
}
