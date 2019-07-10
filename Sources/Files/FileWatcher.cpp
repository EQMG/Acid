#include "FileWatcher.hpp"

namespace acid
{
FileWatcher::FileWatcher(std::filesystem::path path, const Time &delay) :
	m_path{std::move(path)},
	m_delay{delay},
	m_running{true},
	m_thread{&FileWatcher::QueueLoop, this}
{
	for (auto &file : std::filesystem::recursive_directory_iterator(m_path))
	{
		m_paths[file.path().string()] = std::filesystem::last_write_time(file);
	}
}

FileWatcher::~FileWatcher()
{
	if (m_thread.joinable())
	{
		m_running = false;
		m_thread.join();
	}
}

void FileWatcher::QueueLoop()
{
	while (m_running)
	{
		// Wait for "delay" milliseconds
		std::this_thread::sleep_for(std::chrono::microseconds(m_delay));

		// Check if one of the old files was erased
		for (auto it{m_paths.begin()}; it != m_paths.end();)
		{
			if (!std::filesystem::exists(it->first))
			{
				m_onChange(std::filesystem::path{it->first}, Status::Erased);
				it = m_paths.erase(it);
				continue;
			}

			++it;
		}

		// Check if a file was created or modified
		for (auto &file : std::filesystem::recursive_directory_iterator(m_path))
		{
			auto lastWriteTime{file.last_write_time()};

			// File creation
			if (!Contains(file.path().string()))
			{
				// File modification
				m_paths[file.path().string()] = lastWriteTime;
				m_onChange(file.path().string(), Status::Created);
			}
			else
			{
				if (m_paths[file.path().string()] != lastWriteTime)
				{
					m_paths[file.path().string()] = lastWriteTime;
					m_onChange(file.path().string(), Status::Modified);
				}
			}
		}
	}
}

bool FileWatcher::Contains(const std::string &key) const
{
	// TODO C++20: Remove method
	auto el{m_paths.find(key)};
	return el != m_paths.end();
}
}
