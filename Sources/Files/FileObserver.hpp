#pragma once

#include <filesystem>

#include "Maths/Time.hpp"
#include "Helpers/Delegate.hpp"

namespace acid {
/**
 * @brief Class that can listen to file changes on a path recursively.
 */
class ACID_EXPORT FileObserver {
public:
	enum class Status { Created, Modified, Erased };

	/**
	 * Creates a new file watcher.
	 * @param path The path to watch recursively.
	 * @param delay How frequently to check for changes.
	 */
	explicit FileObserver(std::filesystem::path path, const Time &delay = 5s);

	~FileObserver();

	void DoWithFilesInPath(const std::function<void(std::filesystem::path)> &f) const;

	const std::filesystem::path &GetPath() const { return m_path; }
	void SetPath(const std::filesystem::path &path) { m_path = path; }

	const Time &GetDelay() const { return m_delay; }
	void SetDelay(const Time &delay) { m_delay = delay; }

	/**
	 * Called when a file or directory has changed.
	 * @return The delegate.
	 */
	Delegate<void(std::filesystem::path, Status)> &OnChange() { return m_onChange; }

private:
	void QueueLoop();

	bool Contains(const std::string &key) const;

	std::filesystem::path m_path;
	Time m_delay;
	Delegate<void(std::filesystem::path, Status)> m_onChange;

	bool m_running;
	std::thread m_thread;
	std::unordered_map<std::string, std::filesystem::file_time_type> m_paths;
};
}
