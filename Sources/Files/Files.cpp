#include "Files.hpp"

#include <algorithm>
#include <ostream>
#include <fstream>
#include "Helpers/FileSystem.hpp"

namespace acid
{
	std::vector<std::string> Files::SEARCH_PATHS = std::vector<std::string>();

	Files::Files()
	{
	}

	void Files::Update()
	{
	}

	void Files::AddSearchPath(const std::string &path)
	{
		SEARCH_PATHS.emplace_back(path);
	}

	void Files::RemoveSearchPath(const std::string &path)
	{
		SEARCH_PATHS.erase(std::remove(SEARCH_PATHS.begin(), SEARCH_PATHS.end(), path), SEARCH_PATHS.end());
	}

	std::optional<std::string> Files::Read(const std::string &path)
	{
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

		return {};
	}
}
