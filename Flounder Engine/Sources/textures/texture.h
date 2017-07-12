#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>

#include "stb_image.h"
#include "texturebuilder.h"

namespace flounder {
	class texture
	{
	private:
		int m_width;
		int m_height;
		bool m_hasAlpha;
		colour *m_border;
		bool m_clampToBorder;
		bool m_clampEdges;
		bool m_mipmap;
		bool m_anisotropic;
		bool m_nearest;
		int m_numberOfRows;

		GLenum m_glType;
		GLuint m_textureID;
	public:
		/// <summary>
		/// A new OpenGL texture object.
		/// </summary>
		/// <param name="builder"> The texture builder to load the shader from. </param>
		texture(texturebuilder *builder);

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~texture();
	private:
		GLuint loadTexture(const std::string &file);

		GLuint loadCubemap(const int count, std::string *cubemap);
	public:
		/// <summary>
		/// Gets the width of the texture.
		/// </summary>
		/// <returns> The textures width. </returns>
		inline int getWidth() { return m_width; }

		/// <summary>
		/// Gets the height of the texture.
		/// </summary>
		/// <returns> The textures height. </returns>
		inline int getHeight() { return m_height; }

		/// <summary>
		/// Gets if the texture has alpha.
		/// </summary>
		/// <returns> If the texture has alpha. </returns>
		inline bool hasAlpha() { return m_hasAlpha; }

		/// <summary>
		/// Sets if the texture has alpha.
		/// </summary>
		/// <param name="hasAlpha"> If the texture has alpha. </param>
		inline void setHasAlpha(const bool hasAlpha) { m_hasAlpha = hasAlpha; }

		/// <summary>
		/// Gets the number of texture rows.
		/// </summary>
		/// <returns> The number of texture rows. </returns>
		inline int getNumberOfRows() { return m_numberOfRows; }

		/// <summary>
		/// Sets the number of texture rows.
		/// </summary>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		inline void setNumberOfRows(const int numberOfRows) { m_numberOfRows = numberOfRows; }

		/// <summary>
		/// The OpenGL type of texture loaded.
		/// </summary>
		/// <returns> The OpenGL texture type. </returns>
		inline GLenum getGlType() { return m_glType; }

		/// <summary>
		/// Gets the textures ID.
		/// </summary>
		/// <returns> The textures ID. </returns>
		inline GLuint getTextureID() { return m_textureID; }
	};
}
