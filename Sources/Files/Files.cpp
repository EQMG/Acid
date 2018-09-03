#include "Files.hpp"

#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::vector<std::string> Files::SEARCH_PATHS = std::vector<std::string>();

	Files::Files() :
		IModule()
	{
		/*for (auto &search : SEARCH_PATHS)
		{
			if (!FileSystem::FolderExists(search))
			{
				Log::Error("File search path does not exist: '%s'\n", search.c_str());
			}
		}*/
	}

	Files::~Files()
	{
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
			std::string searchPath = search + "/" + filename;

			if (FileSystem::FileExists(searchPath))
			{
				return searchPath;
			}
		}

		Log::Error("Failed to locate: '%s'\n", filename.c_str());
		return filename;
	}
}
