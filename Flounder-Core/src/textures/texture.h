#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdarg.h>

#include "../devices/display.h"
#include "../maths/colour.h"

#include "stb_image.h"

namespace flounder
{
	/// <summary>
	/// Class that represents a loaded texture.
	/// </summary>
	class texture
	{
		/// <summary>
		/// A builder used to set texture parameters for loading.
		/// </summary>
		class builder
		{
		private:
			texture *m_texture;
		public:
			/// <summary>
			/// Creates a new texture builder.
			/// </summary>
			builder();

			/// <summary>
			/// Deconstructor for the texture builder.
			/// </summary>
			~builder();

			/// <summary>
			/// Sets the textures source file.
			/// </summary>
			/// <param name="file"> The source file. </param>
			/// <returns> This. </returns>
			builder *setFile(const std::string &file);

			/// <summary>
			/// Sets the cubemap source files.
			/// </summary>
			/// <param name="n_args"> The number of cubemap files being loaded. </param>
			/// <param name="..."> The source cubemap files. </param>
			/// <returns> This. </returns>
			builder *setCubemap(const int n_args, ...);

			/// <summary>
			/// Clamps the texture to a coloured border.
			/// </summary>
			/// <param name="border"> The coloured border. </param>
			/// <returns> This. </returns>
			builder *clampToBorder(colour *border);

			/// <summary>
			/// Clamps the texture to the edges.
			/// </summary>
			/// <returns> This. </returns>
			builder *clampEdges();

			/// <summary>
			/// Selects nearest filtering.
			/// </summary>
			/// <returns> This. </returns>
			builder *nearestFiltering();

			/// <summary>
			/// Disables mipmapping.
			/// </summary>
			/// <returns> This. </returns>
			builder *noMipmap();

			/// <summary>
			/// Disables anisotropic filtering.
			/// </summary>
			/// <returns> This. </returns>
			builder *noFiltering();

			/// <summary>
			/// Sets the starting number of texture rows (default = 1).
			/// </summary>
			/// <param name="numberOfRows"> The new number of rows. </param>
			/// <returns> This. </returns>
			builder *setNumberOfRows(const int &numberOfRows);

			/// <summary>
			/// Creates a texture from the builder.
			/// </summary>
			/// <returns> The created texture. </returns>
			texture *create();
		};
	public:
		enum typetexture 
		{
			typeTexture2D, typeTextureCubeMap
		};

	protected:
		builder *m_builder;

		std::string m_file;
		std::string *m_cubemap;
		int m_cubemapCount;

		bool m_hasAlpha;
		colour *m_border;
		bool m_clampToBorder;
		bool m_clampEdges;
		bool m_mipmap;
		bool m_anisotropic;
		bool m_nearest;
		int m_numberOfRows;

		typetexture m_textureType;
		int32_t m_width, m_height;
		VkSampler m_sampler;
		VkImage m_image;
		VkImageLayout m_imageLayout;
		VkDeviceMemory m_deviceMemory;
		VkImageView m_view;
		int32_t m_mipLevels;

		/// <summary>
		/// A new OpenGL texture object.
		/// </summary>
		/// <param name="builder"> The texture builder. </param>
		texture(builder *builder);
	public:
		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~texture();

		/// <summary>
		/// Creates a new texture builder that is used to configure a texture.
		/// </summary>
		/// <returns> The texture builder. </returns>
		static builder *newTexture();

		/// <summary>
		/// Gets if the texture has alpha.
		/// </summary>
		/// <returns> If the texture has alpha. </returns>
		inline bool hasAlpha() const { return m_hasAlpha; }

		/// <summary>
		/// Sets if the texture has alpha.
		/// </summary>
		/// <param name="hasAlpha"> If the texture has alpha. </param>
		inline void setHasAlpha(const bool &hasAlpha) { m_hasAlpha = hasAlpha; }

		/// <summary>
		/// Gets the number of texture rows.
		/// </summary>
		/// <returns> The number of texture rows. </returns>
		inline int getNumberOfRows() const { return m_numberOfRows; }

		/// <summary>
		/// Sets the number of texture rows.
		/// </summary>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		inline void setNumberOfRows(const int &numberOfRows) { m_numberOfRows = numberOfRows; }

		/// <summary>
		/// The textures type.
		/// </summary>
		/// <returns> The textures type. </returns>
		inline typetexture getTextureType() const { return m_textureType; }
	private:
		/// <summary>
		/// Loads the texture object from a texture file.
		/// </summary>
		/// <param name="file"> The file to load from. </param>
		void loadFromTexture(const std::string &file);

		/// <summary>
		/// Loads the texture object from a cubemap texture files.
		/// </summary>
		/// <param name="cubemap"> The cubemap files to load from. </param>
		void loadFromCubemap(const int count, std::string *cubemap);
	};
}
