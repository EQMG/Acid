#include "Bitmap.hpp"

namespace acid {

Bitmap::Bitmap(std::filesystem::path filename) :
	m_filename(std::move(filename)) {
	Load(m_filename);
}

Bitmap::Bitmap(const Vector2ui &size, uint32_t bytesPerPixel) :
	m_size(size),
	m_bytesPerPixel(bytesPerPixel) {
}

Bitmap::Bitmap(std::vector<uint8_t> data, const Vector2ui &size, uint32_t bytesPerPixel) :
	m_data(std::move(data)),
	m_size(size),
	m_bytesPerPixel(bytesPerPixel) {
}

void Bitmap::Load(const std::filesystem::path &filename) {
	Registry()[filename.extension().string()].first(this, filename);
}

void Bitmap::Write(const std::filesystem::path &filename) const {
	Registry()[filename.extension().string()].second(this, filename);
}
}
