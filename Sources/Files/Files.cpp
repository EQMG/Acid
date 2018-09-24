#include "Files.hpp"

#include <ostream>
#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::vector<std::string> Files::SEARCH_PATHS = std::vector<std::string>();
	std::vector<std::unique_ptr<FileZip>> Files::SEARCH_ZIPS = std::vector<std::unique_ptr<FileZip>>();

	Files::Files() :
		IModule()
	{
	}

	void Files::Update()
	{
	}

	void Files::AddSearchPath(const std::string &path)
	{
		if (FileSystem::IsDirectory(path))
		{
			SEARCH_PATHS.emplace_back(path);
		}
		else
		{
			SEARCH_ZIPS.emplace_back(std::make_unique<FileZip>(path));
		}
	}

	std::string Files::Search(const std::string &path)
	{
		for (auto &search : SEARCH_PATHS)
		{
			std::string searchPath = std::string(search).append("/").append(path);

			if (FileSystem::Exists(searchPath))
			{
				return searchPath;
			}
		}

		if (FileSystem::Exists(path))
		{
			return path;
		}

	//	Log::Error("Failed to locate file: '%s'\n", path.c_str());
		return path;
	}

	std::optional<std::string> Files::Read(const std::string &path)
	{
		for (auto &zip : SEARCH_ZIPS)
		{
			if (!zip->HasFile(path))
			{
				continue;
			}

			return zip->Read(path);
		}

		for (auto &search : SEARCH_PATHS)
		{
			std::string searchPath = std::string(search).append("/").append(path);

			if (FileSystem::Exists(searchPath))
			{
				std::ifstream file(searchPath.c_str(), std::ios::binary | std::ios::in);
				std::stringstream ss;
				ss << file.rdbuf();
				return ss.str();
			}
		}

		if (FileSystem::Exists(path))
		{
			std::ifstream file(path.c_str(), std::ios::binary | std::ios::in);
			std::stringstream ss;
			ss << file.rdbuf();
			return ss.str();
		}

	//	Log::Error("Failed to read file: '%s'\n", path.c_str());
		return {};
	}
}
