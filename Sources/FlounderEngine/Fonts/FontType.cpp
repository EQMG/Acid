#include "FontType.hpp"

namespace Flounder
{
	FontType::FontType(const std::string &textureFile, const std::string &fontFile) :
		m_texture(Texture::Resource(textureFile)),
		m_metadata(FontMetafile::Resource(fontFile))
	{
	}

	FontType::~FontType()
	{
		delete m_texture;
		delete m_metadata;
	}
}
