#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "Display/Display.hpp"
#include "Resources/Resources.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Renderer/Descriptors/IDescriptor.hpp"

namespace fl
{
	/// <summary>
	/// Class that represents a loaded texture.
	/// </summary>
	class FL_EXPORT Texture :
		public IResource,
		public Buffer,
		public IDescriptor
	{
	private:
		std::string m_filename;

		bool m_hasAlpha;
		bool m_repeatEdges;
		uint32_t m_mipLevels;
		bool m_anisotropic;
		bool m_nearest;
		uint32_t m_numberOfRows;

		int32_t m_components;
		int32_t m_width, m_height;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		static Texture *Resource(const std::string &filename)
		{
			IResource *resource = Resources::Get()->Get(filename);

			if (resource != nullptr)
			{
				return dynamic_cast<Texture *>(resource);
			}

			Texture *result = new Texture(filename);
			Resources::Get()->Add(dynamic_cast<IResource *>(result));
			return result;
		}

		/// <summary>
		/// A new texture object.
		/// </summary>
		Texture(const std::string &filename, const bool &hasAlpha = false, const bool &repeatEdges = false, const bool &mipmap = true,
				const bool &anisotropic = true, const bool &nearest = false, const uint32_t &numberOfRows = 1);

		/// <summary>
		/// A new empty texture object.
		/// </summary>
		Texture(const uint32_t &width, const uint32_t &height, const VkFormat &format, const VkImageLayout &imageLayout, const VkImageUsageFlags &usage, float *pixels = nullptr);

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~Texture();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetVkWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

		std::string GetFilename() override { return m_filename; };

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
		uint32_t GetNumberOfRows() const { return m_numberOfRows; }

		/// <summary>
		/// Sets the number of texture rows.
		/// </summary>
		/// <param name="numberOfRows"> The number of texture rows. </param>
		void SetNumberOfRows(const uint32_t &numberOfRows) { m_numberOfRows = numberOfRows; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }

		static VkDeviceSize LoadSize(const std::string &filepath);

		static VkDeviceSize LoadSize(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes);

		static unsigned char *LoadPixels(const std::string &filepath, int *width, int *height, int *components);

		static unsigned char *LoadPixels(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes, const size_t &bufferSize, int *width, int *height, int *depth, int *components);

		static void DeletePixels(unsigned char *pixels);

		static uint32_t GetMipLevels(const int32_t &width, const int32_t &height, const int32_t &depth);

		static void CreateImage(const int32_t &width, const int32_t &height, const int32_t &depth, const uint32_t &mipLevels, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkImage &image, VkDeviceMemory &imageMemory, const uint32_t &arrayLayers = 1);

		static void TransitionImageLayout(const VkImage &image, const VkImageLayout &oldLayout, const VkImageLayout &newLayout, const uint32_t &mipLevels, const uint32_t &layerCount = 1);

		static void CopyBufferToImage(const int32_t &width, const int32_t &height, const int32_t &depth, const VkBuffer &buffer, const VkImage &image, const uint32_t &layerCount = 1);

		static void CreateMipmaps(const VkImage &image, const int32_t &width, const int32_t &height, const int32_t &depth, const uint32_t &mipLevels, const uint32_t &layerCount = 1);

		static void CreateImageSampler(const bool &anisotropic, const bool &nearest, const uint32_t &mipLevels, VkSampler &sampler);

		static void CreateImageView(const VkImage &image, const VkImageViewType &type, const VkFormat &format, const uint32_t &mipLevels, VkImageView &imageView, const uint32_t &layerCount = 1);
	};
}
