#include "Bitmap.hpp"

#include "Engine/Log.hpp"
#include "Files/Files.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace acid {
Bitmap::Bitmap(std::filesystem::path filename) :
	m_filename(std::move(filename)) {
	Load(m_filename);
}

Bitmap::Bitmap(const Vector2ui &size, uint32_t bytesPerPixel) :
	m_data(std::make_unique<uint8_t[]>(CalculateLength(size, bytesPerPixel))),
	m_size(size),
	m_bytesPerPixel(bytesPerPixel) {
}

Bitmap::Bitmap(std::unique_ptr<uint8_t[]> &&data, const Vector2ui &size, uint32_t bytesPerPixel) :
	m_data(std::move(data)),
	m_size(size),
	m_bytesPerPixel(bytesPerPixel) {
}

void Bitmap::Load(const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Image could not be loaded: ", filename, '\n');
		return;
	}

	// TODO: Check if ext is registered, otherwise stb fallback for now.
	if (auto it = Registry().find(filename.extension().string()); it != Registry().end()) {
		it->second.first(this, filename);
	} else {
		m_data = std::unique_ptr<uint8_t[]>(stbi_load_from_memory(reinterpret_cast<uint8_t *>(fileLoaded->data()), static_cast<uint32_t>(fileLoaded->size()),
			reinterpret_cast<int32_t *>(&m_size.m_x), reinterpret_cast<int32_t *>(&m_size.m_y), reinterpret_cast<int32_t *>(&m_bytesPerPixel), STBI_rgb_alpha));
		m_bytesPerPixel = 4;
	}

	//Registry()[filename.extension().string()].first(this, filename);
	
#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void Bitmap::Write(const std::filesystem::path &filename) const {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	if (auto parentPath = filename.parent_path(); !parentPath.empty()) {
		std::filesystem::create_directories(parentPath);
	}

	// TODO: Check if ext is registered, otherwise stb fallback for now.
	if (auto it = Registry().find(filename.extension().string()); it != Registry().end()) {
		it->second.second(this, filename);
	} else {
		std::ofstream os(filename, std::ios::binary | std::ios::out);
		int32_t len;
		std::unique_ptr<uint8_t[]> png(stbi_write_png_to_mem(m_data.get(), m_size.m_x * m_bytesPerPixel, m_size.m_x, m_size.m_y, m_bytesPerPixel, &len));
		os.write(reinterpret_cast<char *>(png.get()), len);

	}
	
	//Registry()[filename.extension().string()].second(this, filename);

#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

uint32_t Bitmap::GetLength() const {
	return m_size.m_x * m_size.m_y * m_bytesPerPixel;
}

uint32_t Bitmap::CalculateLength(const Vector2ui &size, uint32_t bytesPerPixel) {
	return size.m_x * size.m_y * bytesPerPixel;
}
}
