#include "fonttype.h"

namespace flounder
{
	fonttype::fonttype(const std::string &textureFile, const std::string &fontFile)
	{
		m_loader = new textloader(textureFile, fontFile);
	}

	fonttype::~fonttype()
	{
		delete m_loader;
	}

	void fonttype::loadText(text *text)
	{
		m_loader->loadTextMesh(text);
	}

	texture * fonttype::getTexture()
	{
		return m_loader->getTexture();
	}

	double fonttype::getMaxSizeY()
	{
		return m_loader->getMetadata()->getMaxSizeY();
	}
}
