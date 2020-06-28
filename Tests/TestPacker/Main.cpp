#include <Zippy/Zippy.hpp>

#include <Engine/Log.hpp>
#include "Config.hpp"

std::filesystem::path PATH = acid::ACID_RESOURCES_DEV;

std::unique_ptr<Zippy::ZipArchive> NewArchive(int index) {
	auto zipFilepath = std::filesystem::current_path() / ("data-" + std::to_string(index) + ".zip");
	acid::Log::Out("New zip archive: ", zipFilepath, '\n');

	if (std::filesystem::exists(zipFilepath))
		std::filesystem::remove(zipFilepath);

	return std::make_unique<Zippy::ZipArchive>(zipFilepath.string());
}

int main(int argc, char **argv) {
	auto maxFraction = 16 * 1000000;

	auto archive = NewArchive(0);
	int index = 1;
	int currentSizeBytes = 0;
	
	for (auto &file : std::filesystem::recursive_directory_iterator(PATH)) {
		if (!file.is_regular_file()) continue;

		if (currentSizeBytes > maxFraction) {
			archive->Save();
			archive = NewArchive(index++);
			currentSizeBytes = 0;
		}
		
		acid::Log::Out(file.path(), '\n');
		
		std::ifstream t(file.path(), std::ios::in | std::ios::binary);
		std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());
		archive->AddEntry(file.path().string().substr(PATH.string().length() + 1), str);
		currentSizeBytes += file.file_size();
	}
	
	archive->Save();

	// Pauses the console.
	std::cout << "Press enter to continue...";
	std::cin.get();
	return 0;
}
