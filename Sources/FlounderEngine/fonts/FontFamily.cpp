#include "FontFamily.hpp"

#include "../helpers/HelperFile.hpp"

namespace Flounder
{
	FontFamily::FontFamily(const std::string &name) :
		m_name(name),
		m_typeThin(LoadFontType(name, "thin")),
		m_typeLight(LoadFontType(name, "light")),
		m_typeRegular(LoadFontType(name, "regular")),
		m_typeSemibold(LoadFontType(name, "semibold")),
		m_typeBold(LoadFontType(name, "bold"))
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
		const std::string filepathPng = "res/fonts/" + name + "/" + name + "-" + type + ".png";
		const std::string filepathFnt = "res/fonts/" + name + "/" + name + "-" + type + ".fnt";

		if (!HelperFile::FileExists(filepathPng) || !HelperFile::FileExists(filepathFnt))
		{
			return nullptr;
		}

		return new FontType(filepathPng, filepathFnt);
	}
}
