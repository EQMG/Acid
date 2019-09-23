#pragma once

#include "StdAfx.hpp"
#include "Helpers/Factory.hpp"
#include "Maths/Vector2.hpp"

namespace acid {
class ACID_EXPORT Bitmap : public Factory<Bitmap> {
public:
	Bitmap() = default;
	Bitmap(std::vector<uint8_t> data, const Vector2ui &size, uint32_t bytesPerPixel = 4) :
		m_data(std::move(data)),
		m_size(size),
		m_bytesPerPixel(bytesPerPixel) {
	}

	virtual ~Bitmap() = default;

	virtual void Load(const std::filesystem::path &filename) {}
	virtual void Write(const std::filesystem::path &filename) const {}

	const std::vector<uint8_t> &GetData() const { return m_data; }
	std::vector<uint8_t> &GetData() { return m_data; }
	const Vector2ui &GetSize() const { return m_size; }
	uint32_t GetBytesPerPixel() const { return m_bytesPerPixel; }

protected:
	std::vector<uint8_t> m_data;
	Vector2ui m_size;
	uint32_t m_bytesPerPixel = 0;
};
}

