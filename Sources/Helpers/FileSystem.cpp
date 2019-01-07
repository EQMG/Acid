#include "FileSystem.hpp"

#include <cassert>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>

#if defined(ACID_BUILD_WINDOWS)
#include <io.h>
#include <direct.h>
#include "dirent.h"
typedef struct _stat STAT;
#define stat _stat
#define S_IFREG _S_IFREG
#define S_IFDIR _S_IFDIR
#define access _access
#define mkdir _mkdir
#define rmdir _rmdir
#define GetCurrentDir _getcwd
#else
typedef struct stat STAT;
#include <dirent.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#include "String.hpp"

namespace acid
{
#if defined(ACID_BUILD_WINDOWS)
	const char FileSystem::SEPARATOR = '\\';
	const char FileSystem::ALT_SEPARATOR = '/';
#else
	const char FileSystem::SEPARATOR = '/';
	const char FileSystem::ALT_SEPARATOR = '\\';
#endif

	std::string FileSystem::GetWorkingDirectory()
	{
		char buff[FILENAME_MAX];
		GetCurrentDir(buff, FILENAME_MAX);
		return buff;
	}

	bool FileSystem::Exists(const std::string &path)
	{
		STAT st;

		if (stat(path.c_str(), & st) == -1)
		{
			return false;
		}

#if defined(ACID_BUILD_WINDOWS)
		return (st.st_mode & S_IFREG) == S_IFREG || (st.st_mode & S_IFDIR) == S_IFDIR;
#else
		return S_ISREG(st.st_mode) || S_ISDIR(st.st_mode);
#endif
	}

	bool FileSystem::IsFile(const std::string &path)
	{
		STAT st;

		if (stat(path.c_str(), & st) == -1)
		{
			return false;
		}

#if defined(ACID_BUILD_WINDOWS)
		return (st.st_mode & S_IFREG) == S_IFREG;
#else
		return S_ISREG(st.st_mode);
#endif
	}

	bool FileSystem::IsDirectory(const std::string &path)
	{
		STAT st;

		if (stat(path.c_str(), & st) == -1)
		{
			return false;
		}

#if defined(ACID_BUILD_WINDOWS)
		return (st.st_mode & S_IFDIR) == S_IFDIR;
#else
		return S_ISDIR(st.st_mode);
#endif
	}

	bool FileSystem::IsReadable(const std::string &path)
	{
		return access(path.c_str(), 0x4) == 0;
	}

	bool FileSystem::IsWriteable(const std::string &path)
	{
		return access(path.c_str(), 0x2) == 0;
	}

	std::vector<std::string> FileSystem::FilesInPath(const std::string &path, const bool &recursive)
	{
		std::vector<std::string> result = {};

		struct dirent *de;
		DIR *dr = opendir(path.c_str());

		if (dr == nullptr)
		{
			Log::Error("Could not open current directory: '%s'!\n", path.c_str());
			return result;
		}

		while ((de = readdir(dr)) != nullptr)
		{
			if (String::RemoveAll(de->d_name, '.').empty())
			{
				continue;
			}

			std::string relPath = path + SEPARATOR + de->d_name;

			if (IsDirectory(relPath))
			{
				if (recursive)
				{
					auto filesInFound = FilesInPath(relPath, recursive);
					result.insert(result.end(), filesInFound.begin(), filesInFound.end());
				}
			}
			else
			{
				result.emplace_back(relPath);
			}
		}

		closedir(dr);
		return result;
	}

