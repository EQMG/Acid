#include "FontFamily.hpp"

#include "../Helpers/HelperFile.hpp"

namespace Flounder
{
	FontFamily::FontFamily(const std::string &name) :
		m_name(name),
		m_typeThin(LoadFontType(name, "Thin")),
		m_typeLight(LoadFontType(name, "Light")),
		m_typeRegular(LoadFontType(name, "Regular")),
		m_typeSemibold(LoadFontType(name, "Semibold")),
		m_typeBold(LoadFontType(name, "Bold"))
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

	FontType *FontFamily::LoadFontType(const std::string &name, const std::string &type)
	{
		const std::string filepathPng = "Resources/Fonts/" + name + "/" + name + "-" + type + ".png";
		const std::string filepathFnt = "Resources/Fonts/" + name + "/" + name + "-" + type + ".fnt";

		if (!HelperFile::FileExists(filepathPng) || !HelperFile::FileExists(filepathFnt))
		{
			return nullptr;
		}

		return new FontType(filepathPng, filepathFnt);
	}
}
