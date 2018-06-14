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
		std::string filepathTexture = filename + "/" + suffex + ".png";
		std::string filepathFont = filename + "/" + suffex + ".fnt";
		return std::make_shared<FontType>(filepathTexture, filepathFont);
	}

	std::shared_ptr<FontType> FontFamily::GetType(const FamilyType &familyType)
	{
		switch (familyType)
		{
		case FAMILY_THIN:
			return m_typeThin;
		case FAMILY_LIGHT:
			return m_typeLight;
		case FAMILY_REGULAR:
			return m_typeRegular;
		case FAMILY_SEMIBOLD:
			return m_typeSemibold;
		case FAMILY_BOLD:
			return m_typeBold;
		default:
			return nullptr;
		}
	}
}
