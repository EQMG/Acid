#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "../platforms/Platform.hpp"
#include "../renderer/buffers/Buffer.hpp"
#include "../renderer/pipelines/PipelineCreate.hpp"

namespace Flounder
{
	/// <summary>
	/// Class that represents a loaded texture.
	/// </summary>
	class Texture :
		public Buffer
	{
	private:
		std::string m_filename;

		bool m_hasAlpha;
		bool m_clampEdges;
		uint32_t m_mipLevels;
		bool m_anisotropic;
		bool m_nearest;
		uint32_t m_numberOfRows;

		int32_t m_components;
		int32_t m_width, m_height;

		VkImage m_image;
		VkDeviceMemory m_imageMemory;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		/// <summary>
		/// A new texture object.
		/// </summary>
		Texture(const std::string &filename, const bool &hasAlpha = false,
			const bool &clampEdges = false,
			const uint32_t &mipLevels = 1,
			const bool &anisotropic = true,
			const bool &nearest = false,
			const uint32_t &numberOfRows = 1);

		/// <summary>
		/// A new empty texture object.
		/// </summary>
		Texture(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage);

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~Texture();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorSet &descriptorSet) const;

		/// <summary>
		/// Gets if the texture has alpha.
		/// </summary>
		/// <returns> If the texture has alpha. </returns>
		bool HasAlpha() const { return m_hasAlpha; }

		/// <summary>
		/// Sets if the texture has alpha.
		/// </summary>
		/// <param name="hasAlpha"> If the texture has alpha. </param>
		void SetHasAlpha(const bool &hasAlpha) { m_hasAlpha = hasAlpha; }

		/// <summary>
		/// Gets the number of texture rows.
		/// </summary>
		/// <returns> The number of texture rows. </returns>
		int32_t GetNumberOfRows() const { return m_numberOfRows; }

		/// <summary>
		/// Sets the number of texture rows.
		/// </summary>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		void SetNumberOfRows(const int32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }

		static VkDeviceSize LoadSize(const std::string &filepath);

		static stbi_uc *LoadPixels(const std::string &filepath, int *width, int *height, int *components);
	private:
		void CreateImage(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkImage &image, VkDeviceMemory &imageMemory);

		void TransitionImageLayout(const VkImage &image, const VkImageLayout &oldLayout, const VkImageLayout &newLayout);

		void CopyBufferToImage(const uint32_t &width, const uint32_t &height, const VkBuffer &buffer, const VkImage &image);
	};
}
