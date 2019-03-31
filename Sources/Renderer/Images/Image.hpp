#pragma once

#include <vulkan/vulkan.h>
#include "Renderer/Commands/CommandBuffer.hpp"
#include "Renderer/Descriptors/Descriptor.hpp"

namespace acid
{
/// <summary>
/// Class that represents a Vulkan image, sampler, and view.
/// </summary>
class ACID_EXPORT Image :
	public Descriptor
{
public:
	/// <summary>
	/// Creates a new image object.
	/// </summary>
	/// <param name="extent"> The number of data elements in each dimension of the base level. </param>
	/// <param name="imageType"> The dimensionality of the image. </param>
	/// <param name="format"> The format and type of the texel blocks that will be contained in the image. </param>
	/// <param name="samples"> The number of samples per texel. </param>
	/// <param name="tiling"> The tiling arrangement of the texel blocks in memory. </param>
	/// <param name="usage"> The intended usage of the image. </param>
	/// <param name="properties"> The images memory properties. </param>
	/// <param name="mipLevels"> The number of levels of detail available for minified sampling of the image. </param>
	/// <param name="arrayLayers"> The number of layers in the image. </param>
	Image(const VkExtent3D &extent, const VkImageType &imageType, const VkFormat &format, const VkSampleCountFlagBits &samples, const VkImageTiling &tiling,
		const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, const uint32_t &mipLevels, const uint32_t &arrayLayers);

	/*const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic,
	const VkImageViewType &viewType, const VkImageAspectFlags &imageAspect, const uint32_t &baseMipLevel, const uint32_t &baseArrayLayer*/

	~Image();

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(const uint32_t &binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage,
		const uint32_t &count);

	WriteDescriptorSet GetWriteDescriptor(const uint32_t &binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const override;

	/// <summary>
	/// Gets a copy of the textures pixels from memory.
	/// </summary>
	/// <param name="extent"> The sampled images extents. </param>
	/// <param name="mipLevel"> The mipmap level index to get the pixels from. </param>
	/// <param name="arrayLayer"> The array layer to copy from. </param>
	/// <returns> A copy of the textures pixels. </returns>
	std::unique_ptr<uint8_t[]> GetPixels(VkExtent3D &extent, const uint32_t &mipLevel = 1, const uint32_t &arrayLayer = 0) const;

	/// <summary>
	/// Copies the pixels into this textures memory.
	/// </summary>
	/// <param name="pixels"> The pixels to copy to the image. </param>
	/// <param name="layerCount"> The amount of array layers the pixels represent. </param>
	/// <param name="baseArrayLayer"> The base array layer to copy into. </param>
	void SetPixels(const uint8_t *pixels, const uint32_t &layerCount, const uint32_t &baseArrayLayer);

	const VkExtent3D &GetExtent() const { return m_extent; }

	const VkFormat &GetFormat() const { return m_format; }

	const VkSampleCountFlagBits &GetSamples() const { return m_samples; }

	const VkImageUsageFlags &GetUsage() const { return m_usage; }

	const uint32_t &GetMipLevels() const { return m_mipLevels; }

	const uint32_t &GetArrayLevels() const { return m_arrayLayers; }

	const VkFilter &GetFilter() const { return m_filter; }

	const VkSamplerAddressMode &GetAddressMode() const { return m_addressMode; }

	const bool &IsAnisotropic() const { return m_anisotropic; }

	const VkImageLayout &GetLayout() const { return m_layout; }

	const VkImage &GetImage() { return m_image; }

	const VkDeviceMemory &GetMemory() { return m_memory; }

	const VkSampler &GetSampler() const { return m_sampler; }

	const VkImageView &GetView() const { return m_view; }

	static std::unique_ptr<uint8_t[]> LoadPixels(const std::string &filename, uint32_t &width, uint32_t &height, uint32_t &components, VkFormat &format);

	static void WritePixels(const std::string &filename, const uint8_t *pixels, const int32_t &width, const int32_t &height, const int32_t &components = 4);

	static uint32_t GetMipLevels(const VkExtent3D &extent);

	/// <summary>
	/// Gets if this depth image has a depth component.
	/// </summary>
	/// <returns> If this has a depth component. </returns>
	static bool HasDepth(const VkFormat &format);

	/// <summary>
	/// Gets if this depth image has a stencil component.
	/// </summary>
	/// <returns> If this has a stencil component. </returns>
	static bool HasStencil(const VkFormat &format);

	static void CreateImage(VkImage &image, VkDeviceMemory &memory, const VkExtent3D &extent, const VkFormat &format, const VkSampleCountFlagBits &samples,
		const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, const uint32_t &mipLevels, const uint32_t &arrayLayers,
		const VkImageType &type);

	static void CreateImageSampler(VkSampler &sampler, const VkFilter &filter, const VkSamplerAddressMode &addressMode, const bool &anisotropic, const uint32_t &mipLevels);

	static void CreateImageView(const VkImage &image, VkImageView &imageView, const VkImageViewType &type, const VkFormat &format, const VkImageAspectFlags &imageAspect, 
		const uint32_t &mipLevels, const uint32_t &baseMipLevel, const uint32_t &layerCount, const uint32_t &baseArrayLayer);

	static void CreateMipmaps(const VkImage &image, const VkExtent3D &extent, const VkFormat &format, const VkImageLayout &dstImageLayout, const uint32_t &mipLevels,
		const uint32_t &baseArrayLayer, const uint32_t &layerCount);

	static void TransitionImageLayout(const VkImage &image, const VkFormat &format, const VkImageLayout &srcImageLayout, const VkImageLayout &dstImageLayout,
		const VkImageAspectFlags &imageAspect, const uint32_t &mipLevels, const uint32_t &baseMipLevel, const uint32_t &layerCount, const uint32_t &baseArrayLayer);

	static void InsertImageMemoryBarrier(const CommandBuffer &commandBuffer, const VkImage &image, const VkAccessFlags &srcAccessMask, const VkAccessFlags &dstAccessMask,
		const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout, const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask,
		const VkImageAspectFlags &imageAspect, const uint32_t &mipLevels, const uint32_t &baseMipLevel, const uint32_t &layerCount, const uint32_t &baseArrayLayer);

	static void CopyBufferToImage(const VkBuffer &buffer, const VkImage &image, const VkExtent3D &extent, const uint32_t &layerCount, const uint32_t &baseArrayLayer);

	static bool CopyImage(const VkImage &srcImage, VkImage &dstImage, VkDeviceMemory &dstImageMemory, const VkFormat &srcFormat, const VkExtent3D &extent,
		const VkImageLayout &srcImageLayout, const uint32_t &mipLevel, const uint32_t &arrayLayer);

private:
	VkExtent3D m_extent;
	VkFormat m_format;
	VkSampleCountFlagBits m_samples;
	VkImageUsageFlags m_usage;
	uint32_t m_mipLevels;
	uint32_t m_arrayLayers;

	VkFilter m_filter;
	VkSamplerAddressMode m_addressMode;
	bool m_anisotropic;

	VkImageLayout m_layout;

	VkImage m_image;
	VkDeviceMemory m_memory;
	VkSampler m_sampler;
	VkImageView m_view;
};
}
