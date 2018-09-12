#include "FileSystem.hpp"
#include "String.hpp"

#include <cassert>
#include <algorithm>
#ifdef ACID_BUILD_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <sys/stat.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace acid
{
	bool FileSystem::FileExists(const std::string &filepath)
	{
		if (FILE *file = fopen(filepath.c_str(), "r"))
		{
			fclose(file);
			return true;
		}

		return false;
	}

	bool FileSystem::DeleteFile(const std::string &filepath)
	{
		if (!FileExists(filepath))
		{
			return false;
		}

		remove(filepath.c_str());
		return false;
	}

	bool FileSystem::CreateFile(const std::string &filepath, const bool &createFolders)
	{
		if (FileExists(filepath))
		{
			return false;
		}

		if (createFolders)
		{
			auto lastPos = filepath.find_last_of("\\/");

			if (lastPos != std::string::npos)
			{
				CreateFolder(filepath.substr(0, lastPos));
			}
		}

		FILE *file = fopen(filepath.c_str(), "rb+");

		if (file == nullptr)
		{
			file = fopen(filepath.c_str(), "wb");
		}

		fclose(file);
		return true;
	}

	bool FileSystem::ClearFile(const std::string &filepath)
	{
		DeleteFile(filepath);
		bool created = CreateFile(filepath);
		return created;
	}

	bool FileSystem::CreateFolder(const std::string &path)
	{
		int32_t nError = 0;

#ifdef ACID_BUILD_WINDOWS
		nError = _mkdir(path.c_str());
#else
		mode_t nMode = 0733;
		nError = mkdir(path.c_str(), nMode);
#endif

	//	assert(nError != 0 && "Could not create folder!");
		return nError == 0;
	}

	std::optional<std::string> FileSystem::ReadTextFile(const std::string &filepath)
	{
		if (!FileSystem::FileExists(filepath))
		{
			Log::Error("File does not exist: '%s'\n", filepath.c_str());
			return {};
		}

		FILE *file = fopen(filepath.c_str(), "rb");

		if (file == nullptr)
		{
			Log::Error("Could not open file: '%s'\n", filepath.c_str());
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

	bool FileSystem::WriteTextFile(const std::string &filepath, const std::string &data)
	{
		FILE *file = fopen(filepath.c_str(), "ab");

		if (file == nullptr)
		{
			return false;
		}

		fputs(data.c_str(), file);
		fclose(file);
		return true;
	}

	std::string FileSystem::GetWorkingDirectory()
	{
		char buff[FILENAME_MAX];
		GetCurrentDir(buff, FILENAME_MAX);
		return FixPaths(buff);
	}

	std::string FileSystem::FixPaths(const std::string &filepath)
	{
		std::string result = std::string(filepath);
		std::replace(result.begin(), result.end(), '\\', '/');
		return result;
	}

	std::string FileSystem::FindName(const std::string &filepath)
	{
		uint32_t lastSep = static_cast<uint32_t>(filepath.find_last_of("\\/") + 1);
		uint32_t lastDot = static_cast<uint32_t>(filepath.find_last_of('.'));
		return filepath.substr(lastSep, lastDot - lastSep);
	}

	std::string FileSystem::FindExt(const std::string &filepath)
	{
		std::string result = filepath.substr(filepath.find_last_of('.') + 1, filepath.size());
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return String::Lowercase(result);
	}
}
