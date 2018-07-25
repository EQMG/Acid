#include "FontType.hpp"

namespace acid
{
	FontType::FontType(const std::string &filename, const FamilyType &familyType) :
		IResource(),
		m_filename(ToFilename(filename, familyType)),
		m_texture(Texture::Resource(filename + "/" + FamilyString(familyType) + ".png")),
		m_metadata(FontMetafile::Resource(filename + "/" + FamilyString(familyType) + ".fnt"))
	{
	}

	FontType::~FontType()
	{
	}

	std::string FontType::ToFilename(const std::string &filename, const FamilyType &familyType)
	{
		return "FontType_" + filename + "_" + std::to_string(familyType);
	}

	std::string FontType::FamilyString(const FamilyType &familyType)
	{
		switch (familyType)
		{
		case FAMILY_THIN:
			return "Thin";
		case FAMILY_LIGHT:
			return "Light";
		case FAMILY_REGULAR:
			return "Regular";
		case FAMILY_SEMIBOLD:
			return "Semibold";
		case FAMILY_BOLD:
			return "Bold";
		default:
			return "";
		}
	}
}
