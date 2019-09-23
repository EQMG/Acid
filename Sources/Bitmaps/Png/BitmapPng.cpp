#include "BitmapPng.hpp"

#include "lodepng.cpp"

#include "Files/Files.hpp"

namespace acid {
bool BitmapPng::registered = Register(".png");

void BitmapPng::Load(const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif
	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Bitmap could not be loaded: ", filename, '\n');
		return;
	}

	auto error = lodepng::decode(m_data, m_size.m_x, m_size.m_y, std::vector<uint8_t>(fileLoaded->begin(), fileLoaded->end()));
	m_bytesPerPixel = 4;
#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void BitmapPng::Write(const std::filesystem::path &filename) const {
	lodepng::encode(filename.string(), m_data, m_size.m_x, m_size.m_y);
}
}
