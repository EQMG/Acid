#include "Log.hpp"
#include "Timers/Timers.hpp"

namespace acid {
std::mutex Log::WriteMutex;
std::ostream &Log::OutStream = std::cout;
std::ofstream Log::FileStream;

void Log::OpenLog(const std::filesystem::path &filepath) {
	std::unique_lock<std::mutex> lock(WriteMutex);
	if (auto parentPath = filepath.parent_path(); !parentPath.empty()) {
		std::filesystem::create_directory(parentPath);
	}
	FileStream.open(filepath);
}
}
