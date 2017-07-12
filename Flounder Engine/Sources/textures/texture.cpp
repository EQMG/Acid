#include "texture.h"

namespace flounder {
	texture::texture(texturebuilder *builder)
	{
		m_width = 0;
		m_height = 0;
		m_hasAlpha = false;
		m_border = builder->m_border;
		m_clampToBorder = builder->m_clampToBorder;
		m_clampEdges = builder->m_clampEdges;
		m_mipmap = builder->m_mipmap;
		m_anisotropic = builder->m_anisotropic;
		m_nearest = builder->m_nearest;
		m_numberOfRows = builder->m_numberOfRows;

		if (builder->m_cubemapCount != 0)
		{
			m_glType = GL_TEXTURE_CUBE_MAP;
			m_textureID = loadCubemap(builder->m_cubemapCount, builder->m_cubemap);
		}
		else if (!builder->m_file.empty())
		{
			m_glType = GL_TEXTURE_2D;
			m_textureID = loadTexture(builder->m_file);
		}
		else
		{
			std::cerr << "Could not find a texture or cubemap from the builder: " << builder << std::endl;
		}

		delete builder;
	}

	texture::~texture()
	{
		glDeleteTextures(1, &m_textureID);
	}

	GLuint texture::loadTexture(const std::string &file)
	{
		int numComponents = 0;
		stbi_uc *data = stbi_load(file.c_str(), &m_width, &m_height, &numComponents, 4);

		if (data == NULL)
		{
			std::cerr << "Unable to load texture: " << file << std::endl;
		}

		GLuint result = 0;
		glGenTextures(1, &result);
		glBindTexture(GL_TEXTURE_2D, result);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		if (m_mipmap) 
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

			if (m_anisotropic) 
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4); // TODO: FlounderTextures.get().getAnisotropyLevel()
			}
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
		else if (m_clampToBorder) 
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
			GLfloat *buffer = new GLfloat[4]{ m_border->r, m_border->g, m_border->b, m_border->a };
			glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, buffer);
			delete buffer;
		}
		else 
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}

		return result;
	}

	GLuint texture::loadCubemap(const int count, std::string *cubemap)
	{
		GLuint result = 0;
		glGenTextures(1, &result);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, result);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (int i = 0; i < count; i++)
		{
			int numComponents = 0;
			stbi_uc *data = stbi_load(cubemap[i].c_str(), &m_width, &m_height, &numComponents, 4);

			if (data == NULL)
			{
				std::cerr << "Unable to load texture: " << cubemap[i] << std::endl;
			}

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return result;
	}
}
