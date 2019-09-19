#pragma once

#include "Bitmaps/Bitmap.hpp"

namespace acid {
class ACID_EXPORT BitmapPng : public Bitmap::Registrar<BitmapPng> {
public:
	static void Register() { Registrar::Register(".png"); }
	
	void Load(const std::filesystem::path &filename) override;
	void Write(const std::filesystem::path &filename) const override;
};
}
