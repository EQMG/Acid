#include "fonttype.h"

namespace flounder
{
	fonttype::fonttype(const std::string &textureFile, const std::string &fontFile)
	{
		m_texture = texture::newTexture()->setFile(textureFile)->create();
		m_metadata = new metafile(fontFile);
	}

	fonttype::~fonttype()
	{
		delete m_texture;
		delete m_metadata;
	}
}
