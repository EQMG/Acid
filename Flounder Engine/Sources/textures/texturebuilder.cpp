#include "texturebuilder.h"

namespace flounder {
	texturebuilder::texturebuilder()
	{
		m_file = "";
		m_cubemap = NULL;
		m_cubemapCount = 0;
		m_border = new colour();
		m_clampToBorder = false;
		m_clampEdges = false;
		m_mipmap = true;
		m_anisotropic = true;
		m_nearest = false;
		m_numberOfRows = 1;
	}

	texturebuilder::~texturebuilder()
	{
	}

	texturebuilder *texturebuilder::setFile(const std::string &file)
	{
		m_file = file;
		return this;
	}

	texturebuilder *texturebuilder::setCubemap(const int n_args, ...)
	{
		m_cubemap = new std::string[n_args];
		m_cubemapCount = n_args;

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_cubemap[i] = va_arg(ap, const char*);
		}

		va_end(ap);

		return this;
	}

	texturebuilder *texturebuilder::clampToBorder(colour *border)
	{
		m_border->set(border);
		m_clampToBorder = true;
		m_clampEdges = false;
		return this;
	}

	texturebuilder *texturebuilder::clampEdges()
	{
		m_clampToBorder = false;
		m_clampEdges = true;
		return this;
	}

	texturebuilder *texturebuilder::nearestFiltering()
	{
		m_nearest = true;
		return this;
	}

	texturebuilder *texturebuilder::noMipmap()
	{
		m_mipmap = true;
		m_anisotropic = false;
		return this;
	}

	texturebuilder *texturebuilder::noFiltering()
	{
		m_anisotropic = false;
		return this;
	}

	texturebuilder *texturebuilder::setNumberOfRows(const int numberOfRows)
	{
		m_numberOfRows = numberOfRows;
		return this;
	}
}
