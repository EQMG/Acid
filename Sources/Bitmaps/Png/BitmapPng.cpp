#include "BitmapPng.hpp"

#include <cstring>

#include <lodepng/lodepng.h>

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

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
	
	uint8_t *buffer;
	uint32_t width = 0, height = 0;
	auto error = lodepng_decode_memory(&buffer, &width, &height, reinterpret_cast<const uint8_t *>(fileLoaded->data()), fileLoaded->length(), LCT_RGBA, 8);
	if (buffer && !error) {
		LodePNGColorMode color = {LCT_RGBA, 8};
		auto buffersize = lodepng_get_raw_size(width, height, &color);
		bitmap->SetData(std::make_unique<uint8_t[]>(buffersize));
		std::memcpy(bitmap->GetData().get(), buffer, buffersize);
		bitmap->SetSize({ width, height });
		bitmap->SetBytesPerPixel(buffersize / (width * height));
		free(buffer); // lodepng_free
	}

#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void BitmapPng::Write(const Bitmap *bitmap, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	LodePNGColorType colorType = LCT_GREY;
	if (bitmap->GetBytesPerPixel() == 4)
		colorType = LCT_RGBA;
	else if (bitmap->GetBytesPerPixel() == 3)
		colorType = LCT_RGB;
	else
		Log::Error("Cannot write PNG with ", bitmap->GetBytesPerPixel(), " bytes per pixel\n");

	lodepng::encode(filename.string(), bitmap->GetData().get(), bitmap->GetSize().m_x, bitmap->GetSize().m_y, colorType);

#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
