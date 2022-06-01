#pragma once

#include "Bitmaps/Bitmap.hpp"

namespace acid {
class ACID_CONTEXT_EXPORT PngBitmap : public Bitmap::Registrar<PngBitmap> {
	inline static const bool Registered = Register(".png");
public:
	static void Load(Bitmap &bitmap, const std::filesystem::path &filename);
	static void Write(const Bitmap &bitmap, const std::filesystem::path &filename);
};
}