	bool FileSystem::Create(const std::string &path)
	{
		if (Exists(path))
		{
			return false;
		}

		auto lastFolderPos = path.find_last_of("\\/");

		if (lastFolderPos != std::string::npos)
		{
			std::string folderPath = path.substr(0, lastFolderPos);
			auto splitFolders = SplitPath(folderPath);
			std::stringstream appended;

			for (const auto &folder : splitFolders)
			{
				appended << folder << SEPARATOR;

				if (!Exists(appended.str()) || !IsDirectory(appended.str()))
				{
#if defined(ACID_BUILD_WINDOWS)
					mkdir(appended.str().c_str());
#else
					mkdir(appended.str().c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
#endif
				}
			}
		}

		if (FileSuffix(path).empty())
		{
			return true;
		}

		FILE *file = fopen(path.c_str(), "rb+");

		if (file == nullptr)
		{
			file = fopen(path.c_str(), "wb");
		}

		fclose(file);
		return true;
	}

	bool FileSystem::Delete(const std::string &path)
	{
		if (IsDirectory(path))
		{
			return rmdir(path.c_str()) == 0;
		}
		else if (IsFile(path))
		{
#if defined(ACID_BUILD_WINDOWS)
			return ::remove(path.c_str()) == 0;
#else
			return ::remove(path.c_str()) == 0;
#endif
		}

		return false;
	}

	std::optional<std::string> FileSystem::ReadTextFile(const std::string &filename)
	{
		if (!FileSystem::Exists(filename))
		{
			Log::Error("File does not exist: '%s'\n", filename.c_str());
			return {};
		}

		FILE *file = fopen(filename.c_str(), "rb");

		if (file == nullptr)
		{
			Log::Error("Could not open file: '%s'\n", filename.c_str());
			return {};
		}

		fseek(file, 0, SEEK_END);
		int32_t length = ftell(file);
		assert(length < 100 * 1024 * 1024);
		std::string result(length, 0);
		fseek(file, 0, SEEK_SET);
		fread(&result[0], 1, length, file);
		fclose(file);

		result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
		return result;
	}

	bool FileSystem::WriteTextFile(const std::string &filename, const std::string &data)
	{
		FILE *file = fopen(filename.c_str(), "ab");

		if (file == nullptr)
		{
			return false;
		}

		fputs(data.c_str(), file);
		fclose(file);
		return true;
	}

	std::optional<std::vector<char>> FileSystem::ReadBinaryFile(const std::string &filename, const std::string &mode)
	{
		std::vector<char> data = {};

		const int32_t bufferSize = 1024;
		const bool useFile = filename.c_str() && strcmp("-", filename.c_str());

		if (FILE *fp = (useFile ? fopen(filename.c_str(), mode.c_str()) : stdin))
		{
			char buf[bufferSize];

			while (size_t len = fread(buf, sizeof(char), bufferSize, fp))
			{
				data.insert(data.end(), buf, buf + len);
			}

			if (ftell(fp) == -1L)
			{
				if (ferror(fp))
				{
					Log::Error("Error reading file: '%s'\n", filename.c_str());
					return {};
				}
			}
			else
			{
				if (ftell(fp) % sizeof(char))
				{
					Log::Error("Corrupted word found in file: '%s'\n", filename.c_str());
					return {};
				}
			}

			if (useFile)
			{
				fclose(fp);
			}
		}
		else
		{
			Log::Error("File does not exist: '%s'\n", filename.c_str());
			return {};
		}

		return data;
	}

	bool FileSystem::WriteBinaryFile(const std::string &filename, const std::vector<char> &data, const std::string &mode)
	{
		const bool useStdout = !filename.c_str() || (filename.c_str()[0] == '-' && filename.c_str()[1] == '\0');

		if (FILE *fp = (useStdout ? stdout : fopen(filename.c_str(), mode.c_str())))
		{
			size_t written = fwrite(data.data(), sizeof(char), data.size(), fp);

			if (data.size() != written)
			{
				Log::Error("Could not write to file: '%s'\n", filename.c_str());
				return false;
			}

			if (!useStdout)
			{
				fclose(fp);
			}
		}
		else
		{
			Log::Error("File could not be opened: '%s'\n", filename.c_str());
			return false;
		}

		return true;
	}

	bool FileSystem::ClearFile(const std::string &filename)
	{
		Delete(filename);
		return Create(filename);
	}

	std::string FileSystem::ParentDirectory(const std::string &path)
	{
		if (path.empty())
		{
			return path;
		}

#if defined(ACID_BUILD_WINDOWS)
		std::string::size_type end = path.find_last_of(SEPARATOR + "/");
#else
		std::string::size_type end = path.find_last_of(SEPARATOR);
#endif

		if (end == path.length() - 1)
		{
#if defined(ACID_BUILD_WINDOWS)
			end = path.find_last_of(SEPARATOR + "/", end);
#else
			end = path.find_last_of(SEPARATOR, end);
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
		std::string::size_type start = path.find_last_of(SEPARATOR);

#if defined(ACID_BUILD_WINDOWS)
		// WIN32 also understands '/' as the separator.
		if (start == std::string::npos)
		{
			start = path.find_last_of("/");
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
		std::string::size_type start = path.find_last_of(SEPARATOR);

#if defined(ACID_BUILD_WINDOWS)
		// WIN32 also understands '/' as the separator.
		if (start == std::string::npos)
		{
			start = path.find_last_of("/");
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

		std::string::size_type end = path.find_last_of(".");

		if (end == std::string::npos || end < start)
		{
			return "";
		}
		else
		{
			return path.substr(end);
		}
	}

	std::string FileSystem::JoinPath(const std::vector<std::string> &parts)
	{
		std::string joined;
		std::size_t i = 0;

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
		std::string::size_type previous_index = 0;
		auto separator_index = path.find(delim);

		while (separator_index != std::string::npos)
		{
			auto part = path.substr(previous_index, separator_index - previous_index);

			if (part != "..")
			{
				split.push_back(part);
			}
			else
			{
				split.pop_back();
			}

			previous_index = separator_index + 1;
			separator_index = path.find(delim, previous_index);
		}

		split.push_back(path.substr(previous_index));

		if (split.size() == 1 && delim == SEPARATOR)
		{
			auto alternative = SplitPath(path, ALT_SEPARATOR);

			if (alternative.size() > 1)
			{
				return alternative;
			}
		}

		return split;
	}
}
