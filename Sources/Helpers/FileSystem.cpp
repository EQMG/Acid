#include "FileSystem.hpp"

#include <cassert>
#include <algorithm>
#ifdef FL_BUILD_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <sys/stat.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace fl
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

	void FileSystem::DeleteFile(const std::string &filepath)
	{
		if (!FileExists(filepath))
		{
			return;
		}

		remove(filepath.c_str());
	}

	void FileSystem::CreateFile(const std::string &filepath, const bool &createFolders)
	{
		if (FileExists(filepath))
		{
			return;
		}

		if (createFolders)
		{
			CreateFolder(filepath.substr(0, filepath.find_last_of("\\/")));
		}

		FILE *file = fopen(filepath.c_str(), "rb+");

		if (file == nullptr)
		{
			file = fopen(filepath.c_str(), "wb");
		}

		fclose(file);
	}

	void FileSystem::ClearFile(const std::string &filepath)
	{
		DeleteFile(filepath);
		CreateFile(filepath);
	}

	void FileSystem::CreateFolder(const std::string &path)
	{
		int nError = 0;

#ifdef FL_BUILD_WINDOWS
		nError = _mkdir(path.c_str());
#else
		mode_t nMode = 0733;
		nError = mkdir(path.c_str(), nMode);
#endif

		if (nError != 0)
		{
			//	assert(false && "Could not create folder!");
		}
	}

	std::string FileSystem::ReadTextFile(const std::string &filepath)
	{
		if (!FileSystem::FileExists(filepath))
		{
			printf("File does not exist: '%s'\n", filepath.c_str());
			return "";
		}

		FILE *file = fopen(filepath.c_str(), "rb");

		assert(file != nullptr && "Could not open file!");

		fseek(file, 0, SEEK_END);
		int length = ftell(file);
		assert(length < 100 * 1024 * 1024);
		std::string result(length, 0);
		fseek(file, 0, SEEK_SET);
		fread(&result[0], 1, length, file);
		fclose(file);

		result.erase(std::remove(result.begin(), result.end(), '\r'), result.end());
		return result;
	}

	void FileSystem::WriteTextFile(const std::string &filepath, const std::string &data)
	{
		FILE *fp = fopen(filepath.c_str(), "ab");

		if (fp != nullptr)
		{
			fputs(data.c_str(), fp);
			fclose(fp);
		}
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

	std::string FileSystem::FindExt(const std::string &filepath)
	{
		return filepath.substr(filepath.find_last_of('.') + 1, filepath.size());
	}
}
