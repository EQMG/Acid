#include "BitmapPng.hpp"

#include "Files/Files.hpp"

#include "lodepng.cpp"

namespace acid {
bool BitmapPng::registered = Register(".png");

void BitmapPng::Load(Bitmap *bitmap, const std::filesystem::path &filename) {
	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Bitmap could not be loaded: ", filename, '\n');
		return;
	}

	std::vector<uint8_t> data;
	uint32_t width = 0, height = 0;
	auto error = lodepng::decode(data, width, height, std::vector<uint8_t>(fileLoaded->begin(), fileLoaded->end()));

	bitmap->SetData(std::make_unique<uint8_t[]>(data.size()));
	std::copy(data.begin(), data.end(), bitmap->GetData().get());
	bitmap->SetSize({width, height});
	bitmap->SetBytesPerPixel(data.size() / (width * height));
}

void BitmapPng::Write(const Bitmap *bitmap, const std::filesystem::path &filename) {
	lodepng::encode(filename.string(), bitmap->GetData().get(), bitmap->GetSize().m_x, bitmap->GetSize().m_y);
}
}
