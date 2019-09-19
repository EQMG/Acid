#include "BitmapPng.hpp"

#include "lodepng.cpp"

#include "Files/Files.hpp"

namespace acid {
void BitmapPng::Load(const std::filesystem::path &filename) {
	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Bitmap could not be loaded: ", filename, '\n');
		return;
	}

	auto error = lodepng::decode(m_data, m_size.m_x, m_size.m_y, *fileLoaded);
}

void BitmapPng::Write(const std::filesystem::path &filename) const {
	lodepng::encode(filename.string(), m_data, m_size.m_x, m_size.m_y);
}
}
