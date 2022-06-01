#include "FileObserver.hpp"

namespace acid {
FileObserver::FileObserver(std::filesystem::path path, const Time &delay) :
	path(std::move(path)),
	delay(delay),
	running(true),
	thread(&FileObserver::QueueLoop, this) {
	DoWithFilesInPath([this](const std::filesystem::path &file) {
		paths[file.string()] = std::filesystem::last_write_time(file);
	});
}

FileObserver::~FileObserver() {
	if (thread.joinable()) {
		running = false;
		thread.join();
	}
}

void FileObserver::DoWithFilesInPath(const std::function<void(std::filesystem::path)> &f) const {
	if (!std::filesystem::is_directory(path)) {
		f(path);
		return;
	}
	for (auto &file : std::filesystem::recursive_directory_iterator(path)) {
		f(file.path());
	}
}

void FileObserver::QueueLoop() {
	while (running) {
		// Wait for "delay" milliseconds
		std::this_thread::sleep_for(std::chrono::microseconds(delay));

		// Check if one of the old files was erased
		for (auto it = paths.begin(); it != paths.end();) {
			if (!std::filesystem::exists(it->first)) {
				onChange(std::filesystem::path(it->first), Status::Erased);
				it = paths.erase(it);
				continue;
			}

			++it;
		}
		
		// Check if a file was created or modified
		DoWithFilesInPath([&](const std::filesystem::path &file) {
			auto lastWriteTime = std::filesystem::last_write_time(file);

			// File creation
			if (!Contains(file.string())) {
				// File modification
				paths[file.string()] = lastWriteTime;
				onChange(file.string(), Status::Created);
			} else {
				if (paths[file.string()] != lastWriteTime) {
					paths[file.string()] = lastWriteTime;
					onChange(file.string(), Status::Modified);
				}
			}
		});
	}
}

bool FileObserver::Contains(const std::string &key) const {
	// TODO C++20: Remove method
	auto el = paths.find(key);
	return el != paths.end();
}
}
