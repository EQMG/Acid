#include "FileSystem.hpp"

#include <cassert>
#include <iostream>
#include <fstream>
#include <algorithm>

#ifdef FLOUNDER_PLATFORM_WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <sys/stat.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace Flounder
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

	void FileSystem::ClearFile(const std::string &filepath)
	{
		if (!FileExists(filepath))
		{
			return;
		}

		// TODO: Convert ofsteam to C function.
		std::ofstream ofs;
		ofs.open(filepath, std::ofstream::out | std::ofstream::trunc);
		ofs.close();
	}

	void FileSystem::CreateFile(const std::string &filepath)
	{
		if (FileExists(filepath))
		{
			return;
		}

		FILE *file = fopen(filepath.c_str(), "rb+");

		if (file == nullptr)
		{
			file = fopen(filepath.c_str(), "wb");
		}

		fclose(file);
	}

	void FileSystem::CreateFolder(const std::string &path)
	{
		int nError = 0;

#ifdef FLOUNDER_PLATFORM_WINDOWS
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
		FILE *file = fopen(filepath.c_str(), "rt");

		assert(file != nullptr && "Could not find file!");

		fseek(file, 0, SEEK_END);
		unsigned long length = ftell(file);
		char *data = new char[length + 1];
		memset(data, 0, length + 1);
		fseek(file, 0, SEEK_SET);
		fread(data, 1, length, file);
		fclose(file);

		std::string result = std::string(data);
		delete[] data;
		return result;
	}

	std::vector<char> FileSystem::ReadBinaryFile(const std::string &filepath)
	{
		/*FILE *file = fopen(filepath.c_str(), "rb");

		assert(file != nullptr && "Could not find file!");

		fseek(file, 0L, SEEK_END);
		unsigned long length = ftell(file);
		std::vector<char> data = std::vector<char>(length + 1);
		memset(data.data(), 0, length + 1);
		fseek(file, 0, SEEK_SET);
		fread(data.data(), 1, length, file);
		fclose(file);

		return data;*/

		// TODO: Move from ifsteam to normal C binary file loading.
		std::ifstream ifs = std::ifstream(filepath, std::ios::ate | std::ios::binary);

		assert(ifs.is_open() && "Could not find file!");

		size_t fileSize = static_cast<size_t>(ifs.tellg());
		std::vector<char> buffer(fileSize);

		ifs.seekg(0);
		ifs.read(buffer.data(), fileSize);
		ifs.close();

		return buffer;
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

	void FileSystem::WriteBinaryFile(const std::string &filepath, const std::vector<char> &data)
	{
		// TODO: Convert ofsteam to C function.
		std::string pathname = std::string(filepath);

		std::ofstream textout(pathname.c_str(), std::ios::out | std::ios::binary);
		textout.write(&data[0], data.size());

		textout.close();
	}

	std::string FileSystem::GetWorkingDirectory()
	{
		char buff[FILENAME_MAX];
		GetCurrentDir(buff, FILENAME_MAX);
		std::string currentWorkingDirectory = std::string(buff);
		std::replace(currentWorkingDirectory.begin(), currentWorkingDirectory.end(), '\\', '/');
		return currentWorkingDirectory;
	}

	std::string FileSystem::GetHomeDirectory()
	{
		// TODO: Make this work!
		return "";
	}
}
