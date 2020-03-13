#pragma once

#include <filesystem>
#include <unordered_map>
#include <thread>

#include "Maths/Time.hpp"
#include "Utils/Delegate.hpp"

namespace acid {
/**
 * @brief Class that can listen to file changes on a path recursively.
 */
class ACID_EXPORT FileObserver {
public:
	enum class Status {
		Created, Modified, Erased
	};

	/**
	 * Creates a new file watcher.
	 * @param path The path to watch recursively.
	 * @param delay How frequently to check for changes.
	 */
	explicit FileObserver(std::filesystem::path path, const Time &delay = 5s);
	~FileObserver();

	void DoWithFilesInPath(const std::function<void(std::filesystem::path)> &f) const;

	const std::filesystem::path &GetPath() const { return path; }
	void SetPath(const std::filesystem::path &path) { this->path = path; }

	const Time &GetDelay() const { return delay; }
	void SetDelay(const Time &delay) { this->delay = delay; }

	/**
	 * Called when a file or directory has changed.
	 * @return The delegate.
	 */
	Delegate<void(std::filesystem::path, Status)> &OnChange() { return onChange; }

private:
	void QueueLoop();

	bool Contains(const std::string &key) const;

	std::filesystem::path path;
	Time delay;
	Delegate<void(std::filesystem::path, Status)> onChange;

	bool running;
	std::thread thread;
	std::unordered_map<std::string, std::filesystem::file_time_type> paths;
};
}
