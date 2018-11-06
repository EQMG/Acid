#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Renderer/Descriptors/IDescriptor.hpp"
#include "Resources/IResource.hpp"

namespace acid
{
	/// <summary>
	/// Class that represents a loaded texture.
	/// </summary>
	class ACID_EXPORT Texture :
		public IResource,
		public IDescriptor
	{
	private:
		std::string m_filename;

		VkFilter m_filter;
		VkSamplerAddressMode m_addressMode;
		bool m_anisotropic;
		uint32_t m_mipLevels;
		VkSampleCountFlagBits m_samples;
		VkImageLayout m_imageLayout;

		uint32_t m_components;
		uint32_t m_width, m_height;

		VkImage m_image;
		VkDeviceMemory m_deviceMemory;
		VkImageView m_imageView;
		VkSampler m_sampler;
		VkFormat m_format;

		VkDescriptorImageInfo m_imageInfo;
	public:
		/// <summary>
		/// Will find an existing texture with the same filename, or create a new texture.
		/// </summary>
		/// <param name="filename"> The file to load the texture from. </param>
		static std::shared_ptr<Texture> Resource(const std::string &filename);

		/// <summary>
		/// A new texture object.
		/// </summary>
		/// <param name="filename"> The file to load the texture from. </param>
		/// <param name="filter"> The type of filtering will be use on the texture. </param>
		/// <param name="addressMode"> The sampler address mode to use. </param>
		/// <param name="anisotropic"> If anisotropic filtering will be use on the texture. </param>
		/// <param name="mipmap"> If mipmaps will be generated for the texture. </param>
		explicit Texture(const std::string &filename, const VkFilter &filter = VK_FILTER_LINEAR, const VkSamplerAddressMode &addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
		                 const bool &anisotropic = true, const bool &mipmap = true);

		/// <summary>
		/// A new texture object from a array of pixels.
		/// </summary>
		/// <param name="width"> The textures width. </param>
		/// <param name="height"> The textures height. </param>
		/// <param name="pixels"> The inital pixels to use in the texture. <seealso cref="#GetPixels()"/> to get a copy of the pixels, and <seealso cref="#SetPixels()"/> to set the pixels. </param>
		/// <param name="format"> The textures format. </param>
		/// <param name="imageLayout"> The textures image layout </param>
		/// <param name="usage"> The textures image usage </param>
		/// <param name="filter"> The type of filtering will be use on the texture. </param>
		/// <param name="addressMode"> The sampler address mode to use. </param>
		/// <param name="samples"> The amount of MSAA samples to use. </param>
		/// <param name="anisotropic"> If anisotropic filtering will be use on the texture. </param>
		/// <param name="mipmap"> If mipmaps will be generated for the texture. </param>
		Texture(const uint32_t &width, const uint32_t &height, void *pixels = nullptr, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, const VkFilter &filter = VK_FILTER_LINEAR, const VkSamplerAddressMode &addressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT,
				const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT, const bool &anisotropic = false, const bool &mipmap = false);

		~Texture();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage, const uint32_t &count);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const DescriptorSet &descriptorSet) const override;

		/// <summary>
		/// Gets a copy of the textures pixels from memory, after usage is finished remember to delete the result.
		/// </summary>
		/// <returns> A copy of the textures pixels. </returns>
		uint8_t *GetPixels();

		/// <summary>
		/// Copies the pixels into this textures memory.
		/// </summary>
		/// <param name="pixels"> The pixels to copy to the image. </param>
		void SetPixels(uint8_t *pixels);

		std::string GetFilename() override { return m_filename; };

		VkFilter GetFilter() const { return m_filter; }

		VkSamplerAddressMode GetAddressMode() const { return m_addressMode; }

		bool IsAnisotropic() const { return m_anisotropic; }

		uint32_t GetMipLevels() const { return m_mipLevels; }

		VkSampleCountFlagBits GetSamples() const { return m_samples; }

		VkImageLayout GetImageLayout() const { return m_imageLayout; }

		uint32_t GetComponents() const { return m_components; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		VkImage &GetImage() { return m_image; }

		VkDeviceMemory &GetDeviceMemory() { return m_deviceMemory; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }

		static uint8_t *LoadPixels(const std::string &filename, uint32_t *width, uint32_t *height, uint32_t *components);

		static uint8_t *LoadPixels(const std::string &filename, const std::string &fileSuffix, const std::vector<std::string> &fileSides, uint32_t *width, uint32_t *height, uint32_t *components);

		static bool WritePixels(const std::string &filename, const void *data, const int32_t &width, const int32_t &height, const int32_t &components = 4);

		static void DeletePixels(uint8_t *pixels);

		static uint32_t GetMipLevels(const uint32_t &width, const uint32_t &height);

		static void CreateImage(VkImage &image, VkDeviceMemory &imageMemory, const uint32_t &width, const uint32_t &height, const VkImageType &type, const VkSampleCountFlagBits &samples, const uint32_t &mipLevels, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, const uint32_t &arrayLayers);

		static bool HasStencilComponent(const VkFormat &format);

		static void TransitionImageLayout(const VkImage &image, const VkFormat &format, const VkImageLayout &srcImageLayout, const VkImageLayout &dstImageLayout, const uint32_t &mipLevels, const uint32_t &baseArrayLayer, const uint32_t &layerCount);

		static void CopyBufferToImage(const VkBuffer &buffer, const VkImage &image, const uint32_t &width, const uint32_t &height, const uint32_t &baseArrayLayer, const uint32_t &layerCount);

		static void CreateMipmaps(const VkImage &image, const uint32_t &width, const uint32_t &height, const VkImageLayout &dstImageLayout, const uint32_t &mipLevels, const uint32_t &baseArrayLayer, const uint32_t &layerCount);

		static void CreateImageSampler(VkSampler &sampler, const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic, const uint32_t &mipLevels);

		static void CreateImageView(const VkImage &image, VkImageView &imageView, const VkImageViewType &type, const VkFormat &format, const VkImageAspectFlags &imageAspect, const uint32_t &mipLevels, const uint32_t &baseArrayLayer, const uint32_t &layerCount);

		static bool CopyImage(const VkImage &srcImage, VkImage &dstImage, VkDeviceMemory &dstImageMemory, const uint32_t &width, const uint32_t &height, const bool &srcSwapchain, const uint32_t &baseArrayLayer, const uint32_t &layerCount);

		static void InsertImageMemoryBarrier(const VkCommandBuffer &cmdbuffer, const VkImage &image, const VkAccessFlags &srcAccessMask,
											 const VkAccessFlags &dstAccessMask, const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout,
											 const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask, const VkImageSubresourceRange &subresourceRange);
	};
}
