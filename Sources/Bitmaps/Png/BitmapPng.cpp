#include "BitmapPng.hpp"

#include "lodepng.cpp"

#include "Files/Files.hpp"

namespace acid {
bool BitmapPng::registered = Register(".png");

void BitmapPng::Load(Bitmap *bitmap, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif
	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Bitmap could not be loaded: ", filename, '\n');
		return;
	}

	auto error = lodepng::decode(bitmap->m_data, bitmap->m_size.m_x, bitmap->m_size.m_y, std::vector<uint8_t>(fileLoaded->begin(), fileLoaded->end()));
	bitmap->m_bytesPerPixel = 4;
#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void BitmapPng::Write(const Bitmap *bitmap, const std::filesystem::path &filename) {
	lodepng::encode(filename.string(), bitmap->m_data, bitmap->m_size.m_x, bitmap->m_size.m_y);
}
}
