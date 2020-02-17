#include "Bitmap.hpp"

#include "Engine/Log.hpp"
#include "Files/Files.hpp"

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
	Registry()[filename.extension().string()].first(this, filename);
}

void Bitmap::Write(const std::filesystem::path &filename) const {
	if (auto parentPath = filename.parent_path(); !parentPath.empty()) {
		std::filesystem::create_directories(parentPath);
	}

	Registry()[filename.extension().string()].second(this, filename);
}

uint32_t Bitmap::GetLength() const {
	return m_size.m_x * m_size.m_y * m_bytesPerPixel;
}

uint32_t Bitmap::CalculateLength(const Vector2ui &size, uint32_t bytesPerPixel) {
	return size.m_x * size.m_y * bytesPerPixel;
}
}
