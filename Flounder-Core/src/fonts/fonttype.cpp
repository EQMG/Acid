#include "fonttype.hpp"

namespace Flounder
{
	fonttype::fonttype(const std::string &textureFile, const std::string &fontFile) :
		m_texture(new texture(textureFile)),
		m_metadata(new metafile(fontFile))
	{
	}

	fonttype::~fonttype()
	{
		delete m_texture;
		delete m_metadata;
	}
}
