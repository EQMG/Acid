#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "Display/Display.hpp"
#include "Files/Files.hpp"
#include "Resources/Resources.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Renderer/Descriptors/IDescriptor.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded texture.
	/// </summary>
	class ACID_EXPORT Texture :
		public IResource,
		public Buffer,
		public IDescriptor
	{
	private:
		std::string m_filename;

		bool m_repeatEdges;
		uint32_t m_mipLevels;
		bool m_anisotropic;
		bool m_nearest;

		int32_t m_components;
		int32_t m_width, m_height;

		VkImage m_image;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		static std::shared_ptr<Texture> Resource(const std::string &filename)
		{
			std::string realFilename = Files::SearchFile(filename);
			auto resource = Resources::Get()->Get(realFilename);

			if (resource != nullptr)
			{
				return std::dynamic_pointer_cast<Texture>(resource);
			}

			auto result = std::make_shared<Texture>(realFilename);
			Resources::Get()->Add(std::dynamic_pointer_cast<IResource>(result));
			return result;
		}

		/// <summary>
		/// A new texture object.
		/// </summary>
		Texture(const std::string &filename, const bool &repeatEdges = false, const bool &mipmap = true, const bool &anisotropic = true, const bool &nearest = false);

		/// <summary>
		/// A new texture object from a array of pixels.
		/// </summary>
		Texture(const int32_t &width, const int32_t &height, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, float *pixels = nullptr);

		/// <summary>
		/// Deconstructor for the texture object.
		/// </summary>
		~Texture();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetVkWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

		uint8_t *CopyPixels();

		std::string GetName() override { return m_filename; };

		int32_t GetComponents() const { return m_components; }

		int32_t GetWidth() const { return m_width; }

		int32_t GetHeight() const { return m_height; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }

		static VkDeviceSize LoadSize(const std::string &filepath);

		static VkDeviceSize LoadSize(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes);

		static uint8_t *LoadPixels(const std::string &filepath, int *width, int *height, int *components);

		static uint8_t *LoadPixels(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes, const size_t &bufferSize, int *width, int *height, int *depth, int *components);

		static bool WritePixels(const std::string &filename, const void *data, const int &width, const int &height, const int &components = 4);

		static void DeletePixels(uint8_t *pixels);

		static uint32_t GetMipLevels(const int32_t &width, const int32_t &height, const int32_t &depth);

		static void CreateImage(const int32_t &width, const int32_t &height, const int32_t &depth, const uint32_t &mipLevels, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, VkImage &image, VkDeviceMemory &imageMemory, const uint32_t &arrayLayers = 1);

		static void TransitionImageLayout(const VkImage &image, const VkImageLayout &oldLayout, const VkImageLayout &newLayout, const uint32_t &mipLevels, const uint32_t &layerCount = 1);

		static void CopyBufferToImage(const int32_t &width, const int32_t &height, const int32_t &depth, const VkBuffer &buffer, const VkImage &image, const uint32_t &layerCount = 1);

		static void CreateMipmaps(const VkImage &image, const int32_t &width, const int32_t &height, const int32_t &depth, const uint32_t &mipLevels, const uint32_t &layerCount = 1);

		static void CreateImageSampler(const bool &anisotropic, const bool &nearest, const uint32_t &mipLevels, VkSampler &sampler);

		static void CreateImageView(const VkImage &image, const VkImageViewType &type, const VkFormat &format, const uint32_t &mipLevels, VkImageView &imageView, const uint32_t &layerCount = 1);
	};
}
