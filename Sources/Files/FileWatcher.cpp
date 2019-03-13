#include "FileWatcher.hpp"

#include "FileSystem.hpp"
#include <chrono>

namespace acid
{
FileWatcher::FileWatcher(std::string path, const Time& delay) : m_path(std::move(path)), m_delay(delay), m_thread(std::thread(&FileWatcher::QueueLoop, this)), m_running(true)
{
	for(auto& file : FileSystem::FilesInPath(m_path))
		{
			m_paths[file] = FileSystem::LastModified(file);
		}
}

void FileWatcher::QueueLoop()
{
	while(m_running)
		{
			// Wait for "delay" milliseconds
			std::this_thread::sleep_for(std::chrono::microseconds(m_delay.AsMicroseconds()));

			// Check if one of the old files was erased
			for(auto& el : m_paths)
				{
					if(!FileSystem::Exists(el.first))
						{
							m_onChange(el.first, Status::Erased);
							m_paths.erase(el.first);
						}
				}

			// Check if a file was created or modified
			for(auto& file : FileSystem::FilesInPath(m_path))
				{
					auto lastWriteTime = FileSystem::LastModified(file);

					// File creation
					if(!Contains(file))
						{
							// File modification
							m_paths[file] = lastWriteTime;
							m_onChange(file, Status::Created);
						}
					else
						{
							if(m_paths[file] != lastWriteTime)
								{
									m_paths[file] = lastWriteTime;
									m_onChange(file, Status::Modified);
								}
						}
				}
		}
}

bool FileWatcher::Contains(const std::string& key) const
{
	auto el = m_paths.find(key);
	return el != m_paths.end();
}
}
