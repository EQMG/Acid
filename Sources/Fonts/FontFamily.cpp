#include "FontFamily.hpp"

#include "Helpers/FileSystem.hpp"

namespace fl
{
	FontFamily::FontFamily(const std::string &filename) :
		IResource(),
		m_filename(filename),
		m_typeThin(LoadFontType(filename, "Thin")),
		m_typeLight(LoadFontType(filename, "Light")),
		m_typeRegular(LoadFontType(filename, "Regular")),
		m_typeSemibold(LoadFontType(filename, "Semibold")),
		m_typeBold(LoadFontType(filename, "Bold"))
	{
	}

	FontFamily::~FontFamily()
	{
	}

	std::shared_ptr<FontType> FontFamily::LoadFontType(const std::string &filename, const std::string &suffex)
	{
		std::string filepathPng = filename + "/" + suffex + ".png";
		std::string filepathFnt = filename + "/" + suffex + ".fnt";

		return std::make_shared<FontType>(filepathPng, filepathFnt);
	}
}
