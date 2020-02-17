#pragma once

#include "Bitmaps/Bitmap.hpp"

namespace acid {
class ACID_EXPORT BitmapDng : public Bitmap::Registrar<BitmapDng> {
public:
	static void Load(Bitmap *bitmap, const std::filesystem::path &filename);
	static void Write(const Bitmap *bitmap, const std::filesystem::path &filename);

private:
	static bool registered;
};
}
