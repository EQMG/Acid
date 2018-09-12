#include "Files.hpp"

#include <ostream>
#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::vector<std::string> Files::SEARCH_PATHS = std::vector<std::string>();

	Files::Files() :
		IModule()
	{
		/*for (auto &search : SEARCH_PATHS) // TODO: Ensure paths exist, construct resource tree.
		{
			if (!FileSystem::FolderExists(search))
			{
				Log::Error("File search path does not exist: '%s'\n", search.c_str());
			}
		}*/
	}

	void Files::Update()
	{
	}

	void Files::AddSearchPath(const std::string &path)
	{
		SEARCH_PATHS.emplace_back(path);
	}

	std::string Files::SearchFile(const std::string &filename)
	{
		if (FileSystem::FileExists(filename))
		{
			return filename;
		}

		for (auto &search : SEARCH_PATHS)
		{
			std::stringstream searchPath;
			searchPath << search << "/" << filename;

			if (FileSystem::FileExists(searchPath.str()))
			{
				return searchPath.str();
			}
		}

		Log::Error("Failed to locate: '%s'\n", filename.c_str());
		return filename;
	}
}
