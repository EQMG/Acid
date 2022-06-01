#include "Log.hpp"

namespace acid {
std::mutex Log::WriteMutex;
std::ofstream Log::FileStream;

void Log::OpenLog(const std::filesystem::path &filepath) {
	//std::unique_lock<std::mutex> lock(WriteMutex);
	if (auto parentPath = filepath.parent_path(); !parentPath.empty())
		std::filesystem::create_directories(parentPath);
	FileStream.open(filepath);
}

void Log::CloseLog() {
	FileStream.close();
}
}
