#include "texture.h"

namespace flounder
{
	texture::builder::builder()
	{
		m_texture = new texture(this);
	}

	texture::builder::~builder()
	{
	}

	texture::builder *texture::builder::setFile(const std::string &file)
	{
		m_texture->m_file = file;
		return this;
	}

	texture::builder *texture::builder::setCubemap(const int n_args, ...)
	{
		m_texture->m_cubemap = new std::string[n_args];
		m_texture->m_cubemapCount = n_args;

		va_list ap;
		va_start(ap, n_args);

		for (int i = 0; i < n_args; i++)
		{
			m_texture->m_cubemap[i] = va_arg(ap, const char*);
		}

		va_end(ap);

		return this;
	}

	texture::builder *texture::builder::clampToBorder(colour *border)
	{
		m_texture->m_border->set(*border);
		m_texture->m_clampToBorder = true;
		m_texture->m_clampEdges = false;
		return this;
	}

	texture::builder *texture::builder::clampEdges()
	{
		m_texture->m_clampToBorder = false;
		m_texture->m_clampEdges = true;
		return this;
	}

	texture::builder *texture::builder::nearestFiltering()
	{
		m_texture->m_nearest = true;
		return this;
	}

	texture::builder *texture::builder::noMipmap()
	{
		m_texture->m_mipmap = true;
		m_texture->m_anisotropic = false;
		return this;
	}

	texture::builder *texture::builder::noFiltering()
	{
		m_texture->m_anisotropic = false;
		return this;
	}

	texture::builder *texture::builder::setNumberOfRows(const int &numberOfRows)
	{
		m_texture->m_numberOfRows = numberOfRows;
		return this;
	}

	texture *texture::builder::create()
	{
		if (m_texture->m_cubemapCount != 0)
		{
			m_texture->m_glType = GL_TEXTURE_CUBE_MAP;
			m_texture->loadFromCubemap(m_texture->m_cubemapCount, m_texture->m_cubemap);
		}
		else if (!m_texture->m_file.empty())
		{
			m_texture->m_glType = GL_TEXTURE_2D;
			m_texture->loadFromTexture(m_texture->m_file);
		}
		else
		{
			std::cout << "Could not find a texture or cubemap from the builder: " << this << std::endl;
		}

		//delete this;
		return m_texture;
	}

	texture::texture(builder *builder)
	{
		m_builder = builder;

		m_file = "";
		m_cubemap = NULL;
		m_cubemapCount = 0;

		m_width = 0;
		m_height = 0;
		m_hasAlpha = false;
		m_border = new colour();
		m_clampToBorder = false;
		m_clampEdges = false;
		m_mipmap = true;
		m_anisotropic = true;
		m_nearest = false;
		m_numberOfRows = 1;

		m_glType = NULL;
		m_textureID = NULL;
	}

	texture::~texture()
	{
		glDeleteTextures(1, &m_textureID);

		delete m_builder;

		delete m_border;
	}

	texture::builder *texture::newTexture()
	{
		return new builder();
	}

	void texture::loadFromTexture(const std::string &file)
	{
		int numComponents = 0;
		stbi_uc *data = stbi_load(file.c_str(), &m_width, &m_height, &numComponents, 4);

		if (data == NULL)
		{
			std::cout << "Unable to load texture: " << file << std::endl;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		if (m_mipmap)
		{
#ifndef FLOUNDER_PLATFORM_WEB
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			if (m_anisotropic)
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4); // TODO: FlounderTextures.get().getAnisotropyLevel()
			}
#endif
		}
		else if (m_nearest)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		if (m_clampEdges)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		}
#ifndef FLOUNDER_PLATFORM_WEB
		else if (m_clampToBorder)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat *buffer = new GLfloat[4]{m_border->m_r, m_border->m_g, m_border->m_b, m_border->m_a};
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, buffer);
			delete[] buffer;
		}
#endif
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
	}

	void texture::loadFromCubemap(const int count, std::string *cubemap)
	{
		glGenTextures(1, &m_textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (int i = 0; i < count; i++)
		{
			int numComponents = 0;
			stbi_uc *data = stbi_load(cubemap[i].c_str(), &m_width, &m_height, &numComponents, 4);

			if (data == NULL)
			{
				std::cout << "Unable to load texture: " << cubemap[i] << std::endl;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
