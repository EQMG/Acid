#pragma once

#include <vulkan/vulkan.h>
#include "Maths/Vector2.hpp"
#include "Graphics/Commands/CommandBuffer.hpp"
#include "Graphics/Descriptors/Descriptor.hpp"

namespace acid {
/**
 * @brief A representation of a Vulkan image, sampler, and view.
 */
class ACID_EXPORT
Image
:
public
Descriptor
{
public:
	/**
	 * Creates a new image object.
	 * @param extent The number of data elements in each dimension of the base level.
	 * @param imageType The dimensionality of the image.
	 * @param format The format and type of the texel blocks that will be contained in the image.
	 * @param samples The number of samples per texel.
	 * @param tiling The tiling arrangement of the texel blocks in memory.
	 * @param usage The intended usage of the image.
	 * @param properties The images memory properties.
	 * @param mipLevels The number of levels of detail available for minified sampling of the image.
	 * @param arrayLayers The number of layers in the image.
	 */
	Image(const VkExtent3D &extent, const VkImageType &imageType, const VkFormat &format, const VkSampleCountFlagBits &samples, const VkImageTiling &tiling,
		const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, uint32_t mipLevels, uint32_t arrayLayers);

	/*const VkFilter &filter, const VkSamplerAddressMode &addressMode, bool anisotropic,
	const VkImageViewType &viewType, const VkImageAspectFlags &imageAspect, uint32_t baseMipLevel, uint32_t baseArrayLayer*/

	~Image();

	static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding, const VkDescriptorType &descriptorType, const VkShaderStageFlags &stage,
		uint32_t count);

	WriteDescriptorSet GetWriteDescriptor(uint32_t binding, const VkDescriptorType &descriptorType, const std::optional<OffsetSize> &offsetSize) const override;

	/**
	 * Copies the images pixels from memory.
	 * @param extent The sampled images extent.
	 * @param mipLevel The mipmap level index to sample.
	 * @param arrayLayer The array level to sample.
	 * @return A copy of the images pixels.
	 */
	std::unique_ptr<uint8_t[]> GetPixels(VkExtent3D &extent, uint32_t mipLevel = 0, uint32_t arrayLayer = 0) const;

	/**
	 * Sets the pixels of this image.
	 * @param pixels The pixels to copy from.
	 * @param layerCount The amount of layers contained in the pixels.
	 * @param baseArrayLayer The first layer to copy into.
	 */
	void SetPixels(const uint8_t *pixels, uint32_t layerCount, uint32_t baseArrayLayer);

	const VkExtent3D &GetExtent() const { return m_extent; }

	const VkFormat &GetFormat() const { return m_format; }

	const VkSampleCountFlagBits &GetSamples() const { return m_samples; }

	const VkImageUsageFlags &GetUsage() const { return m_usage; }

	uint32_t GetMipLevels() const { return m_mipLevels; }

	uint32_t GetArrayLevels() const { return m_arrayLayers; }

	const VkFilter &GetFilter() const { return m_filter; }

	const VkSamplerAddressMode &GetAddressMode() const { return m_addressMode; }

	bool IsAnisotropic() const { return m_anisotropic; }

	const VkImageLayout &GetLayout() const { return m_layout; }

	const VkImage &GetImage() { return m_image; }

	const VkDeviceMemory &GetMemory() { return m_memory; }

	const VkSampler &GetSampler() const { return m_sampler; }

	const VkImageView &GetView() const { return m_view; }

	static std::unique_ptr<uint8_t[]> LoadPixels(const std::filesystem::path &filename, Vector2ui &extent, uint32_t &components, VkFormat &format);

	static void WritePixels(const std::filesystem::path &filename, const uint8_t *pixels, const Vector2ui &extent, int32_t components = 4);

	static uint32_t GetMipLevels(const VkExtent3D &extent);

	/**
	 * Gets if a format has a depth component.
	 * @param format The format to check.
	 * @return If the format has a depth component.
	 */
	static bool HasDepth(const VkFormat &format);

	/**
	 * Gets if a format has a depth component.
	 * @param format The format to check.
	 * @return If the format has a depth component.
	 */
	static bool HasStencil(const VkFormat &format);

	static void CreateImage(VkImage &image, VkDeviceMemory &memory, const VkExtent3D &extent, const VkFormat &format, const VkSampleCountFlagBits &samples,
		const VkImageTiling &tiling, const VkImageUsageFlags &usage, const VkMemoryPropertyFlags &properties, uint32_t mipLevels, uint32_t arrayLayers,
		const VkImageType &type);

	static void CreateImageSampler(VkSampler &sampler, const VkFilter &filter, const VkSamplerAddressMode &addressMode, bool anisotropic, uint32_t mipLevels);

	static void CreateImageView(const VkImage &image, VkImageView &imageView, const VkImageViewType &type, const VkFormat &format, const VkImageAspectFlags &imageAspect,
		uint32_t mipLevels, uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);

	static void CreateMipmaps(const VkImage &image, const VkExtent3D &extent, const VkFormat &format, const VkImageLayout &dstImageLayout, uint32_t mipLevels,
		uint32_t baseArrayLayer, uint32_t layerCount);

	static void TransitionImageLayout(const VkImage &image, const VkFormat &format, const VkImageLayout &srcImageLayout, const VkImageLayout &dstImageLayout,
		const VkImageAspectFlags &imageAspect, uint32_t mipLevels, uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);

	static void InsertImageMemoryBarrier(const CommandBuffer &commandBuffer, const VkImage &image, const VkAccessFlags &srcAccessMask, const VkAccessFlags &dstAccessMask,
		const VkImageLayout &oldImageLayout, const VkImageLayout &newImageLayout, const VkPipelineStageFlags &srcStageMask, const VkPipelineStageFlags &dstStageMask,
		const VkImageAspectFlags &imageAspect, uint32_t mipLevels, uint32_t baseMipLevel, uint32_t layerCount, uint32_t baseArrayLayer);

	static void CopyBufferToImage(const VkBuffer &buffer, const VkImage &image, const VkExtent3D &extent, uint32_t layerCount, uint32_t baseArrayLayer);

	static bool CopyImage(const VkImage &srcImage, VkImage &dstImage, VkDeviceMemory &dstImageMemory, const VkFormat &srcFormat, const VkExtent3D &extent,
		const VkImageLayout &srcImageLayout, uint32_t mipLevel, uint32_t arrayLayer);

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

	VkImage m_image = VK_NULL_HANDLE;
	VkDeviceMemory m_memory = VK_NULL_HANDLE;
	VkSampler m_sampler = VK_NULL_HANDLE;
	VkImageView m_view = VK_NULL_HANDLE;
};
}
