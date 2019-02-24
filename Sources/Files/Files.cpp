#include "Files.hpp"

#include <algorithm>
#include <physfs.h>
#include "Engine/Engine.hpp"
#include "FileSystem.hpp"

namespace acid
{
	Files::Files()
	{
		PHYSFS_init(Engine::Get()->GetArgv0().c_str());
	}

	Files::~Files()
	{
		PHYSFS_deinit();
	}

	void Files::Update()
	{
	}

	void Files::AddSearchPath(const std::string &path)
	{
		if (std::find(m_searchPaths.begin(), m_searchPaths.end(), path) != m_searchPaths.end())
		{
			return;
		}

		if (PHYSFS_mount(path.c_str(), nullptr, true) == 0)
		{
			Log::Error("File System error while adding a path or zip(%s): %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return;
		}

		m_searchPaths.emplace_back(path);
	}

	void Files::RemoveSearchPath(const std::string &path)
	{
		auto it = std::find(m_searchPaths.begin(), m_searchPaths.end(), path);

		if (it == m_searchPaths.end())
		{
			return;
		}

		if (PHYSFS_unmount(path.c_str()) == 0)
		{
			Log::Error("File System error while removing a path: %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
			return;
		}

		m_searchPaths.erase(it);
	}

	void Files::ClearSearchPath()
	{
		for (const auto &path : std::vector<std::string>(m_searchPaths))
		{
			RemoveSearchPath(path);
		}
	}

	std::optional<std::string> Files::Read(const std::string &path)
	{
		auto fsFile = PHYSFS_openRead(path.c_str());

		if (fsFile == nullptr)
		{
			if (!FileSystem::Exists(path) || !FileSystem::IsFile(path))
			{
				Log::Error("Error while opening file to load %s: %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
				return {};
			}

			return FileSystem::ReadTextFile(path);
		}

		auto size = PHYSFS_fileLength(fsFile);
		std::vector<uint8_t> data(size);
		PHYSFS_readBytes(fsFile, data.data(), static_cast<PHYSFS_uint64>(size));

		if (PHYSFS_close(fsFile) != 0)
		{
			Log::Error("Error while closing file %s: %s\n", path.c_str(), PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}

		return std::string(data.begin(), data.end());
	}

	std::vector<std::string> Files::FilesInPath(const std::string &path, const bool &recursive)
	{
		std::vector<std::string> result = {};
		auto rc = PHYSFS_enumerateFiles(path.c_str());
		char **i;

		for (i = rc; *i != nullptr; i++)
		{
			/*if (IsDirectory(*i))
			{
				if (recursive)
				{
					auto filesInFound = FilesInPath(*i, recursive);
					result.insert(result.end(), filesInFound.begin(), filesInFound.end());
				}
			}
			else
			{*/
			result.emplace_back(std::string(*i));
			//}
		}

		PHYSFS_freeList(rc);
		return result;
	}
}
