#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <stdarg.h>

#include "../devices/display.hpp"
#include "../renderer/renderer.hpp"
#include "../maths/colour.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a loaded texture.
	/// </summary>
	class texture
	{
	private:
		std::string m_file;
		int m_cubemapCount;
		std::string *m_cubemap;

		bool m_hasAlpha;
		bool m_clampEdges;
		uint32_t m_mipLevels;
		bool m_anisotropic;
		bool m_nearest;
		uint32_t m_numberOfRows;

		VkBuffer m_stagingBuffer;
		VkDeviceMemory m_stagingMemory;

		VkImage m_image;
		VkDeviceMemory m_imageMemory;
		VkImageView m_imageView;
		VkFormat m_format;
		VkImageType m_imageType;

		int32_t m_components;
		int32_t m_width, m_height, m_depth;
	public:
		/// <summary>
		/// A new OpenGL texture object.
		/// </summary>
		/// <param name="file"> The textures file. </param>
		texture(std::string file, const bool &hasAlpha = false,
			const bool &clampEdges = false,
			const uint32_t &mipLevels = 1,
			const bool &anisotropic = true,
			const bool &nearest = false,
			const uint32_t &numberOfRows = 1);

		/// <summary>
		/// A new OpenGL cubemap texture object.
		/// </summary>
		/// <param name="n_args"> The number of cubemap files. </param>
		/// <param name="..."> The list of cubemap texture paths. </param>
		texture(const int n_args, ...);

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~texture();

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
		inline int32_t getNumberOfRows() const { return m_numberOfRows; }

		/// <summary>
		/// Sets the number of texture rows.
		/// </summary>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		inline void setNumberOfRows(const int32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		/// <summary>
		/// The textures type.
		/// </summary>
		/// <returns> The textures type. </returns>
		inline VkImageType getTextureType() const { return m_imageType; }
	private:
		/// <summary>
		/// Loads the texture object from a texture file.
		/// </summary>
		void loadFromTexture();

		/// <summary>
		/// Loads the texture object from a cubemap texture files.
		/// </summary>
		void loadFromCubemap();

		float *loadPixels(const std::string &filepath, int *width, int *height, int *components);

		void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageAspectFlags aspectMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout);

		void setImageLayout(VkCommandBuffer cmdbuffer, VkImage image, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkImageSubresourceRange subresourceRange);
	};
}
