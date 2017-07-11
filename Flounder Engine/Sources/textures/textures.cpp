#include "textures.h"

namespace flounder {
	textures::textures()
		: imodule()
	{
	}

	textures::~textures()
	{
	}

	void textures::init()
	{
		float maxAnisotropy = 0;
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);

		if (m_anisotropyLevel == -1 || m_anisotropyLevel > maxAnisotropy)
		{
			m_anisotropyLevel = maxAnisotropy;
		}
	}

	void textures::update()
	{
	}
}
