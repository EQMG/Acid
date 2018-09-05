#pragma once

#include <string>
#include <vector>
#include <vulkan/vulkan.h>
#include "Files/Files.hpp"
#include "Renderer/Buffers/Buffer.hpp"
#include "Renderer/Descriptors/IDescriptor.hpp"
#include "Resources/IResource.hpp"

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
		VkSampleCountFlagBits m_samples;

		uint32_t m_components;
		uint32_t m_width, m_height;

		VkImage m_image;
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
		/// <param name="repeatEdges"> If UV coords will wrap if outside of edge bounds. </param>
		/// <param name="mipmap"> If mipmaps will be used on the texture. </param>
		/// <param name="anisotropic"> If anisotropic will be use on the texture. </param>
		/// <param name="nearest"> If nearest filtering will be use on the texture. </param>
		Texture(const std::string &filename, const bool &repeatEdges = true, const bool &mipmap = true, const bool &anisotropic = true, const bool &nearest = false);

		/// <summary>
		/// A new empty texture object that can be used to render into.
		/// </summary>
		/// <param name="width"> The textures width. </param>
		/// <param name="height"> The textures height. </param>
		/// <param name="format"> The textures format. </param>
		/// <param name="imageLayout"> The textures image layout </param>
		/// <param name="usage"> The textures image usage </param>
		/// <param name="samples"> The amount of MSAA samples to use. </param>
		Texture(const uint32_t &width, const uint32_t &height, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		/// <summary>
		/// A new texture object from a array of pixels.
		/// </summary>
		/// <param name="width"> The textures width. </param>
		/// <param name="height"> The textures height. </param>
		/// <param name="pixels"> The inital pixels to use in the texture. <seealso cref="#GetPixels()"/> to get a copy of the pixels, and <seealso cref="#SetPixels()"/> to set the pixels</param>
		/// <param name="format"> The textures format. </param>
		/// <param name="imageLayout"> The textures image layout </param>
		/// <param name="usage"> The textures image usage </param>
		/// <param name="samples"> The amount of MSAA samples to use. </param>
		Texture(const uint32_t &width, const uint32_t &height, float *pixels, const VkFormat &format = VK_FORMAT_R8G8B8A8_UNORM, const VkImageLayout &imageLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			const VkImageUsageFlags &usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_STORAGE_BIT, const VkSampleCountFlagBits &samples = VK_SAMPLE_COUNT_1_BIT);

		~Texture();

		static DescriptorType CreateDescriptor(const uint32_t &binding, const VkShaderStageFlags &stage);

		VkWriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const DescriptorSet &descriptorSet) const override;

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

		uint32_t GetComponents() const { return m_components; }

		uint32_t GetWidth() const { return m_width; }

		uint32_t GetHeight() const { return m_height; }

		VkImage GetImage() const { return m_image; }

		VkImageView GetImageView() const { return m_imageView; }

		VkSampler GetSampler() const { return m_sampler; }

		static int32_t LoadSize(const std::string &filepath);

		static int32_t LoadSize(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes);

		static uint8_t *LoadPixels(const std::string &filepath, uint32_t *width, uint32_t *height, uint32_t *components);

		static uint8_t *LoadPixels(const std::string &filename, const std::string &fileExt, const std::vector<std::string> &fileSuffixes, const size_t &bufferSize, uint32_t *width, uint32_t *height, uint32_t *components);

		static bool WritePixels(const std::string &filename, const void *data, const int32_t &width, const int32_t &height, const int32_t &components = 4);

		static void DeletePixels(uint8_t *pixels);

		static uint32_t GetMipLevels(const uint32_t &width, const uint32_t &height);

		static void CreateImage(VkImage &image, VkDeviceMemory &imageMemory, const uint32_t &width, const uint32_t &height, const VkImageType &type, const VkSampleCountFlagBits &samples, const uint32_t &mipLevels, const VkFormat &format, const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, const uint32_t &arrayLayers);

		static bool HasStencilComponent(const VkFormat &format);

		static void TransitionImageLayout(const VkImage &image, const VkFormat &format, const VkImageLayout &srcImageLayout, const VkImageLayout &dstImageLayout, const uint32_t &mipLevels, const uint32_t &layerCount);

		static void CopyBufferToImage(const VkBuffer &buffer, const VkImage &image, const uint32_t &width, const uint32_t &height, const uint32_t &layerCount);

		static void CreateMipmaps(const VkImage &image, const uint32_t &width, const uint32_t &height, const uint32_t &mipLevels, const uint32_t &layerCount);

		static void CreateImageSampler(VkSampler &sampler, const bool &repeatEdges, const bool &anisotropic, const bool &nearest, const uint32_t &mipLevels);

		static void CreateImageView(const VkImage &image, VkImageView &imageView, const VkImageViewType &type, const VkFormat &format, const VkImageAspectFlags &imageAspect, const uint32_t &mipLevels, const uint32_t &layerCount);

		static bool CopyImage(const VkImage &srcImage, VkImage &dstImage, VkDeviceMemory &dstImageMemory, const uint32_t &width, const uint32_t &height, const bool &srcSwapchain);

		static void InsertImageMemoryBarrier(const VkCommandBuffer &cmdbuffer, const VkImage &image, const VkAccessFlags &srcAccessMask,
											 const VkAccessFlags &dstAccessMask, const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout,
											 const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask, const VkImageSubresourceRange &subresourceRange);
	};
}
