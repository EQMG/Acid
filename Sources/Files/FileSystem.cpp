#include "FileSystem.hpp"

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

bool FileSystem::Exists(const std::string &path)
{
	STAT st;

	if (stat(path.c_str(), &st) == -1)
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

	if (stat(path.c_str(), &st) == -1)
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

	if (stat(path.c_str(), &st) == -1)
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

long FileSystem::LastModified(const std::string &path)
{
	STAT st;

	if (stat(path.c_str(), &st) == 0)
	{
		return static_cast<long>(st.st_mtime);
	}

	return 0;
}

void FileSystem::Touch(const std::string &path)
{
	// TODO: Rewrite.
	auto data{ReadBinaryFile(path)};

	if (data)
	{
		WriteBinaryFile(path, *data);
	}
}

std::vector<std::string> FileSystem::FilesInPath(const std::string &path, const bool &recursive)
{
	std::vector<std::string> files;

	struct dirent *de;
	auto dr{opendir(path.c_str())};

	if (dr == nullptr)
	{
		Log::Error("Could not open current directory: '%s'!\n", path);
		return files;
	}

	while ((de = readdir(dr)) != nullptr)
	{
		if (String::RemoveAll(de->d_name, '.').empty())
		{
			continue;
		}

		auto relPath{path + Separator + de->d_name};

		if (IsDirectory(relPath))
		{
			if (recursive)
			{
				auto filesInFound{FilesInPath(relPath, recursive)};
				files.insert(files.end(), filesInFound.begin(), filesInFound.end());
			}
		}
		else
		{
			files.emplace_back(relPath);
		}
	}

	closedir(dr);
	return files;
}

bool FileSystem::Create(const std::string &path)
{
	if (Exists(path))
	{
		return false;
	}

	auto lastFolderPos{path.find_last_of("\\/")};

	if (lastFolderPos != std::string::npos)
	{
		auto folderPath{path.substr(0, lastFolderPos)};
		auto splitFolders{SplitPath(folderPath)};
		std::stringstream appended;

		for (const auto &folder : splitFolders)
		{
			appended << folder << Separator;

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

	auto file{fopen(path.c_str(), "rb+")};

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
	if (IsFile(path))
	{
#if defined(ACID_BUILD_WINDOWS)
		return remove(path.c_str()) == 0;
#else
		return ::remove(path.c_str()) == 0;
#endif
	}

	return false;
}

std::optional<std::string> FileSystem::ReadTextFile(const std::string &filename)
{
	if (!Exists(filename))
	{
		return std::nullopt;
	}

	auto file{fopen(filename.c_str(), "rb")};

	if (file == nullptr)
	{
		return std::nullopt;
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
	auto file{fopen(filename.c_str(), "ab")};

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
	std::vector<char> data;

	const auto bufferSize{1024};
	const auto useFile{filename.c_str() && strcmp("-", filename.c_str()) != 0};

	if (auto fp = (useFile ? fopen(filename.c_str(), mode.c_str()) : stdin))
	{
		char buf[bufferSize];

		while (auto len = fread(buf, sizeof(char), bufferSize, fp))
		{
			data.insert(data.end(), buf, buf + len);
		}

		if (ftell(fp) == -1L)
		{
			if (ferror(fp))
			{
				return std::nullopt;
			}
		}
		else
		{
			if (ftell(fp) % sizeof(char))
			{
				return std::nullopt;
			}
		}

		if (useFile)
		{
			fclose(fp);
		}
	}
	else
	{
		return std::nullopt;
	}

	return data;
}

bool FileSystem::WriteBinaryFile(const std::string &filename, const std::vector<char> &data, const std::string &mode)
{
	const auto useStdout{!filename.c_str() || (filename.c_str()[0] == '-' && filename.c_str()[1] == '\0')};

	if (auto fp = (useStdout ? stdout : fopen(filename.c_str(), mode.c_str())))
	{
		auto written{fwrite(data.data(), sizeof(char), data.size(), fp)};

		if (data.size() != written)
		{
			Log::Warning("Could not write to file: '%s'\n", filename);
			return false;
		}

		if (!useStdout)
		{
			fclose(fp);
		}
	}
	else
	{
		Log::Warning("File could not be opened: '%s'\n", filename);
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
