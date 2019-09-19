#pragma once

#include "StdAfx.hpp"
#include "Helpers/Factory.hpp"
#include "Maths/Vector2.hpp"

namespace acid {
class ACID_EXPORT Bitmap : public Factory<Bitmap> {
public:
	virtual ~Bitmap() = default;

	virtual void Load(const std::filesystem::path &filename) = 0;
	virtual void Write(const std::filesystem::path &filename) const = 0;
	
protected:
	std::vector<uint8_t> m_data;
	Vector2ui m_size;
	uint32_t m_bytesPerPixel = 0;
};
}

