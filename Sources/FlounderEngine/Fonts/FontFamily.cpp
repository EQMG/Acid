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
		delete m_typeThin;
		delete m_typeLight;
		delete m_typeRegular;
		delete m_typeSemibold;
		delete m_typeBold;
	}

	FontType *FontFamily::LoadFontType(const std::string &filename, const std::string &suffex)
	{
		const std::string filepathPng = filename + "/" + suffex + ".png";
		const std::string filepathFnt = filename + "/" + suffex + ".fnt";

		if (!FileSystem::FileExists(filepathPng) || !FileSystem::FileExists(filepathFnt))
		{
			return nullptr;
		}

		return new FontType(filepathPng, filepathFnt);
	}
}
